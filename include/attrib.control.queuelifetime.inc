<?php
// control attribute plugin for
// queuelifetime (maximum number of RCPT TO's)
//
// $Id: attrib.control.queuelifetime.inc,v 2.13 2004-03-11 16:31:54 turbo Exp $

// {{{ queuelifetime_check(type)
function queuelifetime_check($type) {
	global $error, $LANG;

	// number ?
	if(preg_match ("/[^0-9]/", $_REQUEST["queuelifetime"])) {
		$error["queuelifetime"] = $LANG->_('Invalid');
		return false;
	}
	
	// must be > 0
	if($_REQUEST["queuelifetime"] != "" and $error["queuelifetime"] == "" and $_REQUEST["queuelifetime"] <= 0) {
		$error["queuelifetime"] = $LANG->_('Must be above zero');
		return false;
	}

	return true;
}
// }}}

// {{{ queuelifetime_init(host)
// init values
function queuelifetime_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_QUEUELIFETIME"));

	if(!is_null($value))
	  $_REQUEST["queuelifetime"] = $value[0];
}
// }}}


// {{{ queuelifetime_print_view(host)
function queuelifetime_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	queuelifetime_init($host);

	// set empty field to "not set"
	if($_REQUEST["queuelifetime"] == "")
	  $_REQUEST["queuelifetime"] = "604800 ".$LANG->_('Seconds', 4). " (".$LANG->_('Default', 4).")";
	else
	  $_REQUEST["queuelifetime"] .= " ".$LANG->_('Seconds', 4);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">queuelifetime (<?=$LANG->_('Max lifetime of a message in queue')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Remain')?></td>
        <td><?=$_REQUEST["queuelifetime"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=queuelifetime"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ queuelifetime_print_form(void)
function queuelifetime_print_form() {
	global $error, $LANG;
?>
  <form action="<?$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">queuelifetime (<?=$LANG->_('Max lifetime of a message in queue')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remain')?></td>
          <td><?php echo pql_format_error_span($error["queuelifetime"]); ?><input type="text" name="queuelifetime" value="<?=$_REQUEST["queuelifetime"]?>"> <?=$LANG->_('Seconds', 4)?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo pql_complete_constant($LANG->_('Empty field to set to the default value (%default% seconds)'), array('default' => '604800')); ?></td>
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

// {{{ queuelifetime_save(type, host)
function queuelifetime_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_QUEUELIFETIME"),
										 $_REQUEST["queuelifetime"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('queuelifetime options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('queuelifetime options')))
			. ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ queuelifetime_help(void)
function queuelifetime_help() {
	global $LANG;
?>
Number of seconds a message can stay in the queue.
Default: 604800 (one week).  After this time expires,
qmail-send will try the message once more, but it will
treat any temporary delivery failures as permanent
failures.
<?php
}
// }}}

// {{{ queuelifetime_help_cr(void)
function queuelifetime_help_cr() {
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
