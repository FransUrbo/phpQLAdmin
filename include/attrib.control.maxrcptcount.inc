<?php
// control attribute plugin for
// maxrcptcount (maximum number of RCPT TO's)
//
// $Id: attrib.control.maxrcptcount.inc,v 2.9 2003-08-15 08:06:17 turbo Exp $

function maxrcptcount_check($type) {
	global $error, $maxrcptcount, $view, $LANG;

	// missing
	if($maxrcptcount == "") {
		$error["maxrcptcount"] = $LANG->_('Missing');
		return false;
	}

	// number ?
	if(preg_match ("/[^0-9]/", $maxrcptcount)) {
		$error["maxrcptcount"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}

// init values
function maxrcptcount_init($host) {
	global $_pql_control, $maxrcptcount, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_MAXRCPTCOUNT"));

	if(!is_null($value))
	  $maxrcptcount = $value[0];
}

function maxrcptcount_print_view($host) {
	global $_pql_control, $maxrcptcount, $view, $LANG;

	// init data
	maxrcptcount_init($host);

	// set empty field to "not set"
	if($maxrcptcount == "" or $maxrcptcount == 0)
	  $maxrcptcount = "unlimited";
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">maxrcptcount (<?=$LANG->_('Maximum number of RCPT TO\'s')?>)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Maximum RCPT TO\'s')?></td>
        <td><?=$maxrcptcount?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=maxrcptcount"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function maxrcptcount_print_form() {
	global $attrib, $maxrcptcount, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">maxrcptcount (<?=$LANG->_('Maximum number of RCPT TO\'s')?>)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Maximum RCPT TO\'s')?></td>
          <td><?php echo format_error($error["maxrcptcount"]); ?><input type="text" name="maxrcptcount" value="<?=$maxrcptcount?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set to 0 to allow unlimited RCPT TO's</td>
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
// '
function maxrcptcount_save($type, $host) {
	global $_pql_control, $attrib, $maxrcptcount, $error, $view, $LANG;

	switch($type) {
		case "modify";
		case "add";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_GLOB_ATTR_MAXRCPTCOUNT"),
											 $maxrcptcount))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('maxrcptcount options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('maxrcptcount options')))
				. ": " . pql_ldap_error($_pql_control->ldap_linkid);

			attribute_forward($msg);
			break;

		default:
		  die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
									array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
