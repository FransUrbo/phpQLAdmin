<?php
// add a user
// user_add.php,v 1.5 2002/12/13 13:58:04 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// check if domain exist
$dc = ldap_explode_dn($domain, 0); $dc = split('=', $dc[0]);
if(!pql_domain_exist($_pql, $dc[1])){
	echo "Domain &quot;$domain&quot; does not exists";
	exit();
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql, $domain, "defaultdomain");
$basehomedir   = pql_get_domain_value($_pql, $domain, "basehomedir");
$basemaildir   = pql_get_domain_value($_pql, $domain, "basemaildir");

// check formdata

// ------------------------------------------------
// Page 1: surname, name, email, account_type, account_status
if($submit == "") {
    $error = false;
    $error_text = array();

    if($surname == ""){
		$error = true;
		$error_text["surname"] = PQL_LANG_MISSING;
    }
	
    if($name == ""){
		$error = true;
		$error_text["name"] = PQL_LANG_MISSING;
    }
	
    $user = $surname . " " . $name;
    if($error == false
       and $config["PQL_CONF_REFERENCE_USERS_WITH"][$rootdn] == $config["PQL_GLOB_ATTR_CN"]
       and pql_user_exist($_pql->ldap_linkid, $user)) {
		$error = true;
		$error_text["username"] = pql_complete_constant(PQL_LANG_USER_EXIST, array("user" => $user));
		$error_text["name"] = PQL_LANG_EXISTS;
		$error_text["surname"] = PQL_LANG_EXISTS;
    }
	
    if($email == ""){
		$error = true;
		$error_text["email"] = PQL_LANG_MISSING;
    }

    if(!check_email($email)){
		$error = true;
		$error_text["email"] = PQL_LANG_INVALID;
    }
	
    if($error_text["email"] == "" and pql_email_exists($_pql, $email)) {
		$error = true;
		$error_text["email"] = PQL_LANG_EXISTS;
    }
	
    // if an error occured, set displaying form to '' (initial display)
    if(isset($submit) and $error == true){
		$submit = "";
    } else {
		$error_text = array();
    }
}

// ------------------------------------------------
// Page 2: uid, password, host, quota, userhost, quota_user
if($submit == "two"){
	$error_text = array();
	$error = false;

	// Verify/Create uid
	if(!$uid) {
		if (function_exists('user_generate_uid')) {
			$uid = strtolower(user_generate_uid($_pql, $surname, $name, $email, $domain, $account_type));
		} else {
			$submit = "one";
			$error = true;
			$error_text["uid"] = PQL_LANG_MISSING . "(can't autogenerate)";
		}
	}
	
	// Check again. There should be a user name, either from the input
	// form OR from the user_generate_uid() function above...
	if($uid == ""){
		$submit = "one";
		$error = true;
		$error_text["uid"] = PQL_LANG_MISSING;
	}
	
	// Build the COMPLETE email address
	if(! ereg("@", $email)) {
		$email = $email . "@" . $defaultdomain;
	}
}

if(($submit == "two") or (($submit == "one") and !$ADVANCED_MODE)) {
	// fetch dns information
	$userhost = pql_get_mx($_pql, $defaultdomain);
	if(!$userhost[0]) {
		$error_text["userhost"] = PQL_LANG_DNS_NONE;
		$error = true;
	}
}

// ------------------------------------------------
// Page 3a: 
if ($submit == "save") {
	if($uid == ""){
		$error = true;
		$error_text["uid"] = PQL_LANG_MISSING;
	}
	
	if(preg_match("/[^a-z0-9\.@_-]/i", $uid)) {
		$error = true;
		$error_text["uid"] = PQL_LANG_INVALID;
	}
	if($error_text["uid"] == "" and pql_search_attribute($_pql->ldap_linkid, $domain,
														 $config["PQL_GLOB_ATTR_UID"],
														 $uid)) {
		$error = true;
		$error_text["uid"] = PQL_LANG_EXISTS;
	}
}

