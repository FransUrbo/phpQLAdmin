<?php
// control attribute plugin for
// tarpitcount (user id of virtual user)
// tarpitdelay (group id of virtual user)
//
// $Id: attrib.control.tarpitcount.inc,v 2.14 2004-03-11 16:31:55 turbo Exp $

// {{{ tarpitcount_check(type)
function tarpitcount_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["tarpitcount"] == "") {
		$error["tarpitcount"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["tarpitcount"])) {
		$error["tarpitcount"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// missing
	if($_REQUEST["tarpitdelay"] == "") {
		$error["tarpitdelay"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["tarpitdelay"])) {
		$error["tarpitdelay"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	// must be > 0
	if($error["tarpitdelay"] == "" and $_REQUEST["tarpitdelay"] <= 0) {
		$error["tarpitdelay"] = $LANG->_('Must be above zero');
		$is_error = true;
	}
	
	if($is_error == false)
	  return true;
}
// }}}

// {{{ tarpitcount_init(host)
// init values
function tarpitcount_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$attribs = array("tarpitcount" => pql_get_define("PQL_ATTR_TARPITCOUNT"),
					 "tarpitdelay" => pql_get_define("PQL_ATTR_TARPITDELAY"));
	foreach($attribs as $key => $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $_REQUEST[$key] = $value[0];
	}
}
// }}}


// {{{ tarpitcount_print_view(host)
function tarpitcount_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	tarpitcount_init($host);

	if($_REQUEST["tarpitcount"] == "" or $_REQUEST["tarpitcount"] == 0)
	  $_REQUEST["tarpitcount"] = $LANG->_('Disabled');
	else
	  $_REQUEST["tarpitcount"] = pql_complete_constant($LANG->_('Delay submitting after %value% RCPTO\'s'),
													   array('value' => $tarpitcount));
	
	if($_REQUEST["tarpitdelay"] == "")
	  // default value
	  $_REQUEST["tarpitdelay"] = 5;
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">tarpitcount, tarpitdelay (<?=$LANG->_('Tarpitting settings')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Tarpitting settings')?></td>
        <td><?=$_REQUEST["tarpitcount"]?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Tarpit delay')?></td>
        <td><?=$_REQUEST["tarpitdelay"]?> <?=$LANG->_('Seconds', 4)?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=tarpitcount"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options', 4)?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ tarpitcount_print_form(void)
function tarpitcount_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">tarpitcount, tarpitdelay (<?=$LANG->_('Tarpitting settings')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Tarpit count')?></td>
          <td><?php echo pql_format_error_span($error["tarpitcount"]); ?><input type="text" name="tarpitcount" value="<?=$_REQUEST["tarpitcount"]?>"> RCPTOs</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Tarpit delay')?></td>
          <td><?php echo pql_format_error_span($error["tarpitdelay"]); ?><input type="text" name="tarpitdelay" value="<?=$_REQUEST["tarpitdelay"]?>"> <?=$LANG->_('Seconds', 4)?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Set tarpit count to 0 to disable tarpitting')?></td>
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

// tarpitcount_save(type, host)
function tarpitcount_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array("tarpitcount" => pql_get_define("PQL_ATTR_TARPITCOUNT"),
						 "tarpitdelay" => pql_get_define("PQL_ATTR_TARPITDELAY"));
		foreach($attribs as $key => $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
													$attrib, $_REQUEST[$key]);
		
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
// }}}


// {{{ tarpitcount_help(void)
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
// }}}

// {{{ tarpitcount_help_cr(void)
function tarpitcount_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
