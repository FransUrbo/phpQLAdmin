<?php
// add a user
// user_add.php,v 1.5 2002/12/13 13:58:04 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

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
$orgname = pql_get_domain_value($_pql, $domain, 'o');
if(!$orgname) {
	$orgname = urldecode($domain);
}

// check if domain exist
if(!pql_domain_exist($_pql, $domain)) {
	echo "Domain &quot;$domain&quot; does not exists";
	exit();
}

// Get default domain values for this domain
$defaultdomain			= pql_get_domain_value($_pql, $domain, "defaultdomain");
$basehomedir			= pql_get_domain_value($_pql, $domain, "basehomedir");
$basemaildir			= pql_get_domain_value($_pql, $domain, "basemaildir");
$maxusers				= pql_get_domain_value($_pql, $domain, "maximumdomainusers");
$additionaldomainname	= pql_get_domain_value($_pql, $domain, "additionaldomainname");

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

    if($surname == ""){
		$error = true;
		$error_text["surname"] = $LANG->_('Missing');
    }
	
    if($name == ""){
		$error = true;
		$error_text["name"] = $LANG->_('Missing');
    }
	
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
    if(isset($submit) and $error == true) {
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

	// Verify/Create uid
	if(!$uid and function_exists('user_generate_uid')) {
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

	// Verify/Create uid
	if(!$uid and function_exists('user_generate_uid')) {
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
				$userhost = $mx[1];
				$host = "user";
			}
		}
	} elseif($account_type == "forward") {
		if(!check_email($forwardingaddress)){
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Invalid');
		}
		
		if($forwardingaddress == "") {
			$error = true;
			$error_text["forwardingaddress"] = $LANG->_('Missing');
		}
	}

	// No host
	if(!$host or !$userhost) {
		$error = true;
		$error_text["userhost"] = $LANG->_('Missing') . " (" . $LANG->_('can\'t autogenerate') . ")";
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
    // ---------------------------------- NEXT PAGE: 1
	case "":
	  // first
?>
<form action="<?=$PHP_SELF?>" method="post" accept-charset="UTF-8">
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Account properties')?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Type')?></td>

        <td>
          <select name="account_type">
            <option value="normal" SELECTED><?=$LANG->_('Mail account')?></option>
            <option value="system"><?=$LANG->_('System account')?></option>
            <option value="forward"><?=$LANG->_('Forward account')?></option>
            <option value="shell"><?=$LANG->_('Shell account')?></option>
          </select>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('mail'))); ?>: <?=$LANG->_('local mailbox, POP account')?>.
          </table>
        </td>
      </tr>

      <tr>
        <td></td>
        <td colspan="2">
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('system'))); ?>: <?=$LANG->_('loginshell, homedirectory')?>, <?=$LANG->_('local mailbox, POP account')?>.
          </table>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('forward'))); ?>: <?=$LANG->_('forward only, no local mailbox')?>.
          </table>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('shell'))); ?>: <?=$LANG->_('loginshell, homedirectory, no mail etc')?>.
          </table>
        </td>
      </tr>
    </th>
  </table>

  <input type="hidden" name="submit" value="one">
  <input type="hidden" name="domain" value="<?=$domain?>">
  <input type="hidden" name="rootdn" value="<?=$rootdn?>">
  <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
</form>
<?php
      break;
    // ---------------------------------- NEXT PAGE: 2
    case "one":
	  // second form
?>
  <form action="<?=$PHP_SELF?>" method="post" accept-charset="UTF-8">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if(!pql_get_define("PQL_CONF_CREATE_USERNAME", $domain) or
		 (pql_get_define("PQL_CONF_CREATE_USERNAME", $domain) and !function_exists('user_generate_uid')) or
		 ($ADVANCED_MODE)) {
?>
      <th colspan="3" align="left"><?=$LANG->_('Additional account properties')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Username')?></td>
          <td><?php echo format_error($error_text["uid"]); ?><input type="text" name="uid" value="<?=$uid?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('Numbers, letters and the following special chars: @, %, . (dot), _, -.\nIf left out, a username will be created automatically')?>.</td>
        </tr>

<?php	} else { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('username'))); ?>.</td>
        </tr>

