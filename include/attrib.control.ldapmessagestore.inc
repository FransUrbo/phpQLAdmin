<?php
// control attribute plugin for
// ldapmessagestore (default prefix for mailbox paths)
//
// $Id: attrib.control.ldapmessagestore.inc,v 2.9 2003-08-15 08:06:17 turbo Exp $

function ldapmessagestore_check($type) {
	global $error, $ldapmessagestore, $view, $LANG;

	// missing
	if($ldapmessagestore == "") {
		$error["ldapmessagestore"] = $LANG->_('Missing');
		return false;
	}

	// trailing slash ?
	if(!preg_match ("/^(\/)/", $ldapmessagestore)) {
		$error["ldapmessagestore"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}

// init values
function ldapmessagestore_init($host) {
	global $_pql_control, $ldapmessagestore, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_LDAPMESSAGESTORE"));
	
	if(!is_null($value))
	  $ldapmessagestore = $value[0];
}

function ldapmessagestore_print_view($host) {
	global $_pql_control, $ldapmessagestore, $view, $LANG;

	// init data
	ldapmessagestore_init($host);

	// set empty field to "not set"
	if($ldapmessagestore == "")
	  $ldapmessagestore = $LANG->_('Not set');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapmessagestore (<?=$LANG->_('Default mailbox prefix path')?>)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Path')?></td>
        <td><?=$ldapmessagestore?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapmessagestore"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function ldapmessagestore_print_form() {
	global $attrib, $ldapmessagestore, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapmessagestore (<?=$LANG->_('Default mailbox prefix path')?>)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Path')?></td>
          <td><?php echo format_error($error["ldapmessagestore"]); ?><input type="text" name="ldapmessagestore" value="<?=$ldapmessagestore?>"></td>
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

function ldapmessagestore_save($type, $host) {
	global $_pql_control, $attrib, $ldapmessagestore, $error, $view, $LANG;
	
	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_LDAPMESSAGESTORE"),
										 $ldapmessagestore))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapserver options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapserver options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function ldapmessagestore_help() {
	global $LANG;
?>
	The default prefix for paths in mailmessagestore without trailing / If you set this to /maildisk/ for example, the ldap attributes
	<br><br>
	mailmessagestore: joe/Maildir/
	<br>
  mailmessagestore: /maildisk/joe/Maildir/
	<br><br>
	are equivalent.
<?php
}

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
