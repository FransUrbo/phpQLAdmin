<?php
// $Id: user_add-save.inc,v 1.16 2004-04-01 11:59:37 turbo Exp $

// Find out what objectclasses to use when creating user
$objectclasses_included = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $_REQUEST["rootdn"]));

// Get all objectclasses the LDAP server understand
$objectclasses_schema   = pql_get_subschema($_pql->ldap_linkid, 'objectclasses');

// ============================================================================================
// ============== PREPARE THE USERS ATTRIBUTE(S)

// ------------------
// convert uid, email to lowercase
if($_REQUEST["uid"]) {
	$_REQUEST["uid"] = strtolower($_REQUEST["uid"]);
	$entry[pql_get_define("PQL_ATTR_UID")] = $_REQUEST["uid"];
}

// ------------------
// If we've specified to use the object class 'inetOrgPerson', then allow 'givenName'.
if(($_REQUEST["account_type"] != "mail") or in_array("inetorgperson", $objectclasses_included)) {
	// Firstname
	if($_REQUEST["surname"]) {
		$entry[pql_get_define("PQL_ATTR_GIVENNAME")]	= $_REQUEST["surname"];
	} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")) {
		$entry[pql_get_define("PQL_ATTR_GIVENNAME")]	= $_REQUEST["uid"];
	}
}

// If we've specified to use the object class 'person', then allow 'cn' and 'sn'.
if(($_REQUEST["account_type"] != "mail") or in_array("person", $objectclasses_included)) {
	// Lastname
	if($_REQUEST["name"]) {
		$entry[pql_get_define("PQL_ATTR_SN")]			= $_REQUEST["name"];
	} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")) {
		$entry[pql_get_define("PQL_ATTR_SN")]			= $_REQUEST["uid"];
	}
	
	// Full (common) name
	if($_REQUEST["surname"] && $_REQUEST["name"]) {
		$entry[pql_get_define("PQL_ATTR_CN")] = trim($_REQUEST["surname"]) . " " . trim($_REQUEST["name"]);
	} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")) {
		$entry[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["uid"];
	}
}

// ------------------
if($_REQUEST["account_type"] != 'shell') {
	if(in_array('trustaccount', $objectclasses_included))
	  // Only add 'accountStatus' if admin have enabled the 'trustAccount' object class.
	  $entry[pql_get_define("PQL_ATTR_ISACTIVE")]	= $_REQUEST["account_status"];

	// Cleanup the email address a bit
	if(! ereg("@", $_REQUEST["email"])) {
		// Build the COMPLETE email address
		if($_REQUEST["email_domain"])
		  $_REQUEST["email"] = strtolower($_REQUEST["email"] . "@" . $_REQUEST["email_domain"]);
		else
		  $_REQUEST["email"] = strtolower($_REQUEST["email"] . "@" . $defaultdomain);
	} else
	  $_REQUEST["email"] = strtolower($_REQUEST["email"]);

	// Add the primary mail address to the entry array
	$entry[pql_get_define("PQL_ATTR_MAIL")]		= $_REQUEST["email"];

	// Add the additional email addresses as well
	if($include_additional == 'on' and is_array($additionaldomainname)) {
		if(ereg("@", $_REQUEST["email"])) {
			$email_temp = pql_strip_domain($_REQUEST["email"]);
		} else
		  $email_temp = $_REQUEST["email"];
		
		foreach($additionaldomainname as $additional)
		  $entry[pql_get_define("PQL_ATTR_MAILALTERNATE")][] = strtolower($email_temp . "@" . $additional);
	}
}

