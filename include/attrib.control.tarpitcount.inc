<?php
// control attribute plugin for
// tarpitcount (user id of virtual user)
// tarpitdelay (group id of virtual user)
//
// $Id: attrib.control.tarpitcount.inc,v 2.13 2004-02-14 14:25:36 turbo Exp $

function tarpitcount_check($type) {
	global $error, $tarpitcount, $tarpitdelay, $view, $LANG;

	// missing
	if($tarpitcount == "") {
		$error["tarpitcount"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $tarpitcount)) {
		$error["tarpitcount"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	// missing
	if($tarpitdelay == "") {
		$error["tarpitdelay"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $tarpitdelay)) {
		$error["tarpitdelay"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	// must be > 0
	if($error["tarpitdelay"] == "" and $tarpitdelay <= 0) {
		$error["tarpitdelay"] = $LANG->_('Must be above zero');
		$is_error = true;
	}
	
	if($is_error == false)
	  return true;
}

function tarpitcount_init($host) {
	// init values
	global $_pql_control, $tarpitcount, $tarpitdelay, $view;
	
	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_TARPITCOUNT"),
					 pql_get_define("PQL_GLOB_ATTR_TARPITDELAY"));
	
	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function tarpitcount_print_view($host) {
	global $_pql_control, $tarpitcount, $tarpitdelay, $view, $LANG;
	
	// init data
	tarpitcount_init($host);

	if($tarpitcount == "" or $tarpitcount == 0)
	  $tarpitcount = $LANG->_('Disabled');
	else
	  $tarpitcount = pql_complete_constant($LANG->_('Delay submitting after %value% RCPTO\'s'),
										   array('value' => $tarpitcount));
	
	if($tarpitdelay == "")
	  // default value
	  $tarpitdelay = 5;
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">tarpitcount, tarpitdelay (<?=$LANG->_('Tarpitting settings')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Tarpitting settings')?></td>
        <td><?=$tarpitcount?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Tarpit delay')?></td>
        <td><?=$tarpitdelay?> <?=$LANG->_('Seconds', 4)?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=tarpitcount"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options', 4)?></a></td>
      </tr>
    </th>
  </table>

<?php
}

function tarpitcount_print_form() {
	global $attrib, $tarpitcount, $tarpitdelay, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">tarpitcount, tarpitdelay (<?=$LANG->_('Tarpitting settings')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Tarpit count')?></td>
          <td><?php echo pql_format_error_span($error["tarpitcount"]); ?><input type="text" name="tarpitcount" value="<?=$tarpitcount?>"> RCPTO's</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Tarpit delay')?></td>
          <td><?php echo pql_format_error_span($error["tarpitdelay"]); ?><input type="text" name="tarpitdelay" value="<?=$tarpitdelay?>"> <?=$LANG->_('Seconds', 4)?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Set tarpit count to 0 to disable tarpitting')?></td>
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
// '
function tarpitcount_save($type, $host) {
	global $_pql_control, $attrib, $tarpitcount, $tarpitdelay, $error, $mxhost, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_TARPITCOUNT"),
						 pql_get_define("PQL_GLOB_ATTR_TARPITDELAY"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
													$attrib, $$attrib);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('tarpitting options'))) . ": " . pql_format_error(0);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('tarpitting options')));
																							 
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function tarpitcount_help() {
    global $LANG;
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
?>
	<b><?=$LANG->_('Tarpit count')?></b>
	<br>
	The number of RCPT TOs you accept before you start tarpitting. Tarpitting means introducing an delay after each subsequent RCPT TO, therefore sending messages to lots of recipient takes a _lot_ of time and makes life for spammers harder.
	Default: 0 (disabled)
	<br><br>
	<b><?=$LANG->_('Tarpit delay')?></b>
	<br>
	The number of seconds of delay for tarpitting. Default: 5
	<?php
}

function tarpitcount_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
