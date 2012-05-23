<?php
// attribute plugin for
// sudo administration
// $Id: attrib.sudoers.inc,v 2.1 2007-02-26 09:45:05 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
  global $error, $LANG;

  if(($_REQUEST["sudodn"] == 'new') or ($_REQUEST["command"] == 'new'))
	unset($_REQUEST["dosave"]);

  if($_REQUEST["dosave"]) {
	if(!$_REQUEST["sudodn"])
	  $error['sudodn'] = $LANG->_('Missing');
	
	if(!$_REQUEST["userdn"])
	  $error['userdn'] = $LANG->_('Missing');
	
	if(!$_REQUEST["command"])
	  $error['command'] = $LANG->_('Missing');
	
	if(!$_REQUEST["runas"])
	  $error['runas'] = $LANG->_('Missing');
	
	if(!$_REQUEST["computer"])
	  $error['computer'] = $LANG->_('Missing');
	
	if(!is_array($error))
	  return true;
  }

  return false;
}
// }}}

// {{{ attribute_print_view(role)
function attribute_print_view($role) {
  global $LANG, $_pql;

  // Retreive the sudo role
  $sudo_role = $_pql->search($_REQUEST["sudodn"], 'objectClass=*', 'BASE');

  // Role  Users  Commands  Run As  Host
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
<?php
	// {{{ Sudo users
	$class = pql_format_table(0);
	$url_tmp  = "sudo_modify.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
	$url_tmp .= "&type=".pql_get_define("PQL_ATTR_SUDOUSER")."&view=".$_REQUEST["view"];
	$url_tmp .= "&sudodn=".urlencode($_REQUEST["sudodn"]);
	
	$tbl_txt = array('tit' => $LANG->_('Sudo users'),
					 'add' => $LANG->_('Add user to role'),
					 'del' => $LANG->_('Delete user from role'),
					 'mod' => $LANG->_('Modify/change user'));
	$tbl_url = array('add' => $url_tmp."&action=add",
					 'del' => $url_tmp."&action=del",
					 'mod' => $url_tmp."&action=mod");
	pql_format_table_single($tbl_txt, $tbl_url, $sudo_role[pql_get_define("PQL_ATTR_SUDOUSER")],
							pql_get_define("PQL_ATTR_SUDOUSER"), $class, 4);
	// }}}

	// {{{ Sudo commands
	$class = pql_format_table(0);
	$url_tmp  = "sudo_modify.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
	$url_tmp .= "&type=".pql_get_define("PQL_ATTR_SUDOCOMMAND")."&view=".$_REQUEST["view"];
	$url_tmp .= "&sudodn=".urlencode($_REQUEST["sudodn"]);
	
	$tbl_txt = array('tit' => $LANG->_('Sudo commands'),
					 'add' => $LANG->_('Add command to role'),
					 'del' => $LANG->_('Delete command from role'),
					 'mod' => $LANG->_('Modify/change command'));
	$tbl_url = array('add' => $url_tmp."&action=add",
					 'del' => $url_tmp."&action=del",
					 'mod' => $url_tmp."&action=mod");
	pql_format_table_single($tbl_txt, $tbl_url, $sudo_role[pql_get_define("PQL_ATTR_SUDOCOMMAND")],
							pql_get_define("PQL_ATTR_SUDOCOMMAND"), $class, 4);
	// }}}

	// {{{ Sudo run as
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Run command as')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $sudo_role[pql_get_define("PQL_ATTR_SUDORUNAS")]?></td>
        <td>
          <a href="sudo_modify.php?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?php echo urlencode($_REQUEST["domain"])?>&type=<?php echo pql_get_define("PQL_ATTR_SUDORUNAS")?>&view=sudo&sudodn=<?php echo urlencode($_REQUEST["sudodn"])?>&action=mod&<?php echo pql_get_define("PQL_ATTR_SUDORUNAS")?>=<?php echo $sudo_role[pql_get_define("PQL_ATTR_SUDORUNAS")]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify/change run as"></a>
          <a href="sudo_modify.php?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?php echo urlencode($_REQUEST["domain"])?>&type=<?php echo pql_get_define("PQL_ATTR_SUDORUNAS")?>&view=sudo&sudodn=<?php echo urlencode($_REQUEST["sudodn"])?>&action=del&<?php echo pql_get_define("PQL_ATTR_SUDORUNAS")?>=<?php echo $sudo_role[pql_get_define("PQL_ATTR_SUDORUNAS")]?>"><img src="images/del.png"  width="12" height="12" border="0" alt="Delete command from role"></a>
        </td>
      </tr>

<?php
	// }}}

	// {{{ Sudo hosts
	$class = pql_format_table(0);
	$url_tmp  = "sudo_modify.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
	$url_tmp .= "&type=".pql_get_define("PQL_ATTR_SUDOHOST")."&view=".$_REQUEST["view"];
	$url_tmp .= "&sudodn=".urlencode($_REQUEST["sudodn"]);
	
	$tbl_txt = array('tit' => $LANG->_('Sudo hosts'),
					 'add' => $LANG->_('Add host to role'),
					 'del' => $LANG->_('Delete host from role'),
					 'mod' => $LANG->_('Modify/change host'));
	$tbl_url = array('add' => $url_tmp."&action=add",
					 'del' => $url_tmp."&action=del",
					 'mod' => $url_tmp."&action=mod");
	pql_format_table_single($tbl_txt, $tbl_url, $sudo_role[pql_get_define("PQL_ATTR_SUDOHOST")],
							pql_get_define("PQL_ATTR_SUDOHOST"), $class, 4);
	// }}}
?>
    </th>
  </table>
<?php
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
  global $LANG, $_pql, $user_results, $computer_results, $sudo_results, $error;
  
  // {{{ Retreive the sudo role
  // 1.  We're called with a sudo DN
  // 2a. We're modfying an attribute
  // 2b. We're adding a value to exist attribute
  if($_REQUEST["sudodn"] and
	 (($_REQUEST["action"] == 'mod') or
	  (($_REQUEST["action"] == 'add') and $_REQUEST["type"])))
	$sudo_role = $_pql->search($_REQUEST["sudodn"], 'objectClass=*', 'BASE');
  // }}}


  // {{{ Form header
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Create Sudo Roles')?>
        <tr class="c2">
          <td class="title"><?php echo $LANG->_("Role")?></td>
          <td class="title"><?php echo $LANG->_("User")?></td>
          <td class="title"><?php echo $LANG->_("Command")?></td>
          <td class="title"><?php echo $LANG->_("As User")?></td>
          <td class="title"><?php echo $LANG->_("On Host")?></td>
        </tr>

        <tr class="c2">
<?php
// }}}


  // {{{ Sudo role
?>
          <!-- SUDO ROLE -->
          <td>
            <?php echo pql_format_error_span($error["sudodn"])?>
<?php if(@$sudo_role["cn"]) {
		echo $sudo_role["cn"];
?>
            <input type="hidden" name="sudodn" value="<?php echo $sudo_role["dn"]?>">
<?php  } else {
		if(!is_array($sudo_results) or ($_REQUEST["sudodn"] == 'new')) {
		  if($_REQUEST["sudodn"] == 'new') {
			unset($_REQUEST["sudodn"]);
		  }
?>
            <input type="text" name="sudodn" size="20" value="<?php echo $_REQUEST["sudodn"]?>">
<?php	} elseif($_REQUEST["sudodn"]) {
		  // Just in case we choose 'New' on 'SudoCommand' and this is specified...
?>
            <input type="text" name="sudodn" size="20" value="<?php echo $_REQUEST["sudodn"]?>">
<?php	} else { ?>
            <select name="sudodn" onChange="this.form.submit()">
<?php	  for($r = 0; $r < count($sudo_results); $r++) { ?>
              <option value="<?php echo $sudo_results[$r]['dn']?>"<?php if($_REQUEST["sudodn"] == $sudo_results[$r]['dn']) { echo " SELECTED"; }?>><?php echo $sudo_results[$r][pql_get_define("PQL_ATTR_CN")]?></option>
<?php	  } ?>
              <option value="new"><?php echo $LANG->_('New')?></option>
            </select>
<?php	}
	  }
?>
          </td>
<?php
// }}}

  // {{{ Sudo user
?>

          <!-- SUDO USER -->
          <td>
            <?php echo pql_format_error_span($error["userdn"])?>
<?php if($_REQUEST["action"] and ($_REQUEST["type"] == 'sudouser')) {
		// ADDING a value (to existing attribute)
		// CHANGING a sudo user

		// {{{ Retreive all users
		if($_REQUEST["domain"]) {
		  if(pql_get_define("PQL_CONF_SUBTREE_USERS"))
		  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ",";
		  $userdn = $subrdn . $_REQUEST["domain"];
		  $filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
		  $users = $_pql->get_dn($userdn, $filter);
		  
		  // Extract 'human readable' name from the user DN's found
		  $user_results = pql_left_htmlify_userlist($_REQUEST["rootdn"], $_REQUEST["domain"],
													$userdn, $users, ($links = NULL));
		}
// }}}
	  } elseif($_REQUEST["type"]) {
		// {{{ Retreive the CN and DN of the user
		$subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ",";
		$userdn = $subrdn . $_REQUEST["domain"];
		$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=".$sudo_role["sudouser"];
		$user   = $_pql->get_dn($userdn, $filter);

		// Extract 'human readable' name from the user DN's found
		$user_results = pql_left_htmlify_userlist($_REQUEST["rootdn"], $_REQUEST["domain"],
												  $userdn, $user, ($links = NULL));

		foreach($user_results as $user_results => $user_cn)
		  // We only have one value here, so we break right away...
		  // We need the $user_results to be the DN and $user_cn to be the Common Name for output
		  break;
		// }}}
	  } // else
	  // ADDING a sudo role...

	  if(is_array($user_results)) {
?>
            <select name='userdn'>
              <option value="none"<?php if($_REQUEST["userdn"] == 'none') { echo " SELECTED"; } ?>><?php echo $LANG->_("None")?></option>
              <option value="ALL"<?php if($_REQUEST["userdn"] == 'ALL') { echo " SELECTED"; } ?>><?php echo $LANG->_("ALL")?></option>
<?php	foreach($user_results as $dn => $user) { ?>
              <option value="<?php echo $dn?>"<?php if($_REQUEST["userdn"] == $dn) { echo " SELECTED"; } ?>><?php echo $user?></option>
<?php	} ?>
            </select>
<?php } elseif($user_results) {
		if($_REQUEST["action"] and $_REQUEST["type"]) {
		  // ADDING a value (to existing attribute)
		  if($user_cn) {
			echo $user_cn;
?>
<?php	  } else {
			echo $user_results;
		  }
?>
            <input type="hidden" name="userdn" value="<?php echo $user_results?>">
<?php	} else { ?>
            <input type="text"   name="userdn" value="<?php echo $user_results?>">
<?php	}
	  }
?>
          </td>
<?php
// }}}

  // {{{ Sudo command
?>

          <!-- SUDO COMMAND -->
          <td>
            <?php echo pql_format_error_span($error["command"])?>
<?php if(@$sudo_role["sudocommand"] and ($_REQUEST["type"] == 'sudocommand')) {
		// Changing sudo command value
?>
            <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["sudocommand"]?>">
            <input type="text"   name="command"  value="<?php echo $_REQUEST["sudocommand"]?>">
<?php } elseif(is_array($sudo_role)) {
		// Changing something else than sudo command...
		echo $sudo_role["sudocommand"];
?>
            <input type="hidden" name="command" value="<?php echo $sudo_role["sudocommand"]?>">
<?php  } else {
		// ADDING a sudo role...
		if(!is_array($sudo_results) or ($_REQUEST["command"] == 'new')) {
			// Adding a new sudo command
			if($_REQUEST["command"] == 'new')
			  unset($_REQUEST["command"]);
?>
            <input type="text" name="command" size="20" value="<?php echo $_REQUEST["command"]?>">
<?php	} elseif(is_array($sudo_results)) { ?>
            <select name="command" onChange="this.form.submit()">
<?php		for($r = 0; $r < count($sudo_results); $r++) { ?>
              <option value="<?php echo $sudo_results[$r]["sudocommand"]?>"<?php if($_REQUEST["command"] == $sudo_results[$r]["sudocommand"]) { echo " SELECTED"; }?>><?php echo $sudo_results[$r]["sudocommand"]?></option>
<?php		} ?>
              <option value="new"<?php if($_REQUEST["command"] == "new") { echo " SELECTED"; }?>><?php echo $LANG->_('New')?></option>
            </select>
<?php	}
	  }
?>
          </td>
<?php
// }}}

  // {{{ Sudo run as
?>

          <!-- SUDO RUN AS -->
          <td>
            <?php echo pql_format_error_span($error["runas"])?>
<?php if(@$sudo_role["sudorunas"] and ($_REQUEST["type"] == 'sudorunas')) {
		// Changing sudo run-as value
?>
            <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["sudorunas"]?>">
            <input type="text"   name="runas"    value="<?php echo $_REQUEST["sudorunas"]?>">
<?php } elseif(is_array($sudo_role)) {
		// Changing something else than sudo run-as...
		echo $sudo_role["sudorunas"];
?>
            <input type="hidden" name="runas" value="<?php echo $sudo_role["sudorunas"]?>">
<?php } else { ?>
            <input type="text" name="runas" size="20" value="<?php echo $_REQUEST["runas"]?>">
<?php } ?>
          </td>
<?php
// }}}

  // {{{ Sudo host
?>
          <!-- SUDO HOST -->
          <td>
            <?php echo pql_format_error_span($error["computer"])?>
<?php if($_REQUEST["action"] and ($_REQUEST["type"] == 'sudohost')) {
		// ADDING a value (to existing attribute)
		
		// {{{ Retreive all computers
		if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"))
		$subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ",";
		$computer_results = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
										  '(&(cn=*)(objectclass=ipHost)(ipHostNumber=*))');
		if(is_array($computer_results))
		sort($computer_results);
// }}}
	  } elseif($_REQUEST["type"]) {
		$computer_results = $sudo_role["sudohost"];
	  } // else
	  // ADDING a sudo role...

	  if(is_array($computer_results)) {
?>
            <select name="computer">
              <option value="ALL"<?php if($_REQUEST["computer"] == 'ALL') { echo " SELECTED"; } ?>><?php echo $LANG->_("ALL")?></option>
<?php	for($i=0; $i < count($computer_results); $i++) {
			$host = $_pql->get_attribute($computer_results[$i], pql_get_define("PQL_ATTR_CN"));
?>
              <option value="<?php echo $host?>"<?php if($_REQUEST["computer"] == $host) { echo " SELECTED"; } ?>><?php echo $host?></option>
<?php	} ?>
            </select>
<?php } elseif($computer_results) {
		if($_REQUEST["action"] and $_REQUEST["type"]) {
		  // ADDING a value (to existing attribute)
		  echo $computer_results;
?>
            <input type="hidden" name="computer" value="<?php echo $computer_results?>">
<?php	} else { ?>
            <input type="text"   name="computer" value="<?php echo $computer_results?>">
<?php	}
	  }