<?php	}
    if($account_type != "forward") {
		// Get the default password scheme for branch
		$defaultpasswordscheme = pql_get_domain_value($_pql, $domain, "defaultpasswordscheme");

		if(!$defaultpasswordscheme or $ADVANCED_MODE)  {
			// We have no default password scheme - display forms for SYSTEM/MAIL account
?>
        <!-- Password schema -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Password encryption scheme')?></td>
          <td>
<?php		if(eregi(',', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn))) {
				// We got more than one password scheme...

				// Show each of the schemes as radio buttons
				$schemes = split(",", pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn));
				foreach($schemes as $scheme) {
?>
            <input type="radio" name="pwscheme" value="<?=$scheme?>" <?php if($defaultpasswordscheme == $scheme) { echo "CHECKED"; } ?>><?=$scheme?>
<?php			}
			} else { ?>
            Scheme: <b>{<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn); ?>}</b>
            <input type="hidden" name="pwscheme" value="<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn); ?>">
<?php		} ?>
          </td>
        </tr>
<?php	} else { ?>
        <input type="hidden" name="pwscheme" value="<?=$defaultpasswordscheme?>">
<?php	} ?>

        <!-- Password -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <!-- Crude hackery. Using type=password won't be so good if we're using {KERBEROS} -->
          <td>
            <?php echo format_error($error_text["password"]); ?>
            <input type="input" name="password">
            <input type="checkbox" name="crypted"><?=$LANG->_('Password is already encrypted')?>
            <?php echo format_error($error["pwscheme"]); ?>
          </td>
        </tr>

<?php	if(eregi('KERBEROS', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn))) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('If using {KERBEROS} as password scheme, make sure you include the correct REALM (principal@REALM.TLD)')?></td>
        </tr>
<?php	} ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('If you enter an already encrypted password, you must make sure that the password scheme you\'ve choosen is the correct one. Also, choose the checkbox \uPassword is already encrypted\U')?></td>
        </tr>
<?php
    } // account_type != forward

    if(($account_type == "system") or ($account_type == "shell")) {
		// display forms for SYSTEM account

		if($ADVANCED_MODE) {
			$shells = pql_get_valid_shells();
?>

        <!-- Loginshell -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Login shell')?></td>
          <td><?php echo format_error($error["loginshell"]); ?>

            <select name="loginshell">
              <option value="/bin/false" SELECTED>/bin/false</option>
<?php
			foreach($shells as $shell) {
?>
              <option value="<?=$shell?>"><?=$shell?></option>
<?php
			}
?>
            </select>
          </td>
        </tr>
<?php
		} // end if ADVANCED mode
	} // end if account type == system
?>

        <!-- sub branch -->
<?php
	if($ADVANCED_MODE) {
		$branches = pql_get_subbranch($_pql->ldap_linkid, $domain);
		if($branches[1]) {
			// More than one subbranch - show a select menu
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Put user in subbranch')?></td>
          <td>
            <select name="subbranch">
<?php
			for($i=0; $branches[$i]; $i++) {
?>
              <option value="<?=$branches[$i]?>"><?=$branches[$i]?></option>
<?php
			}
?>
            </select>
          </td>
        </tr>
<?php
		} else {
			// Only one subbranch
			if(pql_get_define("PQL_GLOB_SUBTREE_USERS")) {
?>
        <input type="hidden" name="subbranch" value="<?php echo pql_get_define("PQL_GLOB_SUBTREE_USERS").",".$domain?>">
<?php
			} else {
?>
        <input type="hidden" name="subbranch" value="<?=$domain?>">
<?php
			}
		}
	} else {
		if(pql_get_define("PQL_GLOB_SUBTREE_USERS")) {
?>
        <input type="hidden" name="subbranch" value="<?php echo pql_get_define("PQL_GLOB_SUBTREE_USERS").",".$domain?>">
<?php
		} else {
?>
        <input type="hidden" name="subbranch" value="<?=$domain?>">
<?php
		}
	} // end if ADVANCED mode
