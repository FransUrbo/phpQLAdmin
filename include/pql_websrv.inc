<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.8.16.1 2004-12-20 09:02:23 turbo Exp $
//

// {{{ pql_websrv_get_servers(linkid, dn)
function pql_websrv_get_servers($linkid, $dn) {
    $sr = @ldap_search($linkid, $dn, 'objectClass='.pql_get_define("PQL_ATTR_WEBSRV_OC"));
    if($sr) {
		// Oki, we have webserver configuration(s) in LDAP.
		$info = ldap_get_entries($linkid, $sr)
		  or pql_format_error(1);

		$attribs = array(pql_get_define("PQL_ATTR_WEBSRV_SRV_IP"),
						 pql_get_define("PQL_ATTR_WEBSRV_SRV_URL"),
						 pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN"),
						 pql_get_define("PQL_ATTR_WEBSRV_DOCROOT"),
						 pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS"),
						 pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL"),
						 pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH"),
						 pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR"),
						 pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER"),
						 pql_get_define("PQL_ATTR_WEBSRV_OPTIONS"),
						 pql_get_define("PQL_ATTR_CN"));

		for($i=0; $i < $info["count"]; $i++) {
			$name = $info[$i][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")][0];
			$name = ereg_replace("http.*//", "", $name);
			$name = ereg_replace("\/.*$", "", $name);

			foreach($attribs as $attrib) {
				// Remember the DN
				$CONF[$name]["dn"] = $info[$i]["dn"];

				// Remember each attribute stored about this server configuration
				for($j=0; $j < $info[$i][$attrib]["count"]; $j++) {
					if(($attrib == pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")) or
					   ($attrib == pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")))
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
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'websrv');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_websrv_add_server - ou", $dn, $entry);
		if(file_exists("./.DEBUG_ME"))
		  echo $LDIF;
		else {
		  if(! ldap_add($linkid, $dn, $entry)) {
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
	if(file_exists("./.DEBUG_ME"))
	  die($LDIF);

	// Add the server configuration to the database
	if(! ldap_add($linkid, $dn, $entry)) {
	  // failed to add configuration
	  pql_format_error(1);
	  die("$LDIF");
	}

	return true;
}
// }}}

// {{{ pql_websrv_set_record(linkid, dn, record, value)
// This is an exact (!) copy of the pql_bind9_set_record() function
function pql_websrv_set_record($linkid, $dn, $record, $value = '') {
	$dn = urldecode($dn);

	if(!is_array($record)) {
		$record = lc($record);
		$entry[$record] = $value;
	} else
	  // We've been called with an already created 'LDIF'...
	  $entry = $record;
	
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_websrv_set_record", $dn, $entry);

    if(!ldap_mod_replace($linkid, $dn, $entry)) {
		pql_format_error(1);
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
