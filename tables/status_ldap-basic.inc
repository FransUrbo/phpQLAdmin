<?php
// Get the LDAP server version etc
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Monitor", "description");
$tmp = split('\$', $tmp);
$version = $tmp[1];

// Get the LDAP server bootup time
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Start,cn=Time,cn=Monitor", "createTimeStamp");
$timestamp_start = preg_replace('/Z$/', '', $tmp);
$time_start = pql_format_timestamp($tmp);

// Get the current LDAP server time
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Current,cn=Time,cn=Monitor", "modifyTimeStamp");
if($tmp) {
	$timestamp_current = preg_replace('/Z$/', '', $tmp);
	$time_current = pql_format_timestamp($tmp);
} else {
	$timestamp_current = $time_current = "n/a";
}

// Calculate the uptime
if($timestamp_current and $timestamp_start) {
	$time_uptime  = round(($timestamp_current - $timestamp_start) / 60, 2);
} else {
	$time_uptime  = "n/a";
}
?>

  <span class="title1"><?=$LANG->_('Basic LDAP Server Status')?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('LDAP Server Status')?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP Server Version')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$version?></td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('LDAP Server Startup Time')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$time_start?></td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Current Time on Server')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$time_current?></td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('LDAP Server Uptime')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$time_uptime?> <?=$LANG->_('minutes')?></td>
      </tr>
    </th>
  </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
