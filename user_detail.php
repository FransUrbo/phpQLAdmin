<?php
// shows details of a user
// user_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");

$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");

include("./header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and PQL_AUTO_RELOAD){
	if($rlnb == 1) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    refreshFrames();
  //--></script>
<?php
	} elseif($rlnb == 2) {
?>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    parent.frames.pqlnav.location.reload();
  //--></script>
<?php
	}
}
?>

  <span class="title1"><?php echo $user; ?> (<?php echo $defaultdomain; ?>)</span>
  <br><br>
<?php
// check if domain exists
if(!pql_domain_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain)){
    echo "domain &quot;$domain&quot; does not exists";
    exit();
}

// check if user exists
if(!pql_user_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user)){
	echo "user &quot;$user&quot; does not exist";
	exit();
}

// Get basic user information
// Some of these (everything after the 'homedirectory')
// uses 'objectClass: pilotPerson' -> http://rfc-1274.rfcindex.net/
$attribs = array('cn', 'sn', 'uidnumber', 'gidnumber', 'loginshell', 'uid', 'userpassword', 'mailmessagestore', 'mailhost', 'homedirectory', 'roomnumber', 'homephone', 'homepostaladdress', 'secretary', 'personaltitle', 'mobile', 'pager');
foreach($attribs as $attrib) {
    $value = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, $attrib);
    $$attrib = $value[0];

    // Setup edit links
    $link = $attrib . "_link";
    $$link = "<a href=\"user_edit_attribute.php?attrib=$attrib&user=<?php echo urlencode($user); ?>&domain=<?=$domain?>\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify $attrib for $user\"></a>";
}
$quota = pql_get_userquota($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user);

if($userpassword == ""){
    $userpassword = PQL_LDAP_USERPASSWORD_NONE;
} else {
    if(ereg("{KERBEROS}", $userpassword)) {
	$princ = split("}", $userpassword);
	$userpassword = $princ[1] . " " . PQL_LDAP_USERPASSWORD_KERBEROS;
    } else {
	$userpassword = PQL_LDAP_USERPASSWORD_ENCRYPTED;
    }
}

if($mailmessagestore == ""){
    $mailmessagestore = PQL_LDAP_MAILMESSAGESTORE_NONE;
}

if($mailhost == ""){
    $mailhost = PQL_LDAP_MAILHOST_NONE;
}
?>

  <!-- Basic user details - Full name (sn), Login shell etc -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_USER_DATA; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_ID ?></td>
        <td><?php echo $uid; ?></td>
        <td><a href="user_edit_attribute.php?attrib=uid&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $uid ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_UID_CHANGE; ?>" border="0"></a></td>
      </tr>

<?php if($ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></td>
        <td><?php echo $userpassword; ?></td>
        <td><a href="user_edit_attribute.php?attrib=userpassword&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_USERPASSWORD_NEW; ?>" border="0"></a></td>
      </tr>
<?php } ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_DATA_SURNAME . ", " . PQL_USER_DATA_LASTNAME; ?></td>
        <td><?php echo $cn; ?></td>
        <td><a href="user_edit_attribute.php?attrib=cn&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_CN_CHANGE; ?>" border="0"></a></td>
      </tr>

<?php if($ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_LOGINSHELL; ?></td>
        <td><?php if($loginshell){echo $loginshell;}else{echo "none";} ?></td>
        <td><a href="user_edit_attribute.php?attrib=loginshell&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_LOGINSHELL_CHANGE; ?>" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_HOMEDIR; ?></td>
        <td><?php if($homedirectory){echo $homedirectory;}else{echo "none";} ?></td>
        <td><a href="user_edit_attribute.php?attrib=homedirectory&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $homedirectory; ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_HOMEDIRECTORY_CHANGE; ?>" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo "UID;GID"; ?></td>
        <td><?php if($uidnumber && $gidnumber){echo $uidnumber . ";" . $gidnumber;}else{echo "none";} ?></td>
        <td><center>x</center></td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

<?php
if($roomnumber or $homephone or 
   $homepostaladdress or $secretary or 
   $personaltitle or $mobile or
   $pager)
{
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Personal details</th>
<?php if($personaltitle) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Title</td>
        <td><?=$personaltitle?></td>
        <td><?=$personaltitle_link?></td>
      </tr>

<?php } if($roomnumber) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Room number</td>
        <td><?=$roomnumber?></td>
        <td><?=$roomnumber_link?></td>
      </tr>

<?php } if($homephone) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Home telephone number</td>
        <td><?=$homephone?></td>
        <td><?=$homephone_link?></td>
      </tr>

