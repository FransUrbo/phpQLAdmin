<?php
// add a user
// $Id: user_add.php,v 2.93 2004-03-27 12:30:48 turbo Exp $
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

		$attribs = array("autocreatemailaddress" => pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
						 "autocreateusername"	 => pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"));
		foreach($attribs as $key => $attrib) {
			// Get default value
			$value = pql_domain_get_value($_pql, $_REQUEST["domain"], $attrib);
			$$key = pql_format_bool($value);
		}

		// Verify/Create uid - But only if we're referencing users with UID...
		if(empty($_REQUEST["uid"]) and $autocreateusername and function_exists('user_generate_uid')) {
			// Generate the username
			$_REQUEST["uid"] = strtolower(user_generate_uid($_pql, $_REQUEST["surname"],
															$_REQUEST["name"], $email,
															$_REQUEST["domain"], $_REQUEST["account_type"]));
			
			// Check again. There should be a user name, either from the input
			// form OR from the user_generate_uid() function above...
			if(!$_REQUEST["uid"]) {
				$error = true;
				$error_text["uid"] = $LANG->_('Can\'t autogenerate.');
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

	if($_REQUEST["account_type"] == "alias") {
		// {{{ Verify the 'alias' account type
		if($_REQUEST["source"] == "") {
			$error = true;
			$error_text["source"] = $LANG->_('Missing');
		}

		if($_REQUEST["destination"] == "") {
			$error = true;
			$error_text["destination"] = $LANG->_('Missing');
		}

		if($error)
		  $_REQUEST["page_next"] = "one";
		else {
			// Construct the user/alias RDN
			$_REQUEST["user"]  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) . "=" . $_REQUEST["source"];
			$_REQUEST["user"] .= "," . $_REQUEST["subbranch"];

			// Now when we have the RDN, double check that it doesn't already exists!
			if(pql_user_exist($_pql->ldap_linkid, $_REQUEST["user"])) {
				$error = true;
				$error_text["source"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $_REQUEST["user"])) . "<br>";
				$_REQUEST["page_next"] = "one";
			} else {
				// The 'tables/user_add-save.inc' file isn't suitable for creating an alias. Do it here directly
				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'referral';
				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'extensibleObject';
				$entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])] = $_REQUEST["source"];
				
				// Use _this_ host reference in the referral value
				$host = split(';', $_SESSION["USER_HOST"]);
				$entry["ref"] = $host[0]."/".urlencode($_REQUEST["destination"]);
				
				// Create a LDIF object to print in case of error
				$LDIF = pql_create_ldif("pql_user_add - alias creation", $_REQUEST["user"], $entry);

				// Add the object to the database
				if(! ldap_add($_pql->ldap_linkid, $_REQUEST["user"], $entry)) {
					// failed to add user
					pql_format_error(1);
					die($LDIF);
				}
				
				$msg = urlencode($LANG->_('Successfully created the new user'));
				$link  = "user_detail.php?rootdn=".$url["rootdn"]."&";
				$link .= "domain=".$url["domain"]."&user=".$_REQUEST["destination"]."&msg=$msg&rlnb=2";
				header("Location: " . pql_get_define("PQL_CONF_URI") . "$link");
			}
		}
		// }}}
	} else {
		// {{{ Verify all account types EXEPT 'alias'
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
		   and pql_user_exist($_pql->ldap_linkid, $user, $_REQUEST["domain"], $_REQUEST["rootdn"])) {
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
			$error_text["email"] = pql_complete_constant($LANG->_('Mail address %address% already exists'),
														 array("address" => '<i>'.$_REQUEST["email"].'</i>'));
			unset($_REQUEST["email"]);
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
			} else
			  // We got a MX or QmailLDAP/Controls object. Use it.
			  $_REQUEST["userhost"] = $mx;
		}
		
		// Get default {home,mail} directory from the database.
		$attribs = array("basehomedir" => pql_get_define("PQL_ATTR_BASEHOMEDIR"),
						 "basemaildir" => pql_get_define("PQL_ATTR_BASEMAILDIR"));
		foreach($attribs as $attrib) {
			// Get default value
			$value = pql_domain_get_value($_pql, $_REQUEST["domain"], $attrib);
			$$key = $value;
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

		if($error) {
			$_REQUEST["page_next"] = 'one';
		}
		// }}}
	}
	break;
	// }}}
}

if(is_array($error_text)) {
	// Add a HTML newline to (all) the error text(s)
	foreach($error_text as $key => $msg)
	  $error_text[$key] .= "<br>";
}
// }}}

// {{{ Process the next page ($page_next).
include("./header.html");
?>
  <span class="title1">
    <?php echo pql_complete_constant($LANG->_('Create user in domain %domain%'), array("domain" => $orgname)); ?>
<?php
if($_SESSION["ADVANCED_MODE"] && $_REQUEST["account_type"]) {
	if($_REQUEST["account_type"] == 'mail')
	  echo " - ".$LANG->_('Mail account');
	elseif($_REQUEST["account_type"] == 'system')
	  echo " - ".$LANG->_('System account');
	elseif($_REQUEST["account_type"] == 'shell')
	  echo " - ".$LANG->_('Shell account');
	elseif($_REQUEST["account_type"] == 'alias')
	  echo " - ".$LANG->_('Alias object');
	else
	  echo " - ".$LANG->_('Forwarding account');
}
?>
  </span>

  <br><br>

<?php
// ------------------------------------------------
// Select next form to display
switch($_REQUEST["page_next"]) {
  case "":
	// Step 1 - Choose account properties (type of account)
	include("./tables/user_add-properties.inc");
	break;
	
  case "one":
	// Step 2 - Choose user details (name, password etc)
	if($_REQUEST["account_type"] == "alias")
	  include("./tables/user_add-alias.inc");
	else
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
