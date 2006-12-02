<?php
// Modify Host ACLs
// $Id: host_modify.php,v 2.8 2006-12-16 12:02:09 turbo Exp $

// {{{ Setup session etc
if(!@$_SESSION) {
  // Called directly - as in modifying something with a host
  require("./include/pql_session.inc");
  require($_SESSION["path"]."/include/pql_config.inc");
}
require($_SESSION["path"]."/left-head.html");
// }}}

// {{{ Retreive all users
if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ",";
}
$userdn = $subrdn . $_GET["domain"];
$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
$users = pql_get_dn($_pql->ldap_linkid, $userdn, $filter);

// Extract 'human readable' name from the user DN's found
$user_results = pql_left_htmlify_userlist($_pql->ldap_linkid, $_REQUEST["rootdn"],
					  $_REQUEST["domain"], $userdn, $users,
					  ($links = NULL));
// }}}

// {{{ Retreive all physical computers
$computer_results = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
			       '(&(cn=*)(|(objectclass=ipHost)(objectclass=device)))',
			       'ONELEVEL');
// }}}

if(isset($_REQUEST['action'])) {
  if($_REQUEST['action'] == 'remove_user_from_host') {
    // {{{ Remove user from host
    
    // NOTE: Removing the last uniqueMember will result in:
    // ldap_modify: Object class violation (65)
    //				additional info: object class 'groupOfUniqueNames' requires attribute 'uniqueMember'
    // Check to see if this is the last uniqueMember in this groupDN
    for($i=0; $computer_results[$i]; $i++) {
      if(lc($computer_results[$i]) == lc($_REQUEST["host"])) {
	// Retreive uniqueMember from this host
	$host_users = pql_get_attribute($_pql->ldap_linkid, $computer_results[$i], pql_get_define("PQL_ATTR_UNIQUE_GROUP"));
	
	$count = count($host_users);
	$computer_number = $i; // Needed to manual remove below.
	break;
      }
    }
    
    if($count > 1)
      // There's more than one uniqueMember value, remove the requested one.
      $msg = pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["host"],
				  pql_get_define("PQL_ATTR_UNIQUE_GROUP"),
				  $_REQUEST["userdn"], '');
    else
      // This IS the last uniqueMember value, remove the whole object.
      $msg = pql_write_del($_pql->ldap_linkid, $_REQUEST["host"]);
    
    if(isset($msg) && ($msg == 1))
      $msg = pql_complete_constant($LANG->_("Host ACL Updated Successfully. Removed: %what% From: %where%"),
				   array("what"  => $_REQUEST['userdn'], "where" => $_REQUEST['host']));
    else
      $msg = pql_complete_constant($LANG->_("Failed to update Host ACL. Could not remove %what%"),
				   array("what" => $_REQUEST['userdn']));
    
    $url = "host_detail.php?host=".urlencode($_REQUEST["host"])."&msg=".urlencode($msg);
    
    if(pql_get_define("PQL_CONF_DEBUG_ME")) {
      echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url: '<b>$url</b>'<br>";
      die($msg);
    } else
      pql_header($url);
// }}}

  } elseif($_REQUEST['action'] == 'add_user_to_host') {
    // {{{ Add user to host
    if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST['computer'], pql_get_define('PQL_ATTR_UNIQUE_GROUP'), '', $_REQUEST['userdn'])) {
      pql_format_status_msg(pql_complete_constant($LANG->_("Successfully added %user% to host ACL"), array('user' => $_REQUEST['userdn'])));
      
      // Since we've already retreived all hosts, we must manually add the user to the correct part of the array(s).
      if(is_array($computer_results[$computer_number][pql_get_define("PQL_ATTR_UNIQUE_GROUP")])) 
	$computer_results[$computer_number][pql_get_define("PQL_ATTR_UNIQUE_GROUP")][] = $_REQUEST['userdn'];
      else {
	$tmp = $computer_results[$computer_number][pql_get_define("PQL_ATTR_UNIQUE_GROUP")]; // Save the current (flat) value
	unset($computer_results[$computer_number][pql_get_define("PQL_ATTR_UNIQUE_GROUP")]); // Undefine the value, so it can be re-initialized
	
	$computer_results[$computer_number][pql_get_define("PQL_ATTR_UNIQUE_GROUP")][] = $tmp; // Add the original value
	$computer_results[$computer_number][pql_get_define("PQL_ATTR_UNIQUE_GROUP")][] = $_REQUEST['userdn']; // Add the new user DN
      }
    } else
      pql_format_status_msg(pql_complete_constant($LANG->_("Failed to add %user% to Host ACL"), array('user' => $_REQUEST['userdn'])));
// }}}
  }
}

if(is_array($computer_results)) {
  // {{{ Show this domains user access
?>

  <form method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Host Control Access")?>
        <tr class="c2">
          <td class="title"><?=$LANG->_("Allow")?>&nbsp;</td>
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
    print "          <td>\n";
    print "            <select name='userdn'>\n";
    
    foreach($user_results as $dn => $user)
      print "              <option value='$dn'>$user</option>\n";
    print "            </select>\n          </td>\n";
// }}}
  }

  // {{{ Host column
?>

          <td class="title">&nbsp;<?=$LANG->_("access to physical host")."\n"?>&nbsp;</td>
          <td>
            <select name="computer">
<?php
  foreach($computer_results as $host_dn) {
    $host = pql_get_attribute($_pql->ldap_linkid, $host_dn, pql_get_define("PQL_ATTR_CN"));
    print "              <option value='" . $host_dn . "'>" . $host . "</option>\n";
  }
?>
            </select>
          </td>
<?php
// }}}
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="view"   value="hostacl">
    <input type="hidden" name="action" value="add_user_to_host">
    <input type="Submit" name="Submit" value="<?=$LANG->_("Add To Host ACL")?>">
  </form>

<?php
// }}}
} else { 
  // {{{ There's no computers, so we couldn't insert the 'add user to host ACL' form above.
  // Instead, just say there wasn't any hosts to modify...
  echo '<img src="images/info.png" width="16" height="16" alt="" border="0">';
  echo "&nbsp;&nbsp;";
  echo $LANG->_("Could not find any computers/hosts, so I couldn't show you the form. Start by adding a host. You do this at the left Computers frame.");
// }}}
}

// {{{ Find all host with user from this branch
// Setup search filter for finding hosts
// objectClass => ipHost AND groupOfUniqueNames.
$filter  = '(&(objectClass=ipHost)(objectClass=groupOfUniqueNames)(|';
foreach($users as $user) {
  // Search for each and every user in the groupOfUniqueNames.
  $filter .= '('.pql_get_define("PQL_ATTR_UNIQUE_GROUP").'='.$user.')';
}
$filter .= '))';

$hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter);
if($hosts) {
  if(!is_array($hosts)) 
    $hosts = array($hosts);
} else {
  $hosts = array();
}
// }}}

include("./tables/host_details-acl.inc");

/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
