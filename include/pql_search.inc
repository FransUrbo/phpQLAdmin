<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.21 2004-10-18 13:39:33 turbo Exp $
//

// ------------------------------------------------
// PQL_CONF_SEARCH - Core SEARCH API functions
// ------------------------------------------------

// {{{ pql_search(linkid, dn, filter, level, referrals, operational)
// Return all users (whole object!!) matching filter.
function pql_search($linkid, $dn, $filter, $level = "SUBTREE", $referrals = 0, $operational = 0) {
	// URL decode the object DN if it's URL ENcoded
	$dn = urldecode($dn);

    // TODO: Check whether the object is cached.
//    $value = '';
//	if(pql_cache_userentry_get($dn, $attribute, $value))
//	  // The object is cached  -> return the value for the requested attribute  .
//	  return($value);

	// ---------------------
	switch($level) {
	  case "SUBTREE":	$func = "ldap_search";	break;
	  case "ONELEVEL":	$func = "ldap_list";	break;
	  default:			$func = "ldap_read";	break;
	}

	if($operational)
	  $op = array('+');
	else
	  $op = array();
	  

	// ---------------------
	// Retreive the whole object.
	$sr = @$func($linkid, $dn, $filter, $op);
	if(!$sr)
	  // No such object - problem with the search string?
	  return false;

	// ---------------------
	// Get ALL hits and rearrange the array.
	$object = ldap_get_entries($linkid, $sr) or pql_format_error(1);
	for($i=0; $i < $object["count"]; $i++) {
		foreach($object[$i] as $key => $value) {
			if(eregi('^[a-z]', $key) and $key != 'count') {
				// Ignore the 'count' value and values with a number
				// ldap_get_entries() returns a very funny array - flatten
				// it!
				if(is_array($value) and $value[1]) {
					// A multi value - such as 'mailAlternateAddress'
					// or 'objectClass' etc. Take ALL values.
					for($j=0; $value[$j]; $j++) {
						$entry[$i][$key][] = pql_maybe_decode($value[$j]);
					}
				} elseif(!is_array($value))
				  // The 'dn' attribute probably. It is NOT an array...
				  $entry[$i][$key] = pql_maybe_decode($value);
				else
				  // Single valued such as 'uid' and 'mail' etc...
				  // Just take the ONE value
				  $entry[$i][$key] = pql_maybe_decode($value[0]);
			}
		}
	}

	// ---------------------
	// TODO: Finish this, it's taken directly from (the now deleted) function pql_user_get().
//	if($referrals) {
//		// Get any referrals in this branch
//		$resource = ldap_first_reference($linkid, $sr);
//		if($resource) {
//			ldap_parse_reference($linkid, $resource, $referrals);
//			
//			while($resource = ldap_next_reference($linkid, $resource)) {
//				if($resource) {
//					ldap_parse_reference($linkid, $resource, $new);
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

	// ---------------------
	// Cache this/these object
//	for($i=0; $entry[$i]; $i++)
//	  pql_cache_userentry_add($dn, $entry[$i]);

	return($entry);
}
// }}}

// {{{ pql_get_attribute(linkid, dn, attribute, operational)
// Return only a subset of an object (retreived with pql_search()).
function pql_get_attribute($linkid, $dn, $attribute, $operational = 0) {
    $attribute = lc($attribute);

    // Get the object and return only the attributes of interest.
	$object = pql_search($linkid, $dn, "(".pql_get_define("PQL_ATTR_OBJECTCLASS")."=*)", 'BASE', 1, $operational);
    if($object)
	  return($object[0][$attribute]);

	return(false);
}
// }}}

// ------------------------------------------------
// PQL_CONF_SEARCH - Support SEARCH API functions
// ------------------------------------------------

// {{{ pql_get_dn(linkid, dn, filter, level)
// This function returns the DN of each object if finds.
function pql_get_dn($linkid, $dn, $filter, $level = 'SUBTREE') {
	$objects = pql_search($linkid, $dn, $filter, $level, 1);
	for($i=0; $objects[$i]; $i++)
	  $entry[] = $objects[$i]['dn'];

	return($entry);
}
// }}}

