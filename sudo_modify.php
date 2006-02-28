<?php if(isset($_REQUEST['action']) && $_REQUEST['action'] == 'add_sudo_role') {
	$domain = $_REQUEST["domain"];

	if(isset($_REQUEST['role']) && $_REQUEST['role'] == '') {
	  pql_format_status_msg($LANG->_("Failed To Add New Sudo Role, Must Define The Role."));
	} else {
	  if(pql_add_sudo_role($_pql->ldap_linkid, $domain, $_REQUEST['role'], $_REQUEST['userdn'],
			       $_REQUEST['command'], $_REQUEST['runas'], $_REQUEST[''] )) {
	    pql_format_status_msg(pql_complete_constant($LANG->_("New Sudo Role: %role% Has Been Added Successfully"),
							array('role' => $_REQUEST['role'])));
	  } else {
	    pql_format_status_msg(pql_complete_constant($LANG->_("Failed To Add New Sudo Role: %role%"),
							array('role' => $_REQUEST['role'])));
	  }
	}
      } elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'remove_attribute_from_sudoRole') {
	//print_r($_REQUEST);
	if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST['sudodn'], $_REQUEST['attribute'], 
				$_REQUEST[$_REQUEST['attribute']], '')){
	  pql_format_status_msg(pql_complete_constant($LANG->_("Successfully removed %what% from %where%"),
						      array('what'  => $_REQUEST[$_REQUEST['attribute']],
							    'where' => $_REQUEST['sudodn'])));
	} else {
	  pql_format_status_msg(pql_complete_constant("Failed to remove %what% from %where%"),
				array('what'  => $_REQUEST[$_REQUEST['attribute']],
				      'where' => $_REQUEST['sudodn']));
	}
      } elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'update_sudo_role') {
	$dn = $_REQUEST['roledn'];
	
	updateSudo($_pql->ldap_linkid, $dn, 'userdn', pql_get_define("PQL_ATTR_SUDOUSER"));
	updateSudo($_pql->ldap_linkid, $dn, 'command', pql_get_define("PQL_ATTR_SUDOCOMMAND"));
	updateSudo($_pql->ldap_linkid, $dn, 'user', pql_get_define("PQL_ATTR_SUDORUNAS"));
	updateSudo($_pql->ldap_linkid, $dn, 'computer', pql_get_define("PQL_ATTR_SUDOHOST"));
      }
?>
  <form method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Create Sudo Roles
        <tr class="c2">
          <td class="title"><?=$LANG->_("Role")?></td>
          <td class="title"><?=$LANG->_("User")?></td>
          <td class="title"><?=$LANG->_("Command")?></td>
          <td class="title"><?=$LANG->_("As User")?></td>
          <td class="title"><?=$LANG->_("On Host")?></td>
        </tr>

        <tr class="c2">
          <td><input type="text" name="role" size="20"></td>
<?php // No user was passed in in request string, so grab all users from ldap and make a list
      print "          <td>\n            <select name='userdn'>\n              <option value='none'><?=$LANG->_("None")?></option>\n";
      if(pql_get_define("PQL_CONF_SUBTREE_USERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ", ";

      $userdn = $subrdn . $_GET["domain"];
      $filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=*';
      $user_results = pql_search($_pql->ldap_linkid, $userdn, $filter);

      for($i=1; $i < count($user_results); $i++) {
	$user = $user_results[$i][pql_get_define("PQL_ATTR_UID")];
	
	print "              <option value='" . $user . "'>" . $user . "</option>\n";
      }
      print "            </select></td>\n";
?>
          <td><input type="text" name="command" size="20"></td>
          <td><input type="text" name="runas" size="20"></td>
          <td>
            <select name="computer">
              <option value='ALL'><?=$LANG->_("ALL")?></option>

<?php if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ", ";

      $computerdn = $subrdn . $_GET["domain"];
      $filter = "(& (objectClass=ipHost)(cn=*))";
      $computer_results = pql_search($_pql->ldap_linkid, $computerdn, $filter);

      for($i=0; $i < count($computer_results); $i++) {
	$host = $computer_results[$i][pql_get_define("PQL_ATTR_CN")];
	
	print "              <option value='" . $host . "'>" . $host . "</option>\n";
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

  <form method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Current Sudo Roles")?>
        <tr class="c2">
          <td class="title"><?=$LANG->_("Role")?></td>
          <td class="title"><?=$LANG->_("Users")?></td>
          <td class="title"><?=$LANG->_("Commands")?></td>
          <td class="title"><?=$LANG->_("Run As")?></td>
          <td class="title"><?=$LANG->_("Host")?></td>
        </tr>

<?php // No user was passed in in request string, so grab all users from ldap and make a list
      if(pql_get_define("PQL_CONF_SUBTREE_SUDOERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_SUDOERS") . ", ";

      $sudodn = $subrdn . $_REQUEST["domain"];
      $filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=sudoRole';

      $sudo_results = pql_search($_pql->ldap_linkid, $sudodn, $filter);

      $row = 'c1';

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
	listExpand($role, $users, pql_get_define("PQL_ATTR_SUDOUSER"), 
		   "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
	print "</td>";
	
	// To list the Users
	print "          <td>";
	listExpand($role, $commands, pql_get_define("PQL_ATTR_SUDOCOMMAND"), 
		   "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
	print "</td>";

	// To list the runas
	print "          <td>";
	listExpand($role, $runas, pql_get_define("PQL_ATTR_SUDORUNAS"), 
		   "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
	print "</td>";
	
	// To list the runas
	print "          <<td>";
	listExpand($role, $host, pql_get_define("PQL_ATTR_SUDOHOST"), 
		   "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
	print "</td>";
	
	print "        </tr>";
	
	if($row == 'c1') { $row = 'c2'; }
	else { $row = 'c1'; }
      }

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
<?php if(pql_get_define("PQL_CONF_SUBTREE_USERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ", ";

      $userdn = $subrdn . $_GET["domain"];
      $filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=*';
      $user_results = pql_search($_pql->ldap_linkid, $userdn, $filter);

      for($i=1; $i < count($user_results); $i++) {
	$user = $user_results[$i][pql_get_define("PQL_ATTR_UID")];
	
	print "              <option value='" . $user . "'>" . $user . "</option>\n";
      }
      print "            </select>\n          </td>\n";
?>
          <td><input type="text" name="command" size="20"></td>
          <td><input type="text" name="user" size="20"></td>
          <td>
            <select name="computer">
              <option value="none"><?=$LANG->_("None")?></option>
              <option value="ALL"><?=$LANG->_("ALL")?></option>
<?php if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ", ";

      $computerdn = $subrdn . $_GET["domain"];
      $filter = "(& (objectClass=ipHost)(cn=*))";
      $computer_results = pql_search($_pql->ldap_linkid, $computerdn, $filter);

      for($i=0; $i < count($computer_results); $i++) {
	$host = $computer_results[$i][pql_get_define("PQL_ATTR_CN")];
	
	print "              <option value='" . $host . "'>" . $host . "</option>\n";
      }
?>
            </select>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="view" value="sudo">
    <input type="hidden" name="action" value="update_sudo_role">
    <input type="submit" name="Submit" value="Update Sudo Role">
  </form>
