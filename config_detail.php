<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");
include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

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
	<td class="<?php table_bgcolor(); ?>"><?=$USER_HOST?>&nbsp;</td>
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
	<td class="<?php table_bgcolor(); ?>"><?=$dn?>&nbsp;</td>
<?php
		}
?>
</tr>

<?php
	if(PQL_LDAP_CONTROL_USE) {
?>
<tr>
	<td class="title">LDAP control base dn</td>
	<td class="<?php table_bgcolor(); ?>"><?=$USER_SEARCH_DN_CTR?>&nbsp;</td>
</tr>
<?php
	}
	if(defined(PQL_DEFAULT_PATH)) {
?>
<tr>
	<td class="title">Default mailbox path</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_DEFAULT_PATH; ?>&nbsp;</td>
</tr>
<?php
	}
?>

<tr></tr>

<tr>
        <td class="title">Language</td>
        <td class="<?php table_bgcolor(); ?>"><?php echo PQL_LANG; ?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Hostmaster</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_HOSTMASTER; ?>&nbsp;</td>
</tr>

<tr></tr>

<tr>
	<td class="title">Manage Controls DB</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $control_use; ?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Manage EZMLM mailinglists</td>
	<td class="<?php table_bgcolor(); ?>"><?=$ezmlm_use?>&nbsp;</td>
</tr>

<tr>
        <td class="title">Show users (navigation bar)</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $show_users; ?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Automatic reload of navigation bar</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $auto_reload; ?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Automatically replicate domains to locals</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $control_autoaddlocals; ?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Allow absolute mailbox paths</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $allow_absolute_path; ?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Allow change of LDAP server</td>
	<td class="<?php table_bgcolor(); ?>"><?=$allow_change_server?>&nbsp;</td>
</tr>

<tr>
	<td class="title">Verify user/domain deletions etc <b>[recomended!]</b></td>
	<td class="<?php table_bgcolor(); ?>"><?=$verify_delete?>&nbsp;</td>
</tr>

<tr></tr>

<?php
	if($ADVANCED_MODE) {
?>

<tr></tr>

<?php
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
	<td class="<?php table_bgcolor(); ?>"><?=$s?>&nbsp;</td>
<?php
		}
?>
</tr>

<tr></tr>

<tr>
	<td class="title">User objectclasses</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_LDAP_OBJECTCLASS_USERID; ?>&nbsp;</td>
</tr>
<?php
		$new_tr = 0;
		$objectclasses = split(" ", PQL_LDAP_OBJECTCLASS_USER_EXTRA);
		foreach($objectclasses as $oc) {
?>
<tr>
	<td class="title"></td>
	<td class="<?php table_bgcolor(); ?>"><?=$oc?>&nbsp;</td>
</tr>
<?php
		}
?>

<tr></tr>

<?php
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
	<td class="<?php table_bgcolor(); ?>"><?=$oc?>&nbsp;</td>
<?php
		}
?>
</tr>

<tr></tr>

<tr>
	<td class="title">Reference users with</td>
	<td class="<?php table_bgcolor(); ?>"><?=PQL_LDAP_REFERENCE_USERS_WITH?></td>
</tr>

<tr>
	<td class="title">Reference domains with</td>
	<td class="<?php table_bgcolor(); ?>"><?=PQL_LDAP_ATTR_DOMAIN?></td>
</tr>
<?php
	}
?>
	
<tr class="subtitle">
	<td colspan="2"><img src="images/info.png" width="16" height="16" border="0">the phpQLAdmin configuration values are stored in config.inc&nbsp;</td>
</tr>
</table>

</body>
</html>
