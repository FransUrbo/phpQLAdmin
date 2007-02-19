<?php
require($_SESSION["path"]."/include/pql_hostctrl.inc");
if(isset($_REQUEST['action']) && $_REQUEST['action'] == 'add_sudo_role') {
  // {{{ Add sudo role
  $domain = $_REQUEST["domain"];

  if(isset($_REQUEST['role']) && $_REQUEST['role'] == '') {
	pql_format_status_msg($LANG->_("Failed To Add New Sudo Role, Must Define The Role."));
  } else {
	if(pql_add_sudo_role($domain, $_REQUEST['role'], $_REQUEST['userdn'],
						 $_REQUEST['command'], $_REQUEST['runas'], $_REQUEST['computer'] )) {
	  pql_format_status_msg(pql_complete_constant($LANG->_("New Sudo Role: %role% Has Been Added Successfully"),
												  array('role' => $_REQUEST['role'])));
	} else {
	  pql_format_status_msg(pql_complete_constant($LANG->_("Failed To Add New Sudo Role: %role%"),
												  array('role' => $_REQUEST['role'])));
	}
  }
// }}}
} elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'remove_attribute_from_sudoRole') {
  // {{{ Remove attribute from sudo role
  //print_r($_REQUEST);
  if(pql_modify_attribute($_REQUEST['sudodn'], $_REQUEST['attribute'], 
						  $_REQUEST[$_REQUEST['attribute']], '')){
	pql_format_status_msg(pql_complete_constant($LANG->_("Successfully removed %what% from %where%"),
												array('what'  => $_REQUEST[$_REQUEST['attribute']],
													  'where' => $_REQUEST['sudodn'])));
  } else {
	pql_format_status_msg(pql_complete_constant("Failed to remove %what% from %where%"),
						  array('what'  => $_REQUEST[$_REQUEST['attribute']],
								'where' => $_REQUEST['sudodn']));
  }
// }}}
} elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'update_sudo_role') {
  // {{{ Update sudo role
  $dn = $_REQUEST['roledn'];
  
  pql_update_sudo($dn, 'userdn',   pql_get_define("PQL_ATTR_SUDOUSER"));
  pql_update_sudo($dn, 'command',  pql_get_define("PQL_ATTR_SUDOCOMMAND"));
  pql_update_sudo($dn, 'user',     pql_get_define("PQL_ATTR_SUDORUNAS"));
  pql_update_sudo($dn, 'computer', pql_get_define("PQL_ATTR_SUDOHOST"));
  // }}}
}

// {{{ Retreive all users
if($_REQUEST["domain"]) {
  if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ",";
  }
  $userdn = $subrdn . $_GET["domain"];
  $filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
  $users = $_pql->get_dn($userdn, $filter);

  // Extract 'human readable' name from the user DN's found
  $user_results = pql_left_htmlify_userlist($_REQUEST["rootdn"], $_REQUEST["domain"],
											$userdn, $users, ($links = NULL));
}
// }}}

// {{{ Retreive all computers
if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS")) {
  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ",";
}
$computer_results = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
								  '(&(cn=*)(objectclass=ipHost)(ipHostNumber=*))');
if(is_array($computer_results)) {
  sort($computer_results);
}
// }}}

