<?php
// send a testmail to an emailaddress
// user_sendmail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("pql.inc");

$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

include("header.html");
?>
  <span class="title1"><?php echo PQL_SENDMAIL ?></span>
  <br><br>
<?php
if(!pql_user_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user)){
    echo "user &quot;$user&quot; does not exist";
    exit();
}

if($email == ""){
    die("no email address given...");
}

$subject = PQL_TESTMAIL_SUBJECT;
$from = "From: " . PQL_HOSTMASTER . "\n";
$xmailer = "X-Mailer: phpQLAdmin " . PQL_VERSION . "\n";
$vars['MAIL'] = $email;
$vars['UID'] = $user;
$vars['PQL_VERSION'] = PQL_VERSION;

$cn = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_CN);
$vars['CN'] = $cn[0];
$sn = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_SN);
$vars['SN'] = $sn[0];

$quota = pql_get_userquota($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user);

// Does the user have an individual mailbox quota?
if (is_array($quota)) {
    $vars['QUOTA'] = pql_ldap_mailquota($quota);
} else {
    // No quota for this mailbox.
    // Do we use the qmail-ldap/control patch?
    if (!PQL_LDAP_CONTROL_USE) {
	// No -> quota is 'standard'
	$vars['QUOTA'] = PQL_LDAP_MAILQUOTA_DEFAULT;
    } else {
	// qmail-ldap/control patch is used
	// search the standard quota...
	require("pql_control.inc");
	require("pql_control_plugins.inc");
	$_pql_control = new pql_control($USER_HOST_CTR, $USER_DN, $USER_PASS);
	
	$quota = pql_control_get_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "ldapdefaultquota");
	
	$vars['QUOTA'] = pql_ldap_mailquota($quota);
    }
}
	
$message = pql_complete_constant(PQL_TESTMAIL_MAILTEXT, $vars);
	
$header = $from . $xmailer;

// we are in 'auto-send' mode and have already a message from another function
if (!empty($msg)) {
    $msg .= '<br>';
} else {
    $msg = '';
}
	
if(mail($email, $subject, $message, $header)){
    $msg .= pql_complete_constant(PQL_SENDMAIL_OK, array("email" => $email));
} else {
    $msg .= PQL_SENDMAIL_FAILED;
}

$msg = urlencode($msg);
$user = urlencode($user);
$url = "user_detail.php?domain=$domain&user=" . urlencode($user) . "&msg=$msg";

if (isset($rlnb)) $url .= "&rlnb=" . urlencode($rlnb);

header("Location: " . PQL_URI . $url);
?>
</body>
</html>
