<?php
// control attribute plugin for
// databytes (max number of bytes in message)
//
// $Id: attrib.control.databytes.inc,v 2.12 2004-02-14 14:25:35 turbo Exp $

function databytes_check($type) {
	global $error, $databytes, $view, $LANG;

	// number ?
	if($databytes != "" and preg_match ("/[^0-9]/", $databytes)) {
		$error["databytes"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}

// init values
function databytes_init($host) {
	global $_pql_control, $databytes, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_DATABYTES"));
	
	if(!is_null($value))
	  $databytes = $value[0];
}

function databytes_print_view($host) {
	global $_pql_control, $databytes, $view, $LANG;

	// init data
	databytes_init($host);

	// set empty field to "not set"
	if($databytes == "" or $databytes == 0)
	  $databytes = "unlimited";
	else {
		$hr_databytes = "(" . pql_print_filesize($databytes) . ")";
		$databytes .= " ".$LANG->_('Bytes');
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">databytes (<?=$LANG->_('Maximum number databytes in message')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Maximum')?></td>
        <td><?="$databytes $hr_databytes"?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=databytes"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function databytes_print_form() {
	global $attrib, $databytes, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">databytes (<?=$LANG->_('Maximum number databytes in message')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum')?></td>
          <td><?php echo pql_format_error_span($error["databytes"]); ?><input type="text" name="databytes" value="<?=$databytes?>"> <?=$LANG->_('Bytes')?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Set to 0 to allow unlimited message size')?></td>
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

function databytes_save($type, $host) {
	global $_pql_control, $attrib, $databytes, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_DATABYTES"),
										 $databytes))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('databytes options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('databytes options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function databytes_help() {
	global $LANG;
?>
	Maximum number of bytes allowed in a message, or 0 for
	no limit.  Default: 0.  If a message exceeds this
	limit, qmail-smtpd returns a permanent error code to
	the client; in contrast, if the disk is full or qmail-
	smtpd hits a resource limit, qmail-smtpd returns a
	temporary error code.
	<br><br>
	databytes counts bytes as stored on disk, not as
	transmitted through the network.  It does not count the
	qmail-smtpd Received line, the qmail-queue Received
	line, or the envelope.
<?php
}

function databytes_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
