<?php
// add a user
// user_add.php,v 1.5 2002/12/13 13:58:04 turbo Exp
//
session_start();

require("./include/pql.inc");

$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

// check if domain exist
if(!pql_domain_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain)){
	echo "domain &quot;$domain&quot; does not exists";
	exit();
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
$basehomedir   = pql_get_domain_value($_pql->ldap_linkid, $domain, "basehomedir");
$basemaildir   = pql_get_domain_value($_pql->ldap_linkid, $domain, "basemaildir");

// check formdata

// ------------------------------------------------
// Page 1: surname, name, email, account_type, account_status
if($submit == "") {
    $error = false;
    $error_text = array();

    if($surname == ""){
		$error = true;
		$error_text["surname"] = PQL_MISSING;
    }
	
    if($name == ""){
		$error = true;
		$error_text["name"] = PQL_MISSING;
    }
	
    $user = $surname . " " . $name;
    if($error == false
       and PQL_LDAP_REFERENCE_USERS_WITH == PQL_LDAP_ATTR_CN
       and pql_user_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user)){
		$error = true;
		$error_text["username"] = pql_complete_constant(PQL_USER_EXIST, array("user" => $user));
		$error_text["name"] = PQL_EXISTS;
		$error_text["surname"] = PQL_EXISTS;
    }
	
    if($email == ""){
		$error = true;
		$error_text["email"] = PQL_MISSING;
    }

    if(!check_email($email)){
		$error = true;
		$error_text["email"] = PQL_INVALID;
    }
	
    if($error_text["email"] == "" and pql_email_exists($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $email)){
		$error = true;
		$error_text["email"] = PQL_EXISTS;
    }
	
    // if an error occured, set displaying form to '' (initial display)
    if(isset($submit) and $error == true){
		$submit = "";
    } else {
		$error_text = array();
    }
}

// ------------------------------------------------
// Page 2: uid, password, host, quota, host_user, quota_user
if($submit == "two"){
	$error_text = array();
	$error = false;

	// Verify/Create uid
	if(!$uid) {
		if (function_exists('user_generate_uid')) {
			$uid = strtolower(user_generate_uid($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $surname, $name, $email, $domain, $account_type));
		} else {
			$submit = "one";
			$error = true;
			$error_text["uid"] = PQL_MISSING . "(can't autogenerate)";
		}
	}
	
	// Check again. There should be a user name, either from the input
	// form OR from the user_generate_uid() function above...
	if($uid == ""){
		$submit = "one";
		$error = true;
		$error_text["uid"] = PQL_MISSING;
	}
	
	// Build the COMPLETE email address
	if(! ereg("@", $email)) {
		$email = $email . "@" . $defaultdomain;
	}
}

if(($submit == "two") or (($submit == "one") and !$ADVANCED_MODE)) {
	// fetch dns information
	$res = getmxrr($defaultdomain, $rec, $weight);
	if(count($rec) == 0) {
		$error_text["host_user"] = PQL_DNS_NONE;
		$error = true;
	} else {
		// Take the MX with _LOWEST_ priority/weight.
		asort($weight); $old_prio = 65555;
		foreach($weight as $key => $prio){
			if($prio < $old_prio) {
				$old_prio = $prio; $prio_key = $key;
			}
		}

		$host_user = $rec[$prio_key];
	}
}

// ------------------------------------------------
// Page 3a: 
if ($submit == "save") {
	if($uid == ""){
		$error = true;
		$error_text["uid"] = PQL_MISSING;
	}
	
	if(preg_match("/[^a-z0-9\.@_-]/i", $uid)) {
		$error = true;
		$error_text["uid"] = PQL_INVALID;
	}
	if($error_text["uid"] == "" and pql_search_attribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, PQL_LDAP_ATTR_UID, $uid)) {
		$error = true;
		$error_text["uid"] = PQL_EXISTS;
	}
}

