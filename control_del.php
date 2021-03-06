<?php
// Delete a mailserver controls object
// $Id: control_del.php,v 2.29 2007-03-14 12:10:50 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

// {{{ Get users that uses this mailserver - we need to know this twice.
// Once the first time we're called, and once when the acctuall deletion
// will take place.
// Therefor a little function here to simplify, and avoid duplication.
// I'm lazy, so sue me! :)
function control_del_find_users($host) {
  global $_pql;

  // See if there's any users that have this host as mailHost
  $filter = pql_get_define("PQL_ATTR_MAILHOST").'='.$host;
  foreach($_SESSION["BASE_DN"] as $dn) {
	$usrs = $_pql->search($dn, $filter);
	if(is_array($usrs)) {
	  for($i=0; $i < count($usrs); $i++)
		$users[] = $usrs[$i];
	}
  }
  
  return($users);
}
// }}}

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // {{{ include control api if control is used
    include($_SESSION["path"]."/include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
// }}}

	if(!$_REQUEST["submit"] or $_REQUEST["error"]) {
		include($_SESSION["path"]."/header.html");
		
		// Check to see if this object exists
		// TODO: Replace with '$_pql_control->search()'
		$filter = "(".pql_get_define("PQL_ATTR_CN") . "=" . $_REQUEST["mxhost"] . ")";
		$sr = ldap_search($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter,
						  array(pql_get_define("PQL_ATTR_MAILHOST")), "BASE");
		if(ldap_count_entries($_pql_control->ldap_linkid, $sr) > 0) {
			// Exists - get DN
			$dn = ldap_get_dn($_pql_control->ldap_linkid,
							  ldap_first_entry($_pql_control->ldap_linkid, $sr));

			$users = control_del_find_users($_REQUEST["mxhost"]);
			if(is_array($users)) {
				// There's users - get MX and QmailLDAP/Controls objects
				$result = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
												'(&(cn=*)(objectclass=qmailControl))');
				for($i=0; $i < count($result); $i++)
				  $hosts[] = $_pql_control->get_attribute($result[$i], pql_get_define("PQL_ATTR_CN"));
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="GET">
    <input type="hidden" name="oldmx"  value="<?php echo $_REQUEST["mxhost"]?>">

    <span class="title2"><?php echo $LANG->_("There's users on this mail server!<br>What would you like to do with them?")?></span><br>
    <input type="radio"  name="action" value="ignore" CHECKED><?php echo $LANG->_('Ignore')?><br>
    <input type="radio"  name="action" value="delete"><?php echo $LANG->_('Delete all users')?><br>
    <input type="radio"  name="action" value="move" <?php if($_REQUEST["error"]) { ?>CHECKED<?php } ?>>
      <?php echo $LANG->_('Move users to server:')?><br>
<?php		if($_REQUEST["error"]) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
				echo pql_format_error_span($LANG->_("Hostname missing!"));
				echo "<br>";
			}

			for($i=0; $i < count($hosts); $i++) {
				if($hosts[$i] != $_REQUEST["mxhost"]) {
					$host = pql_maybe_idna_decode($hosts[$i]);
?>
    &nbsp;&nbsp;&nbsp;&nbsp;<input type="radio"  name="newmx"  value="<?php echo $host?>"><b><?php echo $host?></b><br>
<?php			}
			}
?>
    &nbsp;&nbsp;&nbsp;&nbsp;<input type="radio"  name="newmx"  value="user"><?php echo $LANG->_('User specified')?>
                            <input type="text"   name="mxhost" value="<?php echo $_REQUEST["newmx"]?>" size="30"><br>
    <br>
    <input type="submit" name="submit" value="<?php echo "--&gt;&gt;"?>">
  </form>
  <table cellspacing="0" cellpadding="3" border="0">
    <th>
      <tr class="<?php pql_format_table(); ?>">
        <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td><?php echo $LANG->_('NOTE: Deleting or moving a user will not touch their mail- or homedirectory! You will have to delete/move them to the new server manually.')?></td>
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
			  // {{{ Don't touch the users - delete object directly
			  $delete_object = 1;
			  break;
			  // }}}

			case "delete":
			  // {{{ 1. Delete all users
			  $users = control_del_find_users($_REQUEST["oldmx"]);
			  if(is_array($users)) {
				  for($i=0; $i < count($users); $i++)
					pql_user_del($_REQUEST["domain"], $users[$i], 1);
			  }
			  // }}}

			  // {{{ 2. Delete object
			  $delete_object = 1;
			  break;
			  // }}}

			case "move":
			  // {{{ 1. Move user(s) to other MX/Host. Where?
			  if($_REQUEST["newmx"] == "") {
				  // We haven't specified a (pre-defined) MX/host to move the users to - do over!
				  $oldmx = $_REQUEST["oldmx"]; unset($_REQUEST);
				  $url = $_SERVER["PHP_SELF"]."?mxhost=$oldmx&error=1";

				  pql_header($url, 1);
			  } else {
				  if(($_REQUEST["newmx"] == 'user') and !$_REQUEST["mxhost"]) {
					  // No (user specified) MX/host specified - do over!
					  $oldmx = $_REQUEST["oldmx"]; unset($_REQUEST);
					  $url = $_SERVER["PHP_SELF"]."?mxhost=$oldmx&error=1";

					  pql_header($url, 1);
				  } else {
					  // Move the user(s) to user specified server
					  // OR
					  // Move the user(s) to specified server
					  if($_REQUEST["newmx"] == 'user')
						$host = $_REQUEST["mxhost"];
					  else
						$host = $_REQUEST["newmx"];

					  $users = control_del_find_users($_REQUEST["oldmx"]);
					  if(is_array($users)) {
						  for($i=0; $i < count($users); $i++) {
							  pql_modify_attribute($users[$i],
												   pql_get_define("PQL_ATTR_MAILHOST"),
												   '', $host);
						  }
					  }
				  }
			  }
			  // }}}

			  // {{{ 2. Delete object
			  $delete_object = 1;
			  break;
			  // }}}
		  }
	}

	// {{{ delete object
	if($delete_object) {
	  if($_REQUEST["mxhost"])
		$mxhost = $_REQUEST["mxhost"];
	  elseif($_REQUEST["oldmx"]) {
		$mxhost = $_REQUEST["oldmx"];

		$dn = pql_get_define("PQL_ATTR_CN").'='.$mxhost.','.$_SESSION["USER_SEARCH_DN_CTR"];
	  }

	  if(pql_get_define("PQL_CONF_DEBUG_ME"))
		die("I'm debugging, so '<b>$dn</b>' isn't deleted!");
	  else {
		if(! $_pql_control->delete($dn)) {
		  // Could not delete object
		  $url = "control_detail.php?mxhost=".$mxhost."&msg=";
		  $url .= urlencode(pql_complete_constant($LANG->_("Failed to delete mailserver %host%.",
														   array('host' => $mxhost))));
		} else {
		  // Successfully deleted object
		  $url = "home.php?rlnb=2&msg=";
		  $url .= urlencode(pql_complete_constant($LANG->_("Successfully deleted mailserver %host%.",
														   array('host' => $mxhost))));
		}

		pql_header($url);
	  }
	}
	// }}}
}
// else - PQL_CONF_CONTROL_USE isn't set!
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