?>
      </th>
    </table>

    <br>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('User data')?></th>
        <!-- Firstname -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Surname')?></td>
          <td>
              <?php echo format_error($error_text["surname"]); ?>
              <input type="text" name="surname" value="<?=$surname?>">
          </td>
        </tr>

        <!-- Lastname -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Lastname')?></td>
          <td>
            <?php echo format_error($error_text["name"]); ?>
            <input type="text" name="name" value="<?=$name?>">
          </td>
        </tr>

<?php if($account_type != "shell") {
		if(!pql_get_define("PQL_CONF_CREATE_ADDRESS", $domain) or
		   (pql_get_define("PQL_CONF_CREATE_ADDRESS", $domain) and !function_exists('user_generate_email')) or
		   $ADVANCED_MODE or $error_text["email"]) {
?>
        <!-- Email address -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Main address')?></td>
          <td>
            <?php echo format_error($error_text["email"]); ?>
            <input type="text" name="email" value="<?=$email?>">
<?php 		if(is_array($additionaldomainname)) { ?>
            <b>@ <select name="email_domain"></b>
              <option value="<?=$defaultdomain?>"><?=$defaultdomain?></option>
<?php			foreach($additionaldomainname as $additional) { ?>
              <option value="<?=$additional?>"><?=$additional?></option>
<?php   		} ?>
            </select>
<?php 		} else { ?>
            <b>@<?=$defaultdomain?></b>
            <input type="hidden" name="email_domain" value="<?=$defaultdomain?>">
<?php 		} ?>
          </td>
        </tr>

<?php 		if(is_array($additionaldomainname)) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Alias')?></td>
          <td>
            <table>
              <td colspan="2"><input type="checkbox" name="include_additional" checked></td>
              <td>
                <?=$LANG->_('Include username in additional domains as alias')?>
              </td>
            </table>
          </td>
        </tr>

        <tr></tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('The email address and the username will be automatically converted to lowercase')?></td>
        </tr>
<?php 		}
		} else {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('email address'))); ?>.</td>
        </tr>

<?php	}
     }
?>
      </th>
    </table>
<?php
	if($account_type == "forward") {
		// display forms for FORWARDING account
?>

    <br>
  
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Forward mails to')?></th>
        <!-- Forwarding address -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Forwarding address')?></td>
          <td>
            <?php echo format_error($error_text["forwardingaddress"]); ?>
            <input type="text" name="forwardingaddress" value="<?=$forwardingaddress?>"><?=$LANG->_('Email')?>
          </td>
        </tr>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('You can add more forwarding address in the user details page')?></td>
        </tr>
      </th>
<?php
	} // account_type == forward
 ?>
    </table>
  
    <br>
  
<?php if($account_type != 'shell') { ?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Account properties')?></th>
        <!-- Account status -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Status')?></td>
          <td>
            <select name="account_status">
              <option value="active" SELECTED><?=$LANG->_('Active')?></option>
              <option value="nopop"><?=$LANG->_('POP locked')?></option>
              <option value="disabled"><?=$LANG->_('Locked')?></option>
            </select>
          </td>
        </tr>
      </th>
    </table>
  
    <br>
  
<?php } else { ?>
    <input type="hidden" name="account_status" value="">
<?php }

	if(($ADVANCED_MODE == 0) or ($account_type == "forward")) {
		// Go to save, no next form...
		if($ADVANCED_MODE == 0) {
			// Autocreate some values by using 'safe' (?) defaults
?>
    <input type="hidden" name="loginshell" value="/bin/false">
    <input type="hidden" name="homedirectory" value="">
    <input type="hidden" name="maildirectory" value="">
    <input type="hidden" name="host" value="default">
<?php	} ?>
    <input type="hidden" name="submit" value="save">
<?php
	} else {
		if($account_type != 'shell') {
?>
    <input type="hidden" name="submit" value="two">
<?php
		} else {
?>
    <input type="hidden" name="submit" value="save">
<?php
		}
	} // account_type == forward
