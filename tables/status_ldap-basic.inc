<?php
// Get the LDAP server version
$version = $_pql->find_ldap_version();

// Calculate the uptime
if($timestamp_current and $timestamp_start) {
	$time_uptime = pql_format_timestamp_human($timestamp_start, $timestamp_current);
} else {
    $time_uptime = "n/a";
}
?>

  <span class="title1"><?=$LANG->_('Basic LDAP Server Status')?></span>
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
// Get LDAP listeners this server have opened
$listeners = pql_get_subentries($_pql->ldap_linkid, "cn=Listeners,".$_pql->ldap_monitor, "labeledURI", "cn=Listener*");
if(is_array($listeners)) {
?>
  <br>

  <span class="title1"><?=$LANG->_('LDAP Server listeners opened')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
<?php foreach($listeners as $nr => $url) {
		$url = urldecode($url);
?>
      <tr>
        <td class="title"><?=$LANG->_('Listener #').$nr?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$url?></td>
      </tr>
<?php } ?>
    </th> 
  </table>
<?php
}
?>

<?php
// Get LDAP overlays this server have opened/started.
$overlays = pql_get_subentries($_pql->ldap_linkid, "cn=Overlay,".$_pql->ldap_monitor, "monitoredInfo", "cn=Overlay*");
if(is_array($overlays)) {
?>
  <br>

  <span class="title1"><?=$LANG->_('LDAP Server overlays')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
<?php foreach($overlays as $nr => $overlay) { ?>
      <tr>
        <td class="title"><?=$LANG->_('Overlay #').$nr?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$overlay?></td>
      </tr>
<?php } ?>
    </th> 
  </table>
<?php
}
?>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
