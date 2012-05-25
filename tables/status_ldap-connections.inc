<?php
// {{{ Get total number of connections
$connections_total = pql_get_status($_pql->ldap_linkid, "cn=Total,cn=Connections,".$_pql->ldap_monitor, "monitorCounter");
if($connections_total) {
	$connections_average = round($connections_total / (($timestamp_current - $timestamp_start) / 3600), 2);
} else {
	$connections_total   = '<i>unknown</i>';
	$connections_average = '<i>unknown</i>';
}
// }}}

// {{{ Get all operations
$operations = array('Bind', 'Unbind', 'Add', 'Delete', 'Modrdn', 'Modify', 'Compare', 'Search', 'Abandon', 'Extended');
foreach($operations as $operation) {
  $key = lc($operation);
  $$key = pql_get_status($_pql->ldap_linkid, "cn=$operation,cn=Operations,".$_pql->ldap_monitor, array("monitorOpInitiated", "monitorOpCompleted"));
  if(!$$key)
	$$key = array("monitoropinitiated" => 0, "monitoropcompleted" => 0);
}
// }}}

// {{{ Get TOTAL number of initiated and completed operations
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Operations,".$_pql->ldap_monitor, array("monitorOpInitiated", "monitorOpCompleted"));
if($tmp) {
	// Initiated operations
	$operations_initiated         = $tmp['monitoropinitiated'];
	$operations_initiated_average = round($tmp['monitoropinitiated'] / (($timestamp_current - $timestamp_start) / 3600), 2);

	// Completed operations
	$operations_completed         = $tmp['monitoropcompleted'];
	$operations_completed_average = round($operations_completed / (($timestamp_current - $timestamp_start) / 3600), 2);
} else {
	$operations_initiated         = '<i>unknown</i>';
	$operations_initiated_average = '<i>unknown</i>';

	$operations_completed         = '<i>unknown</i>';
	$operations_completed_average = '<i>unknown</i>';
}
// }}}

// {{{ Get number of entries sent to client(s)
$sent_entries = pql_get_status($_pql->ldap_linkid, "cn=Entries,cn=Statistics,".$_pql->ldap_monitor, "monitorCounter");
if($sent_entries) {
	$sent_entries_average         = round($sent_entries / (($timestamp_current - $timestamp_start) / 3600), 2);
} else {
	$sent_entries                 = '<i>unknown</i>';
	$sent_entries_average         = '<i>unknown</i>';
}
// }}}

// {{{ Get number of bytes sent to client(s)
$sent_bytes  = pql_get_status($_pql->ldap_linkid, "cn=Bytes,cn=Statistics,".$_pql->ldap_monitor, "monitorCounter");
if($sent_bytes) {
	$sent_kbytes = round($sent_bytes / 1024, 2);
	$sent_kbytes_average = round(($sent_bytes / 1024) / (($timestamp_current - $timestamp_start) / 3600), 2);
} else {
	$sent_bytes                   = '<i>unknown</i>';
	$sent_kbytes                  = '<i>unknown</i>';
	$sent_kbytes_average          = '<i>unknown</i>';
}
// }}}

// {{{ Get number of current open connections
$connections_current = pql_get_status($_pql->ldap_linkid, "cn=Current,cn=Connections,".$_pql->ldap_monitor, "monitorCounter");
if(!$connections_current) {
	$connections_current          = '<i>unknown</i>';
}
// }}}

// {{{ Get number of threads
$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
for($i=0; $i < count($tmp1); $i++) {
	$tmp2 = split('=', $tmp1[$i]);

	if($tmp2[0] == 'max') {
		$threads['max'] = $tmp2[1];
	} elseif($tmp2[0] == 'backload') {
		$threads['backload'] = $tmp2[1];
	}
}

