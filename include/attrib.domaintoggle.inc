<?php
// Attribute plugin for
// autoCreateUsername and autoCreateMailAddress
// $Id: attrib.domaintoggle.inc,v 2.5 2004-02-14 14:25:36 turbo Exp $

// It's a toggle. Change value directly.
function attribute_save() {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	if(!empty($_REQUEST[$attrib]))
	  $newval = 'FALSE';
	else
	  $newval = 'TRUE';

	if(pql_set_domain_value($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, $newval))
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
								   array('what' => $attrib, 'domain' => urldecode(pql_maybe_decode($_REQUEST["domain"]))));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
	
	attribute_forward($msg);
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
