<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");
include("header.html");
?>
  <span class="title1">phpQLAdmin configuration</span>
  <br><br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Configuration</th>
      <tr>
        <td class="title">Language</td>
        <td class="<?php table_bgcolor(); ?>"><?php echo PQL_LANG; ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Show users (navigation bar)</td>
	<?php
		if(PQL_SHOW_USERS){
			$show_users = "yes";
   	} else {
			$show_users = "no";
   	}
	?>
	<td class="<?php table_bgcolor(); ?>"><?php echo $show_users; ?>&nbsp;</td>
</tr>
<tr>
	<?php
		if(PQL_AUTO_RELOAD){
			$auto_reload = "yes";
   	} else {
			$auto_reload = "no";
   	}
	?>
	<td class="title">Automatic reload of navigation bar</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $auto_reload; ?>&nbsp;</td>
</tr>
<tr>
	<td class="title">Hostmaster</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_HOSTMASTER; ?>&nbsp;</td>
</tr>
<tr>
	<td class="title">LDAP host</td>
	<td class="<?php table_bgcolor(); ?>"><?=$USER_HOST_USR?>&nbsp;</td>
</tr>
<tr>
	<td class="title">LDAP base dn</td>
	<td class="<?php table_bgcolor(); ?>"><?=$USER_SEARCH_DN_USR?>&nbsp;</td>
</tr>
<tr>
	<td class="title">use control db</td>
	<?php
		if(PQL_LDAP_CONTROL_USE){
			$control_use = "yes";
   	} else {
			$control_use = "no";
   	}
	?>
	<td class="<?php table_bgcolor(); ?>"><?php echo $control_use; ?>&nbsp;</td>
</tr>
<?php if(PQL_LDAP_CONTROL_USE) { ?>
<tr>
	<td class="title">LDAP control host</td>
	<td class="<?php table_bgcolor(); ?>"><?=$USER_HOST_CTR?>&nbsp;</td>
</tr>
<tr>
	<td class="title">LDAP control base dn</td>
	<td class="<?php table_bgcolor(); ?>"><?=$USER_SEARCH_DN_CTR?>&nbsp;</td>
</tr>
<tr>
	<td class="title">LDAP control root dn</td>
	<td class="<?php table_bgcolor(); ?>"><?=$USER_DN?>&nbsp;</td>
</tr>
<tr>
	<td class="title">value in 'me'</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_LDAP_CONTROL_ME; ?>&nbsp;</td>
</tr>
	<?php
		if(PQL_LDAP_CONTROL_AUTOADDLOCALS){
			$control_autoaddlocals = "yes";
   	} else {
			$control_autoaddlocals = "no";
   	}
	?>
<tr>
	<td class="title">Automatically replicate domains to locals</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $control_autoaddlocals; ?>&nbsp;</td>
</tr>
<?php } ?>
<tr>
	<td class="title">Password encryption schemes</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_PASSWORD_SCHEMES; ?>&nbsp;</td>
</tr>
<tr>
	<td class="title">allow absolute mailbox paths</td>
	<?php
		if(PQL_ALLOW_ABSOLUTE_PATH){
			$allow_absolute_path = "yes";
   	} else {
			$allow_absolute_path = "no";
   	}
	?>
	<td class="<?php table_bgcolor(); ?>"><?php echo $allow_absolute_path; ?>&nbsp;</td>
</tr>
<?php
	if(defined(PQL_DEFAULT_PATH)) {
?>
<tr>
	<td class="title">Default mailbox path</td>
	<td class="<?php table_bgcolor(); ?>"><?php echo PQL_DEFAULT_PATH; ?>&nbsp;</td>
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