// ------------------
if($_REQUEST["account_type"] == "forward") {
	// convert forwardingaddress to lowercase
	$forwardingaddress = strtolower($forwardingaddress);
	
	// set attributes
	$entry[pql_get_define("PQL_ATTR_FORWARDS")]	= $forwardingaddress;
	$entry[pql_get_define("PQL_ATTR_MODE")][]	= "forwardonly";
	$entry[pql_get_define("PQL_ATTR_MODE")][]	= "nombox";
	
	if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))
	  $entry[pql_get_define("PQL_ATTR_HOMEDIR")]= "/tmp";
} else {
	// Any other account than 'forwarding'.

	// -----------------
	// Add [ug]id number to the entry array
	if($_REQUEST["account_type"] != "mail") {
		// Get a free UserID number (which we also use for GroupID number)
		$uidnr = pql_get_next_ugidnumber($_pql, 'uid');
		if($uidnr > 0)
		  $entry[pql_get_define("PQL_ATTR_QMAILUID")] = $uidnr;
		
		$gidnr = pql_get_next_ugidnumber($_pql, 'gid');
		if($gidnr > 0)
		  $entry[pql_get_define("PQL_ATTR_QMAILGID")] = $gidnr;
	
		// -----------------
		// Gecos is needed to do PAM/NSS LDAP login 
		if($_REQUEST["surname"] && $_REQUEST["name"] &&
		   (pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))) {
			$entry[pql_get_define("PQL_ATTR_GECOS")] = $_REQUEST["surname"] . " " . $_REQUEST["name"];
		}
	}
	
	// -----------------
	// Add password (crypted w/ salt) to the entry array
	if(eregi('KERBEROS', $_REQUEST["pwscheme"])) {
		// We're using the {KERBEROS} password scheme. Special circumstances.
		// The userPassword and krb5PrincipalName needs to be set. This is 
		// automagicly created with the help of the username (uid) value and
		// the REALM name.
		if(eregi('@', $_REQUEST["password"]))
		  // User know what he/she's doing. We specified the full principal
		  // name directly in the password field! Use that as userPassword
		  // and krb5PrincipalName.
		  $entry[pql_get_define("PQL_ATTR_KRB5PRINCIPALNAME")] = $_REQUEST["password"];
		elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))
		  // The password really IS a password!
		  $entry[pql_get_define("PQL_ATTR_KRB5PRINCIPALNAME")] = $_REQUEST["uid"] . "@" . pql_get_define("PQL_CONF_KRB5_REALM");
		
		// Encrypt and create the hash using the krb5PrincipalName attribute
		$entry[pql_get_define("PQL_ATTR_PASSWD")] = pql_password_hash($entry[pql_get_define("PQL_ATTR_KRB5PRINCIPALNAME")], $_REQUEST["pwscheme"]);
	} else {
		if($_REQUEST["crypted"])
		  // Password is already encrypted, prefix with choosen password scheme
		  $entry[pql_get_define("PQL_ATTR_PASSWD")] = $_REQUEST["pwscheme"] . $_REQUEST["password"];
		else
		  // Password isn't already encrypted, create the hash using the password value
		  $entry[pql_get_define("PQL_ATTR_PASSWD")] = pql_password_hash($_REQUEST["password"], $_REQUEST["pwscheme"]);
	}
}

// ------------------
if(($_REQUEST["account_type"] == "system") or ($_REQUEST["account_type"] == "shell")) {
	$entry[pql_get_define("PQL_ATTR_LOGINSHELL")] = $_REQUEST["loginshell"];
}

// ------------------
if(($_REQUEST["account_type"] == "system") or ($_REQUEST["account_type"] == "mail")) {
	// normal mailbox account
	
	if($_REQUEST["host"])
	  // If using 'qlc' -> the FQDN
	  $entry[pql_get_define("PQL_ATTR_MAILHOST")]   = $_REQUEST["host"];
	elseif($_REQUEST["userhost"])
	  // We've specified a host, but we never ticked the 'User defined' checkbox.
	  $entry[pql_get_define("PQL_ATTR_MAILHOST")]   = $_REQUEST["userhost"];
	
	$entry[pql_get_define("PQL_ATTR_MODE")] = "localdelivery";
	
	if(!$_REQUEST["maildirectory"])
	  $entry[pql_get_define("PQL_ATTR_MAILSTORE")] = user_generate_mailstore($_pql, $_REQUEST["email"],
																				  $_REQUEST["domain"], $entry,
																				  'user');
	else
	  $entry[pql_get_define("PQL_ATTR_MAILSTORE")] = $_REQUEST["maildirectory"];

	// Verify trailing slash!
    if(! ereg("/$", $entry[pql_get_define("PQL_ATTR_MAILSTORE")]))
	  $entry[pql_get_define("PQL_ATTR_MAILSTORE")] .= "/";
	
	// -----------------
	if($_REQUEST["account_type"] == "system") {
		// Add home directory to the entry array
		if(!$_REQUEST["homedirectory"]) {
			if(($_REQUEST["account_type"] == "mail") and !$_SESSION["ADVANCED_MODE"])
			  // It's a mail account, and we where/is not running in
			  // advanced mode. Use the maildirectory as homedirectory.
			  $entry[pql_get_define("PQL_ATTR_HOMEDIR")] = user_generate_mailstore($_pql, $_REQUEST["email"],
																				   $_REQUEST["domain"], $entry,
																				   'user');
			else
			  $entry[pql_get_define("PQL_ATTR_HOMEDIR")] = user_generate_homedir($_pql, $_REQUEST["email"],
																				 $_REQUEST["domain"], $entry,
																				 'user');
		} else
		  $entry[pql_get_define("PQL_ATTR_HOMEDIR")] = $_REQUEST["homedirectory"];

		// Verify trailing slash!
		if(! ereg("/$", $entry[pql_get_define("PQL_ATTR_HOMEDIR")]))
		  $entry[pql_get_define("PQL_ATTR_HOMEDIR")] .= "/";
	}
}

