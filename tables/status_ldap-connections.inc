<?php
// Get total number of connections
$connections_total = pql_get_status($_pql->ldap_linkid, "cn=Total,cn=Connections,cn=Monitor", "description");
$connections_average = round($connections_total / $time_uptime, 2);

// Get number of initiated operations
$operations_initiated = pql_get_status($_pql->ldap_linkid, "cn=Initiated,cn=Operations,cn=Monitor", "description");
$operations_initiated_average = round($operations_initiated / $time_uptime, 2);

// Get number of completed operations
$operations_completed = pql_get_status($_pql->ldap_linkid, "cn=Completed,cn=Operations,cn=Monitor", "description");
$operations_completed_average = round($operations_completed / $time_uptime, 2);

// Get number of entries sent to client(s)
$sent_entries = pql_get_status($_pql->ldap_linkid, "cn=Entries,cn=Statistics,cn=Monitor", "description");
$sent_entries_average = round($sent_entries / $time_uptime, 2);

// Get number of bytes sent to client(s)
$sent_bytes  = pql_get_status($_pql->ldap_linkid, "cn=Bytes,cn=Statistics,cn=Monitor", "description");
$sent_kbytes = round($sent_bytes / 1024, 2);
$sent_kbytes_average = round(($sent_bytes / 1024) / $time_uptime, 2);

// Get number of current open connections
$connections_current = pql_get_status($_pql->ldap_linkid, "cn=Current,cn=Connections,cn=Monitor", "description");

// Get number of threads
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Threads,cn=Monitor", "description");
$val = split('=', $tmp[0]); $connections_threads  = "Max ".$val[1];
$val = split('=', $tmp[1]); $connections_threads .= ", Backlog ".$val[1];

// Get number of active databases
$databases = pql_get_subentries($_pql->ldap_linkid, "cn=Databases,cn=Monitor");

// Get current connection informations
$connections_information = pql_get_subentries($_pql->ldap_linkid,
											  "cn=Connections,cn=Monitor",
											  'description', "cn=Connection*");
?>
  <span class="title1"><?=$LANG->_('LDAP Server Connection Status')?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('LDAP Server Resource Summary')?>
      <tr>
        <td class="title2"><?=$LANG->_('Resource')?></td>
        <td class="title3"><?=$LANG->_('Usage Since Startup')?></td>
        <td class="title3"><?=$LANG->_('Average Per Minute')?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Connections')?></td>
        <td class="<?=$class?>"><?=$connections_total?></td>
        <td class="<?=$class?>"><?=$connections_average?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Operations Initiated')?></td>
        <td class="<?=$class?>"><?=$operations_initiated?></td>
        <td class="<?=$class?>"><?=$operations_initiated_average?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Entries Sent To Clients')?></td>
        <td class="<?=$class?>"><?=$sent_entries?></td>
        <td class="<?=$class?>"><?=$sent_entries_average?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Kilobytes Sent To Clients')?></td>
        <td class="<?=$class?>"><?=$sent_kbytes?></td>
        <td class="<?=$class?>"><?=$sent_kbytes_average?></td>
      </tr>
    </th>

    <th colspan="3" align="left"><?=$LANG->_('Current LDAP Server Resource Usage')?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Open Connections')?></td>
        <td class="<?=$class?>"><?=$connections_current?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Active Threads')?></td>
        <td class="<?=$class?>"><?=$connections_threads?></td>
      </tr>

      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$LANG->_('Databases In Use')?></td>
        <td class="<?=$class?>"><?=$databases?></td>
      </tr>
    </th>
  </table>

<?php	if($connections_information[0]) { ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Open Connections')?>
      <tr>
        <td class="title3"><?=$LANG->_('Connection Number')?></td>
        <td class="title3"><?=$LANG->_('Time Opened')?></td>
        <td class="title3"><?=$LANG->_('Received')?></td>
        <td class="title3"><?=$LANG->_('Completed')?></td>
        <td class="title3"><?=$LANG->_('Bound As')?></td>
        <td class="title3"><?=$LANG->_('Connected From')?></td>
        <td class="title3"><?=$LANG->_('State')?></td>
        <td class="title3"><?=$LANG->_('Type')?></td>
      </tr>

<?php		for($i=0; $connections_information[$i]; $i++) {
				$tmp = split(' : ', $connections_information[$i]);

				$class = pql_format_table(0);
				$time_opened = pql_format_timestamp($tmp[10]);
				$time_closed = pql_format_timestamp($tmp[11]);

				$compl = split('/', $tmp[2]);
?>
      <tr>
        <td class="title"><?=$tmp[0]?></td>
        <td class="<?=$class?>"><?=$time_opened?></td>
        <td class="<?=$class?>"><?=$compl[0]?></td>
        <td class="<?=$class?>"><?=$compl[3]?></td>
        <td class="<?=$class?>"><?=$tmp[5]?></td>
        <td class="<?=$class?>"><?=$tmp[7]?></td>
        <td class="<?=$class?>"><?=$tmp[4]?></td>
        <td class="<?=$class?>"><?php
			if(eregi('^ldap:', $tmp[6])) {
				echo "LDAP";
			} elseif(eregi('^ldaps:', $tmp[6])) {
				echo "LDAPS";
			} ?></td>
      </tr>
    </th>
  </table>
<?php		}
	  	}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
