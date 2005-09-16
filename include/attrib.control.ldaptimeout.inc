<?php
// control attribute plugin for
// ldaptimeout (max number of bytes in message)
//
// $Id: attrib.control.ldaptimeout.inc,v 2.8 2005-09-16 06:08:46 turbo Exp $

// {{{ ldaptimeout_check(type)
function ldaptimeout_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ ldaptimeout_init(host)
// init values
function ldaptimeout_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["ldaptimeout"] = pql_get_attribute($_pql_control->ldap_linkid,
												 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												 pql_get_define("PQL_ATTR_LDAPTIMEOUT"));

	if(@is_null($_REQUEST["ldaptimeout"]))
	  return true;
}
// }}}


// {{{ ldaptimeout_print_view(host)
function ldaptimeout_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldaptimeout_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldaptimeout (<?=$LANG->_('LDAP Timeout')?>)
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Seconds')?></td>
<?php
	if(!@$_REQUEST["ldaptimeout"]) {
?>
        <td><i><?=$LANG->_('Not set')?></i></td>
<?php
	} else {
?>
        <td><?=$_REQUEST["ldaptimeout"]." ".$LANG->_('seconds')?></td>
<?php
	} // end if is_array
?>
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaptimeout"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ ldaptimeout_print_form(void)
function ldaptimeout_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldaptimeout (<?=$LANG->_('LDAP Timeout')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Seconds')?></td>
          <td><?php echo pql_format_error_span($error["ldaptimeout_0"]); ?><input type="text" name="ldaptimeout" value="<?=$_REQUEST["ldaptimeout"]?>"></td>
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

// {{{ ldaptimeout_save(type, host)
function ldaptimeout_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_LDAPTIMEOUT"), 1, $_REQUEST["ldaptimeout"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldaptimeout options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldaptimeout options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ ldaptimeout_help(void)
function ldaptimeout_help() {
	global $LANG;
?>
    The time the ldap search waits for a response from the ldap server
    Default: 30 seconds
<?php
}
// }}}

// {{{ ldaptimeout_help_cr(void)
function ldaptimeout_help_cr() {
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