<?php } if($mobile) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Mobile telephone number</td>
        <td><?=$mobile?></td>
        <td><?=$mobile_link?></td>
      </tr>

<?php } if($pager) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Pager number</td>
        <td><?=$pager?></td>
        <td><?=$pager_link?></td>
      </tr>

<?php } ?>
    </th>
  </table>

  <br><br>

<?php
}

$email = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_MAIL);
$email = $email[0];

$aliases = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_MAILALTERNATE);
?>
  <!-- Addresses (mail, mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_ADDRESS_REGISTRED; ?></th>
      <tr>
        <td class="title"><?php echo PQL_ADDRESS_TYPE; ?></td>
        <td class="title"><?php echo PQL_EMAIL; ?></td>
        <td class="title"><?php echo PQL_OPTIONS; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo PQL_LDAP_MAIL_TITLE; ?></td>
        <td><?php echo $email; ?></td>
        <td><a href="user_edit_attribute.php?attrib=mail&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&mail=<?php echo pql_strip_domain($email); ?>&oldvalue=<?php echo $email ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAIL_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?php echo $email;?>&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_SENDMAIL; ?>" border="0"></a></td>
      </tr>

<?php
if(is_array($aliases)){
    asort($aliases);
    foreach($aliases as $alias){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo PQL_LDAP_MAILALTERNATEADDRESS_TITLE; ?></td>
        <td><?php echo $alias; ?></td>
        <td><a href="user_edit_attribute.php?attrib=mailalternateaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&mailalternateaddress=<?php echo pql_strip_domain($alias); ?>&oldvalue=<?php echo $alias ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAILALTERNATEADDRESS_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailalternateaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&value=<?php echo $alias ?>"><img src="images/del.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAILALTERNATEADDRESS_DEL; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?php echo $alias;?>&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_SENDMAIL; ?>" border="0"></a></td>
      </tr>
<?php
    }
}
?>
      <tr>
        <td class="subtitle" colspan="3"><a href="user_add_attribute.php?attrib=mailalternateaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LDAP_MAILALTERNATEADDRESS_NEW; ?></a></td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
$forwarders = pql_search_forwarders($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user);
?>
  <!-- forwarders in other accounts to this user  -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LDAP_MAILFORWARDINGADDRESS_OTHER; ?></th>
      <tr>
	<td class="title"><?php echo PQL_EMAIL; ?></td>
	<td class="title"><?php echo PQL_USER; ?></td>
      </tr>

<?php
if(empty($forwarders)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td colspan="3"><?php echo PQL_LDAP_MAILFORWARDINGADDRESS_NONE; ?></td>
      </tr>
<?php
} else {
	foreach($forwarders as $forwarder){
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?php echo $forwarder["email"];?></td>
	<td>
          <a href="user_detail.php?user=<?php echo urlencode($forwarder["reference"]); ?>&domain=<?php echo $forwarder["domain"]; ?>"><?php echo $forwarder[PQL_LDAP_REFERENCE_USERS_WITH];?></a>
        </td>
      </tr>
<?php
	} // end of foreach
} // end of if-else
?>
    </th>
  </table>

  <br><br>

<?php
	$status = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_ISACTIVE);
	$status = $status[0];

	$status = pql_ldap_accountstatus($status);
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LDAP_ACCOUNTSTATUS_TITLE; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?php echo $status; ?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="user_edit_attribute.php?attrib=accountstatus&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&set=active"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE; ?></a>
	| <a href="user_edit_attribute.php?attrib=accountstatus&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&set=nopop"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP; ?></a>
	| <a href="user_edit_attribute.php?attrib=accountstatus&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&set=disabled"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE; ?></a>
	</td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
