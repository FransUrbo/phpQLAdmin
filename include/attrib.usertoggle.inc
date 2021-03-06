<?php
// Attribute plugin for
// senderConfirm and membersOnly
//
// This is the exact same as 'attrib.domaintoggle.inc',
// just applied for users instead of domains...
//
// $Id: attrib.usertoggle.inc,v 2.6 2007-02-15 12:08:11 turbo Exp $

// {{{ attribute_save(void)
// It's a toggle. Change value directly.
function attribute_save() {
    global $_pql, $LANG;
	$success = 0;

	if(!empty($_REQUEST["oldvalue"])) {
		// Delete the value
		if(pql_modify_attribute($_REQUEST["user"], $_REQUEST["attrib"], '', ''))
		  $success = 1;
	} else {
		if(pql_modify_attribute($_REQUEST["user"], $_REQUEST["attrib"], '', 'TRUE'))
		  $success = 1;
	}

	if($success) 
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for user %user%'),
								   array('what' => $_REQUEST["attrib"],
										 'user' => urldecode(pql_format_urls($_REQUEST["user"]))));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $_REQUEST["attrib"]))
		. ": " . ldap_error($_pql->ldap_linkid);

	attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
