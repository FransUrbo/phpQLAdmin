<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.10 2003-11-19 19:38:33 turbo Exp $
//

// ------------------------------------------------
// PQL_CONF_SEARCH - API functions
// ------------------------------------------------

// {{{ pql_search(linkid, basedn, filter)
// Get all users matching filter
function pql_search($linkid, $basedn, $filter, $attrib = 'dn') {
	// URL decode the object DN if it's URL ENcoded
	$basedn = urldecode($basedn);

	$sr   = ldap_search($linkid, $basedn, $filter) or pql_errormsg($linkid);
	$info = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);

	for($i=0; $i < $info["count"]; $i++) {
		if($attrib == '')
		  $dn = $dn + $info[$i];
		else
		  $dn[] = $info[$i][$attrib];
	}

	return $dn;
}
// }}}

// {{{ pql_get_dn(ldap, reference, multiples)
// Retrieve the dn for a given reference
function pql_get_dn($ldap, $reference, $multiples = 0) {
    // check if we already have the dn for this user chached
    $dn = '';
    if(pql_cache_userdn_get($reference, $dn))
	  return($dn);

    // dn not in cache -> search for it
	foreach($ldap->ldap_basedn as $base) {
		$base = urldecode($base);

		// Setup the search filter
		$filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $base) . "=";
		$filter .= pql_maybe_encode($reference, pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $base),
									$ldap->ldap_linkid);

		// Do the search
		$dn = pql_search($ldap->ldap_linkid, $base, $filter);
		if($dn) {
			// we got the dn -> remember it
			pql_cache_userdn_add($reference, $dn);
			
			return($dn);
		}
	}

	return(false);
}
// }}}

// {{{ pql_get_attribute(ldap_linkid, user, attribute)
function pql_get_attribute($ldap_linkid, $user, $attribute) {
    $attribute = lc($attribute);

    // check whether this user is cached
    $value = '';
    
	if(pql_cache_userentry_get($user, $attribute, $value))
	  // the user is cached  -> return the value for the requested attribute  
	  return($value);

    // not yet cached -> get the userentry and cache it
    $entries = pql_get_object($ldap_linkid, $user);
    if($entries) {
		// INTERNATIONALIZE values
		foreach($entries as $attrib => $e) {
			for($i=0; $e[$i]; $i++)
			  $values[$attrib][] = pql_maybe_decode($e[$i]);
		}

		pql_cache_userentry_add($user, $values);
		return($values[$attribute]);	
	} else
	  return(false);
}
// }}}

// {{{ pql_get_object(ldap_linkid, user)
function pql_get_object($ldap_linkid, $user) {
	$sr = @ldap_read($ldap_linkid, $user, "(objectClass=*)");
	if($sr == false)
	  return false;
	
	// build array of all attributes
	$rid = @ldap_first_entry($ldap_linkid, $sr);
	$attribs[] = lc(@ldap_first_attribute($ldap_linkid, $rid, $berid));

	while($attrib = @ldap_next_attribute($ldap_linkid, $rid, $berid))
	  $attribs[] = lc($attrib);

    // build entry array with all attributes and values
	foreach($attribs as $attrib) {
		$values = @ldap_get_values($ldap_linkid, $rid, $attrib);
		unset($values["count"]);
		$entry[$attrib] = $values;
    }
    
    // return entry
    return $entry;
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
