<?php
// shows details of a domain
// domain_detail.php,v 2.2 2002/12/17 06:28:26 turbo Exp
//
session_start();

require("include/pql.inc");

if(PQL_LDAP_CONTROL_USE){
    // include control api if control is used
    include("include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST_CTR, $USER_DN, $USER_PASS);

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
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	refreshFrames();
  //--></script>
<?php
}

$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

// check if domain exist
if(!pql_domain_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain)){
    echo "domain &quot;$domain&quot; does not exists";
    exit();
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
?>
  <span class="title1">Domain: <?=$domain?><?php if($defaultdomain) {echo " ($defaultdomain)";} ?></span>
  <br><br>
<?php
$basehomedir = pql_get_domain_value($_pql->ldap_linkid, $domain, "basehomedir");
$basemaildir = pql_get_domain_value($_pql->ldap_linkid, $domain, "basemaildir");
$basequota   = pql_get_domain_value($_pql->ldap_linkid, $domain, "basequota");
$admins		 = pql_get_domain_value($_pql->ldap_linkid, $domain, "administrator");

// Setup edit links
$defaultdomain_link = "<a href=\"domain_edit_attributes.php?attrib=defaultdomain&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default domainname for $domain\"></a>";
$basehomedir_link   = "<a href=\"domain_edit_attributes.php?attrib=basehomedir&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default homedirectory for $domain\"></a>";
$basemaildir_link   = "<a href=\"domain_edit_attributes.php?attrib=basemaildir&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default maildirectory for $domain\"></a>";
$basequota_link     = "<a href=\"domain_edit_attributes.php?attrib=basequota&domain=$domain\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify default quota for $domain\"></a>";

	if($ADVANCED_MODE == 1) {
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_DOMAIN_DEFAULT_VALUES?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_DOMAIN_DEFAULT_NAME?></td>
        <td><?php if($defaultdomain != $domain){echo $defaultdomain;}else{echo PQL_UNSET;} ?></td>
        <td><?=$defaultdomain_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_DOMAIN_DEFAULT_HOMEDIR?></td>
        <td><?php if($basehomedir != $domain){echo $basehomedir;}else{echo PQL_UNSET;} ?></td>
        <td><?=$basehomedir_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_DOMAIN_DEFAULT_MAILDIR?></td>
        <td><?php if($basemaildir != $domain){echo $basemaildir;}else{echo PQL_UNSET;} ?></td>
        <td><?=$basemaildir_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LDAP_MAILQUOTA_TITLE?></td>
        <td><?php if($basequota != $domain){echo $basequota;}else{echo PQL_UNSET;} ?></td>
        <td><?=$basequota_link?></td>
      </tr>

<?php
	if(is_array($admins)) {
		foreach($admins as $admin) {
			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php
			} else {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_DOMAIN_ADMIN_TITLE?></td>
<?php
			}
?>

        <td><?=$admin?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&domain=<?=$domain?>&administrator=<?=$admin?>&submit=3&action=modify"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify administrators for <?=$domain?>"></a>&nbsp;
          <a href="domain_edit_attributes.php?attrib=administrator&domain=<?=$domain?>&administrator=<?=$admin?>&submit=4&action=delete"><img src="images/del.png" width="12" height="12" alt="<?=PQL_DOMAIN_ADMIN_DELETE?>" border="0"></a>
        </td>
      </tr>

<?php
			$new_tr = 1;
		}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=administrator&domain=<?=$domain?>&submit=3&action=add"><?=PQL_DOMAIN_ADMIN_ADD?></a>
        </td>
      </tr>
<?php
	}
?>
    </th>
  </table>

  <br><br>

<?php
}
$users = pql_get_user($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_USER_REGISTRED?></th>
<?php
if(is_array($users)){
?>
      <tr>
        <td class="title"><?=PQL_USER?></td>
        <td class="title"><?=PQL_USER_ID?></td>
        <td class="title"><?=PQL_LDAP_ACCOUNTSTATUS_STATUS?></td>
        <td class="title"><?=PQL_OPTIONS?></td>
      </tr>

<?php
	asort($users);
	foreach($users as $user){
		$uid = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_UID);
		$uid = $uid[0];
		$cn = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_CN);
		$cn = $cn[0];
		
		$status = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_ISACTIVE);
		$status = pql_ldap_accountstatus($status[0]);
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_detail.php?domain=<?=$domain?>&user=<?=urlencode($user)?>"><?=$cn?></a></td>
        <td><?=$uid?></td>
        <td><?=$status?></td>
        <td><a href="user_detail.php?domain=<?=$domain?>&user=<?=urlencode($user)?>"><img src="images/edit.png" width="12" height="12" alt="<?=PQL_USER_EDIT?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?domain=<?=$domain?>&user=<?=urlencode($user)?>"><img src="images/del.png" width="12" height="12" alt="<?=PQL_USER_DELETE?>" border="0"></a></td>
      </tr>

<?php
	}
} else {
	// no users registred
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="4"><?=PQL_USER_NONE?></td>
      </tr>
<?php
}
?>

      <tr class="subtitle">
        <td colspan="4"><a href="user_add.php?domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=PQL_USER_NEW?></a></td>
      </tr>
    </th>
  </table>
<?php
if(is_array($users)){
?>

  <br><br>
  
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE?></th>
      <tr>
        <td class="title"><?=PQL_DOMAIN_CHANGE_ATTRIBUTE?></td>
        <td class="title"><?=PQL_OPTIONS?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LDAP_ACCOUNTSTATUS_STATUS?></td>
        <td><a href="domain_edit_attributes.php?attrib=accountstatus&domain=<?=$domain?>&set=active"><?=PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&domain=<?=$domain?>&set=nopop"><?=PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&domain=<?=$domain?>&set=disabled"><?=PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE?></a>
        </td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LDAP_MAILQUOTA_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailquota&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LDAP_MAILHOST_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailhost&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LDAP_DELIVERYMODE_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=deliverymode&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
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
if($ADVANCED_MODE == 1) {
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_DNS_TITLE?></th>
<?php
	if(count($rec) == 0){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="2"><?=PQL_DNS_NONE?></td>
      </tr>
<?php
	} else {
?>
      <tr>
        <td class="title"><?=PQL_DNS_MAILHOST?></td>
        <td class="title"><?=PQL_DNS_PRIORITY?></td>
      </tr>
<?php
		asort($weight);
		foreach($weight as $key => $prio){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$rec[$key]?></td>
        <td align="right"><?=$prio?></td>
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
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "locals", $domain)){
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
		
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "rcpthosts", $domain)){
			$rcpthosts = PQL_YES;
			$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&type=del&set=$domain&submit=1\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $domain from rcpthosts\"></a>";
		} else {
			$rcpthosts = PQL_NO;
			$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&type=add&set=$domain&submit=1\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $domain to rcpthosts\"></a>";
		}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_OPTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Defined in locals</td>
        <td><?=$locals?></td>
        <td><?=$locals_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Defined in rcpthosts</td>
        <td><?=$rcpthosts?></td>
        <td><?=$rcpthosts_link?></td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
	} // end if(PQL_LDAP_CONTROL_USE) ...
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="domain_del.php?domain=<?=$domain?>"><?=PQL_DOMAIN_DEL?></a></td>
      </tr>
    </th>
  </table>
<?php
} // end if ADVANCE mode
?>
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