// ------------------------------------------------
// Page 3b: 
if($submit == "save" and ($account_type == "normal" or $account_type == "system")){
    if($password == ""){
		$error = true;
		$error_text["password"] = PQL_LANG_MISSING;
    }
    
    if(eregi("KERBEROS", $pwscheme)) {
		// Should be in the form: userid@DOMAIN.LTD
		// TODO: Is this regexp correct!?
		if(! preg_match("/^[a-zA-Z0-9]+[\._-a-z0-9]*[a-zA-Z0-9]+@[A-Z0-9][-A-Z0-9]+(\.[-A-Z0-9]+)+$/", $password)) {
			$error = true;
			$error_text["password"] = PQL_LANG_INVALID;
		}

		if(! eregi('{', $pwscheme))
		  $pwscheme = '{'.$pwscheme;

		if(! eregi('}', $pwscheme))
		  $pwscheme .= '}';
    } else {
		if(preg_match("/[^a-z0-9]/i", $password)){
			$error = true;
			$error_text["password"] = PQL_LANG_INVALID;
		}
    }
	
    if($host != "default"){
		if($userhost[0] == ""){
			$error = true;
			$error_text["userhost"] = PQL_LANG_MISSING;
		} else {
			if(!preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$userhost[1])) {
				//
				$error_text["userhost"] = PQL_LANG_INVALID;
				$error = true;
			}
		}
    }
}

// ------------------------------------------------
// 2b: forwardingaddress
if($submit == "save" and $account_type == "forward"){
	if(!check_email($forwardingaddress)){
		$error = true;
		$error_text["forwardingaddress"] = PQL_LANG_INVALID;
	}
	
	if($forwardingaddress == ""){
		$error = true;
		$error_text["forwardingaddress"] = PQL_LANG_MISSING;
	}
}

// if an error occurs, display the 2nd form again
if($submit == "save" and $error == true and !$ADVANCED_MODE) {
	$submit = "two";
}

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant(PQL_LANG_USER_ADD_TITLE,array("domain" => $domain)); ?></span>
  <br><br>

