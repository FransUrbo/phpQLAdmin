<?php
// control attribute plugin for
// qmqpservers (max number of bytes in message)
//
// $Id: attrib.control.qmqpservers.inc,v 2.16 2004-02-14 14:25:36 turbo Exp $

function qmqpservers_check($type) {
	global $error, $qmqpservers, $view, $LANG;

	// check each field
	if(is_array($qmqpservers)) {
		foreach($qmqpservers as $key => $server) {
			if($server == "")
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["qmqpservers_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if($ip_part <= 0 or $ip_part > 254) {
							$is_error = true;
							$error["qmqpservers_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if($ip_part < 0 or $ip_part > 255) {
							$is_error = true;
							$error["qmqpservers_$key"] = $LANG->_('Invalid');
						}
					} // end switch
				} // end foreach
			} // end if !preg_match...
		} // end foreach
	} // end if is_array...

	if($is_error)
	  return false;
	
	return true;
}

// init values
function qmqpservers_init($host) {
	global $_pql_control, $qmqpservers, $view;

	// fetch data from ldap server
	$qmqpservers = pql_control_get_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_GLOB_ATTR_QMQPSERVERS"));

	if(is_null($qmqpservers))
	  return true;
}

function qmqpservers_print_view($host) {
	global $_pql_control, $qmqpservers, $view, $LANG;

	// init data
	qmqpservers_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">qmqpservers (<?=$LANG->_('QMQP servers')?>)</th>
      <tr class="title">
        <td>IP</td>
      </tr>
<?php
	if(!is_array($qmqpservers)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($qmqpservers as $server) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($server)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=qmqpservers"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}

function qmqpservers_print_form() {
	global $attrib, $error, $qmqpservers, $qmqpcount, $mxhost, $view, $LANG;

	if(($qmqpcount < count($qmqpservers) - 1) and count($qmqpservers) != 0)
	  $qmqpcount = count($qmqpservers) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">qmqpservers (<?=$LANG->_('QMQP servers')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('IP Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["qmqpservers_0"]); ?><input type="text" name="qmqpservers[0]" value="<?=$qmqpservers[0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $qmqpcount; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["qmqpservers_$i"]); ?><input type="text" name="qmqpservers[<?=$i?>]" value="<?=$qmqpservers[$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?$attrib?>&qmqpcount=<?=($qmqpcount+1)?>&mxhost=<?=$mxhost?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a QMQP server remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function qmqpservers_save($type, $host)  {
	global $_pql_control, $attrib, $qmqpservers, $error, $mxhost, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_QMQPSERVERS"),
										 $qmqpservers))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('qmqpservers options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('qmqpservers options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function qmqpservers_help() {
	global $LANG;
?>
	IP addresses of QMQP servers, one address per line.
	qmail-qmqpc will try each address in turn until it
	establishes a QMQP connection or runs out of addresses.
<?php
}

function qmqpservers_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
