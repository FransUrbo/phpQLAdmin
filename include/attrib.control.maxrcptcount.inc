<?php
// control attribute plugin for
// maxrcptcount (maximum number of RCPT TO's)
//
// $Id: attrib.control.maxrcptcount.inc,v 2.13 2004-03-11 16:31:54 turbo Exp $

// {{{ maxrcptcount_check(type)
function maxrcptcount_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["maxrcptcount"] == "") {
		$error["maxrcptcount"] = $LANG->_('Missing');
		return false;
	}

	// number ?
	if(preg_match ("/[^0-9]/", $_REQUEST["maxrcptcount"])) {
		$error["maxrcptcount"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ maxrcptcount_init(host)
// init values
function maxrcptcount_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_MAXRCPTCOUNT"));

	if(!is_null($value))
	  $_REQUEST["maxrcptcount"] = $value[0];
}
// }}}


// {{{ maxrcptcount_print_view(host)
function maxrcptcount_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	maxrcptcount_init($host);

	// set empty field to "not set"
	if($_REQUEST["maxrcptcount"] == "" or $_REQUEST["maxrcptcount"] == 0)
	  $_REQUEST["maxrcptcount"] = "unlimited";
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">maxrcptcount (<?=$LANG->_('Maximum number of RCPT TO\'s')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Maximum RCPT TO\'s')?></td>
        <td><?=$_REQUEST["maxrcptcount"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=maxrcptcount"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ maxrcptcount_print_form(void)
function maxrcptcount_print_form() {
	global $attrib, $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">maxrcptcount (<?=$LANG->_('Maximum number of RCPT TO\'s')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum RCPT TO\'s')?></td>
          <td><?php echo pql_format_error_span($error["maxrcptcount"]); ?><input type="text" name="maxrcptcount" value="<?=$_REQUEST["maxrcptcount"]?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set to 0 to allow unlimited RCPT TO's</td>
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

// {{{ maxrcptcount_save(type, host)
function maxrcptcount_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
		case "modify";
		case "add";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_ATTR_CN")."=".$host.",".$_SESSION["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_ATTR_MAXRCPTCOUNT"),
											 $_REQUEST["maxrcptcount"]))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('maxrcptcount options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('maxrcptcount options')))
				. ": " . pql_format_error(0);

			attribute_forward($msg);
			break;

		default:
		  die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
									array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
