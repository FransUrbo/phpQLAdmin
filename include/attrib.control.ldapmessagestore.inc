<?php
// control attribute plugin for
// ldapmessagestore (default prefix for mailbox paths)
//
// $Id: attrib.control.ldapmessagestore.inc,v 2.13 2004-03-11 16:31:54 turbo Exp $

// {{{ ldapmessagestore_check(type)
function ldapmessagestore_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["ldapmessagestore"] == "") {
		$error["ldapmessagestore"] = $LANG->_('Missing');
		return false;
	}

	// trailing slash ?
	if(!preg_match ("/^(\/)/", $_REQUEST["ldapmessagestore"])) {
		$error["ldapmessagestore"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ ldapmessagestore_init(host)
// init values
function ldapmessagestore_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"));
	
	if(!is_null($value))
	  $_REQUEST["ldapmessagestore"] = $value[0];
}
// }}}


// {{{ ldapmessagestore_print_view(host)
function ldapmessagestore_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapmessagestore_init($host);

	// set empty field to "not set"
	if($_REQUEST["ldapmessagestore"] == "")
	  $_REQUEST["ldapmessagestore"] = $LANG->_('Not set');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapmessagestore (<?=$LANG->_('Default mailbox prefix path')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Path')?></td>
        <td><?=$_REQUEST["ldapmessagestore"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapmessagestore"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapmessagestore_print_form(void)
function ldapmessagestore_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapmessagestore (<?=$LANG->_('Default mailbox prefix path')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Path')?></td>
          <td><?php echo pql_format_error_span($error["ldapmessagestore"]); ?><input type="text" name="ldapmessagestore" value="<?=$_REQUEST["ldapmessagestore"]?>"></td>
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

// {{{ ldapmessagestore_save(type, host)
function ldapmessagestore_save($type, $host) {
	global $_pql_control, $LANG;
	
	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"),
										 $_REQUEST["ldapmessagestore"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapserver options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapserver options')))
			. ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ ldapmessagestore_help(void)
function ldapmessagestore_help() {
	global $LANG;
?>
	The default prefix for paths in mailmessagestore without trailing / If you set this to /maildisk/ for example, the ldap attributes
<pre>
mailmessagestore: joe/Maildir/
</pre>
and
<pre>
mailmessagestore: /maildisk/joe/Maildir/
</pre>
	are equivalent.
<?php
}
// }}}

// {{{ ldapmessagestore_help_cr(void)
function ldapmessagestore_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>