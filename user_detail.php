<?php
// shows details of a user
// $Id: user_detail.php,v 1.1 2002-12-11 15:09:23 turbo Exp $
//
require("pql.inc");
$_pql = new pql();

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
?>

<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="normal.css" type="text/css">
</head>

<body bgcolor="#e7e7e7" background="images/bkg.png">
<?php
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

<span class="title1"><?php echo $user; ?> (<?php echo $defaultdomain; ?>)</span>
<br><br>
<?php
// check if domain exists
if(!pql_domain_exist($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain)){
	echo "domain &quot;$domain&quot; does not exists";
	exit();
}

// check if user exists
if(!pql_user_exist($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user)){
	echo "user &quot;$user&quot; does not exist";
	exit();
}

// Get basic user information
$cn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"cn");
$sn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"sn");
$uidnr = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"uidnumber");
$gidnr = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"gidnumber");
$shell = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"loginshell");
$uid = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"uid");
$pw = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"userpassword");
$mailbox = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"mailmessagestore");
$mailhost = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"mailhost");
$homedir = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user,"homedirectory");
$quota = pql_get_userquota($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user);

$uid = $uid[0]; $pw = $pw[0]; $mailbox = $mailbox[0]; $mailhost = $mailhost[0];
$homedir = $homedir[0]; $shell = $shell[0];

if($pw == ""){
	$pw = PQL_LDAP_USERPASSWORD_NONE;
} else {
    if(ereg("{KERBEROS}", $pw)) {
	$princ = split("}", $pw);
	$pw = $princ[1] . " " . PQL_LDAP_USERPASSWORD_KERBEROS;
    } else {
	$pw = PQL_LDAP_USERPASSWORD_ENCRYPTED;
    }
}

if($mailbox == ""){
	$mailbox = PQL_LDAP_MAILMESSAGESTORE_NONE;
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
    <tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></td>
	<td><?php echo $pw; ?></td>
	<td><a href="user_edit_attribute.php?attrib=userpassword&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_USERPASSWORD_NEW; ?>" border="0"></a></td>
    </tr>
    <tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_USER_DATA_SURNAME . ", " . PQL_USER_DATA_LASTNAME; ?></td>
	<td><?php echo $cn[0]; ?></td>
	<td><a href="user_edit_attribute.php?attrib=cn&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_CN_CHANGE; ?>" border="0"></a></td>
    </tr>
    <tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_USER_LOGINSHELL; ?></td>
	<td><?php if($shell){echo $shell;}else{echo "none";} ?></td>
	<td><a href="user_edit_attribute.php?attrib=loginshell&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_LOGINSHELL_CHANGE; ?>" border="0"></a></td>
    </tr>
    <tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_USER_HOMEDIR; ?></td>
	<td><?php if($homedir){echo $homedir;}else{echo "none";} ?></td>
	<td><a href="user_edit_attribute.php?attrib=homedirectory&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $homedir; ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_HOMEDIRECTORY_CHANGE; ?>" border="0"></a></td>
    </tr>
    <tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo "UID;GID"; ?></td>
	<td><?php if($uidnr[0] && $gidnr[0]){echo $uidnr[0] . ";" . $gidnr[0];}else{echo "none";} ?></td>
	<td><center>x</center></td>
    </tr>
  </th>
</table>

<br>

<?php
$email = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_MAIL);
$aliases = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_MAILALTERNATE);
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
	<td><?php echo $email[0]; ?></td>
	<td><a href="user_edit_attribute.php?attrib=mail&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&mail=<?php echo pql_strip_domain($email[0]); ?>&oldvalue=<?php echo $email[0] ?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LDAP_MAIL_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?php echo $email[0];?>&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_SENDMAIL; ?>" border="0"></a></td>
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
</table>

<br>
<br>
<?php
$forwarders = pql_search_forwarders($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user);
?>
<!-- forwarders in other accounts to this user  -->
<table cellspacing="0" cellpadding="3" border="0">
<th colspan="2" align="left"><?php echo PQL_LDAP_MAILFORWARDINGADDRESS_OTHER; ?></th>
<tr>
	<td class="title"><?php echo PQL_EMAIL; ?></td>
	<td class="title"><?php echo PQL_USER; ?></td>
