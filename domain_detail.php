<?php
// shows details of a domain
// $Id: domain_detail.php,v 1.2 2002-12-12 11:50:27 turbo Exp $
//
session_start();
require("pql.inc");

if(PQL_LDAP_CONTROL_USE){
    // include control api if control is used
    include("pql_control.inc");
    $_pql_control = new pql_control();

    // Get default domain name for this domain
    $defaultdomain = pql_get_domain_value($_pql_control->ldap_linkid, $domain, "defaultdomain");
}

include("header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and PQL_AUTO_RELOAD) {
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
<span class="title1"><?php echo $domain ?></span>

<br><br>
<?php
$_pql = new pql();

// check if domain exist
if(!pql_domain_exist($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain)){
    echo "domain &quot;$domain&quot; does not exists";
    exit();
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
$basehomedir   = pql_get_domain_value($_pql->ldap_linkid, $domain, "basehomedir");
$basemaildir   = pql_get_domain_value($_pql->ldap_linkid, $domain, "basemaildir");
$basequota     = pql_get_domain_value($_pql->ldap_linkid, $domain, "basequota");

// Setup edit links
$defaultdomain_link = "<a href=\"domain_edit_attributes.php?attrib=defaultdomain&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default domainname for $domain\"></a>";
$basehomedir_link   = "<a href=\"domain_edit_attributes.php?attrib=basehomedir&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default homedirectory for $domain\"></a>";
$basemaildir_link   = "<a href=\"domain_edit_attributes.php?attrib=basemaildir&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default maildirectory for $domain\"></a>";
$basequota_link     = "<a href=\"domain_edit_attributes.php?attrib=basequota&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default quota for $domain\"></a>";
?>

<table cellspacing="0" cellpadding="3" border="0">
  <th colspan="3" align="left"><?php echo PQL_DOMAIN_DEFAULT_VALUES ?></th>
    <tr class="<?php table_bgcolor(); ?>">
      <td class="title"><?php echo PQL_DOMAIN_DEFAULT_NAME; ?></td>
      <td><?php if($defaultdomain != $domain){echo $defaultdomain;}else{echo PQL_UNSET;} ?></td>
      <td><?php echo $defaultdomain_link; ?></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td class="title"><?php echo PQL_DOMAIN_DEFAULT_HOMEDIR; ?></td>
      <td><?php if($basehomedir != $domain){echo $basehomedir;}else{echo PQL_UNSET;} ?></td>
      <td><?php echo $basehomedir_link; ?></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td class="title"><?php echo PQL_DOMAIN_DEFAULT_MAILDIR; ?></td>
      <td><?php if($basemaildir != $domain){echo $basemaildir;}else{echo PQL_UNSET;} ?></td>
      <td><?php echo $basemaildir_link; ?></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td class="title"><?php echo PQL_LDAP_MAILQUOTA_TITLE; ?></td>
      <td><?php if($basequota != $domain){echo $basequota;}else{echo PQL_UNSET;} ?></td>
      <td><?php echo $basequota_link; ?></td>
    </tr>
  </th>
</table>

<br><br>

<?php
$users = pql_get_user($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain);
?>
<table cellspacing="0" cellpadding="3" border="0">
  <th colspan="3" align="left"><?php echo PQL_USER_REGISTRED ?></th>
<?php
if(is_array($users)){
?>
    <tr>
      <td class="title"><?php echo PQL_USER ?></td>
      <td class="title"><?php echo PQL_USER_ID ?></td>
      <td class="title"><?php echo PQL_LDAP_ACCOUNTSTATUS_STATUS ?></td>
      <td class="title"><?php echo PQL_OPTIONS ?></td>
    </tr>
<?php
	asort($users);
	foreach($users as $user){
		$uid = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_UID);
		$uid = $uid[0];
		$cn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_CN);
		$cn = $cn[0];
		
		$status = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_ISACTIVE);
		$status = pql_ldap_accountstatus($status[0]);
?>
    <tr class="<?php table_bgcolor(); ?>">
      <td><a href="user_detail.php?domain=<?php echo $domain ?>&user=<?php echo urlencode($user)?>"><?php echo $cn; ?></a></td>
      <td><?php echo $uid; ?></td>
      <td><?php echo $status; ?></td>
      <td><a href="user_detail.php?domain=<?php echo $domain ?>&user=<?php echo urlencode($user)?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_USER_EDIT ?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?domain=<?php echo $domain;?>&user=<?php echo urlencode($user); ?>"><img src="images/del.png" width="12" height="12" alt="<?php echo PQL_USER_DELETE ?>" border="0"></a></td>
    </tr>
<?php
	}
} else {
	// no users registred
?>
    <tr class="<?php table_bgcolor(); ?>">
      <td colspan="4"><?php echo PQL_USER_NONE ?></td>
    </tr>
<?php
}
?>

    <tr class="subtitle">
      <td colspan="4"><a href="user_add.php?domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_USER_NEW; ?></a></td>
    </tr>
  </th>
