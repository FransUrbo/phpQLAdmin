<?php
// control attribute plugin for
// pbscachesize (max number of bytes in message)
//
// $Id: attrib.control.pbscachesize.inc,v 2.4 2004-10-18 13:39:32 turbo Exp $

// {{{ pbscachesize_check(type)
function pbscachesize_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbscachesize_init(host)
// init values
function pbscachesize_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["pbscachesize"] = pql_get_attribute($_pql_control->ldap_linkid,
												  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												  pql_get_define("PQL_ATTR_PBS_CACHESIZE"));

	if(is_null($_REQUEST["pbscachesize"]))
	  return true;
}
// }}}


// {{{ pbscachesize_print_view(host)
function pbscachesize_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbscachesize_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbscachesize (<?=$LANG->_('POP before SMTP cache size')?>)
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Bytes')?></td>
<?php
	if(!$_REQUEST["pbscachesize"]) {
?>
        <td><i><?=$LANG->_('Not set')?></i></td>
<?php
	} else {
?>
        <td><?=$_REQUEST["pbscachesize"]?></td>
<?php
	} // end if is_array
?>
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=pbscachesize"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbscachesize_print_form(void)
function pbscachesize_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbscachesize (<?=$LANG->_('POP before SMTP cache size')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Bytes')?></td>
          <td><?php echo pql_format_error_span($error["pbscachesize"]); ?><input type="text" name="pbscachesize" value="<?=$_REQUEST["pbscachesize"]?>"></td>
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

// {{{ pbscachesize_save(type, host)
function pbscachesize_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_PBS_CACHESIZE"),
										 $_REQUEST["pbscachesize"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbscachesize options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbscachesize options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ pbscachesize_help(void)
function pbscachesize_help() {
	global $LANG;
?>
    Size in bytes used for the cache. Normally you do not need to set this
    because the default value is big enough (1Mb).
<?php
}
// }}}

// {{{ pbscachesize_help_cr(void)
function pbscachesize_help_cr() {
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
