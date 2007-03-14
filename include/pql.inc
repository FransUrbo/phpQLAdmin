<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 2.231 2007-03-14 12:10:55 turbo Exp $
//

// ----------------------------
// Get release version
$fp = fopen($_SESSION["path"]."/.version", "r");
$_SESSION["VERSION"] = fgets($fp, 50);
$_SESSION["VERSION"] = rtrim($_SESSION["VERSION"], "\n");
fclose($fp);

// ----------------------------
// turn on output-buffering by default
// http://www.sitepoint.com/print/php-anthology-2-5-caching
if(function_exists("ob_get_level")) {
  // Only availible in PHP >= 4.2.0
  if(ob_get_level() == 0)
	// Only start the buffer if it isn't already there
	ob_start();
} else {
  // Start it anyway...
  ob_start();
}

// {{{ PQL - main class
class pql {
  var $ldap_linkid = 0;					// ldap connection identifier
  var $ldap_error  = 0;					// error message incase of failiure
  var $ldap_basedn = 0;					// array of base dns for future searches etc
  
  // ------------------------------------ PRIVATE FUNCTIONS
  
  // {{{ string _find_base_option(attribute)
  function _find_base_option($attrib) {
	$attrib = lc($attrib);
	
	$sr = @ldap_read($this->ldap_linkid, NULL, '(objectClass=*)', array($attrib));
	if(! $sr) die("Can't find base dn - ".ldap_error($this->ldap_linkid));
	$entry = ldap_get_entries($this->ldap_linkid, $sr);
	
	// Build an array of base dn's. It's possible to have multiple in a database
	for($i=0; $i < $entry["count"]; $i++) {
	  if(is_array(@$entry[$i][$attrib])) {
		for($j=0; $j < $entry[$i][$attrib]["count"]; $j++ )
		  $val[] = urlencode(pql_maybe_encode($entry[$i][$attrib][$j], $attrib, $this->ldap_linkid));
	  } elseif($entry[$i]["dn"]) {
		$val[] = urlencode(pql_maybe_encode($entry[$i]["dn"], $attrib, $this->ldap_linkid));
	  } else
		$val[] = urlencode(pql_maybe_encode($entry[$i][$attrib][0], $attrib, $this->ldap_linkid));
	  
	  $success = 1;
	}
	
	// If we got at least one DN, return the array. Else return false
	if($success) {
	  $new = NULL;
	  
	  // Remove any spaces after the RDN separator
	  foreach($val as $dn) {
		while(eregi("%2C\+", $dn)) {
		  // Replace ', ' with ','
		  $new  = eregi_replace("%2C\+", "%2C", $dn);
		}
	  }
	  
	  if(is_array($new))
		return $new;
	  else
		return $val;
	} else
	  return false;
  }
  // }}}
  
  // {{{ string _find_context_rootdns(void)
  function _find_context_rootdns() {
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* namingContexts
	$dns = $this->_find_base_option("namingContexts");
	for($i=0; $dns[$i]; $i++) {
	  $value = $this->get_attribute($dns[$i], 'structuralObjectClass', 1);
	  if(!eregi('auditContainer', $value)) 
		// I'm using the overlay-accesslog on the 'cn=LOG1'
		// backend - filter that out (if structuralObjectClass=auditContainer,
		// then it should NOT be used)!
		// NOTE: There needs to be anonymous read to the attributes
		//       structuralobjectclass,objectClass and entry of this
		//       dn.base for this to work. Othervise there will be
		//       lots of LDAP search warnings etc...
		//
		// I recommend a ACL like this:
		//
		// access to dn.base="cn=LOG1"
		//        attrs=structuralobjectclass,objectClass,entry
		//        by peername.ip="127\.0\.0\.1" read
		//        by peername.path="/var/run/slapd/ldapi" read
		//        by * none
		$tmp[] = $dns[$i];
	  else {
		global $_SESSION;
		if(!$_SESSION["ACCESSLOG_OVERLAY"] or !in_array($dns[$i], $_SESSION["ACCESSLOG_OVERLAY"]))
		  $_SESSION["ACCESSLOG_OVERLAY"][] = $dns[$i];
	  }
	}

	return $tmp;
  }
  // }}}
  
  // {{{ string _find_context_config(void)
  function _find_context_config() {
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* configContext
	global $_SESSION;
	$context = $this->_find_base_option("configContext");
	
	if($context[0] and !$_SESSION["CONFIG_BACKEND_ENABLED"])
	  $_SESSION["CONFIG_BACKEND_ENABLED"] = 1;

	if($context[0])
	  return(urldecode($context[0]));
	else
	  return(false);
  }
  // }}}
  
  // {{{ string _find_context_monitor(void)
  function _find_context_monitor() {
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* monitorContext
	global $_SESSION;
	$context = $this->_find_base_option("monitorContext");
	
	if($context[0] and !$_SESSION["MONITOR_BACKEND_ENABLED"])
	  $_SESSION["MONITOR_BACKEND_ENABLED"] = 1;
	
	if($context[0])
	  return(urldecode($context[0]));
	else
	  return(false);
  }
  // }}}
  
  // {{{ string _find_context_subschema(void)
  function _find_context_subschema() {
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* subschemaSubentry
	$context = $this->_find_base_option("subschemaSubentry");
	
	if($context[0])
	  return(urldecode($context[0]));
	else
	  return(false);
  }
  // }}}
  
