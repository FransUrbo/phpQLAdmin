<?php
require($_SESSION["path"]."/left-head.html");

// {{{ Retreive all users
if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ",";
}
$userdn = $subrdn . $_GET["domain"];
$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
$users = pql_get_dn($_pql->ldap_linkid, $userdn, $filter);

// Extract 'human readable' name from the user DN's found
$user_results = pql_left_htmlify_userlist($_pql->ldap_linkid, $_REQUEST["rootdn"], $_REQUEST["domain"],
										  $userdn, $users, ($links = NULL));
// }}}

// {{{ Retreive all computers
if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS")) {
  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ",";
}
$computerdn = $subrdn . $_GET["domain"];
$filter = "(& (objectClass=ipHost)(cn=*))";
$computer_results = pql_search($_pql->ldap_linkid, $computerdn, $filter);

if(is_array($computer_results) and !@$computer_results[0]) {
  // Make sure it's a numbered array...
  $tmp = $computer_results;
  unset($computer_results);
  $computer_results[] = $tmp;
}
// }}}

if(isset($_REQUEST['action']) && ($_REQUEST['action'] == 'remove_user_from_host')) {
  // {{{ Remove user from host

  // NOTE: Removing the last uniqueMember will result in:
  // ldap_modify: Object class violation (65)
  //				additional info: object class 'groupOfUniqueNames' requires attribute 'uniqueMember'
  // Check to see if this is the last uniqueMember in this groupDN
  for($i=0; $computer_results[$i]; $i++) {
    if(lc($computer_results[$i]["dn"]) == lc($_REQUEST["groupdn"]))
      $count = count($computer_results[$i][pql_get_define("PQL_ATTR_UNIQUE_GROUP")]);
  }

  if($count > 1) {
    // There's more than one uniqueMember value, remove the requested one.
    $msg = pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["groupdn"],
				pql_get_define("PQL_ATTR_UNIQUE_GROUP"),
				$_REQUEST["userdn"], '');

    if(isset($msg) && ($msg == 1))
      pql_format_status_msg(pql_complete_constant($LANG->_("Host ACL Updated Successfully<br>Removed: %what%<br>From: %where% ACL"),
						  array("what"  => $_REQUEST['userdn'], "where" => $_REQUEST['groupdn'])));
  } else {
    // This IS the last uniqueMember value, remove the whole object.
    $msg = pql_write_del($_pql->ldap_linkid, $_REQUEST["groupdn"]);
    if(isset($msg) && ($msg == 1))
      pql_format_status_msg(pql_complete_constant($LANG->_("Host ACL Updated Successfully<br>Removed: %what%<br>From: %where% ACL"),
						  array("what"  => $LANG->_('last user'), "where" => $_REQUEST['groupdn'])));
  }

// }}}
} elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'add_new_host') {
  // {{{ Add new host
  $num = "(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";
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
// }}} 
} elseif(isset($_REQUEST['action']) && $_REQUEST['action'] == 'add_user_to_host') {
  // {{{ Add user to host
  if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST['computer'],
						  pql_get_define('PQL_ATTR_UNIQUE_GROUP'), '', $_REQUEST['userdn'])) {
	pql_format_status_msg(pql_complete_constant($LANG->_("Successfully added %user% to host ACL"),
														 array('user' => $_REQUEST['userdn'])));
  } else {
	pql_format_status_msg(pql_complete_constant($LANG->_("Failed to add %user% to Host ACL"),
												array('user' => $_REQUEST['userdn'])));
  }
// }}}
}

