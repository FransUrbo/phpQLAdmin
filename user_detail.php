<?php
// shows details of a user
// user_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql, $domain, "defaultdomain");

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

$username = pql_get_userattribute($_pql->ldap_linkid, $user, 'cn');
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_userattribute($_pql->ldap_linkid, $user, 'uid');
}
$username = $username[0];
?>

  <span class="title1"><?=$username?></span>
  <br><br>
<?php

// check if user exists
if(!pql_user_exist($_pql->ldap_linkid, $user)){
	echo "user &quot;$user&quot; does not exist";
	exit();
}

// Get basic user information
// Some of these (everything after the 'homedirectory')
// uses 'objectClass: pilotPerson' -> http://rfc-1274.rfcindex.net/
$attribs = array('cn', 'sn', 'uidNumber', 'gidNumber', 'loginShell', 'uid', 'userPassword', 'mailMessageStore', 'mailHost', 'homeDirectory', 'roomNumber', 'telePhoneNumber', 'homePhone', 'homePostalAddress', 'secretary', 'personalTitle', 'mobile', 'pager');
foreach($attribs as $attrib) {
    $attrib = strtolower($attrib);

    $value = pql_get_userattribute($_pql->ldap_linkid, $user, $attrib);
    $$attrib = utf8_decode($value[0]);
    $value = urlencode($$attrib);

    // Setup edit links
    $link = $attrib . "_link";
    $$link = "<a href=\"user_edit_attribute.php?domain=$domain&attrib=$attrib&user=$user&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify $attrib for $username\"></a>";
}
$quota = pql_get_userquota($_pql->ldap_linkid, $user);

if($userpassword == ""){
    $userpassword = PQL_LDAP_USERPASSWORD_NONE;
} else {
    if(eregi("{KERBEROS}", $userpassword)) {
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
        <td><?=$uid?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=uid&user=<?=$user?>&oldvalue=<?=$uid?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_UID_CHANGE; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php if($ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></td>
        <td><?=$userpassword?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=userpassword&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_USERPASSWORD_NEW; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>
<?php } ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_DATA_SURNAME . ", " . PQL_USER_DATA_LASTNAME; ?></td>
        <td><?=$cn?></td>
        <td><a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=cn&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_CN_CHANGE; ?>" border="0"></a></td>
      </tr>

<?php if($ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_LOGINSHELL; ?></td>
        <td><?php if($loginshell){echo $loginshell;}else{echo "none";} ?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=loginshell&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_LOGINSHELL_CHANGE; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_USER_HOMEDIR; ?></td>
        <td><?php if($homedirectory){echo $homedirectory;}else{echo "none";} ?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=homedirectory&user=<?=$user?>&oldvalue=<?=$homedirectory?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_HOMEDIRECTORY_CHANGE; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php
	if(($uidnumber != '') && ($gidnumber != '')) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo "UID;GID"; ?></td>
        <td><?=$uidnumber . ";" . $gidnumber?></td>
        <td></td>
      </tr>
<?php
	}
}
?>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Personal details</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Title</td>
        <td><?php if($personaltitle) { echo $personaltitle; } else { echo PQL_UNSET; }?></td>
        <td><?=$personaltitle_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Room number</td>
        <td><?php if($roomnumber) { echo $roomnumber; } else { echo PQL_UNSET; }?></td>
        <td><?=$roomnumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Work)</td>
        <td><?php if($telephonenumber) { echo $telephonenumber; } else { echo PQL_UNSET; }?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Home)</td>
        <td><?php if($homephone) { echo $homephone; } else { echo PQL_UNSET; }?></td>
        <td><?=$homephone_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Mobile)</td>
        <td><?php if($mobile) { echo $mobile; } else { echo PQL_UNSET; }?></td>
        <td><?=$mobile_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Pager)</td>
        <td><?php if($pager) { echo $pager; } else { echo PQL_UNSET; }?></td>
        <td><?=$pager_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Street address (Home)</td>
        <td><?php if($homepostaladdress) { echo $homepostaladdress; } else { echo PQL_UNSET; }?></td>
        <td><?=$homepostaladdress_link?></td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
$email   = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_MAIL); $email = $email[0];
$aliases = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_MAILALTERNATE);
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
        <td><?=$email?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=mail&user=<?=$user?>&mail=<?=$email?>&oldvalue=<?=$email?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAIL_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;
<?php } ?>
          <a href="user_sendmail.php?email=<?=$email?>&user=<?=$user?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_SENDMAIL; ?>" border="0"></a>
        </td>
      </tr>

<?php
if(is_array($aliases)){
    asort($aliases);
    foreach($aliases as $alias){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo PQL_LDAP_MAILALTERNATEADDRESS_TITLE; ?></td>
        <td><?=$alias?></td>
        <td><a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=mailalternateaddress&domain=<?=$domain?>&user=<?=$user?>&mailalternateaddress=<?php echo pql_strip_domain($alias); ?>&oldvalue=<?=$alias?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAILALTERNATEADDRESS_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailalternateaddress&user=<?=$user?>&value=<?=$alias?>"><img src="images/del.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAILALTERNATEADDRESS_DEL; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?=$alias?>&user=<?=$user?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_SENDMAIL; ?>" border="0"></a></td>
      </tr>
<?php
    }
}
?>
      <tr>
        <td class="subtitle" colspan="3"><a href="user_add_attribute.php?attrib=mailalternateaddress&domain=<?=$domain?>&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LDAP_MAILALTERNATEADDRESS_NEW; ?></a></td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