  // {{{ bool _set_options() {
  function _set_options() {
	// LDAP_OPT_REFERRALS
	//	Automatically follow referrals returned by LDAP server?
	//	LDAP_OPT_OFF			 0
	//	LDAP_OPT_ON				!0 (default)
	//
	// NOTE: Enabling this if the LDAP server is an AD is a bad idea
	ldap_set_option($this->ldap_linkid, LDAP_OPT_REFERRALS,	0);
	
	// LDAP_OPT_DEREF
	//	Deterime how aliases are handled during search.
	//	LDAP_DEREF_NEVER		 0 (default)
	//	LDAP_DEREF_SEARCHING	 1
	//	LDAP_DEREF_FINDING		 2
	//	LDAP_DEREF_ALWAYS		 3
	ldap_set_option($this->ldap_linkid, LDAP_OPT_DEREF,     0);
	
	if($this->ldap_linkid) {
	  // Start with a LDAPv3 bind
	  if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 3)) {
		// Didn't work. Try a LDAPv2 bind...
		echo "Don't support v3 bind, trying to do a v2 bind.<br>";
		
		if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 2))
		  echo "Neither protocol LDAPv3 nor LDAPv2 worked, strange!<p>LDAP Error: ".ldap_error($this->ldap_linkid);
		
		return false;
	  }
	  
	  // We're using LDAPv3, so try a TLS initialization.
	  // Only availible in PHP v4.2.0, hence tripple check
	  // that that function is availible before trying to
	  // use it.
	  if(function_exists("ldap_start_tls") and pql_get_define("PQL_CONF_USE_TLS")) {
		if(! ldap_start_tls($this->ldap_linkid))
		  echo "TLS initialization failed: ".ldap_error($this->ldap_linkid);
	  }
	  
	  return true;
	}
  }
  // }}}
  
  // ------------------------------------ PUBLIC FUNCTIONS
  
  // {{{ bool pql(hostname, binddn, bindpw, noconnect, dodie)
  function pql($host, $binddn = "", $bindpw = "", $no_connect = false, $do_die = 1) {
	// constructor, connect to ldap host
	if($no_connect == true)
	  return false;
	
	// Open a connection to the host
	$this->connect($host) or die("<b>could not connect to ldap server</b>");
	
	// Setup connection (set options etc)
	$this->_set_options($this->ldap_linkid);
	
	// It's possible to be called without binddn, but with a password.
	// Can't use the password in that case...
	if(!$binddn && $bindpw)
	  unset($bindpw);
	
	// Bind to the host
	if(!$this->bind($binddn, $bindpw)) {
	  $this->ldap_error = "<b>Could not bind to ldap server</b>: ";
	  $this->ldap_error = $this->ldap_error . ldap_error($this->ldap_linkid) . "<p>" .
		"Host: $host, Port: $port<br>" .
		"Please <a href=\"index.php?logout=1\" target=\"_top\">relogin</a>.<p>" .
		"binddn='$binddn', bindpw='$bindpw'.";
	  
	  if($do_die)
		die($this->ldap_error);
	  
	  return false;
	}
	
	// Find the base DN(s)
	$this->ldap_basedn = $this->_find_context_rootdns();

	// Find the config context (only OpenLDAP >=2.3)
	$this->ldap_config = $this->_find_context_config();
	
	// Find the monitor context
	$this->ldap_monitor = $this->_find_context_monitor();
	
	// Find the subschema context
	$this->ldap_subschema = $this->_find_context_subschema();

	return true;
  }
  // }}}
  
  // {{{ bool connect(hostname)
  function connect($host) {
	// Check to see if we have a port defined in the host.
	if(ereg(";", $host)) {
	  $ldap = split(";", $host);
	  
	  $host = $ldap[0];
	  $port = $ldap[1];
	} else {
	  $host = $host;
	  $port = 389;
	}
	
	// connect to ldap host and bind to root_dn
	if($port)
	  $this->ldap_linkid = ldap_connect($host, $port);
	else
	  $this->ldap_linkid = ldap_connect($host);
	
	return true;
  }
  // }}}
  
  // {{{ bool bind(binddn, bindpw)
  function bind($binddn = "", $bindpw = "") {
	// If we haven't set the DN/PW -> do a anonymous bind
	if($this->ldap_linkid and @ldap_bind($this->ldap_linkid, $binddn, $bindpw)) {
	  return true;
	}
	
	return false;
  }
  // }}}
  
  // {{{ int close(void)
  function close(){
	// close connection to ldap host
	@ldap_unbind($this->ldap_linkid);
	@ldap_close($$this->ldap_linkid);
  }
  // }}}
  
  // {{{ bool connected(void)
  function connected() {
	if($this->ldap_linkid != false)
	  return true;
	return false;
  }
  // }}}
  
  // {{{ string find_ldap_version(void)
  function find_ldap_version() {
	$sr = ldap_read($this->ldap_linkid, $this->ldap_monitor, 'objectClass=*', array("description", "monitoredInfo"));
	if($sr) {
	  $entry = ldap_get_entries($this->ldap_linkid, $sr);
	  
	  if(ereg('\$', $entry[0]['monitoredinfo'][0])) {
		// Older OpenLDAP...
		$tmp = split('\$', $entry[0]['monitoredinfo'][0]);
		$tmp = split(' \(', $tmp[1]);
		$version = $tmp[0];
	  } else {
		// Newer OpenLDAP (tested: 2.3.32)
		$tmp = split(' ', $entry[0]['monitoredinfo'][0]);
		$version = $tmp[2];
	  }
	  
	  if($version)
		return($version);
	  else
		return(false);
	} else
	  return(false);
  }
  // }}}
  
  // ------------------------------------ READ FUNCTIONS
  
  // {{{ array search(dn, filter, level, referrals, operational, encode)
  // Lock for object(s) which match the filter.
  // Level can be:
  //		SUBTREE		=> default!
  //		ONELEVEL
  //		BASE
  function search($dn, $filter, $level = "SUBTREE", $referrals = 0, $operational = 0, $encode = 0) {
	global $_SESSION, $pql_cache;
	
	// Normalize the DN
	$dn = pql_format_normalize_dn($dn);
	
	// {{{ Convert a DNS TTL RDN to something I (phpQLAdmin) can understand
	if(eregi("^dNSTTL=", $dn))
	  // FROM: dNSTTL=3600 relativeDomainName=@,dc=domain,dc=tld,PQL_CONF_SUBTREE_BIND9,o=Branch,c=SE
	  // TO:   dNSTTL=3600+relativeDomainName=@,dc=domain,dc=tld,PQL_CONF_SUBTREE_BIND9,o=Branch,c=SE
	  $dn = pql_recreate_dnsttl($dn);
	// }}}
	
	// {{{ Which ldap_{search,list,read}() function to use and should cache be used?
	switch($level) {
	case "SUBTREE":		$func = "ldap_search";	break;
	case "ONELEVEL":	$func = "ldap_list";	break;
	case "BASE":		$func = "ldap_read";	break;
	default:			die("Faulty search scope '$level'! Please report this at the bugtracker.");
	}
	
	$subschema_dn = 0; // Can't be NULL (''). Then the 'check if enable cache' don't work...
	if($operational) {
	  // This works with OpenLDAP at least!!
	  $op = array('+');
	  
	  // If '$operational' != integer then it's the subschema DN
	  // This is done in only ONE place: pql_get_subschema().
	  // We need this to know if we should cache the schemas (which
	  // we should - it doesn't have any non-operational attributes
	  // of interest).
	  if(!is_numeric($operational)) {
		$subschema_dn = $operational;
		$operational = 1;
	  }
	} else
	  $op = array();
	
	$enable_cache = 0;
	if(($level == "BASE") and
	   (!$operational or
		((lc($dn) == lc($subschema_dn)) and ($level == 'BASE'))) and
	   @$_SESSION["USER_DN"])
	  // Check for/retreive cached object if:
	  // 1. level == BASE
	  // 2. not operational
	  // 3. looking (BASE) in 'cn=Subschema' (or whatever subschemaSubentry points to in base NULL).
	  // 4. We're logged in!
	  $enable_cache = 1;
	// }}}
	
	// {{{ Try to find object in the cache
	if(file_exists($_SESSION["path"]."/.DEBUG_CACHE") and !file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE")) {
	  echo "<br>/<b>--------------------</b><br>";
	  if($level == 'BASE')
		echo "|search(): Looking for '<b>$dn</b>': ";
	  else
		echo "|search(): Looking below '<b>$dn</b>':<br>";
	}
	
	if($enable_cache and !(($object = $pql_cache->get($dn, $object)) != true)) {
	  // The object is cached -> return this (don't look for it in LDAP).
	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE") and !file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE")) {
		echo "|Returning cached object '$dn'<br>";
		echo "\<b>--------------------</b><p>";
// Disable this - to much data!
//		printr($object);
	  } 
	  
	  return($object);
	} // not BASE, is operational or not cached => fall through.
	// }}}
	
	// {{{ Not found in cache, retreive from LDAP
	// Debugging for what is about to happen
	if(file_exists($_SESSION["path"]."/.DEBUG_CACHE") and !file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE")) {
	  echo "|Searching with: <b>$func(linkid, '$dn', '$filter', ";
	  if(is_array($op)) {
		echo "array(";
		for($i=0; $op[$i]; $i++) {
		  echo $op[$i];
		  if($op[$i+1])
			echo ", ";
		}
		echo ")";
	  } else
		echo "$op";
	  echo ")</b><br>";
	}	

	$sr = @$func($this->ldap_linkid, $dn, $filter, $op);
	if(!$sr) {
	  // No such object - problem with the search string?
	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE") and !file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE")) {
		echo "|No such object - problem with the search string ('$filter')?<br>";
		echo "\<b>--------------------</b><p>";
	  }

	  return false;
	}

	// Get number of objects found.
	$nr_entries = ldap_count_entries($this->ldap_linkid, $sr);
	if(!$nr_entries) {
	  // No objects found.
	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE") and !file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE")) {
		echo "|No objects found!<br>";
		echo "\<b>--------------------</b><p>";
	  }

	  return false;
	}
	// }}}
	
	// {{{ Get ALL hits and rearrange the array.
	// Initialize the resulting array.
	$entry = array();
	
	$entries = 0; // Counter to keep tab on how many entries to return
	$object = ldap_first_entry($this->ldap_linkid, $sr) or pql_format_error(1);
	do {
	  // Get the DN of the object
	  $dn = ldap_get_dn($this->ldap_linkid, $object);
	  
	  // Get all attributes in this object
	  $attributes = ldap_get_attributes($this->ldap_linkid, $object);
	  
	  // Go through all attributes in the object and retreive
	  // the value(s). Ignore the 'count' part and values with
	  // a number (remnants from the ldap_get_attributes() call)..
	  for($i=0; $i < $attributes['count']; $i++) {
		$key   = lc($attributes[$i]); // Save some typing
		$value = ldap_get_values_len($this->ldap_linkid, $object, $attributes[$i]);
		
		if(eregi('^[a-z]', $key) and ($key != 'count')) {
		  if($key == pql_get_define("PQL_ATTR_PHOTO")) {
			// A jpegPhoto. Save it in the resulting array as a base64 encoded value.
			if($nr_entries > 1)
			  $entry[$entries][$key] = base64_encode($value[0]);
			else
			  $entry[$key] = base64_encode($value[0]);
		  } elseif(is_array($value) and !empty($value[1])) {
			// A multi value - such as 'mailAlternateAddress'
			// or 'objectClass' etc. Take ALL values, ignoring
			// any empty values that might exists.
			for($j=0; $j < $value["count"]; $j++) {
			  if($nr_entries > 1) {
				if($encode)
				  $entry[$entries][$key][$j] = $value[$j];
				else
				  $entry[$entries][$key][$j] = pql_maybe_decode($value[$j]);
			  } else {
				if($encode)
				  $entry[$key][$j] = $value[$j];
				else
				  $entry[$key][$j] = pql_maybe_decode($value[$j]);
			  }
			}
		  } else
			// Single valued such as 'uid' and 'mail' etc...
			// Just take the ONE value
			if($nr_entries > 1) {
			  if($encode)
				$entry[$entries][$key] = $value[0];
			  else
				$entry[$entries][$key] = pql_maybe_decode($value[0]);
			} else {
			  if($encode)
				$entry[$key] = $value[0];
			  else
				$entry[$key] = pql_maybe_decode($value[0]);
			}
		}
	  }
	  
	  // Save the DN in the resulting array.
	  if($nr_entries > 1)
		$entry[$entries]["dn"] = $dn;
	  else
		$entry["dn"] = $dn;
	  
	  // -------------
	  if($enable_cache and ($attributes['count'] > 1)) {
		// Cache this object
		if($nr_entries > 1)
		  $pql_cache->add($entry["dn"], $entry[$entries]);
		else
		  $pql_cache->add($entry["dn"], $entry);
	  }

	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE") and !file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
		echo "\<b>--------------------</b><p>";
	  
	  $entries++;
	} while($object = ldap_next_entry($this->ldap_linkid, $object));
	// }}}
	
	// {{{ Chase referals
	// TODO: Finish this, it's taken directly from (the now deleted) function pql_user_get().
	//	if($referrals) {
	//		// Get any referrals in this branch
	//		$resource = ldap_first_reference($this->ldap_linkid, $sr);
	//		if($resource) {
	//			ldap_parse_reference($this->ldap_linkid, $resource, $referrals);
	//			
	//			while($resource = ldap_next_reference($this->ldap_linkid, $resource)) {
	//				if($resource) {
	//					ldap_parse_reference($this->ldap_linkid, $resource, $new);
	//					$referrals = array_merge($referrals, $new);
	//				}
	//			}
	//		}
	//		
	//		// Add all referral references to the user array
	//		if(is_array($referrals)) {
	//			$host = split(';', $_SESSION["USER_HOST"]);
	//			
	//			foreach($referrals as $ref) {
	//				// Remove the LDAP host from the reference.
	//				// TODO: What if it's a referral to outside this host!?!?
	//				$reference = eregi_replace($host[0]."/", "", $ref);
	//				
	//				// Store the URL DEcoded value in the array.
	//				$users[] = urldecode($reference);
	//			}
	//		}
	//	}
	// }}}
	
	if(!empty($entry))
	  return($entry);
	else
	  return false;
  }
  // }}}
  
  // {{{ array get_attribute(dn, attribute, operational, filter)
  // Return only a subset of an object (retreived with search()).
  function get_attribute($dn, $attribute, $operational = 0, $filter = NULL) {
	$attribute = lc($attribute);
	
	if(!$filter) {
	  $filter = "(".pql_get_define("PQL_ATTR_OBJECTCLASS")."=*)";
	  $scope = "BASE";
	} else {
	  $scope = "SUBTREE";
	}
	
	// Get the (WHOLE) object but only return the attributes of interest.
	$object = $this->search($dn, $filter, $scope, 1, $operational);
	if(!empty($object)) {
	  if(!empty($object[0])) {
		if(!empty($object[0][$attribute]))
		  return($object[0][$attribute]);
	  } elseif(!empty($object[$attribute]))
		// Most likley returned from a cached object...
		return($object[$attribute]);
	}
	
	return(false);
  }
  // }}}
  
  // {{{ array get_dn(dn, filter, level)
  // This function returns the DN of each object if finds.
  // Basically a simplified version of search() - one that only returns
  // the DN's of the matches...
  function get_dn($dn, $filter, $level = 'SUBTREE') {
	// NOTE:
	// This function is VERY UNLIKLEY to do a ONE level search,
	// so I'm not going to bother to check for cached objects.
	// Besides, search() is quite clever about that...
	
	$objects = $this->search($dn, $filter, $level, 1);
	if(!empty($objects[0])) {
	  for($i=0; $i < count($objects); $i++)
		$entry[] = pql_maybe_idna_decode(pql_format_normalize_dn($objects[$i]['dn']));
	} elseif($objects and $objects['dn'])
	  $entry[] = pql_maybe_idna_decode(pql_format_normalize_dn($objects['dn']));
	
	if(@is_array($entry))
	  return($entry);
	else
	  return false;
  }
  // }}}
  
  // ------------------------------------ WRITE FUNCTIONS
  
  // {{{ bool add(dn, entry, type, caller)
  // ADD an object - called with an 'ldif' for modification.
  function add($dn, $entry, $type, $caller) {
	global $pql_cache;

	if(!eregi("^dNSTTL=", $dn))
	  $dn = pql_maybe_encode(urldecode($dn));
	
	// Verify that the object have what it needs to be able to be added
	$entry = pql_validate_object($dn, $entry, $type);

	// If there's ACI's and we're running OpenLDAP >2.3, then make sure
	// the ACI's in the correct form (OpenLDAP changed the syntax in 2.3).
	if(@is_array($entry[pql_get_define("PQL_ATTR_LDAPACI")]) and
	   ereg('^2\.3', $this->find_ldap_version()))
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = pql_validate_acis($entry[pql_get_define("PQL_ATTR_LDAPACI")]);
	
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif($caller, $dn, $entry);
	if(pql_get_define("PQL_CONF_DEBUG_ME"))
	  echo $LDIF;
	else {
	  // Add this object to the database
	  if(@$entry["dn"])
		unset($entry["dn"]);
	  
	  if(!ldap_add($this->ldap_linkid, $dn, $entry)) {
		// Failed!
		
		// TODO: Error checking. Some problems might be salvable...
		$errno = ldap_errno($this->ldap_linkid);
		switch($errno) {
		case 68:
		  // Already exists. Fine, return true (?)
		  // TODO: Maybe offer to merge the two?
		  return(true);
		  break;
		}
		
		pql_format_error(1);
		echo $LDIF;
		return(false);
	  }
	  
	  // Cache this object
	  $pql_cache->add($dn, $entry);
	}
	
	pql_flush();
	return(true);
  }
  // }}}
  
  // {{{ bool modify(dn, entry, caller)
  // MODIFY an object - called with an 'ldif' for modification.
  function modify($dn, $entry, $caller = NULL, $count = 0) {
	global $pql_cache;
	
	$dn = urldecode($dn);
	if(eregi("^dNSTTL=", $dn))
	  $dn = pql_recreate_dnsttl($dn);
	$dn = pql_maybe_encode($dn);
	
	$count++;
	
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif($caller, $dn, $entry);
	if(pql_get_define("PQL_CONF_DEBUG_ME") and !$entry[pql_get_define("PQL_ATTR_DEBUG_ME")])
	  echo $LDIF;
	else {
	  // Modify this object
	  if(!ldap_mod_replace($this->ldap_linkid, $dn, $entry)) {
		// Modify failed. Why? Can't support every single
		// failure, but some I CAN rectify (I think :).
		
		// TODO: In some cases I've ldap_mod_del() the attribute
		//       and then ldap_mod_add() it...
		
		$errno = ldap_errno($this->ldap_linkid);
		switch($errno) {
		case "65":
		  // Object class violation - try to add (the) missing objectclass(es).
		  
		  // Get the current object classes in this object
		  $old = $this->get_attribute($dn, pql_get_define("PQL_ATTR_OBJECTCLASS"));
		  if($old and !is_array($old))
			$old = array($old);
		  
		  // Try to find which object classes to ADD.
		  $new = pql_missing_objectclasses($this->ldap_linkid, $dn, $entry);
		  if($new and !is_array($new))
			$new = array($new);
		  
		  // Combine the old with the new.
		  pql_add2array($old, $new);
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_uniq($old);
		  
		  // Recursion... Only try five times...
		  if($count >= 6) { // The very first time $count=1, so...
			$msg = "<p>We're looping in pql.inc:(pql class):modify()";
			if($caller)
			  $msg .= " (from $caller)";
			$msg .= ". No point in trying again. I've got errno=65 (Object class violation) every time...<br>";
			$msg .= "This is the LDIF I'm trying to write to the LDAP server. Please report this problem at the ";
			$msg .= '<a href="http://bugs.bayour.com/" target="_new">bugtracker</a>.';
			
			// Must die() here, not return due to the fact we're recursively called..
			die("$msg<p>".$LDIF);
		  } else
			// Recusive call...
			$this->modify($dn, $entry, $caller, $count);
		  break;
		  
		  //case "17": // Undefined attribute type
		  //	break;
		  //
		  // Found these in ldap.h. Don't seem to be correct, or the ldap API have
		  // changed the return values - '41' is oc violation!
		  //		  case "20": // LDAP_NO_SUCH_OBJECT
		  //		  case "22": // LDAP_INVALID_DN_SYNTAX
		  //		  case "31": // LDAP_INVALID_CREDENTIALS
		  //		  case "32": // LDAP_INSUFFICIENT_ACCESS
		  //		  case "41": // LDAP_OBJECT_CLASS_VIOLATION
		  //			break;
		  
		default:
		  pql_format_error(1);
		  echo $LDIF;
		  return(false);
		}
	  }
	  
	  // Remove this cached object so it's loaded fresh next time around.
	  $pql_cache->del($dn);
	}
	
	return(true);
  }
  // }}}
  
  // {{{ bool rename(old, parent, new, keepold)
  function rename($old, $parent, $new, $keepold = TRUE) {
	global $pql_cache;
	
	// A modrdn LDIF looks a little different...
	$LDIF  = "<pre>\n--- DEBUG: ldap_rename() ---\n";
	$LDIF .= "$old\n$new\n";
	$LDIF .= "--- DEBUG ---\n</pre>\n";
	
	if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	  echo "Renaming DN: '<b>$old</b>' to '<b>$new,$parent</b>'<br>";
	  echo $LDIF;
	  return true;
	} else {
	  $old = pql_maybe_encode($old);
	  $new = pql_maybe_encode($new);
	  $parent = pql_maybe_encode($parent);
	  
	  if(ldap_rename($this->ldap_linkid, $old, $new, $parent, $keepold)) {
		if(!$keepold)
		  // Remove this cached object so it's loaded fresh next time around.
		  $pql_cache->del($dn);
		
		return true;
	  } else {
		pql_format_error(1);
		echo $LDIF;
	  }
	}
	
	return false;
  }
  // }}}
  
  // {{{ bool delete(dn)
  function delete($dn) {
	global $pql_cache;
	
	if(pql_get_define("PQL_CONF_DEBUG_ME"))
	  echo "Removing DN: '<b>$dn</b>'<br>";
	else {
	  if(!ldap_delete($this->ldap_linkid, $dn)) {
		pql_format_error(1);
		return false;
	  }
	}
	
	// Remove this cached object.
	$pql_cache->del($dn);
	
	return true;
  }
  // }}}
}
// }}}

