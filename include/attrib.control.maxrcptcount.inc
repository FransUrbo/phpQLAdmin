<?php
// control attribute plugin for
// maxrcptcount (maximum number of RCPT TO's)
//
// $Id: attrib.control.maxrcptcount.inc,v 2.22 2007-02-15 12:07:37 turbo Exp $

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
	if($host == "Global")
	  $_REQUEST["maxrcptcount"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_MAXRCPTCOUNT"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["maxrcptcount"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_MAXRCPTCOUNT"));

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
    <th colspan="3" align="left">maxrcptcount (<?php echo $LANG->_('Maximum number of RCPT TO\'s')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Maximum RCPT TO\'s')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["maxrcptcount"]) { echo $_REQUEST["maxrcptcount"]; } else { echo '<i>'.$LANG->_('unlimited').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_MAXRCPTCOUNT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_MAXRCPTCOUNT")?>"></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">maxrcptcount (<?php echo $LANG->_('Maximum number of RCPT TO\'s')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Maximum RCPT TO\'s')?></td>
          <td><?php echo pql_format_error_span($error["maxrcptcount"]); ?><input type="text" name="maxrcptcount" value="<?php echo $_REQUEST["maxrcptcount"]?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set to 0 to allow unlimited RCPT TO's</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ maxrcptcount_save(type, host)
function maxrcptcount_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
		case "modify":
		case "add":
			// {{{ save entries
			if($host == "Global") {
			  // Get all QLC host
			  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
								 '(&(cn=*)(objectclass=qmailControl))');

			  $success = 1;
			  foreach($qlcs as $host) {
			    if(!pql_modify_attribute($host,
									     pql_get_define("PQL_ATTR_MAXRCPTCOUNT"), '', $_REQUEST["maxrcptcount"]))
			      $success = 0;
			  }
			} else {
			  if(pql_modify_attribute($host,
									  pql_get_define("PQL_ATTR_MAXRCPTCOUNT"), 1, $_REQUEST["maxrcptcount"]))
				$success = 1;
			}

			if($success)
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('maxrcptcount options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('maxrcptcount options')))
				. ": " . pql_format_error(0);

			attribute_forward($msg);
			break;
			// }}}

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