</table>
<?php
if(is_array($users)){
?>

<br><br>

<table cellspacing="0" cellpadding="3" border="0">
  <th colspan="2" align="left"><?php echo PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE; ?></th>
    <tr>
      <td class="title"><?php echo PQL_DOMAIN_CHANGE_ATTRIBUTE; ?></td>
      <td class="title"><?php echo PQL_OPTIONS; ?></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td><?php echo PQL_LDAP_ACCOUNTSTATUS_STATUS ?></td>
      <td><a href="domain_edit_attributes.php?attrib=accountstatus&domain=<?php echo $domain; ?>&set=active"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE; ?></a>
	| <a href="domain_edit_attributes.php?attrib=accountstatus&domain=<?php echo $domain; ?>&set=nopop"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP; ?></a>
	| <a href="domain_edit_attributes.php?attrib=accountstatus&domain=<?php echo $domain; ?>&set=disabled"><?php echo PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE; ?></a>
      </td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td><?php echo PQL_LDAP_MAILQUOTA_TITLE ?></td>
      <td><a href="domain_edit_attributes.php?attrib=mailquota&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td><?php echo PQL_LDAP_MAILHOST_TITLE ?></td>
      <td><a href="domain_edit_attributes.php?attrib=mailhost&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td><?php echo PQL_LDAP_DELIVERYMODE_TITLE ?></td>
      <td><a href="domain_edit_attributes.php?attrib=deliverymode&domain=<?php echo $domain; ?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
    </tr>
  </th>
</table>
<?php
	}
?>

<br><br>

<?php
// dns informations
$res = getmxrr($defaultdomain, $rec, $weight);
?>
<table cellspacing="0" cellpadding="3" border="0">
  <th colspan="3" align="left"><?php echo PQL_DNS_TITLE ?></th>
<?php
if(count($rec) == 0){
?>
    <tr class="<?php table_bgcolor(); ?>">
      <td colspan="2"><?php echo PQL_DNS_NONE ?></td>
    </tr>
<?php
} else {
?>
    <tr>
      <td class="title"><?php echo PQL_DNS_MAILHOST ?></td>
      <td class="title"><?php echo PQL_DNS_PRIORITY ?></td>
    </tr>
<?php
	foreach($rec as $key => $host){
?>
    <tr class="<?php table_bgcolor(); ?>">
      <td><?php echo $host; ?></td>
      <td align="right"><?php echo $weight[$key]; ?></td>
    </tr>
<?php
	} // end foreach
} // end if (count($rec) == 0)
?>
  </th>
</table>

<br><br>

<?php
if(PQL_LDAP_CONTROL_USE){
    if(pql_control_search_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "locals", $domain)){
	$locals = PQL_YES;
	if(!PQL_LDAP_CONTROL_AUTOADDLOCALS){
	    $locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&type=del&set=$domain&submit=1\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $domain from locals\"></a>";
   	} else {
	    $locals_link = "&nbsp;";
   	}
    } else {
	$locals = PQL_NO;
	if(!PQL_LDAP_CONTROL_AUTOADDLOCALS){
	    $locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&type=add&set=$domain&submit=1\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $domain to locals\"></a>";
   	} else {
	    $locals_link = "&nbsp;";
   	}
    }

    if(pql_control_search_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "rcpthosts", $domain)){
	$rcpthosts = PQL_YES;
	$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&type=del&set=$domain&submit=1\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $domain from rcpthosts\"></a>";
    } else {
	$rcpthosts = PQL_NO;
	$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&type=add&set=$domain&submit=1\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $domain to rcpthosts\"></a>";
    }
?>
<table cellspacing="0" cellpadding="3" border="0">
  <th align="left"><?php echo PQL_OPTIONS ?></th>
    <tr class="<?php table_bgcolor(); ?>">
      <td class="title">Defined in locals</td>
      <td><?php echo $locals; ?></td>
      <td><?php echo $locals_link; ?></td>
    </tr>

    <tr class="<?php table_bgcolor(); ?>">
      <td class="title">Defined in rcpthosts</td>
      <td><?php echo $rcpthosts; ?></td>
      <td><?php echo $rcpthosts_link; ?></td>
    </tr>
  </th>
</table>
<?php
} // end if(PQL_LDAP_CONTROL_USE) ...
?>

<br><br>

<table cellspacing="0" cellpadding="3" border="0">
  <th align="left"><?php echo PQL_ACTIONS; ?></th>
    <tr class="<?php table_bgcolor(); ?>">
      <td><a href="domain_del.php?domain=<?php echo $domain; ?>"><?php echo PQL_DOMAIN_DEL;?></a></td>
    </tr>
  </th>
</table>

</body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