if(is_array($computer_results[0])) {
?>

  <form method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Host Control Access")?></th>
        <tr class="c2">
          <td class="title"><?=$LANG->_("Allow")?></td>
<?php
  if(isset($_REQUEST['userdn']) && isset($_REQUEST['uid'])) {
	// {{{ If user info is passed in the request string then their is no need to grab all the users from ldap
?>
          <td class="title">
            <?php echo $_REQUEST['uid']; ?>
            <input type="hidden" name="userdn" value="<?php echo $_REQUEST['userdn']; ?>">
          </td>
<?php
// }}}
  } else {
	// {{{ No user was passed in in request string so make the list
	print "          <td class='title'>\n";
	print "            <select name='userdn'>\n";
  
	foreach($user_results as $dn => $user)
	  print "              <option value='$dn'>$user</option>\n";
	print "            </select>\n          </td>\n";
// }}}
  }
?>

          <td>
            <?=$LANG->_("To Access")."\n"?>
            <select name="computer">
<?php
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

    <input type="hidden" name="view"   value="hostacl">
    <input type="hidden" name="action" value="add_user_to_host">
    <input type="Submit" name="Submit" value="<?=$LANG->_("Add To Host ACL")?>">
  </form>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr class="title">
        <td><?=$LANG->_("Hostname")?></td>
        <td><?=$LANG->_("IP Address")?></td>
      </tr>
<?php
  $row = 'c1';
  for($i=0; $i < count($computer_results); $i++) {
	$host   = $computer_results[$i][pql_get_define("PQL_ATTR_CN")];
	$hostdn = $computer_results[$i]['dn'];
	$hostIP = $computer_results[$i][pql_get_define("PQL_ATTR_IPHOSTNUMBER")];
?>	
      <tr class="<?=$row?>">
        <!-- HOSTNAME AND USERS -->
        <td>
<?php if($_SESSION["opera"]) { ?>
          <div id="el<?=$i?>Parent" class="parent" onclick="showhide(el<?=$i?>aSpn, el<?=$i?>aImg); showhide(el<?=$i?>bSpn, el<?=$i?>bImg);">
            <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$i?>aImg">
            <a class="item"><font color="black" class="heada"><?=$host?></font></a>
          </div>
<?php } else { ?>
          <div id="el<?=$i."Parent"?>" class="parent">
            <a href="<?=$_SERVER['REQUEST_URI']?>" onClick="if (capable) {expandBase('el<?=$i+1?>', true); return false;}">
              <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$i."Img"?>"></a>
	
            <font color="black" class="heada"><?=$host?></font>
          </div>
<?php } ?>

<?php $users = $computer_results[$i][pql_get_define('PQL_ATTR_UNIQUE_GROUP')];
	  if(@$users) {
		// {{{ Is there any users?
		if(is_array($users)) {
		  if($_SESSION["opera"]) {
?>
          <span id="el<?=$i?>aSpn" style="display:''">
<?php	  } else { ?>
          <div id="<?=$i."Child"?>" class="child">
<?php	  } ?>
<?php	  for($j=0; $j < count($users); $j++) {
			$user = pql_get_attribute($_pql->ldap_linkid, $users[$j],
									  pql_get_define("PQL_ATTR_UID"));
?>
              &nbsp;&nbsp;&nbsp;<img src="images/navarrow.png" width="9" height="9" border="0">&nbsp;<?=$user."\n"?>
              <a href="<?=$_SERVER['REQUEST_URI']?>&groupdn=<?=$hostdn?>&userdn=<?=$users[$j]?>&action=remove_user_from_host">
                <img src="images/del.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %user% from computer"), array('user' => $user))?>"></a>
              <br>
<?php	  }
		} else {
		  $user = pql_get_attribute($_pql->ldap_linkid, $users,
									pql_get_define("PQL_ATTR_UID"));
?>
              &nbsp;&nbsp;&nbsp;&nbsp;
              <img src="images/navarrow.png" width="9" height="9" border="0">
              <?=$user?>
              <a href="<?=$_SERVER['REQUEST_URI']?>&groupdn=<?=$hostdn?>&userdn=<?=$users?>&action=remove_user_from_host">
                <img src="images/del.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %user% from computer"), array('user' => $user))?>"></a>
              <br>
<?php	} ?>
              </nobr>
<?php	if($_SESSION["opera"]) { ?>
            </span>
<?php	} else { ?>
            </div>
<?php	}
// }}}
	  }
?>
          </td>
<?php if($hostIP) {
		// {{{ Is there is an IP address?
		$host_ip = '';
		if(is_array($hostIP)) {
		  for($z = 0; $z < count($hostIP); $z++) {
			$host_ip .= $hostIP[$z];
			if($hostIP[$z+1])
			  $host_ip .= ",&nbsp";
		  }
		} else
		  $host_ip = $hostIP;

?>

          <!-- IP ADDRESS -->
          <td>
<?php	if($_SESSION["opera"]) { ?>
            <div id="el<?=$i+1?>Parent" class="parent" onclick="showhide(el<?=$i+1?>aSpn, el<?=$i+1?>aImg); showhide(el<?=$i+1?>bSpn, el<?=$i+1?>bImg);">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$i+1?>bImg">
              <a class="item"><font color="black" class="heada"><?=$host_ip?></font></a>
            </div>

            <span id="el<?=$i+1?>bSpn" style="display:''">
<?php	} else { ?>
            <div id="el<?=$i+1?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$i?>', true); expandBase('el<?=$i+1?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$i+1?>Img">
              </a>
              <a class="item"><font color="black" class="heada"><?=$host_ip?></font></a>
            </div>

            <div id="el<?=$i+1?>Child" class="child">
<?php	}
		echo "<br>";

		if($_SESSION["opera"]) {
?>
            </span>
<?php	} else { ?>
            </div>
<?php	}

		print "          </td>\n        </tr>\n";
  
		if($row == 'c1')
		  $row = 'c2';
		else
		  $row = 'c1';
// }}}
	  }
  } // for($i=0; $i < count($computer_results); $i++)
?>
      </table>

      <br>

<?php
} else { 
  // {{{ There's no computers, so we couldn't insert the 'add user to host ACL' form above.
  // Instead, just say there wasn't any hosts to modify...
  echo '<img src="images/info.png" width="16" height="16" alt="" border="0">';
  echo "&nbsp;&nbsp;";
  echo $LANG->_("Could not find any computers/hosts, so I couldn't show you the form. Start by adding a host...");
// }}}
}

// {{{ Create the 'Add New Computer' form
?>
      <form method="post">
        <table cellspacing="1" cellpadding="3" border="0">
          <th colspan="3" align="left"><?=$LANG->_("Add New Computer")?>
            <tr class="c2">
              <td class="title"><?=$LANG->_("Fully Qualified Domain Name")?></td>
              <td class="title"><?=$LANG->_("IP Address")?></td>
            </tr>

            <tr>
              <td><input type="text" name="hostname" size="40"></td>
              <td><input type="text" name="hostip"   size="20"></td>
            </tr>
          </th>
        </table>

        <input type="hidden" name="view"   value="hostacl">
        <input type="hidden" name="action" value="add_new_host">
        <input type="submit" name="Submit" value="<?=$LANG->_('Add New Host')?>">
      </form>
<?php
// }}}

/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
