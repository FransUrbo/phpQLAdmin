<?php
// add a user
// user_add.php,v 1.5 2002/12/13 13:58:04 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// Make sure we can have a ' in branch (also affects the user DN).
$user   = eregi_replace("\\\'", "'", $user);
$domain = eregi_replace("\\\'", "'", $domain);

// Look for a URL encoded '=' (%3D). If one isn't found, encode the DN
// These variables ISN'T encoded "the first time", but they are after
// the attribute_print_form() have been executed, so we don't want to
// encode them twice!
if(! ereg("%3D", $rootdn)) {
	// URL encode namingContexts
	$rootdn = urlencode($rootdn);
}
if(! ereg("%3D", $domain)) {
	// .. and/or domain DN
	$domain = urlencode($domain);
}

// Get the organization name, or the DN if it's unset
$orgname = pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($domain);
}

// check if domain exist
if(!pql_domain_exist($_pql, $domain)) {
	echo "Domain &quot;$domain&quot; does not exists";
	exit();
}

// Get default domain values for this domain
$defaultdomain			= pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));
$basehomedir			= pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEHOMEDIR"));
$basemaildir			= pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR"));
$maxusers				= pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_MAXIMUMDOMAINUSERS"));
$additionaldomainname	= pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"));

// check formdata

// ------------------------------------------------
// Page 1: surname, name, email, account_type, account_status
if($submit == "") {
	// Before continuing, let's see how many users there can be
	if($maxusers) {
		if(count(pql_get_user($_pql->ldap_linkid, $domain)) >= $maxusers) {
			// We have reached the maximum amount of users.
			include("./header.html");
?>
  <span class="title1"><?=$LANG->_('Maximum amount of users reached')?></span>
  <br><br>
  Sorry, but the maximum amount of users have been reached in this domain. You are not allowed
  to create more. Please talk to your administrator if you think this is wrong.
<?php
			die();
		}
	}

    $error = false;
    $error_text = array();

    $user = $surname . " " . $name;
    if($error == false
       and pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_CN")
       and pql_user_exist($_pql->ldap_linkid, $user)) {
		$error = true;
		$error_text["username"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $user));
		$error_text["name"]		= $LANG->_('Already exists');
		$error_text["surname"]	= $LANG->_('Already exists');
    }
	
    if($email == "") {
		$error = true;
		$error_text["email"] = $LANG->_('Missing');
    }

    if(!check_email($email)){
		$error = true;
		$error_text["email"] = $LANG->_('Invalid');
    }
	
    if($error_text["email"] == "" and pql_email_exists($_pql, $email)) {
		$error = true;
		$error_text["email"] = $LANG->_('Already exists');
    }
	
    // if an error occured, set displaying form to '' (initial display)
    if($error == true) {
		$submit = "";
    } else {
		$error_text = array();
    }
}

// ------------------------------------------------
// Page 2: uid, password, host, quota, userhost, quota_user
if($submit == "two") {
	$error_text = array();
	$error = false;

	// Verify/Create email address
	if(!$email and function_exists('user_generate_email')) {
		$email = strtolower(user_generate_email($_pql, $surname, $name, $defaultdomain, $domain, $account_type));

		// Replace spaces with underscore
		$email = preg_replace(" ", "_", $email, -1);

		// Check again. There must be a email address.
		if(!$email) {
			$submit = "one";
			$error = true;
			$error_text["email"] = $LANG->_('Missing');
		}
	} else {
		// Build the COMPLETE email address
		if(! ereg("@", $email)) {
			if($email_domain)
			  $email = $email . "@" . $email_domain;
			else
			  $email = $email . "@" . $defaultdomain;
		}
	}

	// Verify/Create uid - But only if we're referencing users with UID...
	if(function_exists('user_generate_uid') and
	   pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
		if(!$uid) {
			$uid = strtolower(user_generate_uid($_pql, $surname, $name, $email, $domain, $account_type));
			
			// Check again. There should be a user name, either from the input
			// form OR from the user_generate_uid() function above...
			if(!$uid) {
				$submit = "one";
				$error = true;
				$error_text["uid"] = $LANG->_('Missing');
			} else {
				if(preg_match("/[^a-z0-9\.@%_-]/i", $uid)) {
					$submit = "one";
					$error = true;
					$error_text["uid"] = $LANG->_('Invalid');
				}
			}
		} else {
			$error = true;
			$error_text["uid"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') . ")";
		}
	}
	
	// Check the mailHost attribute/value
	if(is_array($userhost)) {
		if($host != "default")
		  if(!preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$userhost[1])) {
			  $error_text["userhost"] = $LANG->_('Invalid');
			  $error = true;
		  }
	} elseif($userhost) {
		if(!preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$userhost)) {
			$error_text["userhost"] = $LANG->_('Invalid');
			$error = true;
		}
	} elseif($account_type != "shell") {
		// We're saving, but we don't have a mailHost value!
		
		// Get the primary email address domain name
		if(ereg("@", $email))
		  $domainname = split('@', $email);
		
		// Initiate a connection to the QmailLDAP/Controls DN
		$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
		
		if($_pql_control->ldap_linkid) {
			// Find MX (or QmailLDAP/Controls with locals=$domainname)
			$mx = pql_get_mx($_pql_control->ldap_linkid, $domainname[1]);
			if(!$mx[1]) {
				// There is no MX and no QmailLDAP/Controls with this
				// domain name in locals. Die!
				$submit = "two";
				
				$error = true;
				$error_text["userhost"] = "Sorry, I can't find any MX or any QmailLDAP/Controls object that listens to this domain - <u>".$domainname[1]."</u><br>You will have to specify one manually.";
			} else {
				// We got a MX or QmailLDAP/Controls object. Use it.
				$userhost[1] = $mx[1];
				$host = "default";
			}
		}
	}

	// No host
	if(!$host or !$userhost) {
		$error = true;

		if($_pql_control->ldap_linkid) {
			$error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') . ")";
		} else {
			$error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('not using QmailLDAP/Controls') . ")";
		}
	}

	// Check/Create the mail directory attribute/value
	if(!$maildirectory) {
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
			$maildirectory = user_generate_mailstore($_pql, $email, $domain,
													 array(pql_get_define("PQL_GLOB_ATTR_UID") => $uid),
													 'user');
		} else {
			$maildirectory = user_generate_mailstore($_pql, $email, $domain,
													 array(pql_get_define("PQL_GLOB_ATTR_UID") => $surname." ".$name),
													 'user');
		}

		if($maildirectory) {
			// Replace space(s) with underscore(s)
			$maildirectory = preg_replace('/ /', '_', $maildirectory, -1);
		}
	}

    if($surname == "") {
		$error = true;
		$error_text["surname"] = $LANG->_('Missing');
    }
	
    if($name == "") {
		$error = true;
		$error_text["name"] = $LANG->_('Missing');
    }
}