?>
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="account_type" value="<?=$account_type?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
<?php
	if($ADVANCED_MODE == 1) {
?>
    <input type="submit" value="--&gt;&gt;">
<?php
	} else {
?>
    <input type="submit" value="<?=$LANG->_('Save')?>">
<?php
	}
?>
  </form>

<?php
		break;
    // ---------------------------------- NEXT PAGE: 3
	case "two":
		// third
?>

  <form action="<?=$PHP_SELF?>" method="post" accept-charset="UTF-8">
    <input type="hidden" name="surname" value="<?=$surname?>">
    <input type="hidden" name="name" value="<?=$name?>">
    <input type="hidden" name="email" value="<?=$email?>">
    <input type="hidden" name="uid" value="<?=$uid?>">
<?php if($account_type != "forward") { ?>
    <input type="hidden" name="password" value="<?=$password?>">
    <input type="hidden" name="pwscheme" value="<?=$pwscheme?>">
    <input type="hidden" name="crypted" value="<?=$crypted?>">
<?php } ?>
    <input type="hidden" name="subbranch" value="<?=$subbranch?>">
    <input type="hidden" name="email_domain" value="<?=$email_domain?>">
    <input type="hidden" name="include_additional" value="<?=$include_additional?>">

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Additional account properties')?></th>
<?php
	if($account_type == "normal" or $account_type == "system") {
		// display forms for SYSTEM/MAIL account(s)
?>
        <input type="hidden" name="loginshell" value="<?=$loginshell?>">

        <!-- MailMessageStore -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Path to mailbox')?></td>
          <td><?php
	  if(! ereg("/$", $basemaildir)) {
		  $maildirectory = $basemaildir . "/" . $uid;
	  } else {
		  $maildirectory = $basemaildir . $uid;
	  }
	  echo $maildirectory . "/";
	  ?></td>
        </tr>
<?php	if(($account_type == "normal") and $maildirectory) {
			$homedirectory = $maildirectory;
?>
        <input type="hidden" name="homedirectory" value="<?=$homedirectory?>">
<?php	} ?>

        <tr></tr>

        <!-- Mailhost -->
<?php
		if($error_text["userhost"]) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td><?php echo format_error($error_text["userhost"]); ?></td>
        </tr>
<?php
		}
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Mail server')?></td>
          <td>
<?php	if(is_array($userhost)) { ?>
            <input type="hidden" name="userhost" value="<?=$userhost[1]?>">
            <input type="radio" name="host" value="default" <?php if($userhost[0] and ($host != "user")){ echo "checked";}?>>
<?php	} else { ?>
            <input type="hidden" name="userhost" value="<?=$userhost?>">
            <input type="radio" name="host" value="default" <?php if($userhost and ($host == "user")){ echo "checked";}?>>
<?php	}
		echo "            " . $LANG->_('Standard (DNS entry)') . ": <b>";
		if(is_array($userhost))
		  echo $userhost[1];
		else
		  echo $userhost;

		echo "</b>\n";?>
          </td>
        </tr>
<?php
		if(!$userhost[0] and $userhost[1]) {
			// It's defined, but it comes from LDAP
?>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="host" value="user" checked><?=$LANG->_('QmailLDAP/Controls object')?>: <b><?=$userhost[1]?></b>
          </td>
        </tr>
<?php
		}
