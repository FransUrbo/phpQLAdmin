<?php if(isset($_REQUEST['action']) && $_REQUEST['action'] == 'remove_user_from_host') {
	$msg = pql_modify_attribute($_pql->ldap_linkid, $_REQUEST['groupdn'],
				    pql_get_define('PQL_ATTR_UNIQUE_GROUP'), $_REQUEST['userdn'], '');
	if(isset($msg) && $msg == 1) {
	  pql_format_status_msg(pql_complete_constant($LANG->_("Host ACL Updated Successfully<br>Removed: %what%<br>From %where% ACL"),
						      array('what'  => $_REQUEST['userdn']
							    'where' => $_REQUEST['groupdn'])));
	}
      } elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'add_new_host') {
	$num="(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";
	if(!isset($_REQUEST['hostip']) || !preg_match("/^$num\\.$num\\.$num\\.$num$/", $_REQUEST['hostip']) ) {
	  pql_format_status_msg($LANG->_("Invalid IP address"));
	} elseif(!isset($_REQUEST['hostname']) || !preg_match("/\w\\.\w/i", $_REQUEST['hostname']) ) {
	  pql_format_status_msg($LANG->_("Invalid hostname"));
	} else {
	  // Inputs look good, lets add them
	  if(pql_add_computer($_pql->ldap_linkid, $_REQUEST["domain"],
			      $_REQUEST['hostname'], $_REQUEST['hostip']) ) {
	    pql_format_status_msg(pql_complete_constant($LANG->_("Host %host% added successfully."),
							array('host' => $_REQUEST['hostname'])));
	  } else {
	    pql_format_status_msg(pql_complete_constant($LANG->_("Host %host% failed to add"),
							array('host' => $_REQUEST['hostname'])));
	  }
	}
      } elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'add_user_to_host') {
	if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST['computer'],
                                pql_get_define('PQL_ATTR_UNIQUE_GROUP'), '', $_REQUEST['userdn'])) {
	  pql_format_status_msg(pql_complete_constant($LANG->_("Successfully added %user% to host ACL",
							       array('user' => $_REQUEST['userdn']))));
	} else {
	  pql_format_status_msg(pql_complete_constant($LANG->_("Failed to add %user% to Host ACL"),
						      array('user' => $_REQUEST['userdn'])));
	}
      }
?>
  <form method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Host Control Access")?></th>
        <tr class="c2">
          <td class="title"><?=$LANG->_("Allow")?></td>
<?php // If user info is passed in the request string then their is no need to grab all the users from ldap
      if(isset($_REQUEST['userdn']) && isset($_REQUEST['uid'])) {
?>
          <td class="title">
            <?php echo $_REQUEST['uid']; ?>
            <input type="hidden" name="userdn" value="<?php echo $_REQUEST['userdn']; ?>">
          </td>
<?php } else {
	// No user was passed in in request string, so grab all users from ldap and make a list
	print "          <td class='title'>\n";
	print "            <select name='userdn'>\n";
	if(pql_get_define("PQL_CONF_SUBTREE_USERS"))
	  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ", ";

	$userdn = $subrdn . $_GET["domain"];
	$filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=*';
	$user_results = pql_search($_pql->ldap_linkid, $userdn, $filter);

	for($i=1; $i < count($user_results); $i++) {
	  $user = $user_results[$i][pql_get_define("PQL_ATTR_UID")];
	  $userdn = $user_results[$i]['dn'];
	  
	  print "              <option value='" . $userdn . "'>" . $user . "</option>\n";
	}

	print "            </select></td>\n";
      }
?>
          <td>
            <?=$LANG->_("To Access ")?>
            <select name="computer">

<?php if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ", ";

      $computerdn = $subrdn . $_GET["domain"];
      $filter = "(& (objectClass=ipHost)(cn=*))";
      $computer_results = pql_search($_pql->ldap_linkid, $computerdn, $filter);

      for($i=0; $i < count($computer_results); $i++) {
	$host = $computer_results[$i][pql_get_define("PQL_ATTR_CN")];
	$hostdn = $computer_results[$i]['dn'];
	
	print "              <option value='" . $hostdn . "'>" . $host . "</option>\n";
      }
