<?php
// Attribute plugin for
// autoCreateUsername and autoCreateMailAddress
// $Id: attrib.domaintoggle.inc,v 2.1 2003-06-13 13:12:10 turbo Exp $

// It's a toggle. Change value directly.
function attribute_save() {
	global $error, $attrib, $$attrib, $domain, $rootdn, $type, $_pql, $view;

	echo "$attrib: ".$$attrib."<br>";
	if($$attrib)
	  $newval = 'FALSE';
	else
	  $newval = 'TRUE';

	if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $newval))
	  $msg = "Successfully toggled $attrib for domain";
	else
	  $msg = "Failed to toggle $attrib for domain: &nbsp;".ldap_error($_pql->ldap_linkid);
	
	attribute_forward($msg);
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
