<?php
// control attribute plugin for
// databytes (max number of bytes in message)
//
// $Id: attrib.control.databytes.inc,v 2.13 2004-03-11 16:31:54 turbo Exp $

// {{{ databytes_check(type)
function databytes_check($type) {
	global $error, $LANG;

	// number ?
	if($_REQUEST["databytes"] != "" and preg_match ("/[^0-9]/", $_REQUEST["databytes"])) {
		$error["databytes"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ databytes_init(host)
// init values
function databytes_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_DATABYTES"));
	
	if(!is_null($value))
	  $_REQUEST["databytes"] = $value[0];
}
// }}}


// {{{ databytes_print_view(host)
function databytes_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	databytes_init($host);

	// set empty field to "not set"
	if($_REQUEST["databytes"] == "" or $_REQUEST["databytes"] == 0)
	  $_REQUEST["databytes"] = "unlimited";
	else {
		$hr_databytes = "(" . pql_print_filesize($databytes) . ")";
		$_REQUEST["databytes"] .= " ".$LANG->_('Bytes');
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">databytes (<?=$LANG->_('Maximum number databytes in message')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Maximum')?></td>
        <td><?="$_REQUEST["databytes"] $hr_databytes"?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=databytes"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ databytes_print_form(void)
function databytes_print_form() {
	global $attrib, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">databytes (<?=$LANG->_('Maximum number databytes in message')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum')?></td>
          <td><?php echo pql_format_error_span($error["databytes"]); ?><input type="text" name="databytes" value="<?=$_REQUEST["databytes"]?>"> <?=$LANG->_('Bytes')?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Set to 0 to allow unlimited message size')?></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ databytes_save(type, host)
function databytes_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_DATABYTES"),
										 $_REQUEST["databytes"]))
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
// }}}


// {{{ databytes_help(void)
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
// }}}

// {{{ databytes_help_cr(void)
function databytes_help_cr() {
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