?>
            </select>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="action" value="add_user_to_host">
    <input type="hidden" name="view"   value="host">
    <input type="Submit" name="Submit" value="<?=$LANG->_("Add To Host ACL")?>">
  </form>

   <table cellspacing="0" cellpadding="3" border="0">
    <th><?=$LANG->_("Hostname")?></th>
    <th><?=$LANG->_("IP Address")?></th>
<?php $row = 'c1';
      // die(print_r($computer_results));
      for($i=0; $i < count($computer_results); $i++) {
	$host = $computer_results[$i][pql_get_define("PQL_ATTR_CN")];
	$hostdn = $computer_results[$i]['dn'];
	$hostIP = $computer_results[$i][pql_get_define("PQL_ATTR_IPHOSTNUMBER")];
?>	
        <tr class="<?=$row?>">
          <td>
            <div id="<?=$host."Parent"?>" class="parent">
              <a href="<?=$_SERVER['REQUEST_URI']?>" onClick="if (capable) {expandBase('<?=$host?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="<?=$host."Img"?>">
              </a>
	
              <font color="black" class="heada"><?=$host?></font>
            </div>
	
            <div id="<?=$host."Child"?>" class="child">
              <nobr>
<?php   $users = $computer_results[$i][pql_get_define('PQL_ATTR_UNIQUE_GROUP')];
        if(is_array($users)) {
	  for($j=0; $j < count($users); $j++) {
	    print "              &nbsp;&nbsp;&nbsp;&nbsp;\n";
	    print "              <img src=\"images/navarrow.png\" width=\"9\" height=\"9\" border=\"0\">";
	    $user =  pql_get_uid($_pql->ldap_linkid, $users[$j]);
	    print $user;
	    print "              <a href=\"" . $_SERVER['REQUEST_URI'];
	    print "?groupdn=" . $hostdn;
	    print "&userdn=" . $users[$j] . "&action=remove_user_from_host\">";
	    print "                <img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"";
	    print pql_complete_constant($LANG->_("Delete %user% from computer"), array('user' => $user));
	    print "\">\n              </a>\n              <br>\n";
	  }
	} else {
	  print "              &nbsp;&nbsp;&nbsp;&nbsp;\n";
	  print "              <img src=\"images/navarrow.png\" width=\"9\" height=\"9\" border=\"0\">";
	  $user =  pql_get_uid($_pql->ldap_linkid, $users);
	  print $user;
	  print "              <a href=\"" . $_SERVER['REQUEST_URI'];
	  print "?groupdn=" . $hostdn;
	  print "&userdn=" . $users . "&action=remove_user_from_host\">";
	  print "                <img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"";
	  print pql_complete_constant($LANG->_("Delete %user% from computer"), array('user' => $user));
	}
?>">
              </a>
              <br>
              </nobr>
            </div>
          </td>

          <td>
<?php   if(is_array($hostIP)) {
	  for($z = 0; $z < count($hostIP); $z++) {
	    print $hostIP[$z] . "<br>\n";
	  }
        } else {
	  print $hostIP . "\n";
        }
      
        print "          </td>\n        </tr>\n";
      
        if($row == 'c1') { $row = 'c2'; }
        else { $row = 'c1'; }
      } // for($i=0; $i < count($computer_results); $i++)

      print "      </table>";
?>
      <br>
      <form method="post">
        <table cellspacing="1" cellpadding="3" border="0">
          <th colspan="3" align="left"><?=$LANG->_("Add New Computer")?>
            <tr class="c2">
              <td class="title"><?=$LANG->_("Fully Qualified Domain Name")?></td>
              <td class="title"><?=$LANG->_("IP Address")?></td>
            </tr>

            <tr>
              <td><input type="text" name="hostname" size="20"></td>
              <td><input type="text" name="hostip" size="20"></td>
            </tr>
          </th>
        </table>

        <input type="hidden" name="action" value="add_new_host">
        <input type="submit" name="Submit" value="Add New Host">
      </form>
