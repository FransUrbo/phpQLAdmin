<?php
// shows results of search
// $Id: search.php,v 1.3 2002-12-12 21:52:08 turbo Exp $
//
session_start();
require("pql.inc");
require("pql_search.inc");

include("header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and PQL_AUTO_RELOAD){
?>
  <script language="JavaScript1.2">
  <!--
    // reload navigation frame
    parent.frames.pqlnav.location.reload();
  //-->
  </script>

<?php
}
?>
  <span class="title1"><?php echo PQL_SEARCH_TITLE2; ?></span>
  <br><br>

<?php
$_pql = new pql($USER_DN, $USER_PASS);

// test for submission of variables
if ($attribute == "" || $filter_type == "" || $search_string == "") {
    // invalid form submission
    $msg = urlencode("You have to provide a value to search");
    header("Location: " . PQL_URI . "home.php?msg=$msg");
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
$users = pql_search($_pql->ldap_linkid, PQL_LDAP_BASEDN, $filter);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="4" align="left"><?php echo PQL_USER_REGISTRED ?> (<?php echo sizeof($users); ?>)</th>
<?php
if(is_array($users)){
?>
<tr>
      <td class="title"><?php echo PQL_USER ?></td>
      <td class="title"><?php echo PQL_USER_ID ?></td>
      <td class="title"><?php echo PQL_EMAIL ?></td>
      <td class="title"><?php echo PQL_LDAP_ACCOUNTSTATUS_STATUS ?></td>
      <td class="title"><?php echo PQL_OPTIONS ?></td>
    </tr>
<?php
  asort($users);
  foreach($users as $user){
      // don't know domain, so must figure it out
      $domain = $user["ou"];
      $reference = $user["reference"];
      $uid = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $reference, PQL_LDAP_ATTR_UID);
      $uid = $uid[0];
      $cn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $reference, PQL_LDAP_ATTR_CN);
      $cn = $cn[0];
      $mail = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $reference, PQL_LDAP_ATTR_MAIL);
      $mail = $mail[0];
      $status = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $reference, PQL_LDAP_ATTR_ISACTIVE);
      $status = pql_ldap_accountstatus($status[0]);
?>
    <tr class="<?php table_bgcolor(); ?>">
      <td><a href="user_detail.php?domain=<?php echo $domain ?>&user=<?php echo urlencode($reference)?>"><?php echo $cn; ?></a></td>

      <td><?php echo $uid; ?></td>
      <td><?php echo $mail; ?></td>
      <td><?php echo $status; ?></td>
      <td>
        <a href="user_detail.php?domain=<?php echo $domain ?>&user=<?php echo urlencode($reference)?>">
          <img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_USER_EDIT ?>" border="0">
        </a>
        &nbsp;&nbsp;
        <a href="user_del.php?domain=<?php echo $domain;?>&user=<?php echo urlencode($reference); ?>">
          <img src="images/del.png" width="12" height="12" alt="<?php echo PQL_USER_DELETE ?>" border="0">
        </a>
      </td>
    </tr>
<?php
	}
} else {
	// no users registred
?>
<tr class="<?php table_bgcolor(); ?>">
	<td colspan="5"><?php echo PQL_USER_NONE ?></td>
</tr>
<?php
}
?>
</table>
</body>
</html>