// ------------------------------------------------
// API functions - support
// ------------------------------------------------

// {{{ pql_user_get_quota(user)
// Get formated quota of a user
function pql_user_get_quota($user) {
  global $_pql, $_SESSION;
  
  if($_SESSION["NEW_STYLE_QUOTA"]) {
	$quota_size  = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_QUOTA_SIZE"));
	$quota_count = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_QUOTA_COUNT"));
	
	if($quota_size)
	  $quota   = $quota_size[0]."S";
	else
	  $quota   = '0S';

	if($quota_count)
	  $quota  .= ",".$quota_count[0]."C";
	else
	  $quota  .= ',0C';
  
	if(!$quota)
	  // Just incase we haven't the new style quota in the DB, get the old type
	  $quota = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));
	else
	  $quota = array($quota);
  } else
	// We're not using new style quota, get the old type...
	$quota = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));
  
  if(!is_array($quota))
	return false;
  else
	return pql_parse_quota($quota[0]);
}
// }}}

// {{{ pql_user_get_letter(domain, letra)
// Based in PQL_CONF_GET_USER to search by the cn first letter
function pql_user_get_letter($domain, $letra) {
  global $_pql;

  $filter = "(".pql_get_define("PQL_ATTR_UID")."=".$letra."*)";
  $user = $_pql->search($domain, $filter);
  return $user;
}
// }}}