if($ADVANCED_MODE) {
 ?>
  <!-- Deliverymode -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LDAP_DELIVERYMODE_TITLE; ?></th></th>
      <tr>
	<td class="title"><?php echo PQL_LDAP_DELIVERYMODE_MODE; ?></td>
      </tr>

<?php
  $deliverymode = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_MODE);

  if(empty($deliverymode)){
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?php echo PQL_LDAP_DELIVERYMODE_NULL; ?></td>
      </tr>
<?php
  } else {
    foreach($deliverymode as $mode){
	$mode_text = pql_ldap_deliverymode($mode);
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?php echo $mode_text; ?></td>
      </tr>
<?php
    } // end of foreach
  } // end of if-else
?>

      <tr class="subtitle">
	<td>
          <a href="user_edit_attribute.php?attrib=deliverymode&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?php echo PQL_LDAP_DELIVERYMODE_CHANGE; ?></a>
        </td>
      </tr>
    </th>
  </table>

  <br><br>

  <!-- advanced delivery options -->
<?php
  $qmaildotmode = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_DOTMODE);
  $qmaildotmode = $qmaildotmode[0];

  $deliveryprogrampath = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_PROGRAM);
  $deliveryprogrampath = $deliveryprogrampath[0];

  if($qmaildotmode == ""){
    $qmaildotmode = PQL_LDAP_QMAILDOTMODE_NONE;
  }

  if($deliveryprogrampath == ""){
    $deliveryprogrampath = PQL_LDAP_DELIVERYPROGRAMPATH_NONE;
  }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_USER_DELIVERYPROPERTIES; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_QMAILDOTMODE_TITLE; ?></td>
        <td><?php echo $qmaildotmode; ?></td>
        <td>
          <a href="user_edit_attribute.php?attrib=qmaildotmode&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $qmaildotmode; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_DELIVERYPROGRAMPATH_TITLE; ?></td>
        <td><?php echo $deliveryprogrampath; ?></td>
        <td>
          <a href="user_edit_attribute.php?attrib=deliveryprogrampath&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
    </th>
  </table>

  <br><br>

  <!-- misc MAIL Attributes (mailmessagestore, mailhost, mailquota)-->
<?php
  if(!is_array($quota)){
    $quota = PQL_LDAP_MAILQUOTA_DEFAULT;
  } else {
    //$quota =
    $quota = pql_ldap_mailquota($quota);
  }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_USER_MAILBOXPROPERTIES; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_MAILMESSAGESTORE_TITLE; ?></td>
        <td><?php echo $mailmessagestore; ?></td>
        <td>
          <a href="user_edit_attribute.php?attrib=mailmessagestore&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $mailmessagestore; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_MAILHOST_TITLE; ?></td>
        <td><?php echo $mailhost; ?></td>
        <td>
          <a href="user_edit_attribute.php?attrib=mailhost&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $mailhost; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_MAILQUOTA_TITLE; ?></td>
        <td><?php echo $quota; ?></td>
        <td>
          <a href="user_edit_attribute.php?attrib=mailquota&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
} // end if ADVANCED mode
  $forwarders = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_FORWARDS);
?>
  <!-- Forwarders (mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LDAP_MAILFORWARDINGADDRESS_TITLE; ?></th>
      <tr>
        <td class="title"><?php echo PQL_EMAIL; ?></td>
        <td class="title"><?php echo PQL_OPTIONS; ?></td>
      </tr>

<?php
if(empty($forwarders)){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="2"><?php echo PQL_LDAP_MAILFORWARDINGADDRESS_NONE; ?></td>
      </tr>
<?php
} else {
    foreach($forwarders as $forwarder){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $forwarder;?></td>
        <td>
          <a href="user_edit_attribute.php?attrib=mailforwardingaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&mailforwardingaddress=<?php echo $forwarder; ?>&oldvalue=<?php echo $forwarder ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailforwardingaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&value=<?php echo $forwarder ?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
<?php
	} // end of foreach
} // end of if-else
?>

      <tr class="subtitle">
        <td colspan="2">
          <a href="user_add_attribute.php?attrib=mailforwardingaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LDAP_MAILFORWARDINGADDRESS_NEW; ?></a>
        </td>
      </tr>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_del.php?domain=<?=$domain?>&user=<?php echo urlencode($user); ?>"><?=PQL_USER_DELETE?></a></td>
      </tr>
    </th>
  </table>
</body>
</html>
