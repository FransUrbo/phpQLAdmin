<?php
// control attribute plugin for
// doublebouncehost (username of bounce sender)
// doublebounceto (hostname of bounce sender)
//
// $Id: attrib.control.doublebouncehost.inc,v 2.11.2.1 2003-12-09 16:18:24 dlw Exp $

function doublebouncehost_check($type) {
	global $error, $doublebouncehost, $doublebounceto, $view, $LANG;

	// check validity
	if($doublebounceto != "" and !pql_check_email($doublebounceto . "@domain.tld")) {
		$error["doublebounceto"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if($doublebouncehost != "" and !pql_check_hostaddress($doublebouncehost)) {
		$error["doublebouncehost"] = $LANG->_('Invalid');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}

// init values
function doublebouncehost_init($host) {
	global $_pql_control, $doublebouncehost, $doublebounceto, $view;

	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCEHOST"),
					 pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCETO"));
	
	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function doublebouncehost_print_view($host) {
	global $_pql_control, $doublebouncehost, $doublebounceto, $view, $LANG;

	// init data
	doublebouncehost_init($host);

	// set empty fields to their default value
	if($doublebouncehost == "")
	  $doublebouncehost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
	
	if($doublebounceto == "")
	  $doublebounceto = $LANG->_('Default') . ": postmaster";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">doublebouncehost, doublebounceto (Double bounce)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host')?></td>
        <td><?=$doublebouncehost?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Receiver')?></td>
        <td><?=$doublebounceto?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=doublebouncehost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}

function doublebouncehost_print_form() {
	global $attrib, $doublebouncehost, $doublebounceto, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Double bounce</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["doublebouncehost"]); ?><input type="text" name="doublebouncehost" value="<?=$doublebouncehost?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Receiver')?></td>
          <td><?php echo pql_format_error_span($error["doublebounceto"]); ?><input type="text" name="doublebounceto" value="<?=$doublebounceto?>"> <?=$LANG->_('Default')?>: postmaster</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function doublebouncehost_save($type, $host) {
	global $_pql_control, $attrib, $doublebouncehost, $doublebounceto, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCEHOST"),
						 pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCETO"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
													$attrib,
													$$attrib);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('double bouncing options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('double bouncing options')));
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function doublebouncehost_help() {
	global $LANG;
?>
	<b><?=$LANG->_('Host')?> (doublebouncehost)</b><br>
	Double-bounce host.  Default:  me, if that is supplied;
	otherwise the literal name doublebouncehost, which is
	probably not what you want.Host name of double bounce sender.
	<br><br>
	<b><?=$LANG->_('Receiver')?> (doublebounceto)</b>
	<br>
	User to receive double-bounces.  Default:  postmaster.
	If a single-bounce notice is permanently undeliverable,
	qmail-send sends a double-bounce notice to
	doublebounceto@doublebouncehost.  (If that bounces,
	qmail-send gives up.)
<?php
}

function doublebouncehost_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
