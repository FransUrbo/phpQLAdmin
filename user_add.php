<?php
// add a user
// $Id: user_add.php,v 2.116.2.9 2005-02-22 09:20:52 turbo Exp $
//
// --------------- Pre-setup etc.

// {{{ Setup session etc
session_start();
require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_templates.inc");

$url["domain"]		= pql_format_urls($_REQUEST["domain"]);
$url["rootdn"]		= pql_format_urls($_REQUEST["rootdn"]);
$url["subbranch"]	= pql_format_urls($_REQUEST["subbranch"]);
$url["user"]		= pql_format_urls($_REQUEST["user"]);
// }}}

// {{{ Get the organization name, or the DN if it's unset
$orgname = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($_REQUEST["domain"]);
} elseif(is_array($orgname)) {
	$orgname = $orgname[0];
}
$_REQUEST["orgname"] = $orgname;
// }}}

// {{{ Check if domain exist
if(!pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], '(objectclass=*)', 'BASE')) {
	echo "Domain &quot;".$_REQUEST["domain"]."&quot; does not exists";
	exit();
}
// }}}

// {{{ Get default domain values for this domain
$defaultdomain			= pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
$maxusers				= pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"));
$additionaldomainname	= pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));

// Get the {home,mail} directory values
$attribs = array("basehomedir" => pql_get_define("PQL_ATTR_BASEHOMEDIR"),
				 "basemaildir" => pql_get_define("PQL_ATTR_BASEMAILDIR"));
foreach($attribs as $key => $attrib) {
  // Get default value
  $value = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib);
  if(!ereg('/$', $value))
	$value . '/';

  $$key = $value;
}
// }}}

// {{{ Get all objectclasses the LDAP server understand
$objectclasses_schema   = pql_get_subschema($_pql->ldap_linkid, 'objectclasses');
// }}}

// --------------- Verification and action(s).

// {{{ Verify the input from the current page.  Autogen input for the next page.
if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
  echo "page_curr: '".$_REQUEST["page_curr"]."'<br>";
  echo "page_next: '".$_REQUEST["page_next"]."'<br>";
}