?>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="host" value="user" <?php if((!$userhost[0] and !$userhost[1]) or ($host == "user")){ echo "checked";}?>>
            <?=$LANG->_('User defined')?>&nbsp;<input type="text" name="userhost"><br>
          </td>
        </tr>

<?php
	} // end of if-else
?>
      </th>
    </table>
  
    <br>

<?php	if($account_type != "forward") {?>
    <input type="hidden" name="maildirectory" value="<?=$maildirectory?>">
<?php	}?>
    <input type="hidden" name="account_type" value="<?=$account_type?>">
    <input type="hidden" name="account_status" value="<?=$account_status?>">
    <input type="hidden" name="submit" value="save">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
		break;
    // ---------------------------------- NEXT PAGE: 4
	case "save":
		// code for saving the user

		// convert uid, email to lowercase
		$uid = strtolower($uid);

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
		$entry[pql_get_define("PQL_GLOB_ATTR_UID")]				= $uid;
		if($surname) {
			// Firstname
			$entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]	= $surname;
		} else {
			$entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]	= $uid;
		}

		if($name) {
			// Lastname
			$entry[pql_get_define("PQL_GLOB_ATTR_SN")]			= $name;
		} else {
			$entry[pql_get_define("PQL_GLOB_ATTR_SN")]			= $uid;
		}

        // ------------------
		if($account_type != 'shell') {
			$entry[pql_get_define("PQL_GLOB_ATTR_ISACTIVE")]	= $account_status;
			$entry[pql_get_define("PQL_GLOB_ATTR_MAIL")]		= $email;
			if($include_additional == 'on' and is_array($additionaldomainname)) {
				if(ereg("@", $email)) {
					$email_temp = split('@', $email);
					$email_temp = $email_temp[0];
				} else
				  $email_temp = $email;

				foreach($additionaldomainname as $additional)
				  $entry[pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE")][] = strtolower($email_temp . "@" . $additional);
			}
		}

        // ------------------
		if(($account_type == "system") or ($account_type == "shell"))
		  $entry[pql_get_define("PQL_GLOB_ATTR_LOGINSHELL")] = $loginshell;

        // ------------------
		if($account_type != "forward") {
			if($account_type != "normal") {
				// Get a free UserID number (which we also use for GroupID number)
				$uidnr = pql_get_next_uidnumber($_pql);
				if($uidnr > 0) {
					$entry[pql_get_define("PQL_GLOB_ATTR_QMAILUID")] = $uidnr;
					$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = $uidnr;
				}
			} else {
				// It's a 'Mail account'. Use the forwarding account uidNumber
				// for ALL 'mail only' accounts.
				$entry[pql_get_define("PQL_GLOB_ATTR_QMAILUID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
				$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
			}

			// Gecos is needed to do PAM/NSS LDAP login 
			if($surname && $name) {
				$entry["gecos"]							  = $surname . " " . $name;
			}

			// set attributes
			if(eregi('KERBEROS', $pwscheme)) {
				// We're using the {KERBEROS} password scheme. Special circumstances.
				// The userPassword and krb5PrincipalName needs to be set. This is 
				// automagicly created with the help of the username (uid) value and
				// the REALM name.
				if(eregi('@', $password))
				  // Use know what he/she's doing. We specified the full principal
				  // name directly in the password field! Use that as userPassword
				  // and krb5PrincipalName.
				  $entry["krb5PrincipalName"] = $password;
				else
				  // The password really IS a password!
				  $entry["krb5PrincipalName"] = $uid . "@" . pql_get_define("PQL_GLOB_KRB5_REALM");

				// Encrypt and create the hash using the krb5PrincipalName attribute
				$entry[pql_get_define("PQL_GLOB_ATTR_PASSWD")] = pql_password_hash($entry["krb5PrincipalName"], $pwscheme);
			} else {
				if($crypted)
				  // Password is already encrypted, prefix with choosen password scheme
				  $entry[pql_get_define("PQL_GLOB_ATTR_PASSWD")] = $pwscheme . $password;
				else
				  // Password isn't already encrypted, create the hash using the password value
				  $entry[pql_get_define("PQL_GLOB_ATTR_PASSWD")] = pql_password_hash($password, $pwscheme);
			}

			if(!$homedirectory) {
				if(($account_type == "normal") and !$ADVANCED_MODE)
				  // It's a mail account, and we where/is not running in
				  // advanced mode. Use the maildirectory as homedirectory.
				  $entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")] = user_generate_mailstore($_pql, $email, $domain, $entry);
				else
				  $entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")] = user_generate_homedir($_pql, $email, $domain, $entry);
			} else
			  $entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")] = $homedirectory;
		}

        // ------------------
		if(($account_type == "system") or ($account_type == "normal")) {
			// normal mailbox account

			if($userhost)
			  $entry[pql_get_define("PQL_GLOB_ATTR_MAILHOST")] = $userhost;
			else {
				$domainname = split('@', $entry[pql_get_define("PQL_GLOB_ATTR_MAIL")]);

				// Initiate a connection to the QmailLDAP/Controls DN
				$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

				// Find MX (or QmailLDAP/Controls with locals=$domainname)
				$mx = pql_get_mx($_pql_control->ldap_linkid, $domainname[1]);
				if(is_array($mx))
				  $entry[pql_get_define("PQL_GLOB_ATTR_MAILHOST")] = $mx[1];
			}

			$entry[pql_get_define("PQL_GLOB_ATTR_MODE")]     = "localdelivery";

			if(!$maildirectory)
			  $entry[pql_get_define("PQL_GLOB_ATTR_MAILSTORE")] = user_generate_mailstore($_pql, $email, $domain, $entry);
			else
			  $entry[pql_get_define("PQL_GLOB_ATTR_MAILSTORE")] = $maildirectory;
		} elseif($account_type != "shell") {
			// forwardonly account

			// convert forwardingaddress to lowercase
			$forwardingaddress = strtolower($forwardingaddress);

			// set attributes
			$entry[pql_get_define("PQL_GLOB_ATTR_FORWARDS")]	= $forwardingaddress;
			$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][]		= "forwardonly";
			$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][]		= "nombox";

			// Even forward accounts need UIDNumber! (?!?)
			$entry[pql_get_define("PQL_GLOB_ATTR_QMAILUID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
			$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $rootdn);
			$entry[pql_get_define("PQL_GLOB_ATTR_HOMEDIR")]  = "/tmp";
		}

        // ------------------
		if($surname && $name) {
			$entry[pql_get_define("PQL_GLOB_ATTR_CN")]	= trim($surname) . " " . trim($name);
		} else {
			$entry[pql_get_define("PQL_GLOB_ATTR_CN")] = $uid;
		}

		if($entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]) {
			$entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")]	= $entry[pql_get_define("PQL_GLOB_ATTR_GIVENNAME")];
		}
		if($entry[pql_get_define("PQL_GLOB_ATTR_SN")]) {
			$entry[pql_get_define("PQL_GLOB_ATTR_SN")]			= $entry[pql_get_define("PQL_GLOB_ATTR_SN")];
		}
		if($entry[pql_get_define("PQL_GLOB_ATTR_CN")]) {
			$entry[pql_get_define("PQL_GLOB_ATTR_CN")]			= $entry[pql_get_define("PQL_GLOB_ATTR_CN")];
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

				$url = "domain_detail.php?domain=$domain&msg=$msg";
			}

			$msg .= urlencode($LANG->_('Successfully created the new user'));

			if(pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $rootdn)) {
				$url  = "user_sendmail.php?email=" . urlencode($email) . "&";
				$url .= "domain=$domain&user=" . urlencode($DNs[0]) . "&rlnb=2&msg=$msg";
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
	   		$url = "domain_detail.php?domain=$domain&msg=$msg";
			header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
		}
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
