<?php
// ----------------------------
// pql_websrv.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for web server configuration)
//
// $Id: pql_websrv.inc,v 1.23 2006-12-02 17:42:59 turbo Exp $
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

// {{{ pql_websrv_get_virtual_host_directories(linkid, server, virtualhost, virthost_dn)
function pql_websrv_get_virtual_host_directories($linkid, $server = '', $virtualhost = '', $virthost_dn = '') {
  // Setup the object class filter
  $filter = '(&(objectClass=ApacheSectionObj)(objectClass=ApacheDirectoryObj))';

  // Retreive the Apache web server virtual host directories
  if(!@$virthost_dn)
	$virthost_dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$virtualhost.",".$server;
  elseif((!@$server or !@$virtualhost) and !@$virthost_dn)
	// Jikes! Serious API problem - called with either server OR virtualhost
	// AND empty virthost_dn!
	die("BUG: pql_websrv_get_virtual_host_directories(linkid, $server, $virtualhost, $virthost_dn)");

  $result = pql_search($linkid, $virthost_dn, $filter, "ONELEVEL");
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

// {{{ pql_websrv_get_data(_pql)
function pql_websrv_get_data($_pql) {
  // {{{ Get all physical hosts
  $hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
					  '(&(cn=*)(|(objectclass=ipHost)(objectclass=device)))',
					  'ONELEVEL');
  // }}}

  // {{{ Check if user is controls admin in any of the root DN's
  $controls_admin = 0;
  foreach($_pql->ldap_basedn as $dn)  {
	$dn = pql_format_normalize_dn($dn);
	if(pql_validate_administrator($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"), $_SESSION["USER_DN"]))
	  $controls_admin = 1;
  }
// }}}

  // {{{ Add a 'Global' branch as first branch
  if($_SESSION["ALLOW_BRANCH_CREATE"] or $controls_admin) {
	// Only do this if:
	//	1. User is super admin
	//	2. User is global controls administrator
	$tmp[] = "Global";
	foreach($hosts as $host)
	  $tmp[] = $host;
	$hosts = $tmp;
  }
  // }}}

  // {{{ For each physical host, find web containers and their virtual hosts
  foreach($hosts as $host_dn) {
	// Get web containers for this physical host
	if($host_dn != 'Global') {
	  $web_containers = pql_get_dn($_pql->ldap_linkid, $host_dn, '(&(cn=*)(objectclass=device))', 'ONELEVEL');
	  if(!$web_containers)
		$web_containers = array();
	} else {
	  $web_containers = array('Global'); // Fake this so that the foreach() won't complain.
	}

	foreach($web_containers as $container_dn) {
	  // Retreive virtual hosts for each web container
	  if($container_dn != 'Global') {
		$tmp = pql_get_dn($_pql->ldap_linkid, $container_dn,
						  '(&(objectClass=ApacheVirtualHostObj)(objectClass=ApacheSectionObj))',
						  'ONELEVEL');
		
		for($i=0; $tmp[$i]; $i++) {
		  $virthost_dn = $tmp[$i];
		  
		  // Get the virtual host name (FQDN)
		  $virthost_name = pql_get_attribute($_pql->ldap_linkid, $virthost_dn, pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME"));
		  
		  $HOSTS[$host_dn][$container_dn][$virthost_dn] = $virthost_name;
		}
	  } else
		$HOSTS['Global']['Global'] = array('Global');
	}
  }
  // }}}

  // {{{ Find out in what domains/branches the user is web admin of
  $branches = array(); $virthost_admin = 0;
  foreach($_pql->ldap_basedn as $dn)  {
	$dn = pql_format_normalize_dn($dn);
	
	$filter = pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV").'='.$_SESSION["USER_DN"];
	$tmp = pql_get_dn($_pql->ldap_linkid, $dn, $filter);
	pql_add2array($branches, $tmp);
  }
  
  // For each branch the user is web admin in, get the default and additional domain name(s).
  $domains = array();
  for($i=0; $branches[$i]; $i++) {
	// Get the default domain name
	$tmp = pql_get_attribute($_pql->ldap_linkid, $branches[$i], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	pql_add2array($domains, $tmp);
	
	// Get the additional domain name(s)
	$tmp = pql_get_attribute($_pql->ldap_linkid, $branches[$i], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
	pql_add2array($domains, $tmp);
  }
// }}}

  // ---------------------------------------------------------------------

  // {{{ Remove everything the user does NOT have access to
  if(is_array($HOSTS)) {
	foreach($HOSTS as $host_dn => $host_data) {
	  $container_admin = 0;
	  foreach($host_data as $container_dn => $virt_hosts) {
		if($host_dn != 'Global') {
		  if(pql_validate_administrator($_pql->ldap_linkid, $container_dn, pql_get_define("PQL_ATTR_UNIQUE_GROUP"), $_SESSION["USER_DN"]))
			$container_admin = 1;
		}

		// Is this user web admin of a domains virtual host(s)?
		foreach($virt_hosts as $virthost_dn => $virthost_name) {
		  // Is this virtual host FQDN part of any of the domain names the user have access to?
		  for($j=0; $domains[$j]; $j++) {
			if(!$_SESSION["ALLOW_BRANCH_CREATE"] and
			   !eregi($domains[$j], $virthost_name) and
			   !$container_admin)
			{
			  // No:
			  //	1. Not super admin
			  //	3. No access to this virtual host
			  unset($HOSTS[$host_dn][$container_dn][$virthost_dn]);
			}
		  }
		}

		// Is there any virtual hosts left in this container?
		if(!count($HOSTS[$host_dn][$container_dn]))
		  unset($HOSTS[$host_dn][$container_dn]);
	  }

	  // Is there any web server containers left in this physical host?
	  if(!count($HOSTS[$host_dn]))
		 unset($HOSTS[$host_dn]);
	}
  }
  // }}}

  // {{{ Find out if we have any values other than 'Global'
  $tmp = $HOSTS;
  unset($tmp["Global"]);
  if(!count($tmp)) {
	// Got to have the physical hosts at least!
	for($i=0; $hosts[$i]; $i++) {
	  $dn = $hosts[$i];
	  $HOSTS[$dn] = '';
	}
  }
  // }}} 

  if(is_array($HOSTS))
	return($HOSTS);
  else
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