?>
          </td>
<?php
// }}}


  // {{{ Form trailer
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="dosave" value="1">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="view"   value="sudo">
<?php if($_REQUEST["type"]) { ?>
    <input type="hidden" name="type"   value="<?php echo $_REQUEST["type"]?>">
<?php } ?>
    <input type="hidden" name="rootdn" value="<?php echo $_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain" value="<?php echo $_REQUEST["domain"]?>">
    <br>
<?php if($_REQUEST["action"] and $_REQUEST["type"]) { ?>
    <input type="submit" value="<?php echo $LANG->_('Modify Sudo Role')?>">
<?php } else { ?>
    <input type="submit" value="<?php echo $LANG->_('Add New Sudo Role')?>">
<?php } ?>
  </form>
<?php
// }}}
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
  global $_pql, $LANG;
  
  $mapping = array(pql_get_define("PQL_ATTR_SUDOUSER")    => 'sudouser',
				   pql_get_define("PQL_ATTR_SUDOCOMMAND") => 'command',
				   pql_get_define("PQL_ATTR_SUDORUNAS")   => 'runas',
				   pql_get_define("PQL_ATTR_SUDOHOST")    => 'computer');

  switch($type) {
  case "add":
	// {{{ Add a sudo role (or add a value to existing attribute)
	if(preg_match('/=/', $_REQUEST["sudodn"])) {
	  // Probably a DN - does it exist?
	  if(!(($object = $_pql->search($_REQUEST["sudodn"], 'objectClass=*', 'BASE')) != true)) {
		// {{{ It does - ADD options to this sudo role
		$dn = $_REQUEST['sudodn'];

		// {{{ Setup the array with NEW values
		$_REQUEST["sudouser"] = $_pql->get_attribute($_REQUEST["userdn"],
													 pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]));
		foreach($mapping as $attribute => $key)
		  $MODIFY[$attribute] = $_REQUEST[$key];
		// }}}

		// {{{ See if we should modify the sudoUser attribute
		foreach($mapping as $attrib => $key) {
		  $add = 1; $values = array();
		  if(is_array($object[$attrib])) {
			for($i=0; $object[$attrib][$i]; $i++) {
			  if($object[$attrib][$i] == $_REQUEST[$key])
				$add = 0;
			  $values[] = $object[$attrib][$i];
			}
		  } elseif(@$object[$attrib]) {
			if($object[$attrib] == $_REQUEST[$key])
			  $add = 0;
			$values[] = $object[$attrib];
		  }
		  
		  if($add and is_array($values)) {
			$values[] = $_REQUEST[$key]; // Add the new value
			$entry[$attrib] = $values;
		  }
		}
		// }}}

		if(pql_modify_attribute($dn, '', '', $entry))
		  $msg = pql_complete_constant($LANG->_('Modified sudo role %role%'), array('role' => $dn));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to modify sudo role %role%'), array('role' => $dn));
		// }}}
	  }
	} else {
	  // {{{ Add NEW sudo role
	  if(pql_sudo_add_role($_REQUEST["domain"],  $_REQUEST['sudodn'], $_REQUEST['userdn'],
						   $_REQUEST['command'], $_REQUEST['runas'],  $_REQUEST['computer']))
		$msg = pql_complete_constant($LANG->_("New sudo role %role% has been successfully added"),
									 array('role' => $_REQUEST['sudodn']));
	  else
		$msg = pql_complete_constant($LANG->_("Failed to add new sudo role %role%"),
									 array('role' => $_REQUEST['sudodn']));
// }}}
	}
	break;
	// }}}

  case "mod":
	// {{{ Modify a sudo role
	// Retreive sudo role

	// {{{ There's a discrepancy between $_REQUEST value and attribute... Translate!
	$type = $_REQUEST["type"];
	if($type == 'sudouser')
	  $value = 'userdn';
	if($type == 'sudocommand')
	  $value = 'command';
	if($type == 'sudorunas')
	  $value = 'runas';
	if($type == 'sudohost')
	  $value = 'computer';
	// }}}

	$old = $_pql->get_attribute($_REQUEST["sudodn"], $type);
	if(is_array($old)) {
	  for($i=0; $old[$i]; $i++) {
		if($old[$i] != $_REQUEST[$value])
		  // Not what we're looking for - add old value.
		  $entry[$type][] = $old[$i];
	  }
	} elseif($old and ($old != $_REQUEST[$value])) {
	  // Not what we're looking for - add old value.
	  $entry[$type][] = $old;
	}

	if(($_REQUEST["type"] == 'sudouser') and preg_match('/=/', $_REQUEST["userdn"])) {
	  // Probably a DN - does it exist?
	  if(!(($object = $_pql->search($_REQUEST["userdn"], 'objectClass=*', 'BASE')) != true))
		// Yes. Extract the uid
		$entry[$type][] = $_pql->get_attribute($_REQUEST["userdn"], pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]));
	  else
		// Add new value to array (possibly with old values)
		$entry[$type][] = $_REQUEST[$value];
	}

	if(pql_modify_attribute($_REQUEST["sudodn"], '', '', $entry))
	  $msg = pql_complete_constant($LANG->_('Modified sudo role %role%'), array('role' => $dn));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to modify sudo role %role%'), array('role' => $dn));
	break;
	// }}}
	  
  case "del":
	if($_REQUEST["type"]) {
	  // {{{ Remove attribute from sudo role
	  $type = $_REQUEST["type"];
	  if(pql_modify_attribute($_REQUEST['sudodn'], $type, $_REQUEST[$type], ''))
		$msg = pql_complete_constant($LANG->_("Successfully removed %what% from %where%"),
									 array('what'  => $_REQUEST[$_REQUEST['attribute']],
										   'where' => $_REQUEST['sudodn']));
	  else
		$msg = pql_complete_constant($LANG->_("Failed to remove %what% from %where%"),
									 array('what'  => $_REQUEST[$_REQUEST['attribute']],
										   'where' => $_REQUEST['sudodn']));
	  // }}}
	} else {
	  // {{{ Delete whole sudo role
	  if($_pql->delete($_REQUEST["sudodn"]))
		$msg = pql_complete_constant($LANG->_("Successfully removed sudo role %role%"),
									 array('role' => $_REQUEST['sudodn']));
	  else
		$msg = pql_complete_constant($LANG->_("Failed to remove sudo role %role%"),
									 array('role' => $_REQUEST['sudodn']));
	  // }}}
	}
	break;

  }

  attribute_forward(urlencode($msg));
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
