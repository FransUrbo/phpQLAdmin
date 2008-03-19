<?php
// control attribute plugin for
// queuelifetime (maximum number of RCPT TO's)
//
// $Id: attrib.control.queuelifetime.inc,v 2.23 2008-03-19 13:21:51 turbo Exp $

// {{{ queuelifetime_check(type)
function queuelifetime_check($type) {
	global $error, $LANG;

	// number ?
	if(preg_match ("/[^0-9]/", $_REQUEST["queuelifetime"])) {
		$error["queuelifetime"] = $LANG->_('Invalid');
		return false;
	}
	
	// must be > 0
	if(!@empty($_REQUEST["queuelifetime"]) and @($error["queuelifetime"] == "") and @($_REQUEST["queuelifetime"] <= 0)) {
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
	if($host == "Global")
	  $_REQUEST["queuelifetime"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													 pql_get_define("PQL_ATTR_QUEUELIFETIME"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["queuelifetime"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_QUEUELIFETIME"));

	if(@is_null($_REQUEST["queuelifetime"]))
	  return true;
}
// }}}


// {{{ queuelifetime_print_view(host)
function queuelifetime_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	queuelifetime_init($host);

	if(!@$_REQUEST["queuelifetime"])
	  $queuelifetime = pql_format_timestamp_human(604800);
	else
	  $queuelifetime = pql_format_timestamp_human($_REQUEST["queuelifetime"]);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">queuelifetime (<?=$LANG->_('Max lifetime of a message in queue')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Remain')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["queuelifetime"]) { echo $_REQUEST["queuelifetime"]; } else { echo $LANG->_('Default', 4).": $queuelifetime"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_QUEUELIFETIME")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_QUEUELIFETIME")?>"></a></td>
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
    <input type="hidden" name="type"   value="modify">
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
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_QUEUELIFETIME"), '', $_REQUEST["queuelifetime"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_QUEUELIFETIME"), 1, $_REQUEST["queuelifetime"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('queuelifetime options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('queuelifetime options')))
			. ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
		// }}}

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
