<?php
// $Id: user_add-save.inc,v 1.4.2.1 2003-11-24 18:07:18 dlw Exp $

// Find out what objectclasses to use when creating user
$objectclasses_included = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn));

// Get all objectclasses the LDAP server understand
$objectclasses_schema   = pql_get_subschema($_pql->ldap_linkid, 'objectclasses');

// convert uid, email to lowercase
if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
	$uid = strtolower($uid);
	$entry[pql_get_define("PQL_GLOB_ATTR_UID")] = $uid;
}

if($account_type != 'shell') {
	if(! ereg("@", $email)) {
		// Build the COMPLETE email address
		if($email_domain)
		  $email = strtolower($email . "@" . $email_domain);
		else
		  $email = strtolower($email . "@" . $defaultdomain);
	} else {
		$email = strtolower($email);
	}
}

// prepare the users attributes
if($surname) {
	// Firstname
	$entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]	= $surname;
} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
	$entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]	= $uid;
}

if($name) {
	// Lastname
	$entry[pql_get_define("PQL_GLOB_ATTR_SN")]			= $name;
} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
	$entry[pql_get_define("PQL_GLOB_ATTR_SN")]			= $uid;
}

// ------------------
if($account_type != 'shell') {
	// Check to see if the attribute PQL_GLOB_ATTR_ISACTIVE (should be accountStatus)
	// is a MUST (required attribute) or a MAY (allowed attribute) in any of the
	// objectclasses we have choosen to use.
	$val = pql_check_attribute($objectclasses_schema,
							   $objectclasses_included,
							   pql_get_define("PQL_GLOB_ATTR_ISACTIVE"));
	if($val[0] >= 1)
	  // The attribute is either a MUST or a MAY - which doesn't interest me :)
	  $entry[pql_get_define("PQL_GLOB_ATTR_ISACTIVE")]= $account_status;
	
	$entry[pql_get_define("PQL_GLOB_ATTR_MAIL")]		= $email;
	if($include_additional == 'on' and is_array($additionaldomainname)) {
		if(ereg("@", $email)) {
			$email_temp = pql_strip_domain($email);
		} else
		  $email_temp = $email;
		
		foreach($additionaldomainname as $additional)
		  $entry[pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE")][] = strtolower($email_temp . "@" . $additional);
	}
}

// ------------------
if(($account_type == "system") or ($account_type == "shell")) {
	$entry[pql_get_define("PQL_GLOB_ATTR_LOGINSHELL")] = $loginshell;
}

// ------------------
if($account_type != "forward") {
	if($account_type != "normal") {
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
			// Get a free UserID number (which we also use for GroupID number)
			$uidnr = pql_get_next_uidnumber($_pql);
			if($uidnr > 0) {
				$entry[pql_get_define("PQL_GLOB_ATTR_QMAILUID")] = $uidnr;
				$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = $uidnr;
			}
		}
	} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
		// It's a 'Mail account'. Use the forwarding account uidNumber
		// for ALL 'mail only' accounts.
		$entry[pql_get_define("PQL_GLOB_ATTR_QMAILUID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
		$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
	}
	
	// Gecos is needed to do PAM/NSS LDAP login 
	if($surname && $name &&
	   (pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID"))) {
		$entry[pql_get_define("PQL_GLOB_ATTR_GECOS")] = $surname . " " . $name;
	}
	
	// set attributes
	if(eregi('KERBEROS', $pwscheme)) {
		// We're using the {KERBEROS} password scheme. Special circumstances.
		// The userPassword and krb5PrincipalName needs to be set. This is 
		// automagicly created with the help of the username (uid) value and
		// the REALM name.
		if(eregi('@', $password))
		  // User know what he/she's doing. We specified the full principal
		  // name directly in the password field! Use that as userPassword
		  // and krb5PrincipalName.
		  $entry[pql_get_define("PQL_GLOB_ATTR_KRB5PRINCIPALNAME")] = $password;
		elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID"))
		  // The password really IS a password!
		  $entry[pql_get_define("PQL_GLOB_ATTR_KRB5PRINCIPALNAME")] = $uid . "@" . pql_get_define("PQL_GLOB_KRB5_REALM");
		
		// Encrypt and create the hash using the krb5PrincipalName attribute
		$entry[pql_get_define("PQL_GLOB_ATTR_PASSWD")] = pql_password_hash($entry[pql_get_define("PQL_GLOB_ATTR_KRB5PRINCIPALNAME")], $pwscheme);
	} else {
		if($crypted)
		  // Password is already encrypted, prefix with choosen password scheme
		  $entry[pql_get_define("PQL_GLOB_ATTR_PASSWD")] = $pwscheme . $password;
		else
		  // Password isn't already encrypted, create the hash using the password value
		  $entry[pql_get_define("PQL_GLOB_ATTR_PASSWD")] = pql_password_hash($password, $pwscheme);
	}
	
	if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
		if(!$homedirectory) {
			if(($account_type == "normal") and !$_SESSION["ADVANCED_MODE"])
			  // It's a mail account, and we where/is not running in
			  // advanced mode. Use the maildirectory as homedirectory.
			  $entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")] = user_generate_mailstore($_pql, $email,
																						$domain, $entry,
																						'user');
			else
			  $entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")] = user_generate_homedir($_pql, $email,
																					  $domain, $entry,
																					  'user');
		} else
		  $entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")] = $homedirectory;
	}
}

