<?php
// control attribute plugin for
// queuelifetime (maximum number of RCPT TO's)
//
// $Id: attrib.control.queuelifetime.inc,v 2.11 2004-01-01 12:35:34 turbo Exp $

function queuelifetime_check($type) {
	global $error, $queuelifetime, $view, $LANG;

	// number ?
	if(preg_match ("/[^0-9]/", $queuelifetime)) {
		$error["queuelifetime"] = $LANG->_('Invalid');
		return false;
	}
	
	// must be > 0
	if($queuelifetime != "" and $error["queuelifetime"] == "" and $queuelifetime <= 0) {
		$error["queuelifetime"] = $LANG->_('Must be above zero');
		return false;
	}

	return true;
}

function queuelifetime_init($host) {
	// init values
	global $_pql_control, $queuelifetime, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_QUEUELIFETIME"));

	if(!is_null($value))
	  $queuelifetime = $value[0];
}

function queuelifetime_print_view($host) {
	global $_pql_control, $queuelifetime, $view, $LANG;

	// init data
	queuelifetime_init($host);

	// set empty field to "not set"
	if($queuelifetime == "")
	  $queuelifetime = "604800 ".$LANG->_('Seconds', 4). " (".$LANG->_('Default', 4).")";
	else
	  $queuelifetime .= " ".$LANG->_('Seconds', 4);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">queuelifetime (<?=$LANG->_('Max lifetime of a message in queue')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Remain')?></td>
        <td><?=$queuelifetime?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=queuelifetime"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}

function queuelifetime_print_form() {
	global $attrib, $queuelifetime, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">queuelifetime (<?=$LANG->_('Max lifetime of a message in queue')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remain')?></td>
          <td><?php echo pql_format_error_span($error["queuelifetime"]); ?><input type="text" name="queuelifetime" value="<?=$queuelifetime?>"> <?=$LANG->_('Seconds', 4)?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo pql_complete_constant($LANG->_('Empty field to set to the default value (%default% seconds)'), array('default' => '604800')); ?></td>
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

function queuelifetime_save($type, $host) {
	global $_pql_control, $attrib, $queuelifetime, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_QUEUELIFETIME"),
										 $queuelifetime))
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

function queuelifetime_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
