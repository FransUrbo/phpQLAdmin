<?php
// $Id: user_add-save.inc,v 1.26 2004-11-13 12:49:00 turbo Exp $

// URL Decode the surname and lastname values which was/is encoded in tables/user_add-additional.inc.
$_REQUEST["surname"] = urldecode($_REQUEST["surname"]);
$_REQUEST["name"]    = urldecode($_REQUEST["name"]);

// ============================================================================================
// ============== PREPARE THE USERS ATTRIBUTE(S)

// {{{ Convert uid, email to lowercase
if($_REQUEST["uid"]) {
	$_REQUEST["uid"] = strtolower($_REQUEST["uid"]);
	$entry[pql_get_define("PQL_ATTR_UID")] = $_REQUEST["uid"];
}
// }}}

// {{{ Add Firstname and/or Lastname (if object classes allow etc)
$res = pql_check_attribute($objectclasses_schema, $objectclasses_included, pql_get_define("PQL_ATTR_SN"));
if(($_REQUEST["account_type"] != "mail") or $res[0]) {
	if($_REQUEST["account_type"] != "group") {
		// Firstname
		if($_REQUEST["surname"]) {
			$entry[pql_get_define("PQL_ATTR_GIVENNAME")]	= $_REQUEST["surname"];
		} elseif(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")) {
			$entry[pql_get_define("PQL_ATTR_GIVENNAME")]	= $_REQUEST["uid"];
		}
	}

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
// }}}

// {{{ Add mail and/or mail aliases (if object classes allow etc)
if($_REQUEST["account_type"] != 'shell') {
	if($_REQUEST["account_type"] != "group") {
		$res = pql_check_attribute($objectclasses_schema, $objectclasses_included, pql_get_define("PQL_ATTR_ISACTIVE"));
		if($res[0])
		  $entry[pql_get_define("PQL_ATTR_ISACTIVE")]	= $_REQUEST["account_status"];
	}

	// Cleanup the email address a bit
	if(! ereg("@", $_REQUEST["email"])) {
		$email_temp = strtolower($_REQUEST["email"]);

		// Build the COMPLETE email address
		if($_REQUEST["email_domain"])
		  $_REQUEST["email"] = $email_temp . "@" . $_REQUEST["email_domain"];
		else
		  $_REQUEST["email"] = $email_temp . "@" . $defaultdomain;
	} else {
		$email_temp = pql_strip_domain($_REQUEST["email"]);
		$email_temp = strtolower($email_temp);

		$_REQUEST["email"] = strtolower($_REQUEST["email"]);
	}

	// Add the primary mail address to the entry array
	$entry[pql_get_define("PQL_ATTR_MAIL")]	= $_REQUEST["email"];

	if($_REQUEST["account_type"] == "group") {
		// For group objects, we need a 'catchall' mail alias as well.
		if($_REQUEST["email_domain"]) {
			$entry[pql_get_define("PQL_ATTR_MAILALTERNATE")][]       = $email_temp . "-catchall@" . $_REQUEST["email_domain"];
		} else {
			$entry[pql_get_define("PQL_ATTR_MAILALTERNATE")][] = $email_temp . "-catchall@" . $defaultdomain;
		}
	}

	// Add the additional email addresses as well
	if($include_additional == 'on' and is_array($additionaldomainname)) {
		foreach($additionaldomainname as $additional)
		  $entry[pql_get_define("PQL_ATTR_MAILALTERNATE")][] = strtolower($email_temp . "@" . $additional);
	}
}
// }}}

if($_REQUEST["account_type"] == "forward") {
	// {{{ Add and convert forwardingaddress to lowercase
	$forwardingaddress = strtolower($forwardingaddress);
	
	// set attributes
	$entry[pql_get_define("PQL_ATTR_FORWARDS")]	= $forwardingaddress;
	// }}}

	// {{{ Setup the delivery modes
	if(!$_SESSION["NEW_STYLE_QUOTA"]) {
		// The new QmailLDAP have a changed 'deliveryMode' setup.
		// Don't add this automaticly.
		$entry[pql_get_define("PQL_ATTR_MODE")][]	= "forwardonly";
		$entry[pql_get_define("PQL_ATTR_MODE")][]	= "nombox";
	} else {
		$entry[pql_get_define("PQL_ATTR_MODE")][]	= "nolocal";
	}
	
	if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))
	  $entry[pql_get_define("PQL_ATTR_HOMEDIR")]= pql_get_define("PQL_CONF_VIRTUAL_BASEHOME");
	// }}}
} else {
	// Any other account than 'forwarding'.

	// {{{ Add [ug]id number, gecos etc to the entry array
	$res = pql_check_attribute($objectclasses_schema, $objectclasses_included, pql_get_define("PQL_ATTR_QMAILUID"));
	if(($_REQUEST["account_type"] != "mail") or $res[0]) {
		// We COULD end up here, EVEN if it's a mail account! So we have to tripple check again,
		// and again so that we only add the [ug]idNumber if it's supposed to be added...
		if($_REQUEST["account_type"] == "mail") {
			// It's a mail account but we've decided to use 'posixAccount' when creating users,
			// so use the predefined uid/gid number for this user...
			$virtual_uid_number = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $_REQUEST["rootdn"]);
			
			$entry[pql_get_define("PQL_ATTR_QMAILUID")] = $virtual_uid_number;
			$entry[pql_get_define("PQL_ATTR_QMAILGID")] = $virtual_uid_number;
		} else {
			// It's not a mail account
			
			// Get a free UserID number
			$uidnr = pql_get_next_ugidnumber($_pql, 'uid');
			if($uidnr > 0)
			  $entry[pql_get_define("PQL_ATTR_QMAILUID")] = $uidnr;
			
			// Get a free GroupID number
			$gidnr = pql_get_next_ugidnumber($_pql, 'gid');
			if($gidnr > 0)
			  $entry[pql_get_define("PQL_ATTR_QMAILGID")] = $gidnr;
		}
		
		// Gecos is needed to do PAM/NSS LDAP login 
		if($_REQUEST["surname"] && $_REQUEST["name"] &&
		   (pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")))
		{
			// Gecos is one attribute that can't be internationalized!
			// Decode in two steps: UTF8 decoding and then translate international chars to non-us chars
			$tmp1 = pql_maybe_decode($_REQUEST["surname"]); $tmp1 = pql_format_international($tmp1);
			$tmp2 = pql_maybe_decode($_REQUEST["name"]);    $tmp2 = pql_format_international($tmp2);

			$entry[pql_get_define("PQL_ATTR_GECOS")]  = $tmp1 . " " . $tmp2;
		}
	}
	// }}}
	
	// {{{ Add the password to the entry array
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
		if($_REQUEST["crypted"] and !$_REQUEST["autogenerate"])
		  // Password is already encrypted, prefix with choosen password scheme
		  $entry[pql_get_define("PQL_ATTR_PASSWD")] = $_REQUEST["pwscheme"] . $_REQUEST["password"];
		else
		  // Password isn't already encrypted, create the hash using the password value
		  $entry[pql_get_define("PQL_ATTR_PASSWD")] = pql_password_hash($_REQUEST["password"], $_REQUEST["pwscheme"]);
	}
	// }}}
}

