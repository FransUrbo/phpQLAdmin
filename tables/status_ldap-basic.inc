<?php
// Get the LDAP server version
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Monitor", array("description", "monitoredInfo"));
$tmp = split('\$', $tmp['monitoredinfo']);
$tmp = split(' \(', $tmp[1]);
$version = $tmp[0];

// Calculate the uptime
if($timestamp_current and $timestamp_start) {
	$time_uptime = pql_format_timestamp_human($timestamp_start, $timestamp_current);
} else {
    $time_uptime = "n/a";
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
        <td class="<?=$class?>"><?=$time_uptime?></td>
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