$forwarders = pql_search_forwarders($_pql, $user);
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
	<td><?=$forwarder["email"]?></td>
	<td>
          <a href="user_detail.php?user=<?php echo urlencode($forwarder["reference"]); ?>"><?=$forwarder[PQL_LDAP_REFERENCE_USERS_WITH]?></a>
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
	$status = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_ISACTIVE);
	$status = $status[0];

	$status = pql_ldap_accountstatus($status);
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LDAP_ACCOUNTSTATUS_TITLE; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?=$status?></td>
      </tr>

<?php if(!$SINGLE_USER) { ?>
      <tr class="subtitle">
        <td>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=accountstatus&user=<?=$user?>&set=active"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE; ?></a>
	| <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=accountstatus&user=<?=$user?>&set=nopop"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP; ?></a>
	| <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=accountstatus&user=<?=$user?>&set=disabled"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE; ?></a>
	</td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

<?php if($ADVANCED_MODE) { ?>
  <!-- Deliverymode -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LDAP_DELIVERYMODE_TITLE; ?></th></th>
      <tr>
	<td class="title"><?php echo PQL_LDAP_DELIVERYMODE_MODE; ?></td>
      </tr>

<?php
  $deliverymode = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_MODE);

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
	<td><?=$mode_text?></td>
      </tr>
<?php
    } // end of foreach
  } // end of if-else
?>

<?php if(!$SINGLE_USER) { ?>
      <tr class="subtitle">
	<td>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=deliverymode&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?php echo PQL_LDAP_DELIVERYMODE_CHANGE; ?></a>
        </td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

  <!-- advanced delivery options -->
<?php
  $qmaildotmode = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_DOTMODE);
  $qmaildotmode = $qmaildotmode[0];

  $deliveryprogrampath = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_PROGRAM);
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
        <td><?=$qmaildotmode?></td>
        <td>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=qmaildotmode&user=<?=$user?>&oldvalue=<?=$qmaildotmode?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_DELIVERYPROGRAMPATH_TITLE; ?></td>
        <td><?=$deliveryprogrampath?></td>
        <td>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=deliveryprogrampath&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
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
        <td><?=$mailmessagestore?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=mailmessagestore&user=<?=$user?>&oldvalue=<?=$mailmessagestore?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_MAILHOST_TITLE; ?></td>
        <td><?=$mailhost?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=mailhost&user=<?=$user?>&oldvalue=<?=$mailhost?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_MAILQUOTA_TITLE; ?></td>
        <td><?=$quota?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=mailquota&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
} // end if ADVANCED mode
  $forwarders = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_FORWARDS);
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
        <td><?=$forwarder?></td>
        <td>
          <a href="user_edit_attribute.php?domain=<?=$domain?>&attrib=mailforwardingaddress&user=<?=$user?>&mailforwardingaddress=<?=$forwarder?>&oldvalue=<?=$forwarder?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailforwardingaddress&user=<?=$user?>&value=<?=$forwarder?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
<?php
	} // end of foreach
} // end of if-else
?>

      <tr class="subtitle">
        <td colspan="2">
          <a href="user_add_attribute.php?attrib=mailforwardingaddress&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LDAP_MAILFORWARDINGADDRESS_NEW; ?></a>
        </td>
      </tr>
    </th>
  </table>
<?php if($ADVANCED_MODE and !$SINGLE_USER) { ?>

  <br><br>

  <!-- Access list -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">User access</th>
<?php
    foreach($_pql->ldap_basedn as $branch) {
	$dom = pql_get_domain_value($_pql, $branch, 'administrator', $USER_DN);
	if($dom) {
	    foreach($dom as $d) {
		$domains[] = $d;
	    }
	}
    }
    
    if(isset($domains)) {
	asort($domains);
	$new_tr = 0;
	foreach($domains as $key => $branch) {
	    $class=table_bgcolor(0);

	    if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php
	    } else {
?>
      <tr class="<?=$class?>">
        <td class="title">Access to DN:</td>
<?php
	    }
	    $new_tr = 1;
?>
        <td><?=$branch?></td>
      </tr>
<?php
	}
    }

    if($ALLOW_BRANCH_CREATE) {
	$class=table_bgcolor(0);
?>

      <tr class="<?=$class?>">
        <td class="title">Create branches</td>
<?php
	if(pql_validate_administrator($_pql->ldap_linkid, $_pql->ldap_basedn[0], $user)) {
?>
        <td>Yes</td>
        <td><a href="domain_edit_attributes.php?attrib=administrator&domain=<?=$_pql->ldap_basedn[0]?>&administrator=<?=$user?>&submit=4&action=delete"><img src="images/del.png" width="12" height="12" border="0" alt="Disallow user to create domains"></a></td>
<?php
	} else {
?>
        <td>No</td>
        <td><a href="domain_edit_attributes.php?attrib=administrator&domain=<?=$_pql->ldap_basedn[0]?>&administrator=<?=$user?>&submit=4&action=add"><img src="images/edit.png" width="12" height="12" border="0" alt="Allow user to create domains"></a></td>
<?php
	}
?>
      </tr>
    </th>
  </table>
<?php
    }
}

if(!$SINGLE_USER) {
?>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_del.php?domain=<?=$domain?>&user=<?=$user?>"><?=PQL_USER_DELETE?></a></td>
      </tr>
    </th>
  </table>
<?php
}
?>
</body>
</html>
