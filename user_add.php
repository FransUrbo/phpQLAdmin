<?php
// add a user
// $Id: user_add.php,v 2.85 2004-03-11 18:13:32 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

$url["domain"]		= pql_format_urls($_REQUEST["domain"]);
$url["rootdn"]		= pql_format_urls($_REQUEST["rootdn"]);
$url["subbranch"]	= pql_format_urls($_REQUEST["subbranch"]);
$url["user"]		= pql_format_urls($_REQUEST["user"]);

// Get the organization name, or the DN if it's unset
$orgname = pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($_REQUEST["domain"]);
}

// check if domain exist
if(!pql_domain_exist($_pql, $_REQUEST["domain"])) {
	echo "Domain &quot;".$_REQUEST["domain"]."&quot; does not exists";
	exit();
}

// Get default domain values for this domain
$defaultdomain			= pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
$basehomedir			= pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEHOMEDIR"));
$basemaildir			= pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR"));
$maxusers				= pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"));
$additionaldomainname	= pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));

// {{{ Verify the input from the current page.  Autogen input for the next page.
// Check the input
$error = false; $error_text = array();
switch($_REQUEST["page_curr"]) {
  // {{{ case: "" (make sure a new user can be added.)
  case "":
	// ------------------------------------------------
	// Step 1: Selected account type (see how many users there can be)
	if($maxusers and !$_SESSION["ALLOW_BRANCH_CREATE"]) {
		if(count(pql_user_get($_pql->ldap_linkid, $_REQUEST["domain"])) >= $maxusers) {
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

	if($_REQUEST["page_next"] == "one") {
		// ------------------------------------------------
		// Step 2b: Autogenerate some stuff for the next form

		$attribs = array(pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
						 pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"));
		foreach($attribs as $attrib) {
			// Get default value
			$value = pql_domain_get_value($_pql, $_REQUEST["domain"], $attrib);
			$$attrib = pql_format_bool($value);
		}

		// Verify/Create uid - But only if we're referencing users with UID...
		if(empty($_REQUEST["uid"]) and $autocreateusername and function_exists('user_generate_uid') and 
		   (pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))) {
			// Generate the username
			$_REQUEST["uid"] = strtolower(user_generate_uid($_pql, $_REQUEST["surname"],
															$_REQUEST["name"], $email,
															$_REQUEST["domain"], $_REQUEST["account_type"]));
			
			// Check again. There should be a user name, either from the input
			// form OR from the user_generate_uid() function above...
			if(!$_REQUEST["uid"]) {
				$_REQUEST["page_next"] = "two";
				
				$error = true;
				$error_text["uid"] = $LANG->_('Missing');
			} else {
				if(preg_match("/[^a-z0-9\.@%_-]/i", $_REQUEST["uid"])) {
					$_REQUEST["page_next"] = "two";
					
					$error = true;
					$error_text["uid"] = $LANG->_('Invalid');
				}
			}
		}
		
		// Verify/Create email address
		if(empty($_REQUEST["email"]) and $autocreatemailaddress and function_exists('user_generate_email')) {
			// It's not supplied - generate one
			$_REQUEST["email"] = strtolower(user_generate_email($_pql, $_REQUEST["uid"], "", "",
																$_REQUEST["domain"], $_REQUEST["account_type"]));
			
			if(ereg(" ", $_REQUEST["email"]))
			  // Replace spaces with underscore
			  $_REQUEST["email"] = preg_replace(" ", "_", $_REQUEST["email"], -1);
		}
	}
	break;
	// }}}

  // {{{ case: one (validate user_add-details.inc)
  case "one":
	// ------------------------------------------------
	// Step 2a: Check user details - surname, name, email, account_type, account_status

	// Verify surname
    if($_REQUEST["surname"] == "") {
		$error = true;
		$error_text["surname"] = $LANG->_('Missing');
    }
	$user = $_REQUEST["surname"];
	
	// Verify lastname
    if($_REQUEST["name"] == "") {
		$error = true;
		$error_text["name"] = $LANG->_('Missing');
    }
	$user .= " " . $_REQUEST["name"];

	// Verify username
    if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_CN")
       and pql_user_exist($_pql->ldap_linkid, $_REQUEST["rootdn"], $user)) {
		$error = true;
		$error_text["username"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $user));
    }
	
	// First test if email is valid - must contain an '@'.
	if(! ereg("@", $_REQUEST["email"])) {
		if($_REQUEST["email_domain"])
		  $_REQUEST["email"] = $_REQUEST["email"] . "@" . $_REQUEST["email_domain"];
		else
		  $_REQUEST["email"] = $_REQUEST["email"] . "@" . $defaultdomain;
	}

	// Second test if email is valid
    if(!pql_check_email($_REQUEST["email"])) {
		$error = true;

		$error_text["email"] = $LANG->_('Invalid');
    }
	
	// It exists, it's valid. Does it already exists in the database?
    if($error_text["email"] == "" and pql_email_exists($_pql, $_REQUEST["email"])) {
		$error = true;
		$error_text["email"] = $LANG->_('Already exists');
    }
	
	// Verify the password
	if($_REQUEST["account_type"] != "forward") {
		// Only forward accounts is ok without password
		if($_REQUEST["password"] == "") {
			$error = true;
			$error_text["password"] = $LANG->_('Missing');
		}
		
		if(eregi("KERBEROS", $_REQUEST["pwscheme"])) {
			// Should be in the form: userid@DOMAIN.LTD
			// TODO: Is this regexp correct!?
			if(! preg_match("/^[a-zA-Z0-9]+[\._-a-z0-9]*[a-zA-Z0-9]+@[A-Z0-9][-A-Z0-9]+(\.[-A-Z0-9]+)+$/", $_REQUEST["password"])) {
				$error = true;
				$error_text["password"] = $LANG->_('Invalid');
			}
		} elseif(!$_REQUEST["crypted"]) {
			// A password in cleartext, NOT already encrypted
			if(preg_match("/[^a-z0-9]/i", $_REQUEST["password"])) {
				$error = true;
				$error_text["password"] = $LANG->_('Invalid');
			}
		}

		// Write the password scheme in a way the LDAP server will understand - within {SCHEME}.
		if($_REQUEST["pwscheme"]) {
			if(! eregi('\{', $_REQUEST["pwscheme"]))
			  $_REQUEST["pwscheme"] = '{'.$_REQUEST["pwscheme"];
			
			if(! eregi('\}', $_REQUEST["pwscheme"]))
			  $_REQUEST["pwscheme"] .= '}';
		}
	} else {
		// Forwarding accounts - make sure the forwarding mail address is ok
		if(!pql_check_email($_REQUEST["forwardingaddress"])) {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Invalid');
		}
		
		if($_REQUEST["forwardingaddress"] == "") {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Missing');
		}
	}
		
	// Check the mailHost attribute/value
	if($_REQUEST["account_type"] != "shell") {
		// Find MX (or QmailLDAP/Controls with locals=$email_domain)
		$mx = pql_get_mx($_REQUEST["email_domain"]);
		if(!$mx) {
			// There is no MX and no QmailLDAP/Controls with this
			// domain name in locals. Die!
			$_REQUEST["page_next"] = "two";
			
			$error = true;
			$error_text["userhost"] = pql_complete_constant($LANG->_('Sorry, I can\'t find any MX or any QmailLDAP/Controls object that listens to the domain <u>%domain%</u>.<br>You will have to specify one manually.'),
															array('domain' => pql_maybe_idna_decode($_REQUEST["email_domain"])));
		} else {
			// We got a MX or QmailLDAP/Controls object. Use it.
			$_REQUEST["userhost"][1] = $mx;
			$_REQUEST["host"] = "default";
		}
	}

	// Get default {home,mail} directory from the database.
	$attribs = array(pql_get_define("PQL_ATTR_BASEHOMEDIR"), pql_get_define("PQL_ATTR_BASEMAILDIR"));
	foreach($attribs as $attrib) {
		// Get default value
		$value = pql_domain_get_value($_pql, $_REQUEST["domain"], $attrib);
		$$attrib = $value;
	}

	// Generate the mail directory value
	if(!empty($basemaildir)) {
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")) {
			$_REQUEST["maildirectory"] = user_generate_mailstore($_pql, $_REQUEST["email"], $_REQUEST["domain"],
																 array(pql_get_define("PQL_ATTR_UID") => $_REQUEST["uid"]),
																 'user');
		} else {
			$_REQUEST["maildirectory"] = user_generate_mailstore($_pql, $_REQUEST["email"], $_REQUEST["domain"],
																 array(pql_get_define("PQL_ATTR_UID") => $_REQUEST["surname"]." ".$_REQUEST["name"]),
																 'user');
		}
		
		if($_REQUEST["maildirectory"]) {
			// Replace space(s) with underscore(s)
			$_REQUEST["maildirectory"] = preg_replace('/ /', '_', $_REQUEST["maildirectory"], -1);
		}
	} else {
		// Can't autogenerate!
		$error = true;
		$error_text["maildirectory"] = pql_complete_constant($LANG->_('Attribute <u>%what%</u> is missing. Can\'t autogenerate %type%.'),
															 array('what' => pql_get_define("PQL_ATTR_BASEMAILDIR"), 
																   'type' => 'Path to mailbox'));
	}

	// Generate the home directory value
	if(!empty($basehomedir)) {
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID")) {
			$_REQUEST["homedirectory"] = user_generate_homedir($_pql, $_REQUEST["email"], $_REQUEST["domain"],
															   array(pql_get_define("PQL_ATTR_UID") => $_REQUEST["uid"]),
															   'user');
		} else {
			$_REQUEST["homedirectory"] = user_generate_homedir($_pql, $_REQUEST["email"], $_REQUEST["domain"],
															   array(pql_get_define("PQL_ATTR_UID") => $_REQUEST["surname"]." ".$_REQUEST["name"]),
															   'user');
		}
		
		if($_REQUEST["homedirectory"]) {
			// Replace space(s) with underscore(s)
			$_REQUEST["homedirectory"] = preg_replace('/ /', '_', $_REQUEST["homedirectory"], -1);
		}
	} else {
		// Can't autogenerate!
		$error = true;
		$error_text["homedirectory"] = pql_complete_constant($LANG->_('Attribute <u>%what%</u> is missing. Can\'t autogenerate %type%'),
															 array('what' => pql_get_define("PQL_ATTR_BASEHOMEDIR"), 
																   'type' => 'Path to homedirectory'));
	}
	break;

	// }}}

  // {{{ case: two (user_add-additional.inc)
  case "two":
	// Step 3: Verify additional information (currently only mailhost)

	if(!$_REQUEST["host"] or !$_REQUEST["userhost"]) {
		// No host
		$error = true;

		if(!pql_get_define("PQL_CONF_CONTROL_USE"))
		  $error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') . ")";
		else
		  $error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('not using QmailLDAP/Controls') . ")";
	}
  // }}}
}
// }}}

// {{{ Process the next page ($page_next).
include("./header.html");
?>
  <span class="title1">
    <?php echo pql_complete_constant($LANG->_('Create user in domain %domain%'), array("domain" => $orgname)); ?>
<?php
if($_SESSION["ADVANCED_MODE"] && $_REQUEST["account_type"]) {
	if($_REQUEST["account_type"] == 'normal')
	  echo " - ".$LANG->_('Mail account');
	elseif($_REQUEST["account_type"] == 'system')
	  echo " - ".$LANG->_('System account');
	elseif($_REQUEST["account_type"] == 'shell')
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
switch(($error ? $_REQUEST["page_curr"] : $_REQUEST["page_next"])) {
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
// }}}

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
