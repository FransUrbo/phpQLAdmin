<?php
// add a user
// $Id: user_add.php,v 2.73 2003-11-15 12:19:01 turbo Exp $
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

// Check the input
$error = false; $error_text = array();
switch($page_curr) {
  case "":
	// ------------------------------------------------
	// Step 1: We're just starting (choose account type)

	// See how many users there can be
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
	break;

  case "one":
	// ------------------------------------------------
	// Step 2a: Process name, email etc

	// Verify surname
    if($surname == "") {
		$error = true;
		$error_text["surname"] = $LANG->_('Missing');
    }
	
	// Verify lastname
    if($name == "") {
		$error = true;
		$error_text["name"] = $LANG->_('Missing');
    }

	if($surname and $name)
	  $user = $surname . " " . $name;

	// Verify username
    if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_GLOB_ATTR_CN")
       and pql_user_exist($_pql->ldap_linkid, $user)) {
		$error = true;

		$error_text["username"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $user));
    }
	
	// Check if the email address is supplied
    if($email == "") {
		$error = true;
		$error_text["email"] = $LANG->_('Missing');
	} else {
		// It's not supplied, generate one email address
		if(!$email and function_exists('user_generate_email')) {
			$email = strtolower(user_generate_email($_pql, $surname, $name, $defaultdomain, $domain, $account_type));
			
			// Replace spaces with underscore
			$email = preg_replace(" ", "_", $email, -1);
			
			// Check again. There must be a email address.
			if(!$email) {
				$page_next = "one";
				
				$error = true;
				$error_text["email"] = $LANG->_('Not supplied and could not auto generate');
			}
		}
	}

	// First test if email is valid - must contain an '@'.
	if(! ereg("@", $email)) {
		// Build the COMPLETE email address
		if($email_domain)
		  $email = $email . "@" . $email_domain;
		else
		  $email = $email . "@" . $defaultdomain;
	}
	
	// Second test if email is valid
	if(!check_email($email)) {
		$error = true;
		$error_text["email"] = $LANG->_('Invalid');
	}

	// It exists, it's valid. Does it already exists?
	if($error_text["email"] == "" and pql_email_exists($_pql, $email)) {
		$error = true;
		$error_text["email"] = $LANG->_('Already exists');
	}

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
