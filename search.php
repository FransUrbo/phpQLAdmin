<?php
// shows results of search
// $Id: search.php,v 2.31 2005-02-24 17:04:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include($_SESSION["path"]."/header.html");

// print status message, if one is available
if(isset($msg)) {
    pql_format_status_msg($msg);
}

// reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
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
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// test for submission of variables
if (empty($_REQUEST["attribute"]) || empty($_REQUEST["filter_type"]) || empty($_REQUEST["search_string"])) {
    // invalid form submission
    $msg = urlencode($LANG->_('You have to provide a value to search for'));
    header("Location: " . $_SESSION["URI"] . "home.php?msg=$msg");
    exit();
}

if($_REQUEST["attribute"] == pql_get_define("PQL_ATTR_MAILHOST")) {
	// IDNA decode the FQDN
	$_REQUEST["search_string"] = pql_maybe_idna_encode($_REQUEST["search_string"]);

	// We must force an 'is', since it's not possible to do a substring match
	$_REQUEST["filter_type"] = 'is';
}

// make filter to comply with filter_type and search_string
$filter = "";
switch($_REQUEST["filter_type"]) {
  case "is":
	if($_REQUEST["attribute"] == pql_get_define("PQL_ATTR_MAIL")) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'='.$_REQUEST["search_string"];
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'='.$_REQUEST["search_string"];
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=" . $_REQUEST["search_string"];
    break;
  case "ends_with":
	if($_REQUEST["attribute"] == pql_get_define("PQL_ATTR_MAIL")) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'=*'.$_REQUEST["search_string"];
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'=*'.$_REQUEST["search_string"];
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=*" . $_REQUEST["search_string"];
    break;
  case "starts_with":
	if($_REQUEST["attribute"] == pql_get_define("PQL_ATTR_MAIL")) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'='.$_REQUEST["search_string"].'*';
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'='.$_REQUEST["search_string"].'*';
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=" . $_REQUEST["search_string"] . "*";
    break;
  default:
	if($_REQUEST["attribute"] == pql_get_define("PQL_ATTR_MAIL")) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'=*'.$_REQUEST["search_string"].'*';
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'=*'.$_REQUEST["search_string"].'*';
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=*" . $_REQUEST["search_string"] . "*";
    break;
}

if(!$_SESSION["SINGLE_USER"]) {
	// Admin of some sort - look in the whole database for a user
	// that matches filter
	foreach($_SESSION["BASE_DN"] as $dn) {
		if($_REQUEST["debug"])
		  echo "$dn: $filter<br>";

		$usrs = pql_get_dn($_pql->ldap_linkid, $dn, $filter, 'SUBTREE');
		for($i=0; $usrs[$i]; $i++) {
			$is_group = 0;
			
			// Check if this object is a (posix)Group object.
			$ocs = pql_get_attribute($_pql->ldap_linkid, $usrs[$i], pql_get_define("PQL_ATTR_OBJECTCLASS"));
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
    $dnparts = ldap_explode_dn($_SESSION["USER_DN"], 0);
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
		$ocs = pql_get_attribute($_pql->ldap_linkid, $usrs[$i], pql_get_define("PQL_ATTR_OBJECTCLASS"));
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
			$uid    = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_UID"));

			// DLW: I think displayname would be a better choice.
			$cn     = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_CN"));
			if(is_array($cn)) $cn = $cn[0];
			$mail   = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_MAIL"));
			
			$status = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_ISACTIVE"));
			$status = pql_ldap_accountstatus($status);

			$rootdn = pql_get_rootdn($user, 'search.php');
?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$_REQUEST["domain"]?>&user=<?=urlencode($user)?>" target="_new"><?=$cn?></a></td>
        <td><?=$uid?></td>
        <td><?=$mail?></td>
        <td><?=$status?></td>
        <td>
          <a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$_REQUEST["domain"]?>&user=<?php echo urlencode($user)?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change user data')?>" border="0"></a>
          &nbsp;
          <a href="user_del.php?rootdn=<?=$rootdn?>&domain=<?=$_REQUEST["domain"]?>&user=<?php echo urlencode($user); ?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete user')?>" border="0"></a>
        </td>
      </tr>
<?php	}
	  } else {
		  // no users registred
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="5"><?=$LANG->_('No users found')?></td>
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
