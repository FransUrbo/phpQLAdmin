<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.19.2.5 2006-11-24 17:32:07 turbo Exp $
//

// {{{ pql_websrv_add_server(linkid, dn, entry, type)
function pql_websrv_add_server($linkid, $dn, $entry, $type) {
	if($type == "vrtsrv") {
	  // This is a virtual host object
	  $entry[pql_get_define("PQL_ATTR_CN")] = $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")];
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_split_oldvalues(pql_get_define("PQL_CONF_WEBSRV_OC"));
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")] = 'VirtualHost';
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
		pql_format_error(1);
		echo "Can't add webserver object ".$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]."<p>";
		die("$LDIF");
	  }
	  // }}}
	}

	return true;
}
// }}}

// {{{ pql_websrv_find_servers(linkid, dn)
function pql_websrv_find_servers($linkid, $dn) {
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
  if($result["dn"]) {
	// Oups, flat object - i.e. only ONE object
	$tmp = $result;		// Save the array
	unset($result);		// Unset the old array
	$result[] = $tmp;	// Recreate the array, number indexed
  }

  if(is_array($result)) {
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
	
	if(is_array($virt_host_objs)) {
	  ksort($virt_host_objs);
	  return($virt_host_objs);
	}
  } else
	return(NULL);
}
// }}}

// {{{ pql_websrv_get_virtual_host_directories(linkid, server, virtualhost)
function pql_websrv_get_virtual_host_directories($linkid, $server, $virtualhost) {
  // Setup the object class filter
  $filter = '(&(objectClass=ApacheSectionObj)(objectClass=ApacheDirectoryObj))';

  // Retreive the Apache web server virtual host directories
  $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$virtualhost.",".$server;
  $result = pql_search($linkid, $dn, $filter, "ONELEVEL");
  if($result["dn"]) {
	// Oups, flat object - i.e. only ONE object
	$tmp = $result;		// Save the array
	unset($result);		// Unset the old array
	$result[] = $tmp;	// Recreate the array, number indexed
  }

  if(is_array($result)) {
	for($i=0; $i < count($result); $i++) {
	  // Oki, we have webserver virtual host directories in LDAP.
	  $name = $result[$i][pql_get_define("PQL_ATTR_CN")];
	  $virt_host_dirs[$name] = $result[$i];
	}
	
	ksort($virt_host_dirs);
	return($virt_host_dirs);
  } else
	return(NULL);
}
// }}}

// {{{ pql_websrv_get_virtual_host(linkid, virtualhost)
function pql_websrv_get_virtual_host($linkid, $virtualhost) {
  $object = pql_search($linkid, $virtualhost, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE');
  if(is_array($object)) {
	// Remove some crud...
	unset($object[pql_get_define("PQL_ATTR_OBJECTCLASS")]);
	unset($object[pql_get_define("PQL_ATTR_CN")]);
	unset($object["dn"]);

	return($object);
  }

  return(NULL);
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
