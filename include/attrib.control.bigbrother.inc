<?php
// control attribute plugin for
// bigbrother (max number of bytes in message)
//
// $Id: attrib.control.bigbrother.inc,v 2.2 2004-07-08 11:37:53 turbo Exp $

// {{{ bigbrother_check(type)
function bigbrother_check($type) {
	global $error, $LANG;

	// check each field
	if(is_array($_REQUEST["bigbrother"])) {
		foreach($_REQUEST["bigbrother"] as $key => $server) {
			if($server == "")
			  continue;
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)) {
				$is_error = true;
				$error["bigbrother_$key"] = $LANG->_('Invalid');
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part) {
					switch((int)$ip_key) {
					  case 0:
					  case 3:
						if($ip_part <= 0 or $ip_part > 254) {
							$is_error = true;
							$error["bigbrother_$key"] = $LANG->_('Invalid');
						}
						break;
						
					  default:
						if($ip_part < 0 or $ip_part > 255) {
							$is_error = true;
							$error["bigbrother_$key"] = $LANG->_('Invalid');
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
// }}}

// {{{ bigbrother_init(host)
// init values
function bigbrother_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["bigbrother"] = pql_control_get_attribute($_pql_control->ldap_linkid,
														 pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
														 pql_get_define("PQL_ATTR_QMQPSERVERS"));

	if(is_null($_REQUEST["bigbrother"]))
	  return true;
}
// }}}


// {{{ bigbrother_print_view(host)
function bigbrother_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	bigbrother_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">bigbrother (<?=$LANG->_('_CHANGE THIS_')?>)</th>
      <tr class="title">
        <td>IP</td>
      </tr>
<?php
	if(!is_array($_REQUEST["bigbrother"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["bigbrother"] as $server) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($server)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=bigbrother"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ bigbrother_print_form(void)
function bigbrother_print_form() {
	global $error, $LANG;

	if(($_REQUEST["qmqpcount"] < count($_REQUEST["bigbrother"]) - 1) and count($_REQUEST["qmqpservers"]) != 0)
	  $_REQUEST["qmqpcount"] = count($_REQUEST["bigbrother"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">bigbrother (<?=$LANG->_('_CHANGE THIS_')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('IP Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["bigbrother_0"]); ?><input type="text" name="qmqpservers[0]" value="<?=$_REQUEST["qmqpservers"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["qmqpcount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["bigbrother_$i"]); ?><input type="text" name="qmqpservers[<?=$i?>]" value="<?=$_REQUEST["qmqpservers"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&qmqpcount=<?=($_REQUEST["qmqpcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a QMQP server remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ bigbrother_save(type, host)
function bigbrother_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_QMQPSERVERS"),
										 $_REQUEST["bigbrother"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bigbrother options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bigbrother options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ bigbrother_help(void)
function bigbrother_help() {
	global $LANG;
?>
	IP addresses of QMQP servers, one address per line.
	qmail-qmqpc will try each address in turn until it
	establishes a QMQP connection or runs out of addresses.
<?php
}
// }}}

// {{{ bigbrother_help_cr(void)
function bigbrother_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
