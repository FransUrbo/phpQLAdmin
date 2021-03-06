<?php
// control attribute plugin for
// ldapmessagestore (default prefix for mailbox paths)
//
// $Id: attrib.control.ldapmessagestore.inc,v 2.23 2007-02-15 12:07:26 turbo Exp $

// {{{ ldapmessagestore_check(type)
function ldapmessagestore_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["ldapmessagestore"] == "") {
		$error["ldapmessagestore"] = $LANG->_('Missing');
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
	if($host == "Global")
	  $_REQUEST["ldapmessagestore"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
														pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["ldapmessagestore"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"));

	if(@is_null($_REQUEST["ldapmessagestore"]))
	  return true;
}
// }}}


// {{{ ldapmessagestore_print_view(host)
function ldapmessagestore_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapmessagestore_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapmessagestore (<?php echo $LANG->_('Default mailbox prefix path')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Path')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["ldapmessagestore"]) { echo $_REQUEST["ldapmessagestore"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; } ?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_LDAPMESSAGESTORE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_LDAPMESSAGESTORE")?>"></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapmessagestore (<?php echo $LANG->_('Default mailbox prefix path')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Path')?></td>
          <td><?php echo pql_format_error_span($error["ldapmessagestore"]); ?><input type="text" name="ldapmessagestore" value="<?php echo $_REQUEST["ldapmessagestore"]?>"></td>
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

// {{{ ldapmessagestore_save(type, host)
function ldapmessagestore_save($type, $host) {
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
									 pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"), '', $_REQUEST["ldapmessagestore"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"), 1, $_REQUEST["ldapmessagestore"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapserver options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapserver options')))
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
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
