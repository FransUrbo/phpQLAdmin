<?php
// shows results of search
// search.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_search.inc");

include("./header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and PQL_CONF_AUTO_RELOAD){
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    refreshFrames();
  //--></script>

<?php
}
?>
  <span class="title1"><?php echo PQL_LANG_SEARCH_TITLE2; ?></span>
  <br><br>

<?php
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// test for submission of variables
if ($attribute == "" || $filter_type == "" || $search_string == "") {
    // invalid form submission
    $msg = urlencode("You have to provide a value to search");
    header("Location: " . PQL_CONF_URI . "home.php?msg=$msg");
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

// get userlist that matches filter
$users = pql_search($_pql->ldap_linkid, $filter);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="4" align="left"><?php echo PQL_LANG_USER_REGISTRED ?> (<?php echo sizeof($users); ?>)</th>
<?php
if(is_array($users)){
?>
<tr>
      <td class="title"><?php echo PQL_LANG_USER ?></td>
      <td class="title"><?php echo PQL_LANG_USER_ID ?></td>
      <td class="title"><?php echo PQL_LANG_EMAIL ?></td>
      <td class="title"><?php echo PQL_LANG_ACCOUNTSTATUS_STATUS ?></td>
      <td class="title"><?php echo PQL_LANG_OPTIONS ?></td>
    </tr>
<?php
  asort($users);
  foreach($users as $user){
      // don't know domain, so must figure it out
      $domain = $user["domain"];
      $reference = $user["reference"];
      $uid = pql_get_userattribute($_pql->ldap_linkid, $reference, PQL_CONF_ATTR_UID);
      $uid = $uid[0];
      $cn = pql_get_userattribute($_pql->ldap_linkid, $reference, PQL_CONF_ATTR_CN);
      $cn = $cn[0];
      $mail = pql_get_userattribute($_pql->ldap_linkid, $reference, PQL_CONF_ATTR_MAIL);
      $mail = $mail[0];
      $status = pql_get_userattribute($_pql->ldap_linkid, $reference, PQL_CONF_ATTR_ISACTIVE);
      $status = pql_ldap_accountstatus($status[0]);
?>
    <tr class="<?php table_bgcolor(); ?>">
      <td><a href="user_detail.php?domain=<?=$domain?>&user=<?=urlencode($reference)?>"><?=$cn?></a></td>

      <td><?=$uid?></td>
      <td><?=$mail?></td>
      <td><?=$status?></td>
      <td>
        <a href="user_detail.php?domain=<?php echo $domain ?>&user=<?php echo urlencode($reference)?>">
          <img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_USER_EDIT ?>" border="0">
        </a>
        &nbsp;&nbsp;
        <a href="user_del.php?domain=<?php echo $domain;?>&user=<?php echo urlencode($reference); ?>">
          <img src="images/del.png" width="12" height="12" alt="<?php echo PQL_LANG_USER_DELETE ?>" border="0">
        </a>
      </td>
    </tr>
<?php
	}
} else {
	// no users registred
?>
<tr class="<?php table_bgcolor(); ?>">
	<td colspan="5"><?php echo PQL_LANG_USER_NONE ?></td>
</tr>
<?php
}
?>
</table>
</body>
</html>