// ------------------------------------------------
// Page 3:
if ($submit == "save") {
	// Verify/Create email address
	if(!$email and function_exists('user_generate_email')) {
		$email = strtolower(user_generate_email($_pql, $surname, $name, $defaultdomain, $domain, $account_type));

		// Check again. There must be a email address.
		if(!$email) {
			$submit = "one";
			$error = true;
			$error_text["email"] = $LANG->_('Missing');
		}
	} else {
		// Build the COMPLETE email address
		if(! ereg("@", $email)) {
			if($email_domain)
			  $email = $email . "@" . $email_domain;
			else
			  $email = $email . "@" . $defaultdomain;
		}
	}

	// Verify/Create uid - But only if we're referencing users with UID...
	if(function_exists('user_generate_uid') and
	   pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
		if(!$uid) {
			$uid = strtolower(user_generate_uid($_pql, $surname, $name, $email, $domain, $account_type));
			
			// Check again. There should be a user name, either from the input
			// form OR from the user_generate_uid() function above...
			if(!$uid) {
				$submit = "one";
				$error = true;
				$error_text["uid"] = $LANG->_('Missing');
			} else {
				if(preg_match("/[^a-z0-9\.@%_-]/i", $uid)) {
					$submit = "one";
					$error = true;
					$error_text["uid"] = $LANG->_('Invalid');
				}
			}
		} else {
			$error = true;
			$error_text["uid"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') .")";
		}
		
		if($error_text["uid"] == "" and pql_search_attribute($_pql->ldap_linkid, $domain,
															 pql_get_define("PQL_GLOB_ATTR_UID"),
															 $uid)) {
			$error = true;
			$error_text["uid"] = $LANG->_('Already exists');
		}
	}

	if($account_type == "normal" or $account_type == "system" or $account_type == "shell") {
		if($password == "") {
			$error = true;
			$error_text["password"] = $LANG->_('Missing');
		}
		
		if(eregi("KERBEROS", $pwscheme)) {
			// Should be in the form: userid@DOMAIN.LTD
			// TODO: Is this regexp correct!?
			if(! preg_match("/^[a-zA-Z0-9]+[\._-a-z0-9]*[a-zA-Z0-9]+@[A-Z0-9][-A-Z0-9]+(\.[-A-Z0-9]+)+$/", $password)) {
				$error = true;
				$error_text["password"] = $LANG->_('Invalid');
			}
		} elseif(!$crypted) {
			// A password in cleartext, NOT already encrypted
			if(preg_match("/[^a-z0-9]/i", $password)) {
				$error = true;
				$error_text["password"] = $LANG->_('Invalid');
			}
		}
	} elseif($account_type == "forward") {
		if(!check_email($forwardingaddress)) {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Invalid');
		}
		
		if($forwardingaddress == "") {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Missing');
		}
	}
		
	if(($error == true) and !$ADVANCED_MODE and !$submit)
	  $submit = "two";
}

if($pwscheme) {
	if(! eregi('\{', $pwscheme))
	  $pwscheme = '{'.$pwscheme;

	if(! eregi('\}', $pwscheme))
	  $pwscheme .= '}';
}

include("./header.html");
?>
  <span class="title1">
    <?php echo pql_complete_constant($LANG->_('Create user in domain %domain%'), array("domain" => $orgname)); ?>
<?php
if($ADVANCED_MODE && $account_type) {
	if($account_type == 'normal')
	  echo " - ".$LANG->_('Mail account');
	elseif($account_type == 'system')
	  echo " - ".$LANG->_('System account');
	elseif($account_type == 'shell')
	  echo " - ".$LANG->_('Shell account');
	else
	  echo " - ".$LANG->_('Forwarding account');
}
?>
  </span>

  <br><br>

<?php
// select form to display
switch($submit) {
	case "":
	  // First page (Account properties - type of account)
	  include("./tables/user_add-properties.inc");
      break;

    // ---------------------------------- NEXT PAGE: 2
    case "one":
	  // Second page (User details - name, password etc)
	  include("./tables/user_add-details.inc");
	  break;

    // ---------------------------------- NEXT PAGE: 3
	case "two":
	  // Third page
	  include("./tables/user_add-additional.inc");
	  break;

    // ---------------------------------- NEXT PAGE: 4
	case "save":
	  // code for saving the user
	  include("./tables/user_add-save.inc");
	  break;
} // end of switch($submit)

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