// ------------------
$default_quota = pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEQUOTA"));
if(!empty($default_quota)) {
	if($_SESSION["NEW_STYLE_QUOTA"]) {
		$tmp = pql_parse_quota($default_quota);
		
		$entry[pql_get_define("PQL_ATTR_QUOTA_SIZE")]  = $tmp["maxsize"];
		$entry[pql_get_define("PQL_ATTR_QUOTA_COUNT")] = $tmp["maxmails"];
	} else
	  $entry[pql_get_define("PQL_ATTR_QUOTA_VALUE")]   = $default_quota;
}

// ============================================================================================
// ============== ADD THE USER TO THE DATABASE

// pql_user_add() will first URL encode the value, THEN UTF8 encode it
// Hence, we must reverse this here ($_REQUEST["subbranch"] is already
// URL decoded).
$branch = pql_maybe_decode($_REQUEST["subbranch"]);
$DNs = pql_user_add($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["cn"], $entry, $_REQUEST["account_type"], $branch);
if($DNs[0]) {
	// TODO: DNs[1] (the group object) might still be empty -> failed to add it.
	
	// Now it's time to run the special adduser script if defined...
	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["rootdn"])) {
		// Setup the environment with the user details
		putenv("PQL_CONF_DOMAIN=$domain");
		putenv("PQL_CONF_WEBUSER=".posix_getuid());
		foreach($entry as $key => $e) {
			$key = "PQL_" . strtoupper($key);
			if($key != 'PQL_CONF_OBJECTCLASS')
			  putenv("$key=$e");
		}
		
		if(pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH") and 
		   pql_get_define("PQL_CONF_KRB5_REALM") and
		   pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL") and
		   pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER") and 
		   pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB")) {
			putenv("PQL_KADMIN_CMD=".pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH")."/kadmin");
			putenv("PQL_KADMIN_REALM=".pql_get_define("PQL_CONF_KRB5_REALM"));
			putenv("PQL_KADMIN_PRINC=".pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL"));
			putenv("PQL_KADMIN_SERVR=".pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER"));
			putenv("PQL_KADMIN_KEYTB=".pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB"));
		}
		
		// Execute the user add script (0 => show output)
		if(pql_execute(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["rootdn"]), 0)) {
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
	
	if(pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $_REQUEST["rootdn"])) {
		$link  = "user_sendmail.php?email=" . urlencode($_REQUEST["email"]) . "&";
		$link .= "rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&user=" . urlencode($DNs[0]) . "&rlnb=2&msg=$msg";
	} else {
		$link  = "user_detail.php?rootdn=".$url["rootdn"]."&";
		$link .= "domain=".$url["domain"]."&user=" . urlencode($DNs[0]) . "&rlnb=2&msg=$msg";
	}
	
	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["rootdn"])) {
?>
    <form action="<?=$link?>" method="post">
      <input type="submit" value="Continue">
    </form>
<?php
		die();
	} else {
		header("Location: " . pql_get_define("PQL_CONF_URI") . "$link");
	}
} else {
	$msg = urlencode($LANG->_('Failed to create the new user') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	$link = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&msg=$msg";
	header("Location: " . pql_get_define("PQL_CONF_URI") . "$link");
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
