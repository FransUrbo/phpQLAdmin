<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.4.2.1 2003-11-24 18:07:08 dlw Exp $
//

// {{{ pql_websrv_get_servers(linkid, dn)
function pql_websrv_get_servers($linkid, $dn) {
	global $PQL_ATTRIBUTE;

    $sr = @ldap_search($linkid, $dn, 'objectClass=phpQLAdminWebsrv');
    if($sr) {
		// Oki, we have webserver configuration(s) in LDAP.
		$info = ldap_get_entries($linkid, $sr)
		  or pql_format_error_msg($linkid);

		$attribs = array('webServerIP', 'webServerURL', 'webServerAdmin',
						 'webDocumentRoot', 'webServerAlias', 'webScriptAliasURL',
						 'webScriptAliasPath', 'webLogError', 'webLogTransfer',
						 'webOptions', 'cn', 'dn');

		for($i=0; $i < $info["count"]; $i++) {
			$name = $info[$i]["webserverurl"][0];
			$name = ereg_replace("http.*//", "", $name);
			$name = ereg_replace("\/.*$", "", $name);

			foreach($attribs as $attrib) {
				$attrib = lc($attrib); // Force lowercase, it won't work otherwise

				// Remember the DN
				$CONF[$name]["dn"] = $info[$i]["dn"];

				// Remember each attribute stored about this server configuration
				for($j=0; $j < $info[$i][$attrib]["count"]; $j++) {
					if(($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ALIAS"])) or
					   ($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_OPTIONS"])))
					  // Multi value attribute
					  $CONF[$name][$attrib][] = $info[$i][$attrib][$j];
					else
					  // Single value attribute
					  $CONF[$name][$attrib] = $info[$i][$attrib][$j];
				}
			}
		}
	}

	return $CONF;
}
// }}}

// {{{ pql_websrv_add_server(linkid, domain, server)
function pql_websrv_add_server($linkid, $domain, $server) {
	$dn = "ou=WEB,".$domain;

	// ----------------
	// See if the sub branch exists. If not, create it.
    $sr = @ldap_read($linkid, $dn, 'objectClass=organizationalUnit');
    if(!$sr) {
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'WEB';

		// Add the OpenLDAPaci attribute (maybe)
		if($_SESSION["ACI_SUPPORT_ENABLED"])
		  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($linkid,
																				  $GLOBALS["USER_DN"],
																				  'websrv');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_websrv_add_server - ou", $dn, $entry);

		if(! ldap_add($linkid, $dn, $entry)) {
			// Can't add the organizational unit
			pql_format_error_msg($linkid);
			echo "Can't add the organizational unit...<br>\n";
			die($LDIF);
		}
	}

	// ----------------
	unset($entry);
	$entry = $server;
	$entry["objectclass"][] = 'phpQLAdminWebsrv';
	$entry["cn"] = ereg_replace("http.*//", "", $entry["webserverurl"]);
	$entry["cn"] = ereg_replace("\/.*$", "", $entry["cn"]);

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($linkid,
																			  $GLOBALS["USER_DN"],
																			  'websrv');

	$dn = "cn=".$entry["cn"].",$dn";

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_websrv_add_server", $dn, $entry);

    // Add the server configuration to the database
    if(! ldap_add($linkid, $dn, $entry)) {
		// failed to add configuration
		pql_format_error_msg($linkid);
		die("$LDIF");
	}

	return true;
}
// }}}

// {{{ pql_websrv_set_record(linkid, dn, record, value)
// This is an exact (!) copy of the pql_bind9_set_record() function
function pql_websrv_set_record($linkid, $dn, $record, $value = '') {
	if(!is_array($record)) {
		$record = lc($record);
		$entry[$record] = $value;
	} else
	  // We've been called with an already created 'LDIF'...
	  $entry = $record;
	
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_websrv_set_record", $dn, $entry);

    if(!ldap_mod_replace($linkid, $dn, $entry)) {
		pql_format_error_msg($linkid);
		die($LDIF);
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
