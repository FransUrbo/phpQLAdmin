<?php
// control attribute plugin for
// localiphost (name substituted for local IP address)
//
// $Id: attrib.control.localiphost.inc,v 2.12 2004-01-01 12:35:34 turbo Exp $

function localiphost_check($type) {
	global $error, $localiphost, $view, $LANG;

	// validity
	if($localiphost != "" and !pql_check_hostaddress($localiphost)) {
		$error["localiphost"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}

// init values
function localiphost_init($host) {
	global $_pql_control, $localiphost, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_LOCALIPHOST"));
	
	if(!is_null($value))
	  $localiphost = $value[0];
}

function localiphost_print_view($host) {
	global $_pql_control, $localiphost, $view, $LANG;
	
	// init data
	localiphost_init($host);

	// set empty field to default value
	if($localiphost == "")
	  $localiphost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">localiphost (<?=$LANG->_('Substitution for local IP')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host')?></td>
        <td><?=$localiphost?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=localiphost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function localiphost_print_form() {
	global $attrib, $localiphost, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">localiphost (<?=$LANG->_('Substitution for local IP')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["localiphost"]); ?><input type="text" name="localiphost" value="<?php echo $localiphost; ?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Value in \'me\'')?>)</td>
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

function localiphost_save($type, $host) {
	global $_pql_control, $attrib, $localiphost, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_LOCALIPHOST"),
										 $localiphost))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('localiphost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('localiphost options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function localiphost_help() {
	global $LANG;
?>
	Replacement host name for local IP addresses.  Default:
	me, if that is supplied.  qmail-smtpd is responsible
	for recognizing dotted-decimal addresses for the
	current host.  When it sees a recipient address of the
	form box@[d.d.d.d], where d.d.d.d is a local IP
	address, it replaces [d.d.d.d] with localiphost.  This
	is done before rcpthosts.
<?php
}

function localiphost_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