// ------------------------------------------------
// Page 3b: 
if($submit == "save" and ($account_type == "normal" or $account_type == "system")){
    if($password == ""){
		$error = true;
		$error_text["password"] = PQL_MISSING;
    }
    
    if($pwscheme == "{KERBEROS}") {
		// Should be in the form: userid@DOMAIN.LTD
		// TODO: Is this regexp correct!?
		if(! preg_match("/^[a-zA-Z0-9]+[\._-a-z0-9]*[a-zA-Z0-9]+@[A-Z0-9][-A-Z0-9]+(\.[-A-Z0-9]+)+$/", $password)) {
			$error = true;
			$error_text["password"] = PQL_INVALID;
		}
    } else {
		if(preg_match("/[^a-z0-9]/i", $password)){
			$error = true;
			$error_text["password"] = PQL_INVALID;
		}
    }
	
    if($host != "default"){
		if($host_user == ""){
			$error = true;
			$error_text["host_user"] = PQL_MISSING;
		} else {
			if(!preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$host_user)) {
				//
				$error_text["host_user"] = PQL_INVALID;
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
		$error_text["forwardingaddress"] = PQL_INVALID;
	}
	
	if($forwardingaddress == ""){
		$error = true;
		$error_text["forwardingaddress"] = PQL_MISSING;
	}
}

// if an error occurs, display the 2nd form again
if($submit == "save" and $error == true and !$ADVANCED_MODE) {
	$submit = "two";
}

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant(PQL_USER_ADD_TITLE,array("domain" => $domain)); ?></span>
  <br><br>

<?php
// select form to display
switch($submit){
	case "":
	  // first
?>
<form action="<?php echo $PHP_SELF ?>" method="post">
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_USER_ACCOUNT_PROPERTIES; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_ACCOUNT_TYPE; ?></td>

        <td>
          <select name="account_type">
            <option value="normal" SELECTED><?php echo PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL; ?></option>
            <option value="system"><?php echo PQL_LDAP_DELIVERYMODE_PROFILE_SYSTEM; ?></option>
            <option value="forward"><?php echo PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD; ?></option>
          </select>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php
echo PQL_LDAP_DELIVERYMODE_PROFILE . " " . PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL . PQL_LDAP_DELIVERYMODE_PROFILE_INC .
  PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL_INFO . ".";?>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php
echo PQL_LDAP_DELIVERYMODE_PROFILE . " " . PQL_LDAP_DELIVERYMODE_PROFILE_SYSTEM . PQL_LDAP_DELIVERYMODE_PROFILE_INC .
  PQL_LDAP_DELIVERYMODE_PROFILE_SYSTEM_INFO . ", " . PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL_INFO;?>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php
echo PQL_LDAP_DELIVERYMODE_PROFILE . " " . PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD . PQL_LDAP_DELIVERYMODE_PROFILE_INC .
  PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD_INFO . ".";?>
        </td>
      </tr>
    </th>
  </table>

  <input type="hidden" name="submit" value="one">
  <input type="hidden" name="domain" value="<?php echo $domain; ?>">
  <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
</form>
<?php
      break;
    case "one":
	  // second form
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_USER_ACCOUNT_PROPERTIES_MORE; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_USER_ID; ?></td>
          <td><?php echo format_error($error_text["uid"]); ?><input type="text" name="uid" value="<?php echo $uid; ?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo "<b>" . PQL_USER_ID . ":</b> " . PQL_LDAP_UID_HELP_SHORT; ?></td>
        </tr>

<?php
    if($account_type != "forward") {
                // display forms for SYSTEM/MAIL account
?>
        <!-- Password -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></td>
          <!-- Crude hackery. Using type=password won't be so good if we're using {KERBEROS} -->
          <td><?php echo format_error($error_text["password"]); ?>

            <input type="input" name="password">
            <?php echo format_error($error["pwscheme"]); ?>

            <select name="pwscheme">
              <option value="none">Choose password scheme to use</option>
<?php
              $schemes = split(",", PQL_PASSWORD_SCHEMES);
              foreach($schemes as $scheme) {
?>
              <option value="{<?php echo $scheme; ?>}"><?php echo $scheme; ?></option>
<?php } ?>
            </select>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo "<b>" . PQL_LDAP_USERPASSWORD_TITLE . ":</b> " . PQL_LDAP_USERPASSWORD_HELP_KRB; ?></td>
        </tr>
<?php
    } // account_type != forward

    if($account_type == "system") {
		// display forms for SYSTEM account

		if($ADVANCED_MODE) {
			$shells = pql_get_valid_shells();
?>

        <!-- Loginshell -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_USER_LOGINSHELL; ?></td>
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
      </th>
    </table>

    <br>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_USER_DATA; ?></th>
        <!-- Firstname -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_USER_DATA_SURNAME; ?></td>
          <td><?php echo format_error($error_text["surname"]); ?><input type="text" name="surname" value="<?php echo $surname; ?>"></td>
        </tr>

        <!-- Lastname -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_USER_DATA_LASTNAME; ?></td>
          <td><?php echo format_error($error_text["name"]); ?><input type="text" name="name" value="<?php echo $name; ?>"></td>
        </tr>

        <!-- Email address -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_EMAIL; ?></td>
          <td><?php echo format_error($error_text["email"]); ?><input type="text" name="email" value="<?php echo $email; ?>"><b>@<?php echo $defaultdomain; ?></b></td>
        </tr>

<?php if (isset($error_text["username"])){ ?>
        <tr class="subtitle">
          <td colspan="2"><?php echo format_error($error_text["username"]); ?></td>
        </tr>
<?php } ?>
        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_USER_ADD_HELP2;?>
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
          <td class="title"><?php echo PQL_SEARCH_MAILFORWARDINGADDRESS; ?></td>
          <td><?php echo format_error($error_text["forwardingaddress"]); ?><input type="text" name="forwardingaddress" value="<?php echo $forwardingaddress; ?>"> <?php echo PQL_EMAIL; ?></td>
        </tr>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LDAP_MAILFORWARDINGADDRESS_ADD_HELP; ?></td>
        </tr>
      </th>
<?php
	} // account_type == forward
 ?>
    </table>
  
    <br>
  
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_USER_ACCOUNT_PROPERTIES; ?></th>
        <!-- Account status -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LDAP_ACCOUNTSTATUS_STATUS; ?></td>
          <td>
            <select name="account_status">
              <option value="active" SELECTED><?php echo PQL_LDAP_ACCOUNTSTATUS_ACTIVE; ?></option>
              <option value="nopop"><?php echo PQL_LDAP_ACCOUNTSTATUS_NOPOP; ?></option>
              <option value="disabled"><?php echo PQL_LDAP_ACCOUNTSTATUS_DISABLE; ?></option>
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
    <input type="hidden" name="host_user" value="<?=$host_user?>">
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
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="hidden" name="account_type" value="<?php echo $account_type;?>">
<?php
	if($ADVANCED_MODE == 1) {
?>
    <input type="submit" value="--&gt;&gt;">
<?php
	} else {
?>
    <input type="submit" value="<?=PQL_SAVE?>">
<?php
	}
?>
  </form>

<?php
		break;
	case "two":
		// third
?>

  <form action="<?php echo $PHP_SELF ?>" method="post">
    <input type="hidden" name="surname" value="<?php echo $surname;?>">
    <input type="hidden" name="name" value="<?php echo $name;?>">
    <input type="hidden" name="email" value="<?php echo $email;?>">
    <input type="hidden" name="uid" value="<?php echo $uid;?>">
<?php if($account_type != "forward") { ?>
    <input type="hidden" name="password" value="<?php echo $password;?>">
    <input type="hidden" name="pwscheme" value="<?php echo $pwscheme;?>">
  <?php	}?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_USER_ACCOUNT_PROPERTIES_MORE; ?></th>
<?php
	if($account_type == "system"){
		// Show homedirectory
?>
        <!-- Home directory -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_USER_HOMEDIR; ?></td>
          <td><?php
	  if(! ereg("/$", $basehomedir)) {
		  $homedirectory = $basehomedir . "/" . $uid;
	  } else {
		  $homedirectory = $basehomedir . $uid;
	  }
	  echo $homedirectory . "/";
	  ?></td>
        </tr>
        <input type="hidden" name="loginshell" value="<?php echo $loginshell; ?>">
        <input type="hidden" name="homedirectory" value="<?php echo $homedirectory; ?>">
<?php
	}

	if($account_type == "normal" or $account_type == "system") {
		// display forms for SYSTEM/MAIL account(s)
?>
        <!-- MailMessageStore -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LDAP_MAILMESSAGESTORE_TITLE; ?></td>
          <td><?php
	  if(! ereg("/$", $basemaildir)) {
		  $maildirectory = $basemaildir . "/" . $uid;
	  } else {
		  $maildirectory = $basemaildir . $uid;
	  }
	  echo $maildirectory . "/";
	  ?></td>
        </tr>

        <br><br>

        <!-- Mailhost -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LDAP_MAILHOST_TITLE; ?></td>
          <td>
            <input type="hidden" name="userhost" value="<?=$host_user?>">
            <input type="radio" name="host" value="default" <?php if($host != "user"){ echo "checked";}?>><?php echo PQL_LDAP_MAILHOST_DEFAULT . ": <b>" . $host_user . "</b>";?>
          </td>
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="host" value="user" <?php if($host == "user"){ echo "checked";}?>><?php echo PQL_LDAP_MAILQUOTA_USERDEFINED;?> <?php echo format_error($error_text["host_user"]); ?><input type="text" name="host_user">
          </td>
        </tr>
<?php
	} // end of if-else
?>
      </th>
    </table>
  
    <br>

<?php	if($account_type != "forward") {?>
    <input type="hidden" name="maildirectory" value="<?php echo $maildirectory; ?>">
<?php	}?>
    <input type="hidden" name="account_type" value="<?php echo $account_type;?>">
    <input type="hidden" name="account_status" value="<?php echo $account_status;?>">
    <input type="hidden" name="submit" value="save">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="submit" value="<?=PQL_SAVE?>">
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
		$entry[PQL_LDAP_ATTR_CN]			= trim($surname) . " " . trim($name);
		$entry[PQL_LDAP_ATTR_SN]			= $surname;
		
		// $entry[PQL_LDAP_ATTR_GIVENNAME]	= $name;
		
		$entry[PQL_LDAP_ATTR_MAIL]			= $email;
		$entry[PQL_LDAP_ATTR_UID]			= $uid;
		$entry[PQL_LDAP_ATTR_ISACTIVE]		= $account_status;

        // ------------------
		if($account_type == "system") {
			// Normal system account

			// We should have inetOrgPerson and posixAccount as objectclass for this
			// type of System account
			$entry["objectClass"][] = "inetOrgPerson";
			$entry["objectClass"][] = "posixAccount";

			// set SYSTEM attributes
			$entry[PQL_LDAP_ATTR_LOGINSHELL] = $loginshell;
			if(!$homedirectory) {
				$entry[PQL_LDAP_ATTR_HOMEDIR] = user_generate_homedir($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $email, $domain, $entry);
			} else {
				$entry[PQL_LDAP_ATTR_HOMEDIR] = $homedirectory;
			}

			// Get a free UserID number (which we also use for GroupID number)
			$uidnr = pql_get_next_uidnumber($_pql->ldap_linkid);
			if($uidnr > 0) {
				$entry[PQL_LDAP_ATTR_QMAILUID] = $uidnr;
				$entry[PQL_LDAP_ATTR_QMAILGID] = $uidnr;
			}
		}

        // ------------------
		if($account_type == "normal" or $account_type == "system"){
			// normal mailbox account

			// set attributes
			$entry[PQL_LDAP_ATTR_PASSWD] = pql_password_hash($password, $pwscheme);
			if($host == 'default') {
				$entry[PQL_LDAP_ATTR_MAILHOST] = $userhost;
			} else {
				$entry[PQL_LDAP_ATTR_MAILHOST] = $host_user;
			}
			$entry[PQL_LDAP_ATTR_MODE] = "localdelivery";
			if(!$maildirectory) {
				$entry[PQL_LDAP_ATTR_MAILSTORE] = user_generate_mailstore($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $email, $domain, $entry);
			} else {
				$entry[PQL_LDAP_ATTR_MAILSTORE] = $maildirectory;
			}
		} else {
			// forwardonly account

			// convert forwardingaddress to lowercase
			$forwardingaddress = strtolower($forwardingaddress);

			// set attributes
			$entry[PQL_LDAP_ATTR_FORWARDS]	= $forwardingaddress;
			$entry[PQL_LDAP_ATTR_MODE][]	= "forwardonly";
			$entry[PQL_LDAP_ATTR_MODE][]	= "nombox";
		}

        // ------------------
		// Add the user to the database
		if(pql_add_user($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $cn, $entry, $account_type)){
			// Now it's time to run the special adduser script if defined...
			if(PQL_EXTRA_SCRIPT_CREATE) {
				// Open a bi-directional pipe, so we can write AND read
				$fh = popen(escapeshellcmd(PQL_EXTRA_SCRIPT_CREATE), "w+");
				if($fh) {

					// We're done, close the command file handle...
					pclose($fh);
				} else {
					$msg = urlencode(PQL_USER_ADD_SCRIPT_FAILED);
					$url = "domain_detail.php?domain=$domain&msg=$msg";
					header("Location: " . PQL_URI . "$url");
				}
			}

			$msg = urlencode(PQL_USER_ADD_OK);
			if (PQL_TESTMAIL_AUTOSEND) {
				$url = "user_sendmail.php?email=" . urlencode($email) . "&";
			} else {
				$url = "user_detail.php?";
			}
			
	   		$url .= "domain=$domain&user=" . urlencode($entry[PQL_LDAP_REFERENCE_USERS_WITH]) . "&rlnb=2&msg=$msg";
			header("Location: " . PQL_URI . "$url");
		} else {
			$msg = urlencode(PQL_USER_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	   		$url = "domain_detail.php?domain=$domain&msg=$msg";
			header("Location: " . PQL_URI . "$url");
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
