<?php
// Attribute plugin for
// autoCreateUsername, autoCreateMailAddress, autoCreatePassWord,
// simScanSpamAssassin, simScanClamAntiVirus, simScanTrophie,
// useSudo, useAutoMount, lockUserName, lockEmailAddress,
// lockDomainAddress, lockPassword and lockAccountType
// $Id: attrib.domaintoggle.inc,v 2.17 2007-02-15 12:08:06 turbo Exp $

// {{{ attribute_save(void)
// It's a toggle. Change value directly.
function attribute_save() {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	if(!empty($_REQUEST[$attrib]))
	  $newval = 'FALSE';
	else
	  $newval = 'TRUE';

	if(pql_modify_attribute($_REQUEST["domain"], $attrib, 1, $newval))
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
								   array('what' => $attrib, 'domain' => urldecode(pql_format_urls($_REQUEST["domain"]))));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
	
	attribute_forward(urlencode($msg));
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
