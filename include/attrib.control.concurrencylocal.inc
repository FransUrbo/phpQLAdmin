<?php
// control attribute plugin for
// concurrencylocal (may simultaneous local deliveries)
// concurrencyremote (may simultaneous remote deliveries)
//
// $Id: attrib.control.concurrencylocal.inc,v 2.17 2004-08-27 08:49:33 turbo Exp $

// {{{ concurrencylocal_check(type)
function concurrencylocal_check($type) {
	global $error, $LANG;

	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["concurrencylocal"])) {
		$error["concurrencylocal"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["concurrencyremote"])) {
		$error["concurrencyremote"] = $LANG->_('Invalid');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}
// }}}

// {{{ concurrencylocal_init(host)
// init values
function concurrencylocal_init($host) {
	global $_pql_control, $concurrencylocal, $concurrencyremote, $view;

	// fetch data from ldap server
	$attribs = array("concurrencylocal"  => pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
					 "concurrencyremote" => pql_get_define("PQL_ATTR_CONCURRENCYREMOTE"));
	foreach($attribs as $key => $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $_REQUEST[$key] = $value[0];
	}
}
// }}}


// {{{ concurrencylocal_print_view(host)
function concurrencylocal_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	concurrencylocal_init($host);

	// set empty fields to their default value
	if($_REQUEST["concurrencylocal"] == "")	  $_REQUEST["concurrencylocal"] = "10 (".$LANG->_('Default', 4).")";
	if($_REQUEST["concurrencylocal"] == 0)	  $_REQUEST["concurrencylocal"] = "0 (".$LANG->_('local delivery holded').")";
	
	if($_REQUEST["concurrencyremote"] == "")  $_REQUEST["concurrencyremote"] = "20 (".$LANG->_('Default', 4).")";
	if($_REQUEST["concurrencyremote"] == 0)	  $_REQUEST["concurrencyremote"] = "0 (".$LANG->_('remote delivery holded').")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">concurrencylocal, concurrencyremote (<?=$LANG->_('Maximum simultaneous deliveries')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Local')?></td>
        <td><?=$_REQUEST["concurrencylocal"]?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Remote')?></td>
        <td><?=$_REQUEST["concurrencyremote"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=concurrencylocal"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ concurrencylocal_print_form(void)
function concurrencylocal_print_form() {
	global $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">concurrencylocal, concurrencyremote (<?=$LANG->_('Maximum simultaneous deliveries')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Local')?></td>
          <td><?php echo pql_format_error_span($error["concurrencylocal"]); ?><input type="text" name="concurrencylocal" value="<?=$_REQUEST["concurrencylocal"]?>"> <?=$LANG->_('Default')?>: 10</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Remote')?></td>
          <td><?php echo pql_format_error_span($error["concurrencyremote"]); ?><input type="text" name="concurrencyremote" value="<?=$_REQUEST["concurrencyremote"]?>"> <?=$LANG->_('Default')?>: 20</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to set the default values')?>.</td>
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

// {{{ concurrencylocal_save(type, host)
function concurrencylocal_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array("concurrencylocal"  => pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
						 "concurrencyremote" => pql_get_define("PQL_ATTR_CONCURRENCYREMOTE"));
		foreach($attribs as $key => $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
													$attrib, $_REQUEST[$key]);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bouncing options')))
			. ": " . pql_format_error(0);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bouncing options')));
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ concurrencylocal_help(void)
function concurrencylocal_help() {
	global $LANG;
?>
	<b><?=$LANG->_('Local')?></b>
	<br>
	Maximum number of simultaneous local delivery attempts.
	Default: 10.  If 0, local deliveries will be put on
	hold.  concurrencylocal is limited at compile time to
	120.
	<br><br>
	<b><?=$LANG->_('Remote')?></b>
	<br>
	Maximum number of simultaneous remote delivery
	attempts.  Default: 20.  If 0, remote deliveries will
	be put on hold.  concurrencyremote is limited at
	compile time to 120.
<?php
}
// }}}

// {{{ concurrencylocal_help_cr(void)
function concurrencylocal_help_cr() {
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
