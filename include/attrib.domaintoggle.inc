<?php
// Attribute plugin for
// autoCreateUsername and autoCreateMailAddress
// $Id: attrib.domaintoggle.inc,v 2.4 2003-11-24 12:42:33 turbo Exp $

// It's a toggle. Change value directly.
function attribute_save() {
	global $error, $attrib, $$attrib, $domain, $rootdn, $type, $_pql, $view, $LANG;

	if($$attrib)
	  $newval = 'FALSE';
	else
	  $newval = 'TRUE';

	if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $newval))
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
								   array('what' => $attrib, 'domain' => pql_maybe_decode(urldecode($domain))));
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