if(is_array($threads)) {
	$connections_threads  = "Max ".$threads['max'];
	$connections_threads .= ", Backlog ".$threads['backload'];
	unset($threads);
} else {
	// OpenLDAP 2.4(3?) is different...
	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Max,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['max'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Max Pending,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['max_pending'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Open,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['open'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Starting,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['starting'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Active,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['active'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Pending,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['pending'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Backload,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['backload'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=State,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['state'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Runqueue,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['runqueue'] = $tmp1;

	$tmp1 = pql_get_status($_pql->ldap_linkid, "cn=Tasklist,cn=Threads,".$_pql->ldap_monitor, "monitoredInfo");
	if(isset($tmp1))	  $threads['tasklist'] = $tmp1;

	if(!is_array($threads))
	  $connections_threads  = '<i>unknown</i>';
}
// }}}

// {{{ Get number of active databases
$databases = pql_get_subentries($_pql->ldap_linkid, "cn=Databases,".$_pql->ldap_monitor);
// }}}

// {{{ Get current connection informations
$connections_information = pql_get_subentries($_pql->ldap_linkid,
											  "cn=Connections,".$_pql->ldap_monitor,
											  'monitoredInfo', "cn=Connection*");
// }}}
?>
  <span class="title1"><?php echo $LANG->_('LDAP Server Connection Status')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr>
        <td class="title2"><?php echo $LANG->_('Resource')?></td>
        <td class="title3"><?php echo $LANG->_('Since Startup')?></td>
        <td class="title3"><?php echo $LANG->_('Average Per Minute')?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Total Connections')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $connections_total?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $connections_average?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Entries Sent To Clients')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $sent_entries?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $sent_entries_average?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Kilobytes Sent To Clients')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $sent_kbytes?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $sent_kbytes_average?></td>
      </tr>
    </th>
  </table>

  <br><br>

  <span class="title1"><?php echo $LANG->_('LDAP Server operations')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr>
        <td class="title2"><?php echo $LANG->_('Resource')?></td>
        <td class="title3"><?php echo $LANG->_('Initiated')?></td>
        <td class="title3"><?php echo $LANG->_('Completed')?></td>
        <td class="title3"><?php echo $LANG->_('Average Per Minute')?></td>
      </tr>

<?php foreach($operations as $operation) {
		$key = lc($operation);
		$value = $$key;

		$average = round($value['monitoropcompleted'] / (($timestamp_current - $timestamp_start) / 3600), 2);
?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $operation?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $value["monitoropinitiated"]?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $value["monitoropcompleted"]?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $average?></td>
      </tr>

<?php } ?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Total Operations')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $operations_initiated?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $operations_completed?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $operations_completed_average?></td>
      </tr>
    </th>
  </table>

  <br><br>

  <span class="title1"><?php echo $LANG->_('Current LDAP Server Resource Usage')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Open Connections')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $connections_current?></td>
      </tr>
<?php if($connections_threads) { ?>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Active Threads')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $connections_threads?></td>
      </tr>
<?php } ?>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Databases In Use')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $databases?></td>
      </tr>
    </th>
  </table>

<?php if(is_array($threads)) { ?>
  <br><br>

  <span class="title1"><?php echo $LANG->_('Current LDAP Server Thread Resources')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Maximum number of threads as configured')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['max']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Maximum number of pending threads')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['max_pending']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Number of open threads')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['open']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Number of threads being started')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['starting']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Number of active threads')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['active']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Number of pending threads')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['pending']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Number of active plus pending threads')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['backload']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Thread pool state')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['state']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('Queue of running threads - besides\nthose handling operations')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['runqueue']?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $LANG->_('List of running plus standby threads\n- besides those handling operations')?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $threads['tasklist']?></td>
      </tr>
    </th>
  </table>
<?php } ?>

<?php	if($connections_information[0]) { ?>
  <br><br>

  <span class="title1"><?php echo $LANG->_('Open Connections')?></span>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
      <tr>
        <td class="title2"><?php echo $LANG->_('Connection')?></td>
        <td class="title3"><?php echo $LANG->_('Time Opened')?></td>
        <td class="title3"><?php echo $LANG->_('Received')?></td>
        <td class="title3"><?php echo $LANG->_('Completed')?></td>
        <td class="title3"><?php echo $LANG->_('Bound As')?></td>
        <td class="title3"><?php echo $LANG->_('Connected From')?></td>
        <td class="title3"><?php echo $LANG->_('State')?></td>
        <td class="title3"><?php echo $LANG->_('Type')?></td>
      </tr>

<?php		for($i=0; $i < count($connections_information); $i++) {
				$tmp = split(' : ', $connections_information[$i]);

				$class = pql_format_table(0);
				$time_opened = pql_format_timestamp($tmp[10]);
				$time_closed = pql_format_timestamp($tmp[11]);

				$compl = split('/', $tmp[2]);
?>
      <tr>
        <td class="title" align="right"><?php echo $tmp[0]?>&nbsp;&nbsp;</td>
        <td class="<?php echo $class?>" align="right"><?php echo $time_opened?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $compl[0]?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $compl[3]?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $tmp[5]?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $tmp[7]?></td>
        <td class="<?php echo $class?>" align="right"><?php echo $tmp[4]?></td>
        <td class="<?php echo $class?>" align="right"><?php
			if(preg_match('/^ldap:/i', $tmp[6])) {
				echo "LDAP";
			} elseif(preg_match('/^ldaps:/i', $tmp[6])) {
				echo "LDAPS";
			} elseif(preg_match('/^ldapi:/i', $tmp[6])) {
				echo "LDAPI";
			} ?></td>
      </tr>
<?php		} ?>
    </th>
  </table>
<?php  	}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