// ------------------
if(($account_type == "system") or ($account_type == "normal")) {
	// normal mailbox account
	
	if($userhost)
	  $entry[pql_get_define("PQL_GLOB_ATTR_MAILHOST")] = $userhost;
	else {
		$domainname = pql_strip_username($entry[pql_get_define("PQL_GLOB_ATTR_MAIL")]);
		
		// Initiate a connection to the QmailLDAP/Controls DN
		$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		
		if($_pql_control->ldap_linkid) {
			// Find MX (or QmailLDAP/Controls with locals=$domainname)
			$mx = pql_get_mx($_pql_control->ldap_linkid, $domainname);
			if(is_array($mx))
			  $entry[pql_get_define("PQL_GLOB_ATTR_MAILHOST")] = $mx[1];
		}
	}
	
	$entry[pql_get_define("PQL_GLOB_ATTR_MODE")] = "localdelivery";
	
	if(!$maildirectory)
	  $entry[pql_get_define("PQL_GLOB_ATTR_MAILSTORE")] = user_generate_mailstore($_pql, $email,
																				  $domain, $entry,
																				  'user');
	else
	  $entry[pql_get_define("PQL_GLOB_ATTR_MAILSTORE")] = $maildirectory;
} elseif($account_type != "shell") {
	// forwardonly account
	
	// convert forwardingaddress to lowercase
	$forwardingaddress = strtolower($forwardingaddress);
	
	// set attributes
	$entry[pql_get_define("PQL_GLOB_ATTR_FORWARDS")]	= $forwardingaddress;
	$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][]	= "forwardonly";
	$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][]	= "nombox";
	
	// Even forward accounts need UIDNumber! (?!?)
	$entry[pql_get_define("PQL_GLOB_ATTR_QMAILUID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
	$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
	if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
		$entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")]  = "/tmp";
	}
}

// ------------------
if($surname && $name) {
	$entry[pql_get_define("PQL_GLOB_ATTR_CN")] = trim($surname) . " " . trim($name);
} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
	$entry[pql_get_define("PQL_GLOB_ATTR_CN")] = $uid;
}

if($entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]) {
	$entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]	= $entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")];
}

if($entry[pql_get_define("PQL_GLOB_ATTR_SN")]) {
	$entry[pql_get_define("PQL_GLOB_ATTR_SN")] = $entry[pql_get_define("PQL_GLOB_ATTR_SN")];
}