// {{{ Retreive all sudo roles
if($_REQUEST["domain"]) {
  if(pql_get_define("PQL_CONF_SUBTREE_SUDOERS")) {
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_SUDOERS") . ", ";
  }
  $sudodn = $subrdn . $_REQUEST["domain"];
  $filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=sudoRole';
  $sudo_results = $_pql->search($sudodn, $filter);
  if(is_array($sudo_results))
	asort($sudo_results);
} elseif(($_REQUEST["host"] == 'Global') and ($_REQUEST["ref"] == 'global')) {
  // Called from Computers->Global->Sudo Administration

  // Setup a filter containing all physical hosts we know about
  $filter = '(&(objectClass=sudoRole)(cn=*)(|';
  for($i=0; $computer_results[$i]; $i++) {
	// Get the FQDN from the host DN
	$physical = $_pql->get_attribute($computer_results[$i], pql_get_define("PQL_ATTR_CN"));
	$filter .= "(sudoHost=$physical)";
  }
  $filter .= '))';

  foreach($_pql->ldap_basedn as $dn)  {
	$dn  = pql_format_normalize_dn($dn);
	$tmp = $_pql->get_dn($dn, $filter);
	pql_add2array($sudo_results, $tmp);
  }

  if(is_array($sudo_results)) {
	// Get ALL information about these SUDO roles
	sort($sudo_results);
	for($i=0; $sudo_results[$i]; $i++) {
	  $roles[] = $_pql->search($sudo_results[$i], 'objectClass=*', 'BASE');
	}
  }

  $sudo_results = $roles;
} else {
  // Called from physical host details->Sudo Administration

  // Get the FQDN from the host DN
  $physical = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
  $filter = "(&(objectClass=sudoRole)(cn=*)(sudoHost=$physical))";
  $sudo_results = array();
  foreach($_pql->ldap_basedn as $dn)  {
	$dn  = pql_format_normalize_dn($dn);
	$tmp = $_pql->get_dn($dn, $filter);
	pql_add2array($sudo_results, $tmp);
  }

  if(is_array($sudo_results)) {
	// Get ALL information about these SUDO roles
	sort($sudo_results);
	for($i=0; $sudo_results[$i]; $i++) {
	  $roles[] = $_pql->search($sudo_results[$i], 'objectClass=*', 'BASE');
	}
  }

  $sudo_results = $roles;
}

if(is_array($sudo_results) and !@$sudo_results[0]) {
  // Make sure it's a numbered array...
  $tmp = $sudo_results;
  unset($sudo_results);
  $sudo_results[] = $tmp;
}
// }}}

if($_REQUEST["domain"]) {
?>
  <form method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Create Sudo Roles')?>
        <tr class="c2">
          <td class="title"><?=$LANG->_("Role")?></td>
          <td class="title"><?=$LANG->_("User")?></td>
          <td class="title"><?=$LANG->_("Command")?></td>
          <td class="title"><?=$LANG->_("As User")?></td>
          <td class="title"><?=$LANG->_("On Host")?></td>
        </tr>

        <tr class="c2">
          <td><input type="text" name="role" size="20"></td>
          <td>
            <select name='userdn'>
              <option value='none'><?=$LANG->_("None")?></option>
              <option value='ALL'><?=$LANG->_("ALL")?></option>
<?php foreach($user_results as $dn => $user) {
		print "              <option value='$dn'>$user</option>\n";
      }
      print "            </select></td>\n";
?>
          <td><input type="text" name="command" size="20"></td>
          <td><input type="text" name="runas"   size="20"></td>
          <td>
            <select name="computer">
              <option value='ALL'><?=$LANG->_("ALL")?></option>
<?php if(is_array($computer_results)) {
		for($i=0; $i < count($computer_results); $i++) {
		  $host = $_pql->get_attribute($computer_results[$i], pql_get_define("PQL_ATTR_CN"));
		  print "              <option value='" . $host . "'>" . $host . "</option>\n";
		}
      }
?>
            </select>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="view" value="sudo">
    <input type="hidden" name="action" value="add_sudo_role">
    <input type="submit" name="Submit" value="<?=$LANG->_("Add New Sudo Role")?>">
  </form>

<?php
}

