<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.16 2005-03-14 19:41:10 turbo Exp $
//

// {{{ pql_websrv_add_server(linkid, domain, server)
function pql_websrv_add_server($linkid, $domain, $server) {
	$dn = "ou=WEB,".$domain;

	// {{{ See if the sub branch exists. If not, create it.
	if(!pql_get_dn($linkid, $dn, 'objectClass=organizationalUnit', 'BASE')) {
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'WEB';

		// Add the OpenLDAPaci attribute (maybe)
		if($_SESSION["ACI_SUPPORT_ENABLED"])
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'websrv');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_websrv_add_server - ou", $dn, $entry);
		if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
		  echo $LDIF;
		else {
		  if(!pql_write_add($linkid, $dn, $entry, 'websrv', 'pql_websrv_add_server/1')) {
			// Can't add the organizational unit
			pql_format_error(1);
			echo "Can't add the organizational unit...<br>\n";
			die($LDIF);
		  }
		}
	}
	// }}}

	$entry = $server;
	$dn = pql_get_define("PQL_ATTR_CN")."=".$entry[pql_get_define("PQL_ATTR_CN")].",$dn";
	$entry[pql_get_define("PQL_ATTR_CN")] = $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")];
	$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_split_oldvalues(pql_get_define("PQL_CONF_WEBSRV_OC"));

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_websrv_add_server", $dn, $entry);
	if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
	  echo $LDIF;
	else {
	  // {{{ Add the server configuration to the database
	  if(!pql_write_add($linkid, $dn, $entry, 'websrv', 'pql_websrv_add_server/2')) {
		// failed to add configuration
		pql_format_error(1);
		echo "Can't add webserver object ".$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]."<p>";
		die("$LDIF");
	  }
	  // }}}
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