// {{{ pql_user_get_number(domain)
// Based in PQL_CONF_GET_USER to search by an initial number at cn attribute
function pql_user_get_number($domain) {
  global $_pql;

  // What's the Root DN (namingContexts) for this domain
  $rootdn = pql_get_rootdn($dn, 'pql_user_get_number'); $rootdn = urldecode($rootdn);
  
  for($numero=0; $numero <= 9; $numero++) {
	$filter = "(".pql_get_define("PQL_ATTR_UID")."=".$numero."*)";
	$tmp = $_pql->search($domain, $filter);
	
	if($user)
	  $user = $user + $tmp;
  }
  
  return $user;
}
// }}}

// This functions isn't used any where - yet?
// {{{ pql_user_modify_addressbook(user, attribs)
function pql_user_modify_addressbook($user, $attribs) {
  global $_pql;

  // What's the Root DN (namingContexts) of this user
  $rootdn = pql_get_rootdn($user, 'pql_user_modify_addressbook'); $rootdn = urldecode($rootdn);
  
  // check if addressbook is disabled
  if(!pql_get_define("PQL_CONF_USE_ADDRESSBOOK", $rootdn))
	return false;
  
  // object-class dependencies for addressbook entries
  //
  // ------------------------------------------------------------------------------------
  // | OBJECTCLASS          | Must-Attr. (OpenLDAP 1.2.x) | Must-Attr. (OpenLDAP 2.0.x) |
  // ------------------------------------------------------------------------------------
  // | InetOrgPerson        | none                        | none                        |
  // | organizationalPerson | sn, cn                      | none                        |
  // | country              | c                           | c                           |
  // | organization         | o                           | o                           |
  // | pilotObject          | none                        | none                        |
  // ------------------------------------------------------------------------------------
  //
  // the 'organization' and 'country' objectclass can only be set when an 'o' or a 'c'
  // attribute is given. The other objectclasses will be added by default because the
  // haven't a mandatory attribute
  
  // fetch current registred objectclasses of user
  $oc_user = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_OBJECTCLASS"));
  
  // set all objectclasses to lowercase
  foreach($oc_user as $oc1){
	$oc[] = lc($oc1);
  }
  
  // add addressbook's default object classes if they are not present
  $default_oc = array("inetorgperson", "organizationalperson", "pilotobject");
  
  foreach($default_oc as $oc1){
	if(!in_array($oc1, $oc)){
	  //$oc[] = $oc1;
	}
  }
  
  // check for object class dependencies
  echo var_dump($oc);
}
// }}}

// {{{ pql_get_next_ugidnumber(type)
// Returns the next free UID/GID number
function pql_get_next_ugidnumber($type, $rootdn) {
  global $_pql;

  // Get the minimum allowable ID.
  if($type == 'uid') {
	$attrib  = pql_get_define("PQL_ATTR_QMAILUID");
	$minimum = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $rootdn);
  } else {
	$attrib = pql_get_define("PQL_ATTR_QMAILGID");
	$minimum = pql_get_define("PQL_CONF_MINIMUM_GIDNUMBER", $rootdn);
  }
  
  // Just incase it's not defined, we set the minimum UID/GID
  // number availible to a safe value.
  if($minimum && !is_array($minimum))
	$minimum = array($minimum);
  elseif(!$minimum)
	$minimum = array(100);
  
  $min_id_val = 100;			// Set an absolute min so no one becomes root.
  foreach ($minimum as $min) {
	if ($min > $min_id_val) {
	  $min_id_val = $min;
	}
  }
  
  // Make a list of all the already used IDs.
  $filter = "(&($attrib=*)(".pql_get_define("PQL_ATTR_OBJECTCLASS")."=posixaccount))";
  
  $ids = array();
  foreach($_SESSION["BASE_DN"] as $dn) {
	$results = $_pql->search($dn, $filter);
	for($i=0; $i < count($results); $i++) {
	  if ($results[$i][$attrib] >= $min_id_val) {
		$ids[] = $results[$i][$attrib];
	  }
	}
  }
  
  // Look for a hole in the ID list.
  $found_id = null;
  if (empty($ids))
	$found_id = $min_id_val;
  else {
	sort($ids, SORT_NUMERIC);
	$prev_id = $ids[0];
	foreach ($ids as $id) {
	  if ($id > $prev_id + 1) {
		$found_id = $prev_id + 1;
		break;
	  }

	  $prev_id = $id;
	}
	
	if (!isset($found_id))
	  $found_id = end($ids) + 1;
  }
  
  return $found_id;
}
// }}}

