<?php
// control attribute plugin for
// maxrcptcount (maximum number of RCPT TO's)
//
// $Id: attrib.control.maxrcptcount.inc,v 2.18 2005-09-16 06:08:46 turbo Exp $

// {{{ maxrcptcount_check(type)
function maxrcptcount_check($type) {
	global $error, $LANG;

	// missing
	if(@empty($_REQUEST["maxrcptcount"])) {
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
	$_REQUEST["maxrcptcount"] = pql_get_attribute($_pql_control->ldap_linkid,
												  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												  pql_get_define("PQL_ATTR_MAXRCPTCOUNT"));

	if(@is_null($_REQUEST["maxrcptcount"]))
	  return true;
}
// }}}


// {{{ maxrcptcount_print_view(host)
function maxrcptcount_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	maxrcptcount_init($host);
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">maxrcptcount (<?=$LANG->_('Maximum number of RCPT TO\'s')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Maximum RCPT TO\'s')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["maxrcptcount"]) { echo $_REQUEST["maxrcptcount"]; } else { echo '<i>'.$LANG->_('unlimited').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_MAXRCPTCOUNT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_MAXRCPTCOUNT")?>"></a></td>
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
    <input type="hidden" name="type"   value="modify">
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
		case "modify":
		case "add":
			// save entries
			if(pql_modify_attribute($_pql_control->ldap_linkid,
									pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
									pql_get_define("PQL_ATTR_MAXRCPTCOUNT"), 1, $_REQUEST["maxrcptcount"]))
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
