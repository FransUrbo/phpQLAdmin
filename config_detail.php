<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");
require("./include/pql_config.inc");
include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

if(PQL_SHOW_USERS){
    $show_users = "yes";
} else {
    $show_users = "no";
}

if(PQL_AUTO_RELOAD){
    $auto_reload = "yes";
} else {
    $auto_reload = "no";
}

if(PQL_LDAP_CONTROL_USE){
    $control_use = "yes";
} else {
    $control_use = "no";
}

if(PQL_LDAP_CONTROL_AUTOADDLOCALS){
    $control_autoaddlocals = "yes";
} else {
    $control_autoaddlocals = "no";
}

if(PQL_ALLOW_ABSOLUTE_PATH){
    $allow_absolute_path = "yes";
} else {
    $allow_absolute_path = "no";
}

if(PQL_LDAP_CHANGE_SERVER){
    $allow_change_server = "yes";
} else {
    $allow_change_server = "no";
}

if(PQL_LDAP_EZMLM_USE){
    $ezmlm_use = "yes";
} else {
    $ezmlm_use = "no";
}

if(PQL_VERIFY_DELETE){
    $verify_delete = "yes";
} else {
    $verify_delete = "no";
}

?>
  <span class="title1">phpQLAdmin configuration</span>
  <br><br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Configuration</th>
<tr>
	<td class="title">LDAP host</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$USER_HOST?>&nbsp;</td>
        <td class="<?=$class?>"></td>
</tr>

<?php
$new_tr = 0;
foreach($_pql->ldap_basedn as $dn) {
    if($new_tr) {
?>
<tr>
	<td class="title"></td>
<?php
    } else {
?>
<tr>
	<td class="title">LDAP base dn</td>
<?php
    }
    $new_tr = 1;
?>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$dn?>&nbsp;</td>
        <td class="<?=$class?>"></td>
<?php
		}
?>
</tr>

<?php
	if(PQL_LDAP_CONTROL_USE) {
?>
<tr>
	<td class="title">LDAP control base dn</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$USER_SEARCH_DN_CTR?>&nbsp;</td>
        <td class="<?=$class?>"></td>
</tr>
<?php
	}
	if(defined(PQL_DEFAULT_PATH)) {
?>
<tr>
	<td class="title">Default mailbox path</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo PQL_DEFAULT_PATH; ?>&nbsp;</td>
        <td class="<?=$class?>"></td>
</tr>
<?php
	}
?>

<tr></tr>

<tr>
        <td class="title">Language</td>
	<?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"><?php echo PQL_LANG; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_LANG"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Hostmaster</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo PQL_HOSTMASTER; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_HOSTMASTER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr></tr>

<tr>
	<td class="title">Manage Controls DB</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo $control_use; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_LDAP_CONTROL_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Manage EZMLM mailinglists</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$ezmlm_use?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_LDAP_EZMLM_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
        <td class="title">Show users (navigation bar)</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo $show_users; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_SHOW_USERS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Automatic reload of navigation bar</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo $auto_reload; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_AUTO_RELOAD"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Automatically replicate domains to locals</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo $control_autoaddlocals; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_LDAP_CONTROL_AUTOADDLOCALS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Allow absolute mailbox paths</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo $allow_absolute_path; ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_ALLOW_ABSOLUTE_PATH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Allow change of LDAP server</td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$allow_change_server?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_LDAP_CHANGE_SERVER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Verify user/domain deletions etc <b>[recomended!]</b></td>
	<?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$verify_delete?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_VERIFY_DELETE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr></tr>

<?php
	if($ADVANCED_MODE) {
?>

<tr></tr>

<?php
		$class=table_bgcolor(0);
		$new_tr = 0;
		$schemes = split(",", PQL_PASSWORD_SCHEMES);
		foreach($schemes as $s) {
		    if($new_tr) {
?>
<tr>
	<td class="title"></td>
<?php
		    } else {
?>
<tr>
	<td class="title">Password encryption schemes</td>
<?php
		    }
		    $new_tr = 1;
?>
	<td class="<?=$class?>"><?=$s?>&nbsp;</td>
        <td class="<?=$class?>"></td>
<?php
		}
?>
</tr>

<tr></tr>

<tr>
	<td class="title">User objectclasses</td>
        <?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?php echo PQL_LDAP_OBJECTCLASS_USERID; ?>&nbsp;</td>
        <td class="<?=$class?>"></td>
</tr>
<?php
		$new_tr = 0;
		$objectclasses = split(" ", PQL_LDAP_OBJECTCLASS_USER_EXTRA);
		foreach($objectclasses as $oc) {
?>
<tr>
	<td class="title"></td>
	<td class="<?=$class?>"><?=$oc?>&nbsp;</td>
        <td class="<?=$class?>"></td>
</tr>
<?php
		}
?>

<tr></tr>

<?php
		$class=table_bgcolor(0);
		$objectclasses = '';
		if(eregi(" ", PQL_LDAP_OBJECTCLASS_DOMAIN)) {
		    $objectclasses = split(" ", PQL_LDAP_OBJECTCLASS_DOMAIN);
		} else {
		    $objectclasses[] = PQL_LDAP_OBJECTCLASS_DOMAIN;
		}

		foreach($objectclasses as $oc) {
		    if($new_tr) {
?>
<tr>
	<td class="title"></td>
<?php
		    } else {
?>
<tr>
	<td class="title">Domain objectclasses</td>
<?php
		    }
		    $new_tr = 1;
?>
	<td class="<?=$class?>"><?=$oc?>&nbsp;</td>
        <td class="<?=$class?>"></td>
<?php
		}
?>
</tr>

<tr></tr>

<tr>
	<td class="title">Reference users with</td>
        <?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=PQL_LDAP_REFERENCE_USERS_WITH?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_LDAP_REFERENCE_USERS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>

<tr>
	<td class="title">Reference domains with</td>
        <?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=PQL_LDAP_ATTR_DOMAIN?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_LDAP_ATTR_DOMAIN"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle"></a></td>
</tr>
<?php
	}
?>
	
<tr class="subtitle">
	<td colspan="2"><img src="images/info.png" width="16" height="16" border="0">the phpQLAdmin configuration values are stored in config.inc&nbsp;</td>
        <td></td>
</tr>
</table>

</body>
</html>