// {{{ pql_get_domains(ldap)
// Get all domains listed in ldap-tree (ou|dc-records)
function pql_get_domains($ldap) {
	foreach($ldap->ldap_basedn as $TOP_DN)  {
		$TOP_DN   = urldecode($TOP_DN); unset($info);
		$dn       = pql_get_define("PQL_CONF_SUBTREE_USERS").",$TOP_DN";
		$ocs      = pql_setup_branch_objectclasses(1, $TOP_DN);

		// -------------------
		// Does this top DN contain the ou=People object?
		$filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=*';
		$result = pql_get_dn($ldap->ldap_linkid, $dn, $filter, 'BASE');
		if(is_array($result)) {
			// ------------------- BUG: THIS IS YET UNTESTED AFTER REWRITE!
			// Yes it does - Is there any users just below the ou=People object?
			$filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $TOP_DN)."=*";
			$domains = pql_get_dn($ldap->ldap_linkid, $dn, $filter, 'ONELEVEL');
			if(is_array($domains)) {
				// Just to be safe - is there any branches here?
				$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
				$new = pql_get_dn($ldap->ldap_linkid, $dn, $filter, 'ONELEVEL');
				if(is_array($new))
				  $domains = $domains + $new;
			} else {
				// No users below the ou=People objects. Any users directly below the top DN then?
				$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $TOP_DN)."=*";
				$new    = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');
				if(is_array($domains)) {
					$domains = $domains + $new;

					// Just to be safe - is there any branches here?
					$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
					$new = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');
					if(is_array($new))
					  $domains = $domains + $new;
				}
			}
		} else {
			// -------------------
			// No ou=People object - Is there any users just below the top DN then?
			$filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $TOP_DN)."=*";
			$domains = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');

			// Just to be safe - is there any branches here?
			$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
			$new = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');
			if(is_array($new)) {
				if(is_array($domains))
				  $domains = $domains + $new;
				else
				  $domains = $new;
			}
		}
	}

	return $domains;
}
// }}}

// {{{ pql_search_forwarders($ldap, user)
// Search all accounts with forwarders to $user@$domain
function pql_search_forwarders($ldap, $user) {
	$return = '';
	$linkid = $ldap->ldap_linkid;

    // get all email addresses of a user
    $email   = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_MAIL"));
    $aliases = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_MAILALTERNATE"));
    
    $addresses[] = $email;
    if(is_array($aliases))
	  $addresses = array_merge($addresses, $aliases);
    
    // create filter
    $filter = "(|";
    foreach($addresses as $add)
	  $filter .= "(" . pql_get_define("PQL_ATTR_FORWARDS") ."=" . $add . ")";
    $filter .= ")";

    // Go through each base DN in the database, looking
	// for forwarders to this user
	foreach($ldap->ldap_basedn as $dn) {
		$dn  = urldecode($dn);
		$ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn);

		// Get all users that refere/forwards to this ($dn) object.
		$results = pql_get_dn($linkid, $dn, $filter);
		for($i=0; $results[$i]; $i++) {
			// Retreive this object.
			$object = pql_search($linkid, $results[$i], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE', 0);

			$return[] = array("reference" => $object[0]['dn'],
							  $ref		  => $object[0][$ref],
							  "email"     => $object[0][pql_get_define("PQL_ATTR_MAIL")]);
			
		} // end foreach results
	} // end foreach basedn

    return $return;
}
// }}}

// {{{ pql_email_exists(ldap, email)
// Check if any mail or mailalternateaddress record with this email exists in the ldap tree
function pql_email_exists($ldap, $email) {
	$filter = "(|(" . pql_get_define("PQL_ATTR_MAIL") . "=" . $email . ")(" . pql_get_define("PQL_ATTR_MAILALTERNATE") . "=" . $email . "))";

	foreach($ldap->ldap_basedn as $dn) {
		$dn = urldecode($dn);

		$results = pql_get_dn($ldap->ldap_linkid, $dn, $filter);
		if(is_array($results))
		  return true;
	}

	return false;
}
// }}}

// {{{ pql_validate_administrator(linkid, dn, admin)
function pql_validate_administrator($linkid, $dn, $admin) {
	$result = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"));
	if($result)
	  return(true);

	return(false);
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