</tr>
<?php
if(empty($forwarders)){
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
	<td><a href="user_detail.php?user=<?php echo urlencode($forwarder["reference"]); ?>&domain=<?php echo $forwarder["domain"]; ?>"><?php echo $forwarder["cn"];?>@<?php echo $forwarder["domain"];?></a></td>
</tr>
<?php
	} // end of foreach
} // end of if-else
?>
</table>

<br>
<br>

<?php
	$status = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_ISACTIVE);
	$status = pql_ldap_accountstatus($status[0]);
?>
<!-- accountstatus -->
<table cellspacing="0" cellpadding="3" border="0">
<th colspan="2" align="left"><?php echo PQL_LDAP_ACCOUNTSTATUS_TITLE; ?></th>
<tr class="<?php table_bgcolor(); ?>">
	<td><?php echo $status; ?></td>
</tr>
<tr class="subtitle">
		<td><a href="user_edit_attribute.php?attrib=accountstatus&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&set=active"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE; ?></a>
	| <a href="user_edit_attribute.php?attrib=accountstatus&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&set=nopop"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP; ?></a>
	| <a href="user_edit_attribute.php?attrib=accountstatus&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&set=disabled"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE; ?></a>
	</td>
</tr>
</table>
<br>
<br>
<!-- Deliverymode -->
<table cellspacing="0" cellpadding="3" border="0">
<th colspan="2" align="left"><?php echo PQL_LDAP_DELIVERYMODE_TITLE; ?></th></th>
<tr>
	<td class="title"><?php echo PQL_LDAP_DELIVERYMODE_MODE; ?></td>
</tr>

<?php
$deliverymode = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_MODE);

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
	<td><a href="user_edit_attribute.php?attrib=deliverymode&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?php echo PQL_LDAP_DELIVERYMODE_CHANGE; ?></a></td>
</tr>
</table>
<br>
<br>
<!-- advanced delivery options -->
<?php
$qmaildotmode = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_DOTMODE);
$deliveryprogrampath = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_PROGRAM);

$qmaildotmode = $qmaildotmode[0];
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
	<td><a href="user_edit_attribute.php?attrib=qmaildotmode&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $qmaildotmode; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
</tr>
<tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_LDAP_DELIVERYPROGRAMPATH_TITLE; ?></td>
	<td><?php echo $deliveryprogrampath; ?></td>
	<td><a href="user_edit_attribute.php?attrib=deliveryprogrampath&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
</tr>
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
	<td><?php echo $mailbox; ?></td>
	<td><a href="user_edit_attribute.php?attrib=mailmessagestore&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $mailbox; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
</tr>
<tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_LDAP_MAILHOST_TITLE; ?></td>
	<td><?php echo $mailhost; ?></td>
	<td><a href="user_edit_attribute.php?attrib=mailhost&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&oldvalue=<?php echo $mailhost; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
</tr>
<tr class="<?php table_bgcolor(); ?>">
	<td class="title"><?php echo PQL_LDAP_MAILQUOTA_TITLE; ?></td>
	<td><?php echo $quota; ?></td>
	<td><a href="user_edit_attribute.php?attrib=mailquota&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
</tr>
</table>
<br>
<br>
<?php
$forwarders = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_FORWARDS);
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
	<td><a href="user_edit_attribute.php?attrib=mailforwardingaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&mailforwardingaddress=<?php echo $forwarder; ?>&oldvalue=<?php echo $forwarder ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailforwardingaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain; ?>&value=<?php echo $forwarder ?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
</tr>
<?php
	} // end of foreach
} // end of if-else
?>
<tr class="subtitle">
	<td colspan="2"><a href="user_add_attribute.php?attrib=mailforwardingaddress&user=<?php echo urlencode($user); ?>&domain=<?php echo $domain ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LDAP_MAILFORWARDINGADDRESS_NEW; ?></a></td>
</tr>
</table>

</body>
</html>