if(is_array($sudo_results)) {
	  if($_REQUEST["domain"]) {
?>
  <form method="post">
<?php } ?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Current Sudo Roles")."\n"?>
        <tr class="c2">
          <td class="title"><?=$LANG->_("Role")?></td>
          <td class="title"><?=$LANG->_("Users")?></td>
          <td class="title"><?=$LANG->_("Commands")?></td>
          <td class="title"><?=$LANG->_("Run As")?></td>
          <td class="title"><?=$LANG->_("Host")?></td>
        </tr>

<?php	$row = 'c1';
		for($i=0; $i < count($sudo_results); $i++) {
		  $sudodn   = $sudo_results[$i]['dn'];
		  $users    = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDOUSER")];
		  $commands = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDOCOMMAND")];
		  $runas    = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDORUNAS")];
		  $host     = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDOHOST")];
		  $role     = $sudo_results[$i][pql_get_define("PQL_ATTR_CN")];
		  print "        <tr class=\"". $row . "\">\n";
		  print "          <td class=\"title\">" . $role . "</td>";
		  
		  // To list the Users
		  print "          <td>";
		  pql_list_expand($role, $users, pql_get_define("PQL_ATTR_SUDOUSER"), 
						  "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
		  print "</td>";
		  
		  // To list the Commands
		  print "          <td>";
		  pql_list_expand($role, $commands, pql_get_define("PQL_ATTR_SUDOCOMMAND"), 
						  "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
		  print "</td>";
		  
		  // To list the runas
		  print "          <td>";
		  pql_list_expand($role, $runas, pql_get_define("PQL_ATTR_SUDORUNAS"), 
						  "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
		  print "</td>";
		  
		  // To list the runas
		  print "          <td>";
		  pql_list_expand($role, $host, pql_get_define("PQL_ATTR_SUDOHOST"), 
						  "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
		  print "</td>";
		  
		  print "        </tr>";
		  
		  if($row == 'c1')
			$row = 'c2';
		  else
			$row = 'c1';
		}
									
		if($_REQUEST["domain"]) {
		  print "        <tr class='c2'>";
		  print "          <td class='title'>\n";
		  print "            <select name='roledn'>\n";
		  for($r = 0; $r < count($sudo_results); $r++) {
			print "            <option value='" . $sudo_results[$r]['dn'] . "'>";
			print  $sudo_results[$r][pql_get_define("PQL_ATTR_CN")] . "</option>\n";
		  }
?>
            </select>
          </td>
          <td>
            <select name='userdn'>
              <option value='none'><?=$LANG->_("None")?></option>
              <option value='ALL'><?=$LANG->_("ALL")?></option>
<?php	  foreach($user_results as $dn => $user) {
			print "              <option value='$user'>$user</option>\n";
		  }
		  print "            </select>\n          </td>\n";
?>
          <td><input type="text" name="command" size="20"></td>
          <td><input type="text" name="user" size="20"></td>
          <td>
            <select name="computer">
              <option value="none"><?=$LANG->_("None")?></option>
              <option value="ALL"><?=$LANG->_("ALL")?></option>
<?php	  for($i=0; $i < count($computer_results); $i++) {
			$host = $computer_results[$i][pql_get_define("PQL_ATTR_CN")];
			print "              <option value='" . $host . "'>" . $host . "</option>\n";
		  }
?>
            </select>
          </td>
        </tr>
<?php	} else { ?>
        <tr class="c1">
          <td colspan="5">
            <img src="images/info.png" width="16" height="16" alt="" border="0">
            <?=$LANG->_('Note that you will have to go into a branch to \uadd\U users to a host.')?>
          </td>
        </tr>
<?php	} ?>
      </th>
    </table>
<?php if($_REQUEST["domain"]) { ?>

    <input type="hidden" name="view"   value="sudo">
    <input type="hidden" name="action" value="update_sudo_role">
    <input type="submit" name="Submit" value="<?=$LANG->_('Update Sudo Role')?>">
  </form>
<?php }
} else {
  // No sudo rules!
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Current Sudo Roles")."\n"?>
        <tr class="c1">
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0">
          </td>

          <td>
            <table>
              <th align="left">
                <tr>
                  <?=$LANG->_('No sudo rules for this host.');?><br>
                  <?=$LANG->_('Note that you will have to go into a branch to \uadd\U users to a host.')?>
                </tr>
              </th>
            </table>
          </td>
        </tr>
      </th>
    </table>
<?php
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
