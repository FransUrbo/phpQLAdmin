<?php
// send a testmail to an emailaddress
// $Id: user_sendmail.php,v 2.33 2007-02-15 12:07:14 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/header.html");
?>
  <span class="title1"><?php echo $LANG->_('Send testmail')?></span>
  <br><br>
<?php
if(!$_pql->get_dn($_REQUEST["user"], '(objectclass=*)', 'BASE')) {
    echo pql_complete_constant($LANG->_('User %user% does not exist'), array('user', $_REQUEST["user"]));
    exit();
}

if($email == "") {
    die($LANG->_('No email address given'));
}

$subject = pql_get_define("PQL_ATTR_TESTMAIL_SUBJECT");
$from = "From: " . pql_get_define("PQL_ATTR_HOSTMASTER") . "\n";
$xmailer = "X-Mailer: phpQLAdmin ".$_SESSION["VERSION"]."\n";
$vars['MAIL'] = $email;
$vars['UID'] = $_REQUEST["user"];
$vars['VERSION'] = $_SESSION["VERSION"];

$cn = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_CN"));
$vars['CN'] = $cn[0];
$sn = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_SN"));
$vars['SN'] = $sn[0];

$quota = pql_user_get_quota($_REQUEST["user"]);

// Does the user have an individual mailbox quota?
if (is_array($quota)) {
    $vars['QUOTA'] = pql_ldap_mailquota($quota);
} else {
    // No quota for this mailbox.
    // Do we use the qmail-ldap/control patch?
    if (!pql_get_define("PQL_CONF_CONTROL_USE")) {
	// No -> quota is 'standard'
	$vars['QUOTA'] = $LANG->_('Standard');
    } else {
	// QmailLDAP/Controls patch is used - get the standard quota
	require($_SESSION["path"]."/include/pql_control.inc");
	$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
	
	$quota = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
				   pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"));
	if($quota)
	  $vars['QUOTA'] = pql_ldap_mailquota($quota);
    }
}
	
$message = pql_complete_constant(pql_get_define("PQL_CONF_TESTMAIL_MAILTEXT"), $vars);
	
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

$url = "user_detail.php?domain=".$_REQUEST["domain"]."&user=".urlencode($_REQUEST["user"])."&msg=".urlencode($msg);
if(isset($_REQUEST["rlnb"]))
     $url .= "&rlnb=".$_REQUEST["rlnb"];
pql_header($url);
?>
</body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
