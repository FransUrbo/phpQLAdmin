<?php
// control attribute plugin for
// concurrencylocal (may simultaneous local deliveries)
// concurrencyremote (may simultaneous remote deliveries)
//
// $Id: attrib.control.concurrencylocal.inc,v 2.11 2003-10-31 07:22:56 turbo Exp $

function concurrencylocal_check($type) {
	global $error, $concurrencylocal, $concurrencyremote, $view, $LANG;

	// check validity
	if(preg_match("/[^0-9]/", $concurrencylocal)) {
		$error["concurrencylocal"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if(preg_match("/[^0-9]/", $concurrencyremote)) {
		$error["concurrencyremote"] = $LANG->_('Invalid');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}

// init values
function concurrencylocal_init($host) {
	global $_pql_control, $concurrencylocal, $concurrencyremote, $view;

	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_CONCURRENCYLOCAL"),
					 pql_get_define("PQL_GLOB_ATTR_CONCURRENCYREMOTE"));

	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function concurrencylocal_print_view($host) {
	global $_pql_control, $concurrencylocal, $concurrencyremote, $view, $LANG;

	// init data
	concurrencylocal_init($host);

	// set empty fields to their default value
	if($concurrencylocal == "")
	  $concurrencylocal = "10 (".$LANG->_('Default', 4).")";
	
	if($concurrencylocal == 0)
	  $concurrencylocal = "0 (".$LANG->_('local delivery holded').")";
	
	if($concurrencyremote == "")
	  $concurrencyremote = "20 (".$LANG->_('Default', 4).")";
	
	if($concurrencyremote == 0)
	  $concurrencyremote = "0 (".$LANG->_('remote delivery holded').")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">concurrencylocal, concurrencyremote (<?=$LANG->_('Maximum simultaneous deliveries')?>)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Local')?></td>
        <td><?=$concurrencylocal?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Remote')?></td>
        <td><?=$concurrencyremote?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=concurrencylocal"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function concurrencylocal_print_form() {
	global $attrib, $concurrencylocal, $concurrencyremote, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">concurrencylocal, concurrencyremote (<?=$LANG->_('Maximum simultaneous deliveries')?>)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Local')?></td>
          <td><?php echo format_error($error["concurrencylocal"]); ?><input type="text" name="concurrencylocal" value="<?=$concurrencylocal?>"> <?=$LANG->_('Default')?>: 10</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Remote')?></td>
          <td><?php echo format_error($error["concurrencyremote"]); ?><input type="text" name="concurrencyremote" value="<?=$concurrencyremote?>"> <?=$LANG->_('Default')?>: 20</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to set the default values')?>.</td>
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

function concurrencylocal_save($type, $host) {
	global $_pql_control, $attrib, $concurrencylocal, $concurrencyremote, $error, $mxhost, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_CONCURRENCYLOCAL"),
						 pql_get_define("PQL_GLOB_ATTR_CONCURRENCYREMOTE"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
													$attrib, $$attrib);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bouncing options')));
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
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

function concurrencylocal_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
