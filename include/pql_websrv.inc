<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.12 2004-11-15 12:50:35 turbo Exp $
//

// {{{ pql_websrv_add_server(linkid, domain, server)
function pql_websrv_add_server($linkid, $domain, $server) {
	$dn = "ou=WEB,".$domain;

	// ----------------
	// See if the sub branch exists. If not, create it.
	if(!pql_get_dn($linkid, $dn, 'objectClass=organizationalUnit', 'BASE')) {
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'WEB';

		if(!pql_write_add($linkid, $dn, $entry, 'websrv', 'pql_websrv_add_server/1')) {
			// Can't add the organizational unit
			pql_format_error(1);
			die("Can't add the organizational unit leading up to ".$server[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]);
		}
	}

	// ----------------
	$entry = $server;
	$entry["objectclass"][] = pql_get_define("PQL_ATTR_WEBSRV_OC");

	$dn = pql_get_define("PQL_ATTR_CN")."=".$entry[pql_get_define("PQL_ATTR_CN")].",$dn";
	$entry[pql_get_define("PQL_ATTR_CN")] = $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")];

	// Add the server configuration to the database
	if(!pql_write_add($linkid, $dn, $entry, 'websrv', 'pql_websrv_add_server/2')) {
		// failed to add configuration
		pql_format_error(1);
		die("Can't add webserver object ".$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]);
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
