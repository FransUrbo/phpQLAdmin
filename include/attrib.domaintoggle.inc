<?php
// Attribute plugin for
// autoCreateUsername and autoCreateMailAddress
// $Id: attrib.domaintoggle.inc,v 2.8 2004-03-03 07:59:28 turbo Exp $

// It's a toggle. Change value directly.
function attribute_save() {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	if(!empty($_REQUEST[$attrib]))
	  $newval = 'FALSE';
	else
	  $newval = 'TRUE';

	if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, $newval))
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
								   array('what' => $attrib, 'domain' => urldecode(pql_format_urls($_REQUEST["domain"]))));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
	
	attribute_forward(urlencode($msg));
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
