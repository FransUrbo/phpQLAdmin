<?php
// control attribute plugin for
// timeoutsmtpd (how long to wait for smtp client)
//
// $Id: attrib.control.timeoutsmtpd.inc,v 2.11 2003-11-20 08:01:37 turbo Exp $

function timeoutsmtpd_check($type) {
	global $error, $timeoutsmtpd, $LANG;

	// number ?
	if(preg_match ("/[^0-9]/", $timeoutsmtpd)) {
		$error["timeoutsmtpd"] = $LANG->_('Invalid');
		return false;
	}

	// must be > 0
	if($error["timeoutsmtpd"] == "" and $timeoutsmtpd <= 0) {
		$error["timeoutsmtpd"] = $LANG->_('Must be above zero');
		return false;
	}

	return true;
}

function timeoutsmtpd_init($host) {
	// init values
	global $_pql_control, $timeoutsmtpd;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_TIMEOUTSMTPD"));
	
	if(!is_null($value))
	  $timeoutsmtpd = $value[0];
}

function timeoutsmtpd_print_view($host) {
	global $_pql_control, $timeoutsmtpd, $LANG;

	// init data
	timeoutsmtpd_init($host);

	// set empty field to "not set"
	if($timeoutsmtpd == "")
	  $timeoutsmtpd = "1200 ".$LANG->_('Seconds', 4)." (".$LANG->_('Default', 4).")";
	else
	  $timeoutsmtpd .= " ".$LANG->_('Seconds', 4);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Timeoutsmtpd (How long to wait for SMTP client)')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Wait')?></td>
        <td><?=$timeoutsmtpd?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=timeoutsmtpd"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options', 4)?></a></td>
      </tr>
    </th>
  </table>

<?php
}

function timeoutsmtpd_print_form() {
	global $attrib, $timeoutsmtpd, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Timeoutsmtpd (How long to wait for SMTP client)')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Wait')?></td>
          <td><?php echo pql_format_error_span($error["timeoutsmtpd"]); ?><input type="text" name="timeoutsmtpd" value="<?=$timeoutsmtpd?>"><?=$LANG->_('Seconds', 4)?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo pql_complete_constant($LANG->_('Empty field to set to the default value (%default% seconds)'), array('default' => '1200')); ?></td>
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

function timeoutsmtpd_save($type, $host) {
	global $_pql_control, $attrib, $timeoutsmtpd, $error, $mxhost, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_TIMEOUTSMTPD"),
										 $timeoutsmtpd))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('timeoutsmtpd options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('timeoutsmtpd options'))) . ": " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function timeoutsmtpd_help() {
	global $LANG;
?>
	Number of seconds qmail-smtpd will wait for each new
	buffer of data from the remote SMTP client.  Default:
	1200.
<?php
}

function timeoutsmtpd_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
