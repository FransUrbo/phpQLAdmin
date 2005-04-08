<?php
// phpQLAdmin cache Application Programming Interface (API)
//
// $Id: pql_cache.inc,v 2.2 2003-11-11 07:21:33 turbo Exp $
//

// global variables
$pql_cache_userdn = array();
$pql_cache_userentry = array();

// {{{ pql_cache_userentry_add(dn, entries)
// Adds an userentry to the cache
function pql_cache_userentry_add($dn, $entries) {
    global $pql_cache_userentry;
    
    foreach($entries as $attribute => $value) {
		$pql_cache_userentry[$dn][$attribute] = $value;
    }
}
// }}}

// {{{ pql_cache_userentry_get(dn, attribute, value)
// Searches the cache for an attribute and returns it if found 
function pql_cache_userentry_get($dn, $attribute, &$value) {
    global $pql_cache_userentry;
    
    // we don't have this user yet
    if (!isset($pql_cache_userentry[$dn])) {
		return(false);
    }
    
    // the user is cached, but he doesn't have the requested attribute
    if (!isset($pql_cache_userentry[$dn][$attribute])) {
		$value = NULL;
		return(true);
    }
    
    $value = $pql_cache_userentry[$dn][$attribute]; 
    return(true);
}
// }}}

// {{{ pql_cache_userentry_remove(dn)
// Removes an userentry from the cache
function pql_cache_userentry_remove($dn) {
    global $pql_cache_userentry;
    
    unset($pql_cache_userentry[$dn]);
}
// }}}

// {{{ pql_cache_userdn_add(reference, dn)
// Add the dn of an user to the cache
function pql_cache_userdn_add($reference, $dn) {
    global $pql_cache_userdn;
    
    $pql_cache_userdn[$reference] = $dn;
}
// }}}

// {{{ pql_cache_userdn_get(reference, &dn)
// Searches the cache for an user and returns his dn if found
function pql_cache_userdn_get($reference, &$dn) {
    global $pql_cache_userdn;
    
    if (isset($pql_cache_userdn[$reference])) {
		$dn = $pql_cache_userdn[$reference];
		return(true);
    } else {
		return(false);
    }
}
// }}}

// {{{ pql_cache_userdn_remove(reference)
// Removes an dn from the cache
function pql_cache_userdn_remove($reference) {
    global $pql_cache_userdn;
    
    unset($pql_cache_userdn[$reference]);
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