// {{{ Add login shell if requested
if(($_REQUEST["account_type"] == "system") or ($_REQUEST["account_type"] == "shell")) {
	$entry[pql_get_define("PQL_ATTR_LOGINSHELL")] = $_REQUEST["loginshell"];
}
// }}}

// {{{ Add mail store directory if requested
if(($_REQUEST["account_type"] == "system") or
   ($_REQUEST["account_type"] == "mail") or
   ($_REQUEST["account_type"] == "group"))
{
	if(!$_REQUEST["maildirectory"])
	  $entry[pql_get_define("PQL_ATTR_MAILSTORE")] = user_generate_mailstore($_pql, $_REQUEST["email"],
																			 $_REQUEST["domain"], $entry,
																			 'user');
	else
	  $entry[pql_get_define("PQL_ATTR_MAILSTORE")] = $_REQUEST["maildirectory"];
	
	// Verify trailing slash!
    if(! ereg("/$", $entry[pql_get_define("PQL_ATTR_MAILSTORE")]))
	  $entry[pql_get_define("PQL_ATTR_MAILSTORE")] .= "/";
}
// }}}

// {{{ Add mail host, home directory and add local delivery to delivery mode if requested
if(($_REQUEST["account_type"] == "system") or ($_REQUEST["account_type"] == "mail")) {
	// normal mailbox account
	
	if($_REQUEST["host"])
	  // If using 'qlc' -> the FQDN
	  $entry[pql_get_define("PQL_ATTR_MAILHOST")]   = $_REQUEST["host"];
	elseif($_REQUEST["userhost"]) {
		// We've specified a host, but we never ticked the 'User defined' checkbox.
		if(is_array($_REQUEST["userhost"]) and $_REQUEST["userhost"]["dns"])
		  // We have a DNS entry. This is default!
		  $entry[pql_get_define("PQL_ATTR_MAILHOST")]   = $_REQUEST["userhost"]["dns"];
		elseif(is_array($_REQUEST["userhost"]) and $_REQUEST["userhost"]["qlc"][0])
		  // No DNS entry, but we have at least one QLC host. Take the first one we find...
		  $entry[pql_get_define("PQL_ATTR_MAILHOST")]   = $_REQUEST["userhost"]["qlc"][0];
		else
		  $entry[pql_get_define("PQL_ATTR_MAILHOST")]   = $_REQUEST["userhost"];
	}
	
	if(!$_SESSION["NEW_STYLE_QUOTA"])
	  // The new QmailLDAP have a changed 'deliveryMode' setup.
	  // Don't add this automaticly.
	  $entry[pql_get_define("PQL_ATTR_MODE")] = "localdelivery";
	
	// -----------------
	if(($_REQUEST["account_type"] == "system") or in_array('posixaccount', $objectclasses_included)) {
		// Add home directory to the entry array
		if(!$_REQUEST["homedirectory"]) {
			if(($_REQUEST["account_type"] == "mail") and !$_SESSION["ADVANCED_MODE"])
			  // It's a mail account, and we where/is not running in
			  // advanced mode. Use the maildirectory as homedirectory.
			  $entry[pql_get_define("PQL_ATTR_HOMEDIR")] = user_generate_homedir($_pql, $_REQUEST["email"],
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
} elseif($_REQUEST["account_type"] == "group") {
	// Because we require the 'posixAccount' ('uid' attrib) when creating a
	// group object, we just add the virtual homedirectory
	$entry[pql_get_define("PQL_ATTR_HOMEDIR")] = pql_get_define("PQL_CONF_VIRTUAL_BASEHOME");
}
// }}}

// {{{ Add the mail quota
$default_quota = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEQUOTA"));
if(!empty($default_quota)) {
	if($_SESSION["NEW_STYLE_QUOTA"]) {
		$tmp = pql_parse_quota($default_quota);
		
		$entry[pql_get_define("PQL_ATTR_QUOTA_SIZE")]  = $tmp["maxsize"];
		$entry[pql_get_define("PQL_ATTR_QUOTA_COUNT")] = $tmp["maxmails"];
	} else
	  $entry[pql_get_define("PQL_ATTR_QUOTA_VALUE")]   = $default_quota;
}
// }}}

// {{{ Create the RDN for the user
if($entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])])
	 $userdn = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) . "=" .
	 $entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])] . ",";
	 elseif($entry[pql_get_define("PQL_ATTR_UID")])
	 $userdn = pql_get_define("PQL_ATTR_UID")."=".$entry[pql_get_define("PQL_ATTR_UID")].",";
else {
	echo "I can't figure out how to name the RDN!<br>";
	echo "Both the user reference value (".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]);
	echo ") and the backup reference (".pql_get_define("PQL_ATTR_UID").") is empty...<br>";
	die("I have no idea how to continue from here...");
}

if($_REQUEST["subbranch"]) {
	$userdn .= urldecode($_REQUEST["subbranch"]);
} else { 
	$userdn .= urldecode($_REQUEST["domain"]);
}
$userdn = pql_maybe_encode($userdn);
// }}}

// ============================================================================================
// ============== ADD THE USER TO THE DATABASE

// pql_user_add() will first URL encode the value, THEN UTF8 encode it
// Hence, we must reverse this here ($_REQUEST["subbranch"] is already
// URL decoded).
$branch = pql_maybe_decode($_REQUEST["subbranch"]);
$DNs = pql_user_add($_pql->ldap_linkid, $_REQUEST["domain"], $userdn, $entry, $_REQUEST["account_type"], $branch);
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
