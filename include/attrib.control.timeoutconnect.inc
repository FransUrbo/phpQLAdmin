<?php
// control attribute plugin for
// timeoutconnect (how long to wait for smtp connection)
// timeoutremote (how long to wait for remote server)
//
// $Id: attrib.control.timeoutconnect.inc,v 2.19.2.1 2006-03-23 09:13:15 turbo Exp $

// {{{ timeoutconnect_check(type)
function timeoutconnect_check($type) {
	global $error, $LANG;

	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["timeoutconnect"])) {
		$error["timeoutconnect"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// must be > 0
	if(@empty($error["timeoutconnect"]) and !@empty($_REQUEST["timeoutconnect"]) and ($_REQUEST["timeoutconnect"] <= 0)) {
		$error["timeoutconnect"] = $LANG->_('Must be above zero');
		$is_error = true;
	}

	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["timeoutremote"])) {
		$error["timeoutremote"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// must be > 0
	if(@empty($error["timeoutremote"]) and !@empty($_REQUEST["timeoutremote"]) and ($_REQUEST["timeoutremote"] <= 0)) {
		$error["timeoutremote"] = $LANG->_('Must be above zero');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}
// }}}

// {{{ timeoutconnect_init(host)
// init values
function timeoutconnect_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$attribs = array("timeoutconnect" => pql_get_define("PQL_ATTR_TIMEOUTCONNECT"),
					 "timeoutremote"  => pql_get_define("PQL_ATTR_TIMEOUTREMOTE"));
	foreach($attribs as $key => $attrib) {
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											$attrib);
	}
}
// }}}


// {{{ timeoutconnect_print_view(host)
function timeoutconnect_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	timeoutconnect_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Timeoutconnect, timeoutremote (<?=$LANG->_('Timeouts')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('SMTP connection')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["timeoutconnect"]) { echo $_REQUEST["timeoutconnect"]; } else { echo $LANG->_('Default', 4).': &nbsp;&nbsp;&nbsp;&nbsp;60 '; }?><?=$LANG->_('Seconds', 4)?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_TIMEOUTCONNECT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options', 4).' '.pql_get_define("PQL_ATTR_TIMEOUTCONNECT")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Remote server')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["timeoutremote"]) { echo $_REQUEST["timeoutremote"]; } else { echo $LANG->_('Default', 4).': 1200 '; }?><?=$LANG->_('Seconds', 4)?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_TIMEOUTREMOTE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options', 4).' '.pql_get_define("PQL_ATTR_TIMEOUTREMOTE")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ timeoutconnect_print_form(void)
function timeoutconnect_print_form() {
	global $error, $LANG;

	$string  = $LANG->_('Seconds', 4) . ". ";
	$string .= $LANG->_('Default');
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">timeoutconnect, timeoutremote (<?=$LANG->_('Timeouts')?>)</th>
<?php if($_REQUEST["attrib"] == "timeoutconnect") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMTP connection')?></td>
          <td><?php echo pql_format_error_span($error["timeoutconnect"]); ?><input type="text" name="timeoutconnect" value="<?=$_REQUEST["timeoutconnect"]?>"><?=$string?>: 60</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "timeoutremote") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remote server')?></td>
          <td><?php echo pql_format_error_span($error["timeoutremote"]); ?><input type="text" name="timeoutremote" value="<?=$_REQUEST["timeoutremote"]?>"> <?=$string?>: 1200</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"> <?=$LANG->_('Leave the fields empty to set the default values')?></td>
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

// {{{ timeoutconnect_save(type, host)
function timeoutconnect_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		$attribs = array("timeoutconnect" => pql_get_define("PQL_ATTR_TIMEOUTCONNECT"),
						 "timeoutremote"  => pql_get_define("PQL_ATTR_TIMEOUTREMOTE"));
		foreach($attribs as $key => $attrib) {
		  if(!@empty($_REQUEST[$key]))
			$return[] = pql_modify_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											 $attrib, 1, $_REQUEST[$key]);
		}
		
		if(in_array(false, $return)) {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('remote timeout options')));
		} else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('remote timeout options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ timeoutconnect_help(void)
function timeoutconnect_help() {
    global $LANG;
?>
<?php if($_REQUEST["attrib"] == "timeoutconnect") { ?>
	<b>SMTP connection</b><br>
	Number of seconds qmail-remote will wait for the remote
	SMTP server to accept a connection.  Default: 60.  The
	kernel normally imposes a 75-second upper limit.
<?php } if($_REQUEST["attrib"] == "timeoutremote") { ?>	
	<b>Remote server</b><br>
	Number of seconds qmail-remote will wait for each
	response from the remote SMTP server.  Default: 1200.
<?php }
}
// }}}

// {{{ timeoutconnect_help_cr(void)
function timeoutconnect_help_cr() {
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
