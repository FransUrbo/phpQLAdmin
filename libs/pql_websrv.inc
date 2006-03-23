<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.1.2.2 2006-03-23 09:13:17 turbo Exp $
//

// {{{ pql_websrv_add_server(linkid, domain, dn, entry, type)
function pql_websrv_add_server($linkid, $domain, $dn, $entry, $type) {
	// {{{ See if the sub branch exists. If not, create it.
	if(!pql_get_dn($linkid, 'ou=WEB,'.$domain, 'objectClass=organizationalUnit', 'BASE')) {
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'WEB';

		// Add the OpenLDAPaci attribute (maybe)
		if($_SESSION["ACI_SUPPORT_ENABLED"])
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'websrv');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_websrv_add_server - ou", $dn, $entry);
		if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV"))
		  echo $LDIF;
		else {
		  if(!pql_write_add($linkid, $dn, $entry, 'websrv', 'pql_websrv_add_server/1')) {
			// Can't add the organizational unit
			pql_format_error();
			echo "Can't add the organizational unit...<br>\n";
			die($LDIF);
		  }
		}
	}
	// }}}

	if($type == "vrtsrv") {
	  // This is a virtual host object
	  $entry[pql_get_define("PQL_ATTR_CN")] = $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")];
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_split_oldvalues(pql_get_define("PQL_CONF_WEBSRV_OC"));
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")] = 'VirtualHost';
	} elseif($type == "location") {
	  $entry[pql_get_define("PQL_ATTR_CN")] = $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")] = "Location";
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "device";
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "ApacheSectionObj";
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "ApacheDirectoryObj";
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "ApacheModAccessObj";
	} elseif($type == "websrv") {
	  // A server object
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = "device";
	} else {
	  die("pql_websrv_add_server(): Wrong type.");
	}

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_websrv_add_server", $dn, $entry);
	if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV"))
	  echo $LDIF;
	else {
	  // {{{ Add the server configuration to the database
	  if(!pql_write_add($linkid, $dn, $entry, 'websrv', 'pql_websrv_add_server/2')) {
		// failed to add configuration
		pql_format_error();
		echo "Can't add webserver object ".$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]."<p>";
		die("$LDIF");
	  }
	  // }}}
	}

	return true;
}
// }}}

// {{{ pql_websrv_find_servers(linkid, domain)
function pql_websrv_find_servers($linkid, $domain) {
  // Search below the ou=WEB of the domain.
  $dn = "ou=WEB,".$domain;
  
  $temp = pql_get_dn($linkid, $dn, "objectClass=device", "ONELEVEL");
  if(is_array($temp)) {
	foreach($temp as $s) {
	  $servers[] = $s;
	}
	
	if($servers[0]) {
	  // Uniqify the servers array
	  $servers = pql_uniq($servers);
	  
	  // Rearrange the array to be 'indexed' with FQDN
	  unset($temp);
	  foreach($servers as $srv) {
		$tmp = split(',', $srv);
		$tmp = split('=', $tmp[0]);
		
		$fqdn = $tmp[1];
		$temp{$fqdn} = $srv;
		
		if(!$server) {
		  // Choose the first server as view
		  $server = $temp{$fqdn};
		}
	  }
	  
	  unset($servers);
	  $servers = $temp;
	  
	  if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
		$no_servers = count($servers);

		echo "DEBUG (servers=$no_servers):";
		printr($servers);
	  }
	  
	  return($servers);
	}
	
	return(array());
  }
}
// }}}

// {{{ pql_websrv_get_virtual_host_objects(linkid, server)
function pql_websrv_get_virtual_host_objects($linkid, $server) {
  // Setup the object class filter
  $filter = '(&(objectClass=ApacheSectionObj)(objectClass=ApacheVirtualHostObj))';

  // These must be a multileveled array for the pql_format_table_single()
  // below to work!
  $attribs = array(pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY"),
				   pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW"),
				   pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS"));
  
  // Retreive the Apache web server virtual host object(s)
  $result = pql_search($linkid, $server, $filter, "ONELEVEL");
  for($i=0; $i < count($result); $i++) {
	// Oki, we have webserver configuration(s) in LDAP.
	$name = $result[$i][pql_get_define("PQL_ATTR_CN")];
	
	foreach($attribs as $attrib) {
	  if(@$result[$i][$attrib] && !@is_array($result[$i][$attrib])) {
		$tmp = $result[$i][$attrib];
		unset($result[$i][$attrib]);
		
		$result[$i][$attrib][] = $tmp;
	  }
	}
	
	$virt_host_objs[$name] = $result[$i];
  }

  ksort($virt_host_objs);
  return($virt_host_objs);
}
// }}}

// {{{ pql_websrv_get_virtual_host_directories(linkid, server, virtualhost)
function pql_websrv_get_virtual_host_directories($linkid, $server, $virtualhost) {
  // Setup the object class filter
  $filter = '(&(objectClass=ApacheSectionObj)(objectClass=ApacheDirectoryObj))';

  // Retreive the Apache web server virtual host directories
  $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$virtualhost.",".$server;
  $result = pql_search($linkid, $dn, $filter, "ONELEVEL");
  for($i=0; $i < count($result); $i++) {
	// Oki, we have webserver virtual host directories in LDAP.
	$name = $result[$i][pql_get_define("PQL_ATTR_CN")];
	$virt_host_dirs[$name] = $result[$i];
  }

  ksort($virt_host_dirs);
  return($virt_host_dirs);
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
