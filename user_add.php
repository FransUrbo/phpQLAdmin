<?php
// add a user
// $Id: user_add.php,v 2.80 2004-02-14 14:01:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

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
$orgname = pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($domain);
}

// check if domain exist
if(!pql_domain_exist($_pql, $domain)) {
	echo "Domain &quot;$domain&quot; does not exists";
	exit();
}

// Get default domain values for this domain
$defaultdomain			= pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));
$basehomedir			= pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEHOMEDIR"));
$basemaildir			= pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR"));
$maxusers				= pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_MAXIMUMDOMAINUSERS"));
$additionaldomainname	= pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"));

// Check the input
$error = false; $error_text = array();
switch($page_curr) {
  case "":
	// ------------------------------------------------
	// Step 1: Selected account type (see how many users there can be)
	if($maxusers and !$_SESSION["ALLOW_BRANCH_CREATE"]) {
		if(count(pql_user_get($_pql->ldap_linkid, $domain)) >= $maxusers) {
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
	break;

  case "one":
	// ------------------------------------------------
	// Step 2a: Check user details - surname, name, email, account_type, account_status

	// Verify surname
    if($surname == "") {
		$error = true;
		$error_text["surname"] = $LANG->_('Missing');
    }
	$user = $surname;
	
	// Verify lastname
    if($name == "") {
		$error = true;
		$error_text["name"] = $LANG->_('Missing');
    }
	$user .= " " . $name;

	// Verify username
    if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_CN")
       and pql_user_exist($_pql->ldap_linkid, $rootdn, $user)) {
		$error = true;
		$error_text["username"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $user));
    }
	
	// Check if the email address is supplied
    if($email == "") {
		// It's not supplied - generate one
		if(function_exists('user_generate_email')) {
			$email = strtolower(user_generate_email($_pql, $surname, $name, $defaultdomain, $domain, $account_type));
			
			// Replace spaces with underscore
			$email = preg_replace(" ", "_", $email, -1);
			
			// Check again. There must be a email address.
			if(!$email) {
				$page_next = "one";

				$error = true;
				$error_text["email"] = $LANG->_('Missing');
			}
		}
	}

	// First test if email is valid - must contain an '@'.
	if(! ereg("@", $email)) {
		if($email_domain)
		  $email = $email . "@" . $email_domain;
		else
		  $email = $email . "@" . $defaultdomain;
	}

	// Second test if email is valid
    if(!pql_check_email($email)) {
		$error = true;

		$error_text["email"] = $LANG->_('Invalid');
    }
	
	// It exists, it's valid. Does it already exists?
    if($error_text["email"] == "" and pql_email_exists($_pql, $email)) {
		$error = true;
		$error_text["email"] = $LANG->_('Already exists');
    }
	
	// Verify the password
	if($account_type != "forward") {
		// Only forward accounts is ok without password
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

		// Write the password scheme in a way the LDAP server will understand - within {SCHEME}.
		if($pwscheme) {
			if(! eregi('\{', $pwscheme))
			  $pwscheme = '{'.$pwscheme;
			
			if(! eregi('\}', $pwscheme))
			  $pwscheme .= '}';
		}
	} else {
		// Forwarding accounts - make sure the forwarding mail address is ok
		if(!pql_check_email($forwardingaddress)) {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Invalid');
		}
		
		if($forwardingaddress == "") {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Missing');
		}
	}
		
	// ------------------------------------------------
	// Step 2b: Autogenerate some stuff for the next form
	// Verify/Create uid - But only if we're referencing users with UID...
	if(!$uid) {
		if(function_exists('user_generate_uid') and
		   pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
			// Generate the username
			$uid = strtolower(user_generate_uid($_pql, $surname, $name, $email, $domain, $account_type));
			
			// Check again. There should be a user name, either from the input
			// form OR from the user_generate_uid() function above...
			if(!$uid) {
				$page_next = "two";

				$error = true;
				$error_text["uid"] = $LANG->_('Missing');
			} else {
				if(preg_match("/[^a-z0-9\.@%_-]/i", $uid)) {
					$page_next = "two";

					$error = true;
					$error_text["uid"] = $LANG->_('Invalid');
				}
			}
		} else {
			$error = true;
			$error_text["uid"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') . ")";
		}
	} else {
		$error = true;
		$error_text["uid"] = $LANG->_('Missing');
	}
	
	// Check the mailHost attribute/value
	if($account_type != "shell") {
		// Find MX (or QmailLDAP/Controls with locals=$email_domain)
		$mx = pql_get_mx($email_domain);
		if(!$mx) {
			// There is no MX and no QmailLDAP/Controls with this
			// domain name in locals. Die!
			$page_next = "two";
			
			$error = true;
			$error_text["userhost"] = pql_complete_constant($LANG->_('Sorry, I can\'t find any MX or any QmailLDAP/Controls object that listens to the domain <u>%domain%</u>.<br>You will have to specify one manually.'),
															array('domain' => pql_maybe_idna_decode($email_domain)));
		} else {
			// We got a MX or QmailLDAP/Controls object. Use it.
			$userhost[1] = $mx;
			$host = "default";
		}
	}

	// Generate the mail directory value
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

	// Generate the home directory value
	if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_UID")) {
		$homedirectory = user_generate_homedir($_pql, $email, $domain,
											   array(pql_get_define("PQL_GLOB_ATTR_UID") => $uid),
											   'user');
	} else {
		$homedirectory = user_generate_homedir($_pql, $email, $domain,
											   array(pql_get_define("PQL_GLOB_ATTR_UID") => $surname." ".$name),
											   'user');
	}

	if($homedirectory) {
		// Replace space(s) with underscore(s)
		$homedirectory = preg_replace('/ /', '_', $homedirectory, -1);
	}
	break;

  case "two":
	// Step 3: Verify additional information (currently only mailhost)

	if(!$host or !$userhost) {
		// No host
		$error = true;

		if(!pql_get_define("PQL_GLOB_CONTROL_USE"))
		  $error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') . ")";
		else
		  $error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('not using QmailLDAP/Controls') . ")";
	}
}

include("./header.html");
?>
  <span class="title1">
    <?php echo pql_complete_constant($LANG->_('Create user in domain %domain%'), array("domain" => $orgname)); ?>
<?php
if($_SESSION["ADVANCED_MODE"] && $account_type) {
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
// ------------------------------------------------
// Select next form to display
switch($page_next) {
  case "":
	// Step 1 - Choose account properties (type of account)
	include("./tables/user_add-properties.inc");
	break;
	
  case "one":
	// Step 2 - Choose user details (name, password etc)
	include("./tables/user_add-details.inc");
	break;
	
  case "two":
	// Step 3 - Choose additional information (mailhost, maildir etc)
	include("./tables/user_add-additional.inc");
	break;
	
  case "save":
	// Step 4 - Save the user into DB
	include("./tables/user_add-save.inc");
	break;
}

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
