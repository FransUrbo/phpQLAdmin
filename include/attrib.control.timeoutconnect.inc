<?php
// control attribute plugin for
// timeoutconnect (how long to wait for smtp connection)
// timeoutremote (how long to wait for remote server)
//
// $Id: attrib.control.timeoutconnect.inc,v 2.11 2003-11-20 08:01:36 turbo Exp $

function timeoutconnect_check($type) {
	global $error, $timeoutconnect, $timeoutremote, $view, $LANG;

	// check validity
	if(preg_match("/[^0-9]/", $timeoutconnect)) {
		$error["timeoutconnect"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// must be > 0
	if($error["timeoutconnect"] == "" and $timeoutconnect != "" and $timeoutconnect <= 0) {
		$error["timeoutconnect"] = $LANG->_('Must be above zero');
		$is_error = true;
	}

	// check validity
	if(preg_match("/[^0-9]/", $timeoutremote)) {
		$error["timeoutremote"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// must be > 0
	if($error["timeoutremote"] == "" and $timeoutremote != "" and $timeoutremote <= 0) {
		$error["timeoutremote"] = $LANG->_('Must be above zero');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}

function timeoutconnect_init($host) {
	// init values
	global $_pql_control, $timeoutconnect, $timeoutremote, $view;

	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_TIMEOUTCONNECT"),
					 pql_get_define("PQL_GLOB_ATTR_TIMEOUTREMOTE"));

	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);

		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function timeoutconnect_print_view($host) {
	global $_pql_control, $timeoutconnect, $timeoutremote, $view, $LANG;

	// init data
	timeoutconnect_init($host);

	// set empty fields to their default value
	if($timeoutconnect == "")
	  $timeoutconnect = "60 ".$LANG->_('Seconds', 4)." (".$LANG->_('Default', 4).")";
	else
	  $timeoutconnect .= " ".$LANG->_('Seconds', 4);
	
	if($timeoutremote == "")
	  $timeoutremote = "1200 ".$LANG->_('Seconds', 4)." (".$LANG->_('Default', 4).")";
	else
	  $timeoutremote .= " ".$LANG->_('Seconds', 4);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Timeoutconnect, timeoutremote (<?=$LANG->_('Timeouts')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('SMTP connection')?></td>
        <td><?=$timeoutconnect?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Remote server')?></td>
        <td><?=$timeoutremote?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=timeoutconnect"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options', 4)?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function timeoutconnect_print_form() {
	global $attrib, $timeoutconnect, $timeoutremote, $PHP_SELF, $error, $mxhost, $view, $LANG;
	$string  = $LANG->_('Seconds', 4) . ". ";
	$string .= $LANG->_('Default');
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">timeoutconnect, timeoutremote (<?=$LANG->_('Timeouts')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('SMTP connection')?></td>
          <td><?php echo pql_format_error_span($error["timeoutconnect"]); ?><input type="text" name="timeoutconnect" value="<?=$timeoutconnect?>"><?=$string?>: 60</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remote server')?></td>
          <td><?php echo pql_format_error_span($error["timeoutremote"]); ?><input type="text" name="timeoutremote" value="<?=$timeoutremote?>"> <?=$string?>: 1200</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"> <?=$LANG->_('Leave the fields empty to set the default values')?></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function timeoutconnect_save($type, $host) {
	global $_pql_control, $attrib, $timeoutconnect, $timeoutremote, $error, $mxhost, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_TIMEOUTCONNECT"),
						 pql_get_define("PQL_GLOB_ATTR_TIMEOUTREMOTE"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
													$attrib, $$attrib);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('remote timeout options'))) . ": " . pql_ldap_error($_pql_control->ldap_linkid);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('remote timeout options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function timeoutconnect_help() {
    global $LANG;
?>
<b>SMTP connection</b><br>
Number of seconds qmail-remote will wait for the remote
SMTP server to accept a connection.  Default: 60.  The
kernel normally imposes a 75-second upper limit.
<br><br>
<b>Remote server</b><br>
Number of seconds qmail-remote will wait for each
response from the remote SMTP server.  Default: 1200.
<?php
}

function timeoutconnect_help_cr() {
    global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