// {{{ pql_get_next_username(domain)
// Returns the next free UID/GID number
function pql_get_next_username($domain) {
  global $_pql;

  // Get the prefix from the branch
  $prefix = $_pql->get_attribute($domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX"));
  $suffix_length = $_pql->get_attribute($domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"));
  
  if(!$suffix_length)
	$suffix_length = "%04d";
  else
	$suffix_length = "%0".$suffix_length."d";
  
  if($prefix) {
	// Under what namingContexts is this domain/branch located?
	$rootdn = pql_get_rootdn($domain, 'pql_get_next_username');
	$rootdn = urldecode($rootdn);
	
	// Setup the LDAP search filter
	$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . '=' . $prefix . '*';
	
	// Search the whole database for all users with this prefix
	foreach($_SESSION["BASE_DN"] as $dn) {
	  $sr  = ldap_search($_pql->ldap_linkid, $dn, $filter, array(pql_get_define("PQL_ATTR_UID")));
	  $tmp = ldap_get_entries($_pql->ldap_linkid, $sr) or pql_format_error(1);
	  for($i=0; $i < $tmp["count"]; $i++)
		$USERS[] = $tmp[$i][pql_get_define("PQL_ATTR_UID")][0];
	}
	
	if(empty($USERS))
	  $suffix = sprintf("$suffix_length", 1);
	else {
	  // We have previous values in the DB. 
	  sort($USERS);
	  $nr  = eregi_replace($prefix, "", $USERS[count($USERS)-1]);
	  
	  $suffix = sprintf("$suffix_length", $nr + 1);
	}
	
	return($prefix.$suffix);
  }
}
// }}}

// {{{ pql_get_valid_shells()
// Load list of allowed shells from /etc/shells
function pql_get_valid_shells() {
  $fp = fopen("/etc/shells", "r");
  while (!feof ($fp)) {
	$buffer = fgets($fp, 4096);
	$shell = split(" ", $buffer);
	
	if(!eregi("^#", $shell[0]) and !eregi("^$", $shell[0]))
	  $shells[] = rtrim($shell[0]);
  }
  fclose ($fp);
  asort($shells);
  
  return $shells;
}
// }}}

// {{{ pql_get_mx(domainname)
function pql_get_mx($domainname) {
  $mx = array();
  
  $res = getmxrr($domainname, $rec, $weight);
  if(count($rec) > 0) {
	// Take the MX with _LOWEST_ priority/weight.
	asort($weight); $old_prio = 65555;
	foreach($weight as $key => $prio) {
	  if($prio < $old_prio) {
		$old_prio = $prio;
		$prio_key = $key;
	  }
	}

	$mx["dns"] = $rec[$prio_key];
  }
  
  // It's possible that we have a QmailLDAP/Controls object as well...
  if(pql_get_define("PQL_CONF_CONTROL_USE") and class_exists("pql_control")) {
	// Look for a qmailControl object which lists this domain...
	
	// Initiate a connection to the QmailLDAP/Controls DN
	$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
	if($_pql_control->ldap_linkid) {
	  $filter = "(&(objectclass=qmailControl)(".pql_get_define("PQL_ATTR_LOCALS")."=$domainname))";
	  $info = $_pql_control->search($_SESSION["USER_SEARCH_DN_CTR"], $filter);
	  for($i=0; $i < count($info); $i++) {
		if($info[$i]["cn"]) {
		  if(is_array($info[$i]["cn"]))
			$mx["qlc"][] = $info[$i]["cn"][0];
		  else
			$mx["qlc"][] = $info[$i]["cn"];
		}
	  }
	  
	  // Make sure we don't use it any more than we have to...
	  ldap_close($_pql_control->ldap_linkid);
	}
  }
  
  return($mx);
}
// }}}

// {{{ smbpassword(passwd, type)
function smbpassword($passwd, $type){
  // Using Samba Password Support requires pear's Crypt_CHAP
  require_once 'Crypt/CHAP.php';

  $hash = &new Crypt_CHAP_MSv2();
  $hash->password = $passwd;
  if($type == 'lmPassword')
	$passwd = strtoupper(bin2hex($hash->lmPasswordHash()));
  elseif($type == 'ntPassword')
	$passwd = strtoupper(bin2hex($hash->ntPasswordHash()));
  else
	$passwd = '**DISABLED**';
  
  return($passwd);
}
// }}}

// {{{ pql_password_hash(password, hash)
function pql_password_hash($password, $hash) {
  // creates a hash of a password
  // - {SSHA}askdfjklödjfnddnu
  // - {SHA}kldfaldkjadsfdsad=
  // - {MD5}lkasjfdndiasdfee=
  // - {CRYPT}cryptedpassword
  // - {NTPASSWD}NT Password
  // - {LMPASSWD}Lan Manager
  // - {SASL}principal@realm
  // - cleartextpassword
  
  if(!function_exists("mhash") and ($hash = "{MD5}")) {
	// We do not have access to MD5 encryption - force {CRYPT}!
	unset($hash);
	
	$log = date("M d H:i:s");
	$log .= "MD5 encryption (mhash) not compiled into PHP. Please review the file doc/INSTALL. Falling back to using {CRYPT}";
	@error_log($log, 3, "phpQLadmin.log");
  }
  
  switch($hash) {
  case "{SSHA}":
	// SHA1 hashing (salted)
	if(function_exists("mhash"))
	  return "{SSHA}" . base64_encode(mhash(MHASH_SHA1, $password, uniqid(microtime())));
	
  case "{SHA}":
	// SHA1 hashing
	if(function_exists("mhash"))
	  return "{SHA}" . base64_encode(mhash(MHASH_SHA1, $password));
	
  case "{MD5}":
	// MD5 hashing
	if(function_exists("mhash"))
	  return "{MD5}" . base64_encode(mhash(MHASH_MD5, $password));
	
  case "{CLEAR}":
	// Password in the clear (JUCK! :)
	return $password;
	
  case "{ntpasswd}":
	// NT Password
	return smbpassword($password, 'ntPassword');
	
  case "{lmpasswd}":
	// Lan Manager
	return smbpassword($password, 'lmPassword');
	
  case "{KERBEROS}":
	// Kerberos V mapping
  case "{SASL}":
	// NEW FORMAT: '{SASL}principal@REALM.TLD'
	return '{SASL}' . $password;
	
  default:
	$salt_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"; 
	$max_idx = strlen($salt_chars) - 1; 
	$salt = $salt_chars[rand(0, $max_idx)] . $salt_chars[rand(0, $max_idx)]; 
	
	return "{crypt}" . crypt($password, $salt);
  }
}
// }}}

// {{{ pql_generate_password(void)
function pql_generate_password() {
  $amount = 8;
  
  for(;;) {
	$pwd = NULL; $o = NULL;
	
	for($x=0; $x < $amount; ) {
	  $y = rand(1, 1000);
	  if(($y > 350) and ($y < 601)) $d = chr(rand(48, 57));
	  if($y < 351) $d=chr(rand(65, 90));
	  if($y > 600) $d=chr(rand(97, 122));
	  if($d != $o) {
		$o = $d;
		$pwd .= $d;
		$x++;
	  }
	}
	
	if(ereg("^[a-zA-Z]{1}([a-zA-Z]+[0-9][a-zA-Z]+)+", $pwd))
	  break;
  }
  
  return($pwd);
}
// }}}

// {{{ pql_check_email(email)
function pql_check_email($email) {
  $split = split("@", $email);
  
  if(count($split) != 2)
	// Does not contain @ or has to much of it
	return false;
  
  // Check user part
  if(!preg_match("/^[a-z0-9-+=#]+([\._a-z0-9-+=#]?[a-z0-9]+)*$/i", $split[0]))
	// user part is not valid:
	// - must start with a valid character (a-Z, 0-9)
	// - may contain a special char (. _ + = # -), but not at start and end
	//   and following another special char
	// - must end with a valid character (a-Z, 0-9)
	// - must contain at least 1 character
	return false;
  
  // Check host part (force a dot in the domain!)
  return pql_check_hostaddress($split[1], 1);
}
// }}}

// {{{ pql_check_hostaddress(host, force_dot)
function pql_check_hostaddress($host, $force_dot = 0) {
  // Checks an fqdn - if force_dot is true, it must contain a dot
  if($force_dot)
	return preg_match("/^([0-9a-z-]+\.[0-9a-z-]+)+$/i", $host);
  else
	return preg_match("/[^0-9a-z]/i", $host);
}
// }}}

// {{{ pql_execute(command, hide=true)
function pql_execute($command, $hide=true) {
  $command = "(" . escapeshellcmd($command) . ") 2>&1";
  
  if(!$hide)
	echo "\n<pre>\n----\n";
  
  flush(passthru($command, $ret));
  
  if($ret)
	$code = '=> <b><u>FAILED!!</u></b>';
  else
	$code = '=> <b>SUCCESS</b>';
  
  if(!$hide)
	echo "----\nreturn value: $ret $code\n</pre>";
  
  return $ret;
}
// }}}

// {{{ pql_ldap_accountstatus(status)
// Returns text of account-status
function pql_ldap_accountstatus($status) {
  global $LANG;
  
  // active [default]
  // nopop		(not availible in newer QmailLDAP it seems).
  // noaccess
  // disabled
  // deleted
  
  switch($status) {
  case "nopop":
	return $LANG->_('POP locked');
	
  case "noaccess":
	return $LANG->_('Locked/Noaccess');
	
  case "disabled":
	return $LANG->_('Locked/Disabled');
	
  case "deleted":
	return $LANG->_('Deleted');
	
  default:
	return $LANG->_('Active');
  }
}
// }}}

// {{{ pql_ldap_deliverymode(mode)
// Returns text of delivery mode
function pql_ldap_deliverymode($mode) {
  global $LANG;
  
  if($_SESSION["NEW_STYLE_QUOTA"]) {
	// multi field entries of these keywords
	// - (normal)	put message into maildir/mbox, plus forward and program delivery
	// - noforward	do not forward (ignores forwarding entries in ldap and .qmail)
	// - nolocal	do not put message into maildir/mbox (ignores also .qmail)
	// - noprogram	do not do program deliveries (ignores deliveryprogrampath, .qmail)
	// - reply		send an auto_reply mail with text from mailReplyText
	switch($mode) {
	case "normal":
	  return $LANG->_('Save in local mailbox, allow forwarding and program delivery');
	  
	case "noforward":
	  return $LANG->_('Save in local mailbox only (no forwarding)');
	  
	case "nolocal":
	  return $LANG->_('Do not save in local mailbox, allow forwarding and program delivery');
	  
	case "noprogram":
	  return $LANG->_('Do not allow program delivery (ignores deliveryProgramPath and .qmail)');
	  
	case "reply":
	  return $LANG->_('Send an automatic reply mail');
	}
  } else {
	// normal [default]
	// forwardonly
	// nombox
	// localdelivery
	// reply
	// echo
	switch($mode) {
	case "forwardonly":
	  return $LANG->_('Only forward');
	  
	case "nombox":
	  return $LANG->_('No local mailbox');
	  
	case "localdelivery":
	  return $LANG->_('Save in local mailbox');
	  
	case "reply":
	  return $LANG->_('Send an automatic reply mail');
	  
	case "echo":
	  return $LANG->_('Echo to console (tricky)');
	  
	default:
	  return $LANG->_('Normal');
	}
  }
}
// }}}

// {{{ pql_split_oldvalues(value)
function pql_split_oldvalues($value) {
  // (Possibly) split the old value array
  if(eregi(" ", $value))
	$values = split(" ", $value);
  elseif(eregi(",", $value))
	$values = split(",", $value);
  elseif($value)
	$values[] = $value;
  
  if(is_array($values)) {
	asort($values);
	return($values);
  } else
	return false;
}
// }}}

// {{{ pql_missing_objectclasses(dn, entry)
// TODO: This could be a lot smarter!
function pql_missing_objectclasses($dn, $entry) {
  // Get objectclasses from the LDAP server, finding the objectclass
  // that MUST (or if non is found, a MAY) have the attribute we're
  // trying to modify.
  $objectclasses = pql_get_subschema('objectclasses');
  if(is_array($objectclasses)) {
	foreach($entry as $attrib => $val) {
	  // For each of the attributes (exept the objectclass one)
	  // in the object, let's see if we can find it's objectclass.
	  
	  if($attrib != pql_get_define("PQL_ATTR_OBJECTCLASS")) {
		foreach($objectclasses as $key => $oc) {
		  // Go through the MUST attributes
		  for($i=0; $i < $oc[MUST]['count']; $i++)
			if(lc($oc[MUST][$i]) == lc($attrib))
			  $objectclass[] = $key;
		  
		  if(!is_array($objectclass)) {
			// Didn't find a MUST, try to find a MAY attribute
			for($i=0; $i < $oc[MAY]['count']; $i++)
			  if(lc($oc[MAY][$i]) == lc($attrib))
				$objectclass[] = $key;
		  }
		}
	  }
	}
	
	if(is_array($objectclass)) {
	  // We've found objectclass(es) that provide the attributes in the object.
	  for($i=0; $i < count($existing); $i++)
		$objectclass[] = $existing[$i];
	  
	  return($objectclass);
	}
  } else
	return false;
}
// }}}

// {{{ pql_recreate_dnsttl(dn)
function pql_recreate_dnsttl($dn) {
  // De urldecode (part of) the DNS DN.
  // We're missing the '+' between dNSTTL and relativeDomainName
  // (which urldecode() removes!):
  // 
  // dNSTTL=3600+relativeDomainName=@,dc=domain,dc=tld,PQL_CONF_SUBTREE_BIND9,o=Branch,c=SE
  $newdn = '';
  $dn_parts = explode(',', $dn);
  
  // Create the new DN with all but the very first part.
  for($i=1; $i < count($dn_parts); $i++) {
	$newdn .= $dn_parts[$i];
	if(!empty($dn_parts[$i+1]))
	  $newdn .= ",";
  }
  
  // Add the missing part
  $tmp   = split(' ', $dn_parts[0]);
  $newdn = $tmp[0]."+".$tmp[1].",".$newdn;
  
  return($newdn);
}
// }}}

// {{{ pql_header(url, force)
function pql_header($url, $force = 0) {
  $uri = $_SESSION["URI"].$url;
  
  if(pql_get_define("PQL_CONF_DEBUG_ME") and !$force) {
    echo "<p>If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
    die("<b>$uri</b>");
  } else {
	if(function_exists("http_redirect"))
	  http_redirect($uri);
	else {
	  if(function_exists("ob_clean"))
		ob_clean();
	  
	  header("Location: $uri");
	}

	exit;
  }
}
// }}}

// {{{ pql_fix_path(path)
// Replace some 'invalid' characters from the path
function pql_fix_path($path) {
  // Replace space(s), '&' and '@' with underscore(s)
  $path = preg_replace('/ /',  '_', $path, -1);
  $path = preg_replace('/&/',  '_', $path, -1);
  $path = preg_replace('/@/',  '_', $path, -1);
  $path = preg_replace('/\./', '_', $path, -1);
  
  return($path);
}
// }}}

// {{{ pql_add2array(&$old, $new)
function pql_add2array(&$old, $new) {
  if(!@$new)
	return($old);
  
  // Make sure 'old' is an array
  if($old and !is_array($old))
	$old = array($old);
  
  // Make sure 'new' is an array
  if(!is_array($new))
	$new = array($new);
  
  // Add all entries in 'new' to 'old'
  foreach($new as $entry) {
	if(!@in_array($entry, $old))
	  // This is a unique entry, add it
	  $old[] = $entry;
  }
  
  return($old);
}
// }}}

// {{{ pql_flush(void)
function pql_flush() {
  if(headers_sent()) {
	if(function_exists("ob_flush"))
	  // Only exists in PHP >= 4.2.0
	  ob_flush();
	
	flush();
  }
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
