<?php
// control attribute plugin for
// helohost (host name used in SMTP HELO command)
//
// $Id: attrib.control.helohost.inc,v 2.10 2003-11-20 08:01:36 turbo Exp $

function helohost_check($type) {
	global $error, $helohost, $view, $LANG;

	// validity
	if($helohost != "" and !pql_check_hostaddress($helohost)) {
		$error["helohost"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}

// init values
function helohost_init($host) {
	global $_pql_control, $helohost, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_HELOHOST"));

	if(!is_null($value))
	  $helohost = $value[0];
}

function helohost_print_view($host) {
	global $_pql_control, $helohost, $view, $LANG;

	// init data
	helohost_init($host);

	// set empty field to default value
	if($helohost == "")
	  $helohost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">helohost (Helo host)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host')?></td>
        <td><?=$helohost?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=helohost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function helohost_print_form() {
	global $attrib, $helohost, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">helohost (Helo host)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["helohost"]); ?><input type="text" name="helohost" value="<?=$helohost?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

function helohost_save($type, $host) {
	global $_pql_control, $attrib, $helohost, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_HELOHOST"),
										 $helohost))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('helohost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('helohost options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function helohost_help() {
	global $LANG;
?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
<?php
}

function helohost_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
