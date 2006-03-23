<?php
// Attribute plugin for
// autoCreateUsername, autoCreateMailAddress, autoCreatePassWord,
// simScanSpamAssassin, simScanClamAntiVirus, simScanTrophie,
// useHostACL and useSudo
// $Id: attrib.domaintoggle.inc,v 2.12.2.1 2006-03-23 09:13:15 turbo Exp $

// {{{ attribute_save(void)
// It's a toggle. Change value directly.
function attribute_save() {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	if(!empty($_REQUEST[$attrib]))
	  $newval = 'FALSE';
	else
	  $newval = 'TRUE';

	if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, 1, $newval)) {
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
								   array('what' => $attrib, 'domain' => urldecode(pql_format_urls($_REQUEST["domain"]))));

	  $attribs = array(pql_get_define("PQL_ATTR_HOSTACL_USE") => pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"),
					   pql_get_define("PQL_ATTR_SUDO_USE")    => pql_get_define("PQL_CONF_SUBTREE_SUDOERS"));
	  foreach($attribs as $attr => $branch) {
		if(($attrib == $attr) and $branch and ($newval == 'TRUE')) {
		  $dn = $branch.",".$_REQUEST["domain"];
		  if(!pql_get_dn($_pql->ldap_linkid, $dn, 'objectClass=*', 'BASE')) {
			// DN don't exists, create it.
			$tmp = split('=', $branch);
			$ou = $tmp[1];
			
			$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = "organizationalUnit";
			$entry[pql_get_define("PQL_ATTR_OU")] = $ou;
			
			// Add the OpenLDAPaci attribute (maybe)
			if($_SESSION["ACI_SUPPORT_ENABLED"])
			  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'unit');
			
			if(!pql_write_add($_pql->ldap_linkid, $dn, $entry, 'unit', 'attrib.domaintoggle.inc:attribute_save')) {
			  // Can't add the organizational unit
			  pql_format_error();
			  echo "Can't add the organizational unit...<br>\n";
			  die($LDIF);
			}
		  }
		}
	  }
	} else
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
