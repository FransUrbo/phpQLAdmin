<?php
// control attribute plugin for
// bouncemaxbytes (max number of bytes in message)
//
// $Id: attrib.control.bouncesize.inc,v 2.5 2004-10-19 10:40:40 turbo Exp $

// {{{ bouncemaxbytes_check(type)
function bouncemaxbytes_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ bouncemaxbytes_init(host)
// init values
function bouncemaxbytes_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["bouncemaxbytes"] = pql_get_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_BOUNCESIZE"));

	if(is_null($_REQUEST["bouncemaxbytes"]))
	  return true;
}
// }}}


// {{{ bouncemaxbytes_print_view(host)
function bouncemaxbytes_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	bouncemaxbytes_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">bouncemaxbytes (<?=$LANG->_('Maximum bounce message size.')?>)
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Bytes</td>
<?php
	if(!$_REQUEST["bouncemaxbytes"]) {
?>
        <td><i><?=$LANG->_('Not set')?></i></td>
<?php
	} else {
?>
        <td><?=$_REQUEST["bouncemaxbytes"]?></td>
<?php
	} // end if is_array
?>
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=bouncemaxbytes"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ bouncemaxbytes_print_form(void)
function bouncemaxbytes_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">bouncemaxbytes (<?=$LANG->_('Maximum bounce message size.')?>)
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Bytes')?></td>
          <td><?php echo pql_format_error_span($error["bouncemaxbytes"]); ?><input type="text" name="bouncemaxbytes" value="<?=$_REQUEST["bouncemaxbytes"]?>"></td>
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

// {{{ bouncemaxbytes_save(type, host)
function bouncemaxbytes_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_BOUNCESIZE"), '', $_REQUEST["bouncemaxbytes"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bouncemaxbytes options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bouncemaxbytes options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ bouncemaxbytes_help(void)
function bouncemaxbytes_help() {
	global $LANG;
?>
    The maximal number of bytes to be included in a bounce message.
<?php
}
// }}}

// {{{ bouncemaxbytes_help_cr(void)
function bouncemaxbytes_help_cr() {
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
