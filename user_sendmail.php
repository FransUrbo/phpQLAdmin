<?php
// send a testmail to an emailaddress
// $Id: user_sendmail.php,v 1.1 2002-12-11 15:09:23 turbo Exp $
//
require("pql.inc");
$_pql = new pql();
?>
<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="normal.css" type="text/css">
</head>

<body bgcolor="#e7e7e7" background="images/bkg.png">
<span class="title1"><?php echo PQL_SENDMAIL ?></span>
<br><br>
<?php
	if(!pql_user_exist($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user)){
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
	
	$cn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_CN);
	$vars['CN'] = $cn[0];
	$sn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_SN);
	$vars['SN'] = $sn[0];
	
	$quota = pql_get_userquota($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user);
	
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
			$_pql_control = new pql_control();
			
			$quota = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "ldapdefaultquota");
			
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