if($entry[pql_get_define("PQL_GLOB_ATTR_CN")]) {
	$entry[pql_get_define("PQL_GLOB_ATTR_CN")] = $entry[pql_get_define("PQL_GLOB_ATTR_CN")];
}

// ------------------
// Add the user to the database
$DNs = pql_user_add($_pql->ldap_linkid, $domain, $cn, $entry, $account_type, $subbranch);
if($DNs[0]) {
	// TODO: DNs[1] (the group object) might still be empty -> failed to add it.
	
	// Now it's time to run the special adduser script if defined...
	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $rootdn)) {
		// Setup the environment with the user details
		putenv("PQL_CONF_DOMAIN=$domain");
		putenv("PQL_CONF_WEBUSER=".posix_getuid());
		foreach($entry as $key => $e) {
			$key = "PQL_" . strtoupper($key);
			if($key != 'PQL_CONF_OBJECTCLASS')
			  putenv("$key=$e");
		}
		
		if(pql_get_define("PQL_GLOB_KRB5_ADMIN_COMMAND_PATH") and 
		   pql_get_define("PQL_GLOB_KRB5_REALM") and
		   pql_get_define("PQL_GLOB_KRB5_ADMIN_PRINCIPAL") and
		   pql_get_define("PQL_GLOB_KRB5_ADMIN_SERVER") and 
		   pql_get_define("PQL_GLOB_KRB5_ADMIN_KEYTAB")) {
			putenv("PQL_KADMIN_CMD=".pql_get_define("PQL_GLOB_KRB5_ADMIN_COMMAND_PATH")."/kadmin");
			putenv("PQL_KADMIN_REALM=".pql_get_define("PQL_GLOB_KRB5_REALM"));
			putenv("PQL_KADMIN_PRINC=".pql_get_define("PQL_GLOB_KRB5_ADMIN_PRINCIPAL"));
			putenv("PQL_KADMIN_SERVR=".pql_get_define("PQL_GLOB_KRB5_ADMIN_SERVER"));
			putenv("PQL_KADMIN_KEYTB=".pql_get_define("PQL_GLOB_KRB5_ADMIN_KEYTAB"));
		}
		
		// Execute the user add script (0 => show output)
		if(pql_execute(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $rootdn), 0)) {
			echo pql_complete_constant($LANG->_('The %what% add script failed'),
									   array('what' => $LANG->_('user'))) . "!<br>";
			$msg = urlencode(pql_complete_constant($LANG->_('The %what% add script failed'),
												   array('what' => $LANG->_('user'))) ."!") . ".&nbsp;<br>";
		} else {
			echo "<b>" . pql_complete_constant($LANG->_('Successfully executed the %what% add script'),
											   array('what' => $LANG->_('user'))) . "</b><br>";
			$msg = urlencode(pql_complete_constant($LANG->_('Successfully executed the %what% add script'),
												   array('what' => $LANG->_('user')))) . ".&nbsp;<br>";
		}
	}
	
	$msg .= urlencode($LANG->_('Successfully created the new user'));
	
	if(pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $rootdn)) {
		$url  = "user_sendmail.php?email=" . urlencode($email) . "&";
		$url .= "rootdn=$rootdn&domain=$domain&user=" . urlencode($DNs[0]) . "&rlnb=2&msg=$msg";
	} else {
		$url  = "user_detail.php?rootdn=$rootdn&";
		$url .= "domain=$domain&user=" . urlencode($DNs[0]) . "&rlnb=2&msg=$msg";
	}
	
	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $rootdn)) {
?>
    <form action="<?=$url?>" method="post">
      <input type="submit" value="Continue">
    </form>
<?php
		die();
	} else {
		header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
	}
} else {
	$msg = urlencode($LANG->_('Failed to create the new user') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	$url = "domain_detail.php?rootdn=$rootdn&domain=$domain&msg=$msg";
	header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
