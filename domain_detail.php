<?php
// shows details of a domain
// domain_detail.php,v 2.2 2002/12/17 06:28:26 turbo Exp
//
session_start();
require("./include/pql_config.inc");

if($config["PQL_GLOB_CONTROL_USE"]) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
}

include("./header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and $config["PQL_GLOB_AUTO_RELOAD"]) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// check if domain exist
$dc = ldap_explode_dn($domain, 0); $dc = split('=', $dc[0]);
if(!pql_domain_exist($_pql, $dc[1])){
    echo "Domain &quot;$domain&quot; does not exists<br><br>";
	echo "Is this perhaps a Top Level DN (namingContexts), and you haven't configured ";
	echo "how to reference domains/branches in this database!?<br><br>";
	echo "Please go to <a href=\"config_detail.php\">Show configuration</a> and double check.<br>";
	echo "Look at the config option 'Reference domains with'.";
    exit();
}

// Get some default values for this domain
// Some of these (everything after the 'o' attribute)
// uses 'objectClass: dcOrganizationNameForm' -> http://rfc-2377.rfcindex.net/
$attribs = array('defaultdomain', 'basehomedir', 'basemaildir', 'o', 'l',
				 'postalcode', 'postaladdress', 'telephonenumber', 'street',
				 'facsimiletelephonenumber', 'postofficebox', 'st', 'basequota',
				 'maximumdomainusers');
foreach($attribs as $attrib) {
	// Get default value
	$value = pql_get_domain_value($_pql, $domain, $attrib);
	$$attrib = utf8_decode($value);

	// Setup edit links. If it's a dcOrganizationNameForm attribute, then
	// we add a delete link as well.
	$link = $attrib . "_link";
	if(($attrib != 'defaultdomain') and ($attrib != 'basehomedir') and ($attrib != 'basemaildir')) {
		if(($attrib == 'maximumdomainusers') and !$value)
		  $value = 0;

		// A dcOrganizationNameForm attribute
		$$link = "<a href=\"domain_edit_attributes.php?type=modify&attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=". urlencode($value) ."\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify attribute $attrib for $domain\"></a>&nbsp;<a href=\"domain_edit_attributes.php?type=delete&submit=2&attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=". urlencode($value) ."\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Delete attribute $attrib for $domain\"></a>";
	} else {
		// A phpQLAdminBranch attribute
		$$link = "<a href=\"domain_edit_attributes.php?attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify $attrib for $domain\"></a>";
	}
}
$admins	   = pql_get_domain_value($_pql, $domain, "administrator");
$seealso   = pql_get_domain_value($_pql, $domain, "seealso");
$basequota = pql_ldap_mailquota(pql_parse_quota($basequota));

// Get the organization name, or show 'Not set' with an URL to set it
$domainname = pql_get_domain_value($_pql, $domain, 'o');
if(!$domainname) {
	$domainname = "<a href=\"domain_edit_attributes.php?type=modify&attrib=o&rootdn=$rootdn&domain=$domain\">".PQL_LANG_UNSET."</a>";
}
?>
  <span class="title1">Organization: <?=urldecode($domainname)?></span>
<?php
if($ADVANCED_MODE == 1) {
?>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_LANG_DOMAIN_DEFAULT_VALUES?></th>
<?php if($ALLOW_BRANCH_CREATE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Domain/Branch DN</td>
        <td><?=$domain?></td>
        <td><?=// TODO: Should we be able to rename the RDN? ?></td>
      </tr>
  
<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_DEFAULT_NAME?></td>
        <td><?php if($defaultdomain){echo $defaultdomain;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$defaultdomain_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_DEFAULT_HOMEDIR?></td>
        <td><?php if($basehomedir){echo $basehomedir;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$basehomedir_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_DEFAULT_MAILDIR?></td>
        <td><?php if($basemaildir){echo $basemaildir;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$basemaildir_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_MAILQUOTA_TITLE?></td>
        <td><?php if($basequota){echo $basequota;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$basequota_link?></td>
      </tr>

<?php if($ALLOW_BRANCH_CREATE and $ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Maximum allowed users in branch</td>
        <td><?php if($maximumdomainusers){echo $maximumdomainusers;}else{echo "unlimited";} ?></td>
        <td><?=$maximumdomainusers_link?></td>
      </tr>

<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_ADMIN_TITLE?></td>
<?php
	if(is_array($admins)) {
		$new_tr = 0;
		foreach($admins as $admin) {
			$username = pql_get_userattribute($_pql->ldap_linkid, $admin, 'cn');
			if(!$username[0])
			  $username = $admin;
			else
			  $username = $username[0];

			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php
			}
?>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$admin?>"><?=urldecode($username)?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&administrator=<?=$admin?>&submit=3&action=modify"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify administrators for <?=$domain?>"></a>
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&administrator=<?=$admin?>&submit=4&action=delete"><img src="images/del.png" width="12" height="12" alt="<?=PQL_LANG_DOMAIN_ADMIN_DELETE?>" border="0"></a>
        </td>
      </tr>

<?php
			$new_tr = 1;
		}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add"><?=PQL_LANG_DOMAIN_ADMIN_ADD?></a>
        </td>
      </tr>
<?php
	} else {
?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add"><?=PQL_LANG_DOMAIN_ADMIN_ADD?></a>
        </td>
<?php
	}
}
?>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Branch owner</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Organization name</td>
        <td><?php if($o) {echo urldecode($o);}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$o_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Postal code</td>
        <td><?php if($postalcode) {echo $postalcode;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$postalcode_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Post box</td>
        <td><?php if($postofficebox) {echo $postofficebox;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$postofficebox_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Postal address</td>
        <td><?php if($postaladdress) {echo $postaladdress;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$postaladdress_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Street address</td>
        <td><?php if($street) {echo urldecode($street);}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$street_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">City</td>
        <td><?php if($l) {echo urldecode($l);}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$l_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">State</td>
        <td><?php if($st) {echo $st;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$st_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number</td>
        <td><?php if($telephonenumber) {echo $telephonenumber;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Fax number</td>
        <td><?php if($facsimiletelephonenumber) {echo $facsimiletelephonenumber;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$facsimiletelephonenumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Contact person</td>
<?php
	if(is_array($seealso)) {
		$new_tr = 0;
		foreach($seealso as $sa) {
			$username = pql_get_userattribute($_pql->ldap_linkid, $sa, 'cn');
			if(!$username[0])
			  $username = $sa;
			else
			  $username = $username[0];

			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php
			}
?>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$sa?>"><?=urldecode($username)?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&seealso=<?=$sa?>&submit=3&action=modify"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify contact persons for <?=$o?>"></a>
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&seealso=<?=$sa?>&submit=4&action=delete"><img src="images/del.png" width="12" height="12" alt="Remove contact person from <?=$o?>" border="0"></a>
        </td>
      </tr>

<?php
			$new_tr = 1;
		}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add">Add contact person for domain</a>
        </td>
      </tr>
<?php
	} else {
?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add">Add contact person for domain</a>
        </td>
<?php
	}
?>
    </th>
  </table>

  <br><br>

<?php $users = pql_get_user($_pql->ldap_linkid, $domain); ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_LANG_USER_REGISTRED?></th>
<?php
if(is_array($users)) {
?>
      <tr>
        <td class="title"><?=PQL_LANG_USER?></td>
        <td class="title"><?=PQL_LANG_USER_ID?></td>
        <td class="title"><?=PQL_LANG_ACCOUNTSTATUS_STATUS?></td>
        <td class="title"><?=PQL_LANG_OPTIONS?></td>
      </tr>

<?php
	asort($users);
	foreach($users as $user){
		$uid   = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_UID"]); $uid = $uid[0];
		$cn    = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_CN"]); $cn = $cn[0];
		$uidnr = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_QMAILUID"]); $uidnr = $uidnr[0];
		
		$status = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_ISACTIVE"]);
		$status = pql_ldap_accountstatus($status[0]);

		if(($uid != 'root') or ($uidnr != '0')) {
			// Do NOT show root user(s) here! This should (for safty's sake)
			// not be availible to administrate through phpQLAdmin!
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=urlencode($user)?>"><?=$cn?></a></td>
        <td><?=$uid?></td>
        <td><?=$status?></td>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="<?=PQL_LANG_USER_EDIT?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$user?>"><img src="images/del.png" width="12" height="12" alt="<?=PQL_LANG_USER_DELETE?>" border="0"></a></td>
      </tr>

<?php
		}
	}
} else {
	// no users registred
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="4"><?=PQL_LANG_USER_NONE?></td>
      </tr>
<?php
}
?>

      <tr class="subtitle">
        <td colspan="4"><a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=PQL_LANG_USER_NEW?></a></td>
      </tr>
    </th>
  </table>
<?php
if(is_array($users)){
?>

  <br><br>
  
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE_TITLE?></th>
      <tr>
        <td class="title"><?=PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE?></td>
        <td class="title"><?=PQL_LANG_OPTIONS?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_ACCOUNTSTATUS_STATUS?></td>
        <td><a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=active"><?=PQL_LANG_ACCOUNTSTATUS_CHANGE_ACTIVE?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=nopop"><?=PQL_LANG_ACCOUNTSTATUS_CHANGE_NOPOP?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=disabled"><?=PQL_LANG_ACCOUNTSTATUS_CHANGE_DISABLE?></a>
        </td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_MAILQUOTA_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailquota&rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_MAILHOST_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailhost&rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_DELIVERYMODE_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=deliverymode&rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
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
    <th colspan="3" align="left"><?=PQL_LANG_DNS_TITLE?></th>
<?php
	if(count($rec) == 0){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="2"><?=PQL_LANG_DNS_NONE?></td>
      </tr>
<?php
	} else {
?>
      <tr>
        <td class="title"><?=PQL_LANG_DNS_MAILHOST?></td>
        <td class="title"><?=PQL_LANG_DNS_PRIORITY?></td>
      </tr>
<?php
		asort($weight);
		foreach($weight as $key => $prio) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$rec[$key]?></td>
        <td align="right"><?=$prio?></td>
      </tr>
<?php
		} // end foreach
	} // end if (count($rec) == 0)
?>

      <tr class="subtitle">
        <td colspan="4"><a href="dnszonetemplate.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&defaultdomain=<?=$defaultdomain?>"><img src="images/edit.png" width="12" height="12" border="0">Create DNS template zone file</a></td>
      </tr>
    </th>
  </table>

  <br><br>

<?php
	if($config["PQL_GLOB_CONTROL_USE"]) {
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "locals", $defaultdomain)){
			$locals = PQL_LANG_YES;
			if(!$config["PQL_GLOB_CONTROL_AUTOADDLOCALS"][$rootdn]) {
				$locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&rootdn=<?=$rootdn?>&type=del&set=$defaultdomain&submit=1\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $defaultdomain from locals\"></a>";
			} else {
				$locals_link = "&nbsp;";
			}
		} else {
			$locals = PQL_LANG_NO;
			if(!$config["PQL_GLOB_CONTROL_AUTOADDLOCALS"][$rootdn]) {
				$locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&rootdn=<?=$rootdn?>&type=add&set=$defaultdomain&submit=1\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $defaultdomain to locals\"></a>";
			} else {
				$locals_link = "&nbsp;";
			}
		}
		
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "rcpthosts", $defaultdomain)){
			$rcpthosts = PQL_LANG_YES;
			$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&rootdn=<?=$rootdn?>&type=del&set=$defaultdomain&submit=1\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $defaultdomain from rcpthosts\"></a>";
		} else {
			$rcpthosts = PQL_LANG_NO;
			$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&rootdn=<?=$rootdn?>&type=add&set=$defaultdomain&submit=1\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $defaultdomain to rcpthosts\"></a>";
		}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_LANG_OPTIONS?></th>
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
	} // end if(PQL_GLOB_CONTROL_USE) ...
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_LANG_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="domain_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><?=PQL_LANG_DOMAIN_DEL?></a></td>
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