// Check the input
$error = false; $error_text = array();
switch($_REQUEST["page_curr"]) {
  // {{{ '':    Make sure a new user can be added OR that we autogenerate stuff for the very first page)
  case "":
	if($_REQUEST["page_next"] == "one") {
		// {{{ Retreive the template definition
		$template = pql_get_template($_pql->ldap_linkid, $_REQUEST["template"]);
		// }}}

		// {{{ Retreive some stuff for autogeneration
		$attribs = array("autocreatemailaddress" => pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
						 "autocreateusername"	 => pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
						 "autocreatepassword"	 => pql_get_define("PQL_ATTR_AUTOCREATE_PASSWORD"));
		foreach($attribs as $key => $attrib) {
			// Get default value
			$value = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib);
			$$key  = pql_format_bool($value);
		}
		// }}}

		// {{{ Verify/Create uid
		// But only if:
		// 1. We haven't specified this ourself (Hmmm, how could we!? We have not been given a choice before this! TODO)
		// 2. We have set autoCreateUserName to TRUE for this branch/domain
		// 3. The function 'user_generate_uid()' is defined in include/config.inc.
		// 4. At least one of the objects we've choosen to use when creating users MAY or MUST the 'uid' attribute..
		if(empty($_REQUEST["uid"]) and pql_templates_check_attribute($_pql->ldap_linkid, $template, 'uid')) {
		  if(function_exists('user_generate_uid') and empty($_REQUEST["uid"]) and
			 (($autocreateusername and pql_templates_check_attribute($_pql->ldap_linkid, $template, 'uid', 'MUST'))
			  or
			  (pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == 'uid')))
		  {
			// Generate the username
			$_REQUEST["uid"] = strtolower(user_generate_uid($_pql, $_REQUEST["surname"],
															$_REQUEST["name"], $email,
															$_REQUEST["domain"], $_REQUEST["template"]));
			
			// Check again. There should be a user name, either from the input
			// form OR from the user_generate_uid() function above...
			if(empty($_REQUEST["uid"])) {
				$error = true;

				if(pql_templates_check_attribute($_pql->ldap_linkid, $template, 'uid', 'MUST'))
				  $error_text["uid"] = "&nbsp;&nbsp;".$LANG->_("Can't autogenerate, but this value is required.");
				else
				  $error_text["uid"] = $LANG->_("Can't autogenerate.");
			} else {
				if(preg_match("/[^a-z0-9\.@%_-]/i", $_REQUEST["uid"])) {
					$error = true;
					$error_text["uid"] = $LANG->_('Invalid');
				}
			}
		  }
		}
		// }}}
		
		// {{{ Verify/Create email address
		if(empty($_REQUEST["mail"]) and $autocreatemailaddress and function_exists('user_generate_email') and
		   pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_MAIL"), 'MUST'))
		{
			// It's not supplied - generate one
			$_REQUEST["mail"] = strtolower(user_generate_email($_pql, $_REQUEST["uid"], "", "",
																$_REQUEST["domain"], $_REQUEST["template"]));
			
			if(ereg(" ", $_REQUEST["mail"]))
			  // Replace spaces with underscore
			  $_REQUEST["mail"] = preg_replace(" ", "_", $_REQUEST["mail"], -1);
		}
		// }}}

		// {{{ Generate a password
		if(($_REQUEST["template"] != "group") and empty($_REQUEST["password"]) and
		   $autocreatepassword and function_exists('pql_generate_password') and
		   pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_PASSWD")))
		  $_REQUEST["password"] = pql_generate_password();
		// }}}
	} else {
	  // {{{ Step 1: Get user template definitions
	  $templates = pql_get_templates($_pql->ldap_linkid);
	  // }}}
	  
	  // {{{ Step 2: Make sure we have at least one user template
	  if(!is_array($templates)) {
		include($_SESSION["path"]."/header.html");
?>
  <span class="title1"><?=$LANG->_('No user templates')?></span>
  <br><br>
  <?=$LANG->_('You do not have any user templates defined. As of phpQLAdmin version 2.1.4, this is the new way of doing things. No more hard coded stuff! Please go to <a href="config_detail.php?view=template">the template editor</a> and define some...')?>
<?php
		die();
	  }
	  // }}}

	  // {{{ Step 3: Make sure that the attribute we're using for user reference(s) is availible
	  //             in at least ONE object class we've choosen to use when creating users...
	  $attrib_is_availible = 0;
	  for($i=0; $templates[$i]; $i++) {
		if(pql_templates_check_attribute($_pql->ldap_linkid, $templates[$i],
										 pql_get_define("PQL_CONF_REFERENCE_USERS_WITH",
														$_REQUEST["rootdn"])))
		{
		  $attrib_is_availible = 1;
		  last;
		}
	  }

	  if(!$attrib_is_availible) {
		include("./header.html");
?>
  <span class="title1"><?=pql_complete_constant($LANG->_('Attribute %attrib% is not availible.'), array("attrib" => pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])))?></span>
  <br><br>
  Sorry, but the attribute you're choosen to use to reference users 
  (<b><?=pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])?></b>)
  with isn't availible as a <i>MUST</i> nor a <i>MAY</i> in any of the defined
  user templates.
  <p>
  There's little point in continuing from here, please go to the
  <a href="config_detail.php?view=template">configuration</a>
  and setup a correct user template...
