<?php
// phpQLAdmin cache Application Programming Interface (API)
//
// $Id: pql_cache.inc,v 2.9 2007-02-09 12:18:19 turbo Exp $
//

// {{{ Initialize the cache 'area'
if(!file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE") and !@$pql_cache) {
  // Cache not disabled, continue...
  if (class_exists('Memcache') and function_exists("memcache_connect")) {
	// The whole point with memcached is that it runs on localhost, so
	// the TCP/IP connection doesn't give any overhead! Change at your own
	// peril!
	$pql_cache = @memcache_connect('localhost', 11211);
	if(!@$pql_cache)
	  $msg = " WARNING: Connection to memcached failed. Using normal apache memory cache.\n";
  } else
	$msg = " WARNING: Memcache module not loaded. Using normal apache memory cache.\n";
  
  if(!@$pql_cache) {
	$log = date("M d H:i:s");
	$log .= $msg;
	@error_log($log, 3, "phpQLadmin.log");
	
	$pql_cache = array();
  }
}
// }}}

// {{{ pql_cache_add(object)
// Adds an LDAP object to the cache
// NOTE: This function will NOT check if a previous value
//       already exists, it's up to phpQLAdmin API!
function pql_cache_add($object) {
	global $pql_cache, $_SESSION;
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return;

	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode(lc($object["dn"])));

	if(file_exists($_SESSION["path"]."/.DEBUG_CACHE")) {
	  echo "<font size=\"+1\">Caching '$dn':</font>";
	  printr($object);
	}
	
	if(is_array($pql_cache))
	  // It might exists, but we don't care! Overwrite it if do exist...
	  $pql_cache[$dn] = $object;
	else
	  // Doesn't already exists. Add it.
	  $pql_cache->set($dn, $object, 1, (60*10));
}
// }}}

// {{{ pql_cache_get(dn, object)
// Searches the cache for an object and returns it if found 
function pql_cache_get($dn, &$object) {
    global $pql_cache, $_SESSION;

	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return(false);
	if(!isset($dn))
	  return(false);

	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode(lc($dn)));
    
	if(is_array($pql_cache)) {
	  if(empty($pql_cache[$dn])) {
		// We don't have this object yet.
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$dn' <b>not</b> cached!<br>";
		
		return(false);
	  } else {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$dn' <b>is</b> cached<br>";
		
		$object = $pql_cache[$dn];
		return(true);
	  }
	} else {
	  $object = $pql_cache->get($dn);
	  if(@$object) {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$dn' <b>is</b> cached<br>";

		return(true);
	  } else {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$dn' <b>not</b> cached!<br>";

		return(false);
	  }
	}
}
// }}}

// {{{ pql_cache_remove(dn)
// Removes an userentry from the cache
function pql_cache_remove($dn) {
    global $pql_cache;
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return(false);
	if(!@$dn)
	  return(false);

	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode(lc($dn)));
    
	if(is_array($pql_cache)) {
	  if($pql_cache[$dn])
		unset($pql_cache[$dn]);
	} else {
	  if($pql_cache->get($dn))
		// There is such an object - delete it
		$pql_cache->delete($dn);
    }
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
