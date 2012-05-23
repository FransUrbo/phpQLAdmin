<?php
// Get the LDAP server version
$version = $_pql->find_ldap_version();

// Calculate the uptime
if($timestamp_current and $timestamp_start) {
	$time_uptime = pql_format_timestamp_human($timestamp_start, $timestamp_current);
} else {
    $time_uptime = "n/a";
}

// {{{ Basic LDAP server status
?>

  <span class="title1"><?php echo $LANG->_('Basic LDAP Server Status')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('LDAP Server Status')?>
      <tr>
        <td class="title"><?php echo $LANG->_('LDAP Server Version')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $version?></td>
      </tr>

      <tr>
        <td class="title"><?php echo $LANG->_('LDAP Server Startup Time')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $time_start?></td>
      </tr>

      <tr>
        <td class="title"><?php echo $LANG->_('Current Time on Server')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $time_current?></td>
      </tr>

      <tr>
        <td class="title"><?php echo $LANG->_('LDAP Server Uptime')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $time_uptime?></td>
      </tr>
    </th>
  </table>

<?php
// }}}

// {{{ Get LDAP listeners this server have opened
$listeners = pql_get_subentries($_pql->ldap_linkid, "cn=Listeners,".$_pql->ldap_monitor, "labeledURI", "cn=Listener*");
if(is_array($listeners)) {
?>
  <br>

  <span class="title1"><?php echo $LANG->_('LDAP Server listeners opened')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
<?php foreach($listeners as $nr => $url) {
		$url = urldecode($url);
?>
      <tr>
        <td class="title"><?php echo $LANG->_('Listener #').$nr?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $url?></td>
      </tr>
<?php } ?>
    </th> 
  </table>
<?php
}
?>

<?php
// }}}

// {{{ Get LDAP overlays this server have opened/started.
// NOTE: Is it 'cn=Overlay' or 'cn=Overlays'?
//       In 2.4, it's the later...
if($_pql->get_dn("cn=Overlay,".$_pql->ldap_monitor, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*')) {
  // OpenLDAP <2.4
  $overlays = pql_get_subentries($_pql->ldap_linkid, "cn=Overlay,".$_pql->ldap_monitor, "monitoredInfo", "cn=Overlay*");
} elseif($_pql->get_dn("cn=Overlays,".$_pql->ldap_monitor, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*')) {
  // OpenLDAP >2.4
  $overlays = pql_get_subentries($_pql->ldap_linkid, "cn=Overlays,".$_pql->ldap_monitor, "monitoredInfo", "cn=Overlay*");
} else {
  echo "Can't find out where to look for overlays... Tried both 'cn=Overlay,".$_pql->ldap_monitor."' and cn=Overlays,".$_pql->ldap_monitor;
  echo "<br>but neither worked...<br>";
}

if(is_array($overlays)) {
?>
  <br>

  <span class="title1"><?php echo $LANG->_('LDAP Server overlays')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
<?php foreach($overlays as $nr => $overlay) { ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Overlay #').$nr?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $overlay?></td>
      </tr>
<?php } ?>
    </th> 
  </table>
<?php
}
// }}}

// {{{ Misc LDAP server configuration
$max_file_descriptors = pql_get_subentries($_pql->ldap_linkid, "cn=Connections,".$_pql->ldap_monitor,
										   "monitorCounter", "cn=Max File Descriptors");
if(is_array($max_file_descriptors)) {
  $max_file_descriptors = $max_file_descriptors[0];
?>
  <br>

  <span class="title1"><?php echo $LANG->_('LDAP Server Configuration Setup')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr>
        <td class="title"><?php echo $LANG->_('Max File Descriptors')?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?php echo $class?>"><?php echo $max_file_descriptors?></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
