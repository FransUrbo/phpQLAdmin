<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.11 2004-11-05 13:44:41 turbo Exp $
//

// {{{ pql_websrv_add_server(linkid, domain, server)
function pql_websrv_add_server($linkid, $domain, $server) {
	$dn = "ou=WEB,".$domain;

	// ----------------
	// See if the sub branch exists. If not, create it.
	if(!pql_get_dn($linkid, $dn, 'objectClass=organizationalUnit', 'BASE')) {
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'WEB';

		// Add the OpenLDAPaci attribute (maybe)
		if($_SESSION["ACI_SUPPORT_ENABLED"])
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'websrv');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_websrv_add_server - ou", $dn, $entry);
		if(file_exists("./.DEBUG_ME")) {
			echo "$LDIF";
		} else {
			if(!ldap_add($linkid, $dn, $entry)) {
				// Can't add the organizational unit
				pql_format_error(1);
				echo "Can't add the organizational unit...<br>\n";
				die($LDIF);
			}
		}
	}

	// ----------------
	unset($entry);
	$entry = $server;
	$entry["objectclass"][] = pql_get_define("PQL_ATTR_WEBSRV_OC");
	$entry[pql_get_define("PQL_ATTR_CN")] = $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")];

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'websrv');

	$dn = pql_get_define("PQL_ATTR_CN")."=".$entry[pql_get_define("PQL_ATTR_CN")].",$dn";

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_websrv_add_server", $dn, $entry);
	if(file_exists("./.DEBUG_ME")) {
		echo "$LDIF";
	} else {
		// Add the server configuration to the database
		if(!ldap_add($linkid, $dn, $entry)) {
			// failed to add configuration
			pql_format_error(1);
			die("$LDIF");
		}
	}

	return true;
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
