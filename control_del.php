<?php
// Delete a mailserver controls object
// $Id: control_del.php,v 2.10.8.1 2004-09-17 10:51:45 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

// Get users that uses this mailserver - we need to know this twice.
// Once the first time we're called, and once when the acctuall deletion
// will take place.
// Therefor a little function here to simplify, and avoid duplication.
// I'm lazy, so sue me! :)
function control_del_find_users($link, $host) {
	// See if there's any users that have this host as mailHost
	$filter = pql_get_define("PQL_ATTR_MAILHOST").'='.$host;
	foreach($link->ldap_basedn as $dn) {
		$dn = urldecode($dn);
		$usrs = pql_search($link->ldap_linkid, $dn, $filter);
		if(is_array($usrs)) {
			for($i=0; $usrs[$i]; $i++)
			  $users[] = $usrs[$i];
		}
	}

	return($users);
}


if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	if(!$_REQUEST["submit"] or $_REQUEST["error"]) {
		include("./header.html");
		
		// Check to see if this object exists
		$sr = ldap_search($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
						  "(cn=" . $_REQUEST["mxhost"] . ")", array("cn"), "BASE");
		if(ldap_count_entries($_pql_control->ldap_linkid, $sr) > 0) {
			// Exists - get DN
			$dn = ldap_get_dn($_pql_control->ldap_linkid,
							  ldap_first_entry($_pql_control->ldap_linkid, $sr));

			$users = control_del_find_users($_pql, $_REQUEST["mxhost"]);
			if(is_array($users)) {
				// There's users - get MX and QmailLDAP/Controls objects
				$hosts = pql_control_get_hosts($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"]);
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="GET">
    <input type="hidden" name="oldmx"  value="<?=$_REQUEST["mxhost"]?>">

    <span class="title2"><?=$LANG->_("There's users on this mail server!<br>What would you like to do with them?")?></span><br>
    <input type="radio"  name="action" value="ignore" CHECKED><?=$LANG->_('Ignore')?><br>
    <input type="radio"  name="action" value="delete"><?=$LANG->_('Delete all users')?><br>
    <input type="radio"  name="action" value="move" <?php if($_REQUEST["error"]) { ?>CHECKED<?php } ?>>
      <?=$LANG->_('Move users to server:')?><br>
<?php		if($_REQUEST["error"]) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
				echo pql_format_error_span($LANG->_("Hostname missing!"));
				echo "<br>";
			}

			for($i=0; $hosts[$i]; $i++) {
				if($hosts[$i] != $_REQUEST["mxhost"]) {
					$host = pql_maybe_idna_decode($hosts[$i]);
?>
    &nbsp;&nbsp;&nbsp;&nbsp;<input type="radio"  name="newmx"  value="<?=$host?>"><b><?=$host?></b><br>
<?php			}
			}
?>
    &nbsp;&nbsp;&nbsp;&nbsp;<input type="radio"  name="newmx"  value="user"><?=$LANG->_('User specified')?>
                            <input type="text"   name="mxhost" value="<?=$_REQUEST["newmx"]?>" size="30"><br>
    <br>
    <input type="submit" name="submit" value="<?="--&gt;&gt;"?>">
  </form>
  <table cellspacing="0" cellpadding="3" border="0">
    <th>
      <tr class="<?php pql_format_table(); ?>">
        <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td><?=$LANG->_('NOTE: Deleting or moving a user will not touch their mail- or homedirectory! You will have to delete/move them to the new server manually.')?></td>
      </tr>
    </th>
  </table>
<?php		} else
				// No users using this mailserver...
				$delete_object = 1;
		}
	} else {
		// We've submitted
		switch($_REQUEST["action"]) {
			case "ignore":
			  // Don't touch the users - delete object directly
			  $delete_object = 1;
			  break;

			case "delete":
			  // 1. Delete all users
			  $users = control_del_find_users($_pql, $_REQUEST["oldmx"]);
			  if(is_array($users)) {
				  for($i=0; $users[$i]; $i++)
					pql_user_del($_pql, $_REQUEST["domain"], $users[$i], 1);
			  }

			  // 2. Delete object
			  $delete_object = 1;
			  break;

			case "move":
			  // 1. Move user(s) to other MX/Host. Where?
			  if($_REQUEST["newmx"] == "") {
				  // We haven't specified a (pre-defined) MX/host to move the users to - do over!
				  $oldmx = $_REQUEST["oldmx"]; unset($_REQUEST);
				  $url = $_SERVER["PHP_SELF"]."?mxhost=$oldmx&error=1";

				  header("Location: " . pql_get_define("PQL_CONF_URI") . $url);
			  } else {
				  if(($_REQUEST["newmx"] == 'user') and !$_REQUEST["mxhost"]) {
					  // No (user specified) MX/host specified - do over!
					  $oldmx = $_REQUEST["oldmx"]; unset($_REQUEST);
					  $url = $_SERVER["PHP_SELF"]."?mxhost=$oldmx&error=1";

					  header("Location: " . pql_get_define("PQL_CONF_URI") . $url);
				  } else {
					  // Move the user(s) to user specified server
					  // OR
					  // Move the user(s) to specified server
					  if($_REQUEST["newmx"] == 'user')
						$host = $_REQUEST["mxhost"];
					  else
						$host = $_REQUEST["newmx"];

					  $users = control_del_find_users($_pql, $_REQUEST["oldmx"]);
					  if(is_array($users)) {
						  for($i=0; $users[$i]; $i++) {
							  pql_replace_attribute($_pql->ldap_linkid,
													$users[$i],
													pql_get_define("PQL_ATTR_MAILHOST"),
													$host);
						  }
					  }
				  }
			  }

			  // 2. Delete object
			  $delete_object = 1;
			  break;
		  }
	}

	if($delete_object) {
		$dn = pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["oldmx"].','.$_SESSION["USER_SEARCH_DN_CTR"];
		if(! ldap_delete($_pql_control->ldap_linkid, $dn)) {
			// Could not delete object
			$msg = urlencode("Failed to delete mailserver $host.");
			header("Location: " . pql_get_define("PQL_CONF_URI") . 
				   "control_detail.php?mxhost=".$_REQUEST["oldmx"]."&msg=$msg");
		} else {
			// Successfully deleted object
			$msg = urlencode("Successfully deleted mailserver $host.");
			header("Location: " . pql_get_define("PQL_CONF_URI") . "home.php?msg=$msg&rlnb=2");
		}
	}
}
// else - PQL_CONF_CONTROL_USE isn't set!

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
