<?php
// send a testmail to an emailaddress
// $Id: user_sendmail.php,v 2.17 2003-11-19 19:38:19 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

include("./header.html");
?>
  <span class="title1"><?=$LANG->_('Send testmail')?></span>
  <br><br>
<?php
if(!pql_user_exist($_pql->ldap_linkid, $user)) {
    echo pql_complete_constant($LANG->_('User %user% does not exist'), array('user', $user));
    exit();
}

if($email == "") {
    die($LANG->_('No email address given'));
}

$subject = PQL_CONF_TESTMAIL_SUBJECT;
$from = "From: " . pql_get_define("PQL_GLOB_HOSTMASTER") . "\n";
$xmailer = "X-Mailer: phpQLAdmin $VERSION\n";
$vars['MAIL'] = $email;
$vars['UID'] = $user;
$vars['VERSION'] = $VERSION;

$cn = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_CN"));
$vars['CN'] = $cn[0];
$sn = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_SN"));
$vars['SN'] = $sn[0];

$quota = pql_get_userquota($_pql->ldap_linkid, $user);

// Does the user have an individual mailbox quota?
if (is_array($quota)) {
    $vars['QUOTA'] = pql_ldap_mailquota($quota);
} else {
    // No quota for this mailbox.
    // Do we use the qmail-ldap/control patch?
    if (!pql_get_define("PQL_GLOB_CONTROL_USE")) {
	// No -> quota is 'standard'
	$vars['QUOTA'] = $LANG->_('Standard');
    } else {
	// QmailLDAP/Controls patch is used - get the standard quota
	require("./include/pql_control.inc");
	$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
	
	$quota = pql_control_get_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR,
					   pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTQUOTA"));
	
	$vars['QUOTA'] = pql_ldap_mailquota($quota);
    }
}
	
$message = pql_complete_constant(PQL_CONF_TESTMAIL_MAILTEXT, $vars);
	
$header = $from . $xmailer;

// we are in 'auto-send' mode and have already a message from another function
if (!empty($msg)) {
    $msg .= '<br>';
} else {
    $msg = '';
}

if(mail($email, $subject, $message, $header)){
    $msg .= pql_complete_constant($LANG->_('Successfully sended mail to %email%"'), array("email" => $email));
} else {
    $msg .= $LANG->_('Failed sending mail');
}

$url = "user_detail.php?domain=$domain&user=".urlencode($user)."&msg=".urlencode($msg);

if(isset($rlnb))
     $url .= "&rlnb=$rlnb";

header("Location: " . pql_get_define("PQL_GLOB_URI") . $url);
?>
</body>
</html>