<?php
// select form to display
switch($submit){
	case "":
	  // first
?>
<form action="<?=$PHP_SELF?>" method="post">
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_LANG_USER_ACCOUNT_PROPERTIES; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_USER_ACCOUNT_TYPE; ?></td>

        <td>
          <select name="account_type">
            <option value="normal" SELECTED><?php echo PQL_LANG_DELIVERYMODE_PROFILE_LOCAL; ?></option>
            <option value="system"><?php echo PQL_LANG_DELIVERYMODE_PROFILE_SYSTEM; ?></option>
            <option value="forward"><?php echo PQL_LANG_DELIVERYMODE_PROFILE_FORWARD; ?></option>
          </select>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php
echo PQL_LANG_DELIVERYMODE_PROFILE . " " . PQL_LANG_DELIVERYMODE_PROFILE_LOCAL . PQL_LANG_DELIVERYMODE_PROFILE_INC .
  PQL_LANG_DELIVERYMODE_PROFILE_LOCAL_INFO . ".";?>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php
echo PQL_LANG_DELIVERYMODE_PROFILE . " " . PQL_LANG_DELIVERYMODE_PROFILE_SYSTEM . PQL_LANG_DELIVERYMODE_PROFILE_INC .
  PQL_LANG_DELIVERYMODE_PROFILE_SYSTEM_INFO . ", " . PQL_LANG_DELIVERYMODE_PROFILE_LOCAL_INFO;?>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php
echo PQL_LANG_DELIVERYMODE_PROFILE . " " . PQL_LANG_DELIVERYMODE_PROFILE_FORWARD . PQL_LANG_DELIVERYMODE_PROFILE_INC .
  PQL_LANG_DELIVERYMODE_PROFILE_FORWARD_INFO . ".";?>
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
    case "one":
	  // second form
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_USER_ACCOUNT_PROPERTIES_MORE; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USER_ID; ?></td>
          <td><?php echo format_error($error_text["uid"]); ?><input type="text" name="uid" value="<?=$uid?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo "<b>" . PQL_LANG_USER_ID . ":</b> " . PQL_LANG_UID_HELP_SHORT; ?></td>
        </tr>

<?php
    if($account_type != "forward") {
                // display forms for SYSTEM/MAIL account
?>
        <!-- Password -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USERPASSWORD_TITLE; ?></td>
          <!-- Crude hackery. Using type=password won't be so good if we're using {KERBEROS} -->
          <td><?php echo format_error($error_text["password"]); ?>

            <input type="input" name="password">
            <?php echo format_error($error["pwscheme"]); ?>

<?php
		if(eregi(',', $config["PQL_CONF_PASSWORD_SCHEMES"][$rootdn])) {
			// We got more than one password scheme...
?>
            <select name="pwscheme">
              <option value="none">Choose password scheme to use</option>
<?php		$schemes = split(",", $config["PQL_CONF_PASSWORD_SCHEMES"][$rootdn]);
			foreach($schemes as $scheme) {
?>
              <option value="{<?=$scheme?>}"><?=$scheme?></option>
<?php		} ?>
            </select>
<?php	} else { ?>
            Scheme: <b>{<?=$config["PQL_CONF_PASSWORD_SCHEMES"][$rootdn]?>}</b>
            <input type="hidden" name="pwscheme" value="<?=$config["PQL_CONF_PASSWORD_SCHEMES"][$rootdn]?>">
<?php	} ?>
          </td>
        </tr>

<?php	if(eregi('KERBEROS', $config["PQL_CONF_PASSWORD_SCHEMES"][$rootdn])) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo "<b>" . PQL_LANG_USERPASSWORD_TITLE . ":</b> " . PQL_LANG_USERPASSWORD_HELP_KRB; ?></td>
        </tr>
<?php	}
    } // account_type != forward

    if($account_type == "system") {
		// display forms for SYSTEM account

		if($ADVANCED_MODE) {
			$shells = pql_get_valid_shells();
?>

        <!-- Loginshell -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USER_LOGINSHELL; ?></td>
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
          <td class="title">Put user in subbranch</td>
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
			if($config["PQL_CONF_SUBTREE_USERS"]) {
?>
        <input type="hidden" name="subbranch" value="<?=$config["PQL_CONF_SUBTREE_USERS"].",".$domain?>">
<?php
			} else {
?>
        <input type="hidden" name="subbranch" value="<?=$domain?>">
<?php
			}
		}
	} else {
		if($config["PQL_CONF_SUBTREE_USERS"]) {
?>
        <input type="hidden" name="subbranch" value="<?=$config["PQL_CONF_SUBTREE_USERS"].",".$domain?>">
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
      <th colspan="3" align="left"><?php echo PQL_LANG_USER_DATA; ?></th>
        <!-- Firstname -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USER_DATA_SURNAME; ?></td>
          <td>
              <?php echo format_error($error_text["surname"]); ?>
              <input type="text" name="surname" value="<?=$surname?>">
          </td>
        </tr>

        <!-- Lastname -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USER_DATA_LASTNAME; ?></td>
          <td>
            <?php echo format_error($error_text["name"]); ?>
            <input type="text" name="name" value="<?=$name?>">
          </td>
        </tr>

        <!-- Email address -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
          <td>
            <?php echo format_error($error_text["email"]); ?>
            <input type="text" name="email" value="<?=$email?>"><b>@<?=$defaultdomain?></b>
          </td>
        </tr>

<?php if (isset($error_text["username"])){ ?>
        <tr class="subtitle">
          <td colspan="2"><?php echo format_error($error_text["username"]); ?></td>
        </tr>
<?php } ?>
        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_USER_ADD_HELP2;?>
          </td>
        </tr>
      </th>
    </table>
<?php
	if($account_type == "forward") {
		// display forms for FORWARDING account
?>

    <br>
  
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Forward mails to</th>
        <!-- Forwarding address -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_SEARCH_MAILFORWARDINGADDRESS; ?></td>
          <td>
            <?php echo format_error($error_text["forwardingaddress"]); ?>
            <input type="text" name="forwardingaddress" value="<?=$forwardingaddress?>"> <?php echo PQL_LANG_EMAIL; ?>
          </td>
        </tr>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_MAILFORWARDINGADDRESS_ADD_HELP; ?></td>
        </tr>
      </th>
<?php
	} // account_type == forward
 ?>
    </table>
  
    <br>
  
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_USER_ACCOUNT_PROPERTIES; ?></th>
        <!-- Account status -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_ACCOUNTSTATUS_STATUS; ?></td>
          <td>
            <select name="account_status">
              <option value="active" SELECTED><?php echo PQL_LANG_ACCOUNTSTATUS_ACTIVE; ?></option>
              <option value="nopop"><?php echo PQL_LANG_ACCOUNTSTATUS_NOPOP; ?></option>
              <option value="disabled"><?php echo PQL_LANG_ACCOUNTSTATUS_DISABLE; ?></option>
            </select>
          </td>
        </tr>
      </th>
    </table>
  
    <br>
  
<?php
	if(($ADVANCED_MODE == 0) or ($account_type == "forward")) {
		// Go to save, no next form...
		if($ADVANCED_MODE == 0) {
?>
    <input type="hidden" name="loginshell" value="/bin/false">
    <input type="hidden" name="homedirectory" value="">
    <input type="hidden" name="maildirectory" value="">
    <input type="hidden" name="userhost" value="<?=$userhost[1]?>">
    <input type="hidden" name="host" value="default">
<?php
		}
?>
    <input type="hidden" name="submit" value="save">
<?php
	} else {
?>
    <input type="hidden" name="submit" value="two">
<?php
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
    <input type="submit" value="<?=PQL_LANG_SAVE?>">
<?php
	}
?>
  </form>

<?php
		break;
	case "two":
		// third
?>

  <form action="<?=$PHP_SELF?>" method="post">
    <input type="hidden" name="surname" value="<?=$surname?>">
    <input type="hidden" name="name" value="<?=$name?>">
    <input type="hidden" name="email" value="<?=$email?>">
    <input type="hidden" name="uid" value="<?=$uid?>">
<?php if($account_type != "forward") { ?>
    <input type="hidden" name="password" value="<?=$password?>">
    <input type="hidden" name="pwscheme" value="<?=$pwscheme?>">
<?php } ?>
    <input type="hidden" name="subbranch" value="<?=$subbranch?>">

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_USER_ACCOUNT_PROPERTIES_MORE; ?></th>
<?php
	if($account_type == "system"){
		// Show homedirectory
?>
        <!-- Home directory -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USER_HOMEDIR; ?></td>
          <td><?php
	  if(! ereg("/$", $basehomedir)) {
		  $homedirectory = $basehomedir . "/" . $uid;
	  } else {
		  $homedirectory = $basehomedir . $uid;
	  }
	  echo $homedirectory . "/";
	  ?></td>
        </tr>
        <input type="hidden" name="loginshell" value="<?=$loginshell?>">
        <input type="hidden" name="homedirectory" value="<?=$homedirectory?>">
<?php
	}

	if($account_type == "normal" or $account_type == "system") {
		// display forms for SYSTEM/MAIL account(s)
?>

        <!-- MailMessageStore -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_MAILMESSAGESTORE_TITLE; ?></td>
          <td><?php
	  if(! ereg("/$", $basemaildir)) {
		  $maildirectory = $basemaildir . "/" . $uid;
	  } else {
		  $maildirectory = $basemaildir . $uid;
	  }
	  echo $maildirectory . "/";
	  ?></td>
        </tr>

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
          <td class="title"><?php echo PQL_LANG_MAILHOST_TITLE; ?></td>
          <td>
            <input type="hidden" name="mx" value="<?=$userhost[1]?>">
            <input type="radio" name="host" value="default" <?php if($userhost[0] and ($host != "user")){ echo "checked";}?>><?php
		echo PQL_LANG_MAILHOST_DEFAULT . ": <b>";
		if($userhost[0]) {
			echo "$userhost[1]";
		}
		echo "</b>";?>
          </td>
        </tr>
<?php
		if(!$userhost[0] and $userhost[1]) {
			// It's defined, but it comes from LDAP
?>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="userhost" value="user" checked>qmailControls object: <b><?=$userhost[1]?></b>
          </td>
        </tr>
<?php
		}
?>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="userhost" value="user" <?php if((!$userhost[0] and !$userhost[1]) or ($host == "user")){ echo "checked";}?>><?php echo PQL_LANG_MAILQUOTA_USERDEFINED;?>  <input type="text" name="userhost"><br>
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
    <input type="submit" value="<?=PQL_LANG_SAVE?>">
  </form>
<?php
		break;
	case "save":
		// code for saving the user

		// convert uid, email to lowercase
		$uid = strtolower($uid);

		if(! ereg("@", $email)) {
			// Build the COMPLETE email address
			$email = strtolower($email . "@" . $defaultdomain);
		} else {
			$email = strtolower($email);
		}

		// prepare the users attributes
		$cn = $surname . " " . $name;
		$entry[$config["PQL_GLOB_ATTR_CN"]]			= trim($surname) . " " . trim($name);
		$entry[$config["PQL_GLOB_ATTR_SN"]]			= $surname;
		$entry[$config["PQL_GLOB_ATTR_GIVENNAME"]]	= $name;
		$entry[$config["PQL_GLOB_ATTR_MAIL"]]		= $email;
		$entry[$config["PQL_GLOB_ATTR_UID"]]		= $uid;
		$entry[$config["PQL_GLOB_ATTR_ISACTIVE"]]	= $account_status;

        // ------------------
		if($account_type == "system") {
			// Normal system account

			// set SYSTEM attributes
			$entry[$config["PQL_GLOB_ATTR_LOGINSHELL"]] = $loginshell;
			if(!$homedirectory) {
				$entry[$config["PQL_GLOB_ATTR_HOMEDIR"]] = user_generate_homedir($_pql, $email, $domain, $entry);
			} else {
				$entry[$config["PQL_GLOB_ATTR_HOMEDIR"]] = $homedirectory;
			}

			// Get a free UserID number (which we also use for GroupID number)
			$uidnr = pql_get_next_uidnumber($_pql);
			if($uidnr > 0) {
				$entry[$config["PQL_GLOB_ATTR_QMAILUID"]] = $uidnr;
				$entry[$config["PQL_GLOB_ATTR_QMAILGID"]] = $uidnr;
			}

			// Gecos is needed to do PAM/NSS LDAP login 
			$entry["gecos"] = $surname . " " . $name;
		}

        // ------------------
		if($account_type == "normal" or $account_type == "system"){
			// normal mailbox account

			// set attributes
			$entry[$config["PQL_GLOB_ATTR_PASSWD"]]   = pql_password_hash($password, $pwscheme);
			if($pwscheme == "{KERBEROS}")
			  // Make sure that 'User objectclasses' contain krb5Principal (in the domain/branch config).
			  // See the 'Show phpQLAdmin configuration' page...
			  $entry["krb5PrincipalName"] = $password;

			if($host == 'default') {
				// TODO: If there is no defaultDomain for the domain, get MX of the domain in the email address
				if($mx)
				  $entry[$config["PQL_GLOB_ATTR_MAILHOST"]] = $mx;
				else {
					$domainname = split('@', $entry[$config["PQL_GLOB_ATTR_MAIL"]]);
					$entry[$config["PQL_GLOB_ATTR_MAILHOST"]] = pql_get_mx($domainname[1], $defaultdomain);
				}
			} else {
				if($mx)
				  $entry[$config["PQL_GLOB_ATTR_MAILHOST"]] = $mx;
				else
				  $entry[$config["PQL_GLOB_ATTR_MAILHOST"]] = $userhost[1];
			}
			$entry[$config["PQL_GLOB_ATTR_MODE"]]     = "localdelivery";
			if(!$maildirectory) {
				$entry[$config["PQL_GLOB_ATTR_MAILSTORE"]] = user_generate_mailstore($_pql, $email, $domain, $entry);
			} else {
				$entry[$config["PQL_GLOB_ATTR_MAILSTORE"]] = $maildirectory;
			}
		} else {
			// forwardonly account

			// convert forwardingaddress to lowercase
			$forwardingaddress = strtolower($forwardingaddress);

			// set attributes
			$entry[$config["PQL_GLOB_ATTR_FORWARDS"]]	= $forwardingaddress;
			$entry[$config["PQL_GLOB_ATTR_MODE"]][]	= "forwardonly";
			$entry[$config["PQL_GLOB_ATTR_MODE"]][]	= "nombox";

			// Even forward accounts need UIDNumber! (?!?)
			$entry[$config["PQL_GLOB_ATTR_QMAILUID"]] = $config["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"][$rootdn];
			$entry[$config["PQL_GLOB_ATTR_QMAILGID"]] = $config["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"][$rootdn];
			$entry[$config["PQL_GLOB_ATTR_HOMEDIR"]]  = "/tmp";
		}

        // ------------------
		// Add the user to the database
		$dns = pql_user_add($_pql->ldap_linkid, $domain, $cn, $entry, $account_type, $subbranch);
		if($dns[0]) {
			// TODO: dns[1] (the group object) might still be empty -> failed to add it.

			// Now it's time to run the special adduser script if defined...
			if($config["PQL_CONF_EXTRA_SCRIPT_CREATE_USER"][$rootdn]) {
				// Setup the environment with the user details
				putenv("PQL_CONF_DOMAIN=$domain");
				putenv("PQL_CONF_WEBUSER=".posix_getuid());
				foreach($entry as $key => $e) {
					$key = "PQL_" . strtoupper($key);
					if($key != 'PQL_CONF_OBJECTCLASS')
					  putenv("$key=$e");
				}

				// Execute the user add script (0 => show output)
				if(pql_execute($config["PQL_CONF_EXTRA_SCRIPT_CREATE_USER"][$rootdn], 0)) {
					echo PQL_LANG_USER_ADD_SCRIPT_FAILED;
					$msg = urlencode(PQL_LANG_USER_ADD_SCRIPT_FAILED) . ".&nbsp;";
				} else {
					echo "<b>" . PQL_LANG_USER_ADD_SCRIPT_OK . "</b><br>";
					$msg = urlencode(PQL_LANG_USER_ADD_SCRIPT_OK) . ".&nbsp;";
				}

				$url = "domain_detail.php?domain=$domain&msg=$msg";
			}

			$msg .= urlencode(PQL_LANG_USER_ADD_OK);

			if($config["PQL_CONF_TESTMAIL_AUTOSEND"][$rootdn]) {
				$url  = "user_sendmail.php?email=" . urlencode($email) . "&";
				$url .= "domain=$domain&user=" . urlencode($dns[0]) . "&rlnb=2&msg=$msg";
			} else {
				$url  = "user_detail.php?";
				$url .= "domain=$domain&user=" . urlencode($dns[0]) . "&rlnb=2&msg=$msg";
			}

			if($config["PQL_CONF_EXTRA_SCRIPT_CREATE_USER"][$rootdn]) {
?>
    <form action="<?=$url?>" method="post">
      <input type="submit" value="Continue">
    </form>
<?php
				die();
			} else {
				header("Location: " . $config["PQL_GLOB_URI"] . "$url");
			}
		} else {
			$msg = urlencode(PQL_LANG_USER_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	   		$url = "domain_detail.php?domain=$domain&msg=$msg";
			header("Location: " . $config["PQL_GLOB_URI"] . "$url");
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
