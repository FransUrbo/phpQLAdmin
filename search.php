<?php
// shows results of search
// $Id: search.php,v 2.19 2003-11-19 16:26:38 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

// print status message, if one is available
if(isset($msg)) {
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    refreshFrames();
  //--></script>

<?php
}
?>
  <span class="title1"><?=$LANG->_('Search Results')?></span>
  <br><br>

<?php
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// test for submission of variables
if ($attribute == "" || $filter_type == "" || $search_string == "") {
    // invalid form submission
    $msg = urlencode($LANG->_('You have to provide a value to search for'));
    header("Location: " . pql_get_define("PQL_GLOB_URI") . "home.php?msg=$msg");
    exit();
}

// make filter to comply with filter_type and search_string
$filter = "";
switch($filter_type) {
  case "is":
    $filter = $attribute . "=" . $search_string;
    break;
  case "ends_with":
    $filter = $attribute . "=*" . $search_string;
    break;
  case "starts_with":
    $filter = $attribute . "=" . $search_string . "*";
    break;
  default:
    $filter = $attribute . "=*" . $search_string . "*";
    break;
}

if(!$GLOBALS["SINGLE_USER"]) {
	// Admin of some sort - look in the whole database for a user
	// that matches filter
	foreach($_pql->ldap_basedn as $dn) {
		$dn = urldecode($dn);
		
		$usrs = pql_search($_pql->ldap_linkid, $dn, $filter);
		for($i=0; $usrs[$i]; $i++) {
			$is_group = 0;
			
			// Check if this object is a (posix)Group object.
			$ocs = pql_get_userattribute($_pql->ldap_linkid,
										 $usrs[$i], pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS"));
			for($j=0; $ocs[$j]; $j++) {
				if(eregi('group', $ocs[$j]))
				  $is_group = 1;
			}
			
			if(!$is_group)
			  // It's NOT a (posix)Group object, show it in the list...
			  $users[] = $usrs[$i];
		}
	}
} else {
	// Single user - only look in the same branch as the user is located in
	$dn = '';

	// Get branch for user
    $dnparts = ldap_explode_dn($USER_DN, 0);
	for($i=1; $dnparts[$i]; $i++) {
		$dn .= $dnparts[$i];
		if($dnparts[$i+1])
		  $dn .= ",";
	}

	// Search for the user below this DN
	$usrs = pql_search($_pql->ldap_linkid, $dn, $filter);
	for($i=0; $usrs[$i]; $i++) {
		$is_group = 0;
		
		// Check if this object is a (posix)Group object.
		$ocs = pql_get_userattribute($_pql->ldap_linkid, $usrs[$i], pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS"));
		for($j=0; $ocs[$j]; $j++) {
			if(eregi('group', $ocs[$j]))
			  $is_group = 1;
		}
		
		if(!$is_group)
		  // It's NOT a (posix)Group object, show it in the list...
		  $users[] = $usrs[$i];
	}
}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="4" align="left"><?=$LANG->_('Registred users')?>: <?php echo count($users); ?></th>
<?php if(is_array($users)) { ?>
      <tr>
        <td class="title"><?=$LANG->_('User')?></td>
        <td class="title"><?=$LANG->_('Username')?></td>
        <td class="title"><?=$LANG->_('Email')?></td>
        <td class="title"><?=$LANG->_('Status')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>
<?php
		asort($users);
		foreach($users as $user) {
			$uid    = pql_get_userattribute($_pql->ldap_linkid, $user,
											pql_get_define("PQL_GLOB_ATTR_UID"));
			$uid    = $uid[0];
			
			$cn     = pql_get_userattribute($_pql->ldap_linkid, $user,
											pql_get_define("PQL_GLOB_ATTR_CN"));
			$cn     = $cn[0];
			
			$mail   = pql_get_userattribute($_pql->ldap_linkid, $user,
											pql_get_define("PQL_GLOB_ATTR_MAIL"));
			$mail   = $mail[0];
			
			$status = pql_get_userattribute($_pql->ldap_linkid, $user,
											pql_get_define("PQL_GLOB_ATTR_ISACTIVE"));
			$status = pql_ldap_accountstatus($status[0]);

			$rootdn = pql_get_rootdn($user);
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=urlencode($user)?>"><?=$cn?></a></td>
        <td><?=$uid?></td>
        <td><?=$mail?></td>
        <td><?=$status?></td>
        <td>
          <a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?php echo urlencode($user)?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change user data')?>" border="0"></a>
          &nbsp;
          <a href="user_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?php echo urlencode($user); ?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete user')?>" border="0"></a>
        </td>
      </tr>
<?php	}
	  } else {
		  // no users registred
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="5"><?=$LANG->_('No users registred')?></td>
      </tr>
<?php }

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
    </table>
  </body>
</html>
