<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.19 2004-05-13 06:00:27 turbo Exp $
//

// ------------------------------------------------
// PQL_CONF_SEARCH - API functions
// ------------------------------------------------

// {{{ pql_search(linkid, basedn, filter, level)
// Get all users matching filter
function pql_search($linkid, $basedn, $filter, $attrib = 'dn', $level = "SUBTREE") {
	// URL decode the object DN if it's URL ENcoded
	$basedn = urldecode($basedn);

	if($level == "SUBTREE") 
	  $sr = @ldap_search($linkid, $basedn, $filter);
	elseif($level == "ONELEVEL")
	  $sr = @ldap_list($linkid, $basedn, $filter);
	else  // $level == "BASE"
	  $sr = @ldap_read($linkid, $basedn, $filter);

	if(!$sr)
	  // No such object
	  return false;

	$info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
	for($i=0; $i < $info["count"]; $i++) {
		if($attrib == '')
		  $dn[] = $info[$i];
		else {
			if(is_array($info[$i][$attrib])) {
				for($j=0; $j < $info[$i][$attrib]["count"]; $j++)
				  $dn[] = $info[$i][$attrib][$j];
			} else
			  $dn[] = $info[$i][$attrib];
		}
	}

	return $dn;
}
// }}}

// {{{ pql_get_dn(ldap, reference, multiples, default_reference)
// Retrieve the dn for a given reference
function pql_get_dn($ldap, $reference, $multiples = 0, $default = 'uid') {
    $dn = ''; $resonable_default = 0;

    // Check if we already have the dn for this user chached
    if(pql_cache_userdn_get($reference, $dn))
	  return($dn);

    // DN not in cache -> search for it
	foreach($ldap->ldap_basedn as $base) {
		$base = urldecode($base);

		// Get user reference
		$user_ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $base);
		if(!$user_ref) {
			// Resonable default...
			$user_ref = $default;
			$resonable_default = 1;
		}

		// Setup the search filter
		$filter  = $user_ref . "=" . pql_maybe_encode($reference, $user_ref, $ldap->ldap_linkid);

		// Do the search
		$dn = pql_search($ldap->ldap_linkid, $base, $filter);
		if($dn) {
			// we got the dn -> remember it
			pql_cache_userdn_add($reference, $dn);
			
			return($dn);
		} elseif($resonable_default) {
			// Not found, and we've specified a resonable default
			// which didn't work. Have one more shot at finding the object...
			// -> Recursivly call again.
			pql_get_dn($ldap, $reference, $multiples, 'cn');
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
			for($i=0; isset($e[$i]); $i++)
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
