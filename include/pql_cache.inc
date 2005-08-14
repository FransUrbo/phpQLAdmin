<?php
// phpQLAdmin cache Application Programming Interface (API)
//
// $Id: pql_cache.inc,v 2.5 2005-08-14 15:01:40 turbo Exp $
//

// Global variables
$pql_cache = array();

// {{{ pql_cache_add(object)
// Adds an LDAP object to the cache
function pql_cache_add($object) {
	global $pql_cache, $_SESSION;
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return;

	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode(lc($object["dn"])));

	// It might exists, but we don't care! Overwrite it if do exist...
	$pql_cache[$dn] = $object;

	if(file_exists($_SESSION["path"]."/.DEBUG_CACHE")) {
	  echo "Caching '$dn':";
	  printr($object);
	}
}
// }}}

// {{{ pql_cache_get(dn, object)
// Searches the cache for an object and returns it if found 
function pql_cache_get($dn, &$object) {
    global $pql_cache, $_SESSION;
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return(false);

	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode(lc($dn)));
    
    if(!isset($pql_cache[$dn])) {
	  // We don't have this object yet.
	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		echo "Object '$dn' <b>not</b> cached!<br>";

	  return(false);
	}

	if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
	  echo "Object '$dn' <b>is</b> cached<br>";

	$object = $pql_cache[$dn];
	return(true);
}
// }}}

// {{{ pql_cache_remove(dn)
// Removes an userentry from the cache
function pql_cache_remove($dn) {
    global $pql_cache;
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return;

	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode(lc($dn)));
    
	if($pql_cache[$dn])
	  unset($pql_cache[$dn]);
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