<?php
		die();
	  }
	  // }}}
	  
	  // {{{ Step 4: Selected account type (see how many users there can be)
	  if($maxusers and !$_SESSION["ALLOW_BRANCH_CREATE"]) {
		// Create a user search filter (only look for mail users - !?!?).
		$filter  = "(&(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*)(";
		$filter .= pql_get_define("PQL_ATTR_MAIL")."=*))";

		// Retreive all users in this branch/domain.
	    $users   = pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], $filter);
		if(count($users) >= $maxusers) {
			// We have reached the maximum amount of users.
			include($_SESSION["path"]."/header.html");
?>
  <span class="title1"><?=pql_complete_constant($LANG->_('Maximum amount (%max%) of users reached'), array("max" => $maxusers))?></span>
  <br><br>
  Sorry, but the maximum amount of users have been reached in this domain. You are not allowed
  to create more. Please talk to your administrator if you think this is wrong.
<?php
			die();
		}
	  }
	  // }}}
	}
	break;
	// }}}

  // {{{ 'one': Validate all user details specified in the very first page
  // Values such as: surname, name, email, template, account_status etc
  case "one":
	if($_REQUEST["template"] == "alias") {
		// {{{ Verify the 'alias' account type
		if($_REQUEST["source"] == "") {
			$error = true;
			$error_text["source"] = $LANG->_('Missing');
		}

		if($_REQUEST["destination"] == "") {
			$error = true;
			$error_text["destination"] = $LANG->_('Missing');
		}

		if(!$error) {
			// Construct the user/alias RDN
			$_REQUEST["user"]  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) . "=" . $_REQUEST["source"];
			$_REQUEST["user"] .= "," . $_REQUEST["subbranch"];

			// Now when we have the RDN, double check that it doesn't already exists!
			if(pql_get_dn($_pql->ldap_linkid, $_REQUEST["user"], '(objectclass=*)', 'BASE')) {
				$error = true;
				$error_text["source"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $_REQUEST["user"])) . "<br>";
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
				header("Location: " . $_SESSION["URI"] . "$link");
			}
		}
		// }}}
	} else {
		// {{{ Verify all account types EXEPT 'alias'
		// ------------------------

		// {{{ Verify surname
		if(empty($_REQUEST["surname"]) and pql_templates_check_attribute($_pql->ldap_linkid, $template, 'sn', 'MUST')) {
			$error = true;
			$error_text["surname"] = $LANG->_('Missing');
		}
		$user = $_REQUEST["surname"];
		// }}}

		// {{{ Verify lastname
		if(empty($_REQUEST["name"]) and pql_templates_check_attribute($_pql->ldap_linkid, $template, 'sn', 'MUST')) {
			$error = true;
			$error_text["name"] = $LANG->_('Missing');
		}
		$user .= " " . $_REQUEST["name"];
		// }}}

		// {{{ Verify username
		$filter = "(&(objectclass=*)(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=$user))";
		if(pql_templates_check_attribute($_pql->ldap_linkid, $template, 'sn') and
		   (pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_CN")) and
		   pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], $filter, 'BASE'))
		{
			$error = true;
			$error_text["username"] = pql_complete_constant($LANG->_('User %user% already exists'), array("user" => $user));
		}
		// }}}

		// {{{ Verify (and/or generate) the mail address
		// If email is set and allowed.
		// or:
		// If email isn't set but is required.
		if(($_REQUEST["mail"] and
			pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_MAIL")))
		   or
		   (empty($_REQUEST["mail"]) and
			pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_MAIL"), 'MUST')))
		{
		  if(!ereg("@", $_REQUEST["mail"])) {
			if($_REQUEST["email_domain"])
			  $_REQUEST["mail"] = $_REQUEST["mail"] . "@" . $_REQUEST["email_domain"];
			else
			  $_REQUEST["mail"] = $_REQUEST["mail"] . "@" . $defaultdomain;
			
			if(!pql_check_email($_REQUEST["mail"])) {
			  $error = true;
			  $error_text["mail"] = $LANG->_('Invalid');
			}
		  }
		
		  // It exists, it's valid. Does it already exists in the database?
		  if($error_text["mail"] == "" and pql_email_exists($_pql, $_REQUEST["mail"])) {
			$error = true;
			$error_text["mail"] = pql_complete_constant($LANG->_('Mail address %address% already exists'),
														 array("address" => '<i>'.$_REQUEST["mail"].'</i>'));
			unset($_REQUEST["mail"]);
		  } else {
			// The mail address is perfectly ok, but if user specify an exact email address (which most
			// likley don't match the email_domain we must change the email_domain request value for
			// the rest of the checks to work correctly.
			$tmp = split('@', $_REQUEST["mail"]);
			$_REQUEST["email_domain"] = $tmp[1];
		  }
		}
		// }}}

		// {{{ Verify (and/or generate) the password
		// If password is set and allowed.
		// or:
		// If password isn't set but required.
		// or:
		// If password isn't set, we have been asked to autogenerate one AND it's allowed.
		if(($_REQUEST["password"] and
			pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_PASSWD")))
		   or
		   (empty($_REQUEST["password"]) and
            pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_PASSWD"), 'MUST'))
		   or
		   (empty($_REQUEST["password"]) and ($_REQUEST["autogenerate"] == "on") and
			pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_PASSWD"))))
		{
			// Only forward and group accounts is ok without password
			if(empty($_REQUEST["password"])) {
				if(empty($_REQUEST["autogenerate"])) {
					$error = true;
					$error_text["password"] = $LANG->_('Missing');
				} elseif(function_exists('pql_generate_password')) {
					// Autogenerate password
					$_REQUEST["password"] = pql_generate_password();
				}
			}
			
			if(eregi("KERBEROS", $_REQUEST["pwscheme"])) {
				// Should be in the form: userid@DOMAIN.LTD
				// TODO: Is this regexp correct!?
				if(! preg_match("/^[a-zA-Z0-9]+[\._-a-z0-9]*[a-zA-Z0-9]+@[A-Z0-9][-A-Z0-9]+(\.[-A-Z0-9]+)+$/", $_REQUEST["password"])) {
					$error = true;
					$error_text["password"] = $LANG->_('Invalid');
				}
			} elseif(empty($_REQUEST["crypted"])) {
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
		}
		// }}}

		// {{{ Verify mail forwarding address
		// If it's a forwarding accounts (allowing 'mailForwardingAddress') make sure the forwarding mail address is ok
		if(pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_FORWARDS"), "MUST")) {
			if(!pql_check_email($_REQUEST["forwardingaddress"])) {
				$error = true;
				$error_text["forwardingaddress"] = $LANG->_('Invalid');
			}
			
			if($_REQUEST["forwardingaddress"] == "") {
				$error = true;
				$error_text["forwardingaddress"] = $LANG->_('Missing');
			}
		}
		// }}}

		// {{{ Generate the mailHost attribute/value
		if($_REQUEST["mail"] and
		   pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_MAILHOST")))
		{
			// Find MX (or QmailLDAP/Controls with locals=$email_domain)
			$mx = pql_get_mx($_REQUEST["email_domain"]);
			if(!$mx) {
				if(!$error)
				  // Only set this to 'mx' if it's not already set.
				  // This so that the switch at the bottom won't break.
				  $error = "mx";

				$error_text["userhost"] = pql_complete_constant($LANG->_('Sorry, I can\'t find any MX or any QmailLDAP/Controls object that listens to the domain <u>%domain%</u>.<br>You will have to specify one manually.'),
																array('domain' => pql_maybe_idna_decode($_REQUEST["email_domain"])));
			} else
			  // We got a MX or QmailLDAP/Controls object. Use it.
			  $_REQUEST["userhost"] = $mx;
		}
		// }}}

		// {{{ Generate the mail directory value
		if($_REQUEST["mail"] and
		   pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_MAILSTORE")))
		{
		  if(!empty($basemaildir)) {
			if(function_exists("user_generate_mailstore")) {
			  if((pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))
				 and $_REQUEST["uid"])
				$reference = $_REQUEST["uid"];
			  elseif($_REQUEST["surname"] and $_REQUEST["name"])
				$reference = $_REQUEST["surname"]." ".$_REQUEST["name"];
			  elseif($_REQUEST["surname"])
				$reference = $_REQUEST["surname"];
			  elseif($_REQUEST["name"])
				$reference = $_REQUEST["name"];
			  elseif($_REQUEST["mail"]) {
				$reference = split('@', $_REQUEST["mail"]);
				$reference = $reference[0];
			  }
			  
			  $_REQUEST["maildirectory"] = user_generate_mailstore($_pql, $_REQUEST["mail"], $_REQUEST["domain"],
																   array(pql_get_define("PQL_ATTR_UID") => $reference),
																   'user');
			} else {
			  // Function user_generate_mailstore() doesn't exists but we have a base mail directory.
			  // Try creating the mail directory manually, using the username.
			  
			  if(pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["rootdn"])) {
				// Absolute path is ok - create 'baseMailDir/username/'
				$ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]);
				if($_REQUEST[$ref])
				  $_REQUEST["maildirectory"] = $basemaildir.$_REQUEST[$ref]."/";
			  } else
				// We're not allowing an absolute path - don't use the baseMailDir.
				$_REQUEST["maildirectory"] = $_REQUEST["uid"]."/";
			}
			
			if($_REQUEST["maildirectory"])
			  // Replace space(s) with underscore(s)
			  $_REQUEST["maildirectory"] = preg_replace('/ /', '_', $_REQUEST["maildirectory"], -1);
		  } else {
			// Can't autogenerate!
			$error_text["maildirectory"] = pql_complete_constant($LANG->_('Attribute <u>%what%</u> is missing. Can\'t autogenerate %type%.'),
																 array('what' => pql_get_define("PQL_ATTR_BASEMAILDIR"), 
																	   'type' => 'Path to mailbox'));
		  }
		}
		// }}}

		// {{{ Generate the home directory value
		if(pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_HOMEDIR"), 'MUST')) {
		  if(!empty($basehomedir)) {
			if(function_exists("user_generate_homedir")) {
			  if((pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))
				 and $_REQUEST["uid"])
				$reference = $_REQUEST["uid"];
			  elseif($_REQUEST["surname"] and $_REQUEST["name"])
				$reference = $_REQUEST["surname"]." ".$_REQUEST["name"];
			  elseif($_REQUEST["surname"])
				$reference = $_REQUEST["surname"];
			  elseif($_REQUEST["name"])
				$reference = $_REQUEST["name"];
			  elseif($_REQUEST["mail"]) {
				$reference = split('@', $_REQUEST["mail"]);
				$reference = $reference[0];
			  }
			  
			  $_REQUEST["homedirectory"] = user_generate_homedir($_pql, $_REQUEST["mail"], $_REQUEST["domain"],
																 array(pql_get_define("PQL_ATTR_UID") => $reference),
																 'user');
			} else {
			  // Function user_generate_homedir() doesn't exists but we have a base home directory.
			  // Try creating the home directory manually, using the username.
			  
			  if(pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["rootdn"])) {
				// Absolute path is ok - create 'baseHomeDir/username/'
				$ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]);
				if($_REQUEST[$ref])
				  $_REQUEST["homedirectory"] = $basehomedir.$_REQUEST[$ref]."/";
			  } else
				// We're not allowing an absolute path - don't use the baseHomeDir.
				$_REQUEST["homedirectory"] = $_REQUEST["uid"]."/";
			}
			
			if($_REQUEST["homedirectory"]) {
			  // Replace space(s) with underscore(s)
			  $_REQUEST["homedirectory"] = preg_replace('/ /', '_', $_REQUEST["homedirectory"], -1);
			}
		  } else {
			// Can't autogenerate!
			$error_text["homedirectory"] = pql_complete_constant($LANG->_('Attribute <u>%what%</u> is missing. Can\'t autogenerate %type%'),
																 array('what' => pql_get_define("PQL_ATTR_BASEHOMEDIR"), 
																	   'type' => 'Path to homedirectory'));
		  }
		}
		// }}}

		// {{{ If we have an error, what page should come after this?
		if($error and !empty($error_text)) {
			if(ereg("mx", $error)) {
				// This is the only case where we really SHOULD go to the next page - MX problems.
				$_REQUEST["page_next"] = 'two';
			} else {
				// Redisplay the current page
				$_REQUEST["page_next"] = 'one';
			}
		}
		// }}}
		// }}}
	}
	break;
	// }}}

  // {{{ 'two': Tripplecheck the mail host value
  case "two":
	if($_REQUEST["mail"] and ($_REQUEST["page_next"] == 'save') and !isset($_REQUEST["host"]) and
	   pql_templates_check_attribute($_pql->ldap_linkid, $template, pql_get_define("PQL_ATTR_MAILHOST")))
	{
	  $error = true;
	  $error_text["userhost"] = $LANG->_('Missing');
	  $_REQUEST["page_next"] = 'two';
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
include($_SESSION["path"]."/header.html");
?>
  <span class="title1">
    <?php echo pql_complete_constant($LANG->_('Create account in domain %domain%'),
									 array("domain" => $_REQUEST["orgname"])); ?>
<?php
if($_SESSION["ADVANCED_MODE"] && is_array($template)) {
  echo ': '.$template["short"];
} elseif(!is_array($template)) {
  // Retreive template we're using
  $template = pql_get_template($_pql->ldap_linkid, $_REQUEST["template"]);
  echo ': '.$template["short"];
} else {
  echo '.';
}
?>
  </span>

  <br><br>

<?php
// ------------------------------------------------
// Select next form to display using 'page_next'.
// This will be set correctly above if there's
// an error.
if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
  echo "Request array: "; printr($_REQUEST);
  echo "Error array: "; printr($error_text);
}

switch($_REQUEST["page_next"]) {
  case "":
	// Step 1 - Choose account properties (type of account)
	include("./tables/user_add-properties.inc");
	break;
	
  case "one":
	// Step 2 - Choose user details (name, password etc)
	if($_REQUEST["template"] == "alias")
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
	if($_REQUEST["page_curr"] and $_REQUEST["autogenerate"] and empty($_REQUEST["password_shown"]))
	  include("./tables/user_add-show_password.inc");
	else
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
