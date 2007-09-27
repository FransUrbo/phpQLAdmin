<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.36.2.1.2.3 2005-11-28 08:44:12 turbo Exp $
//

// ------------------------------------------------
// Core SEARCH API functions
// ------------------------------------------------

// {{{ pql_search(linkid, dn, filter, level, referrals, operational)
// Return all users (whole object!!) matching filter.
function pql_search($linkid, $dn, $filter, $level = "SUBTREE", $referrals = 0, $operational = 0) {
	// URL decode the object DN if it's URL ENcoded
	$dn = pql_maybe_encode(urldecode($dn));

	if(eregi("^dNSTTL=", $dn))
	  $dn = pql_recreate_dnsttl($dn);

    // {{{ Check whether the object is cached - TODO
//    $value = '';
//	if(pql_cache_userentry_get($dn, $attribute, $value))
//	  // The object is cached  -> return the value for the requested attribute  .
//	  return($value);
	// }}}

	// {{{ Which ldap_{search,list,read}() function to use
	switch($level) {
	  case "SUBTREE":	$func = "ldap_search";	break;
	  case "ONELEVEL":	$func = "ldap_list";	break;
	  default:			$func = "ldap_read";	break;
	}
	// }}}

	if($operational)
	  $op = array('+');
	else
	  $op = array();

	// {{{ Retreive the whole object.
	$sr = @$func($linkid, $dn, $filter, $op);
	if(!$sr)
	  // No such object - problem with the search string?
	  return false;
	// }}}

	// {{{ Get ALL hits and rearrange the array.
	$object = ldap_get_entries($linkid, $sr) or pql_format_error(1);
	for($i=0; $i < $object["count"]; $i++) {
		foreach($object[$i] as $key => $value) {
			if(eregi('^[a-z]', $key) and $key != 'count') {
				// Ignore the 'count' value and values with a number
				// ldap_get_entries() returns a very funny array - flatten
				// it!
				if(is_array($value) and $value[1]) {
					// A multi value - such as 'mailAlternateAddress'
					// or 'objectClass' etc. Take ALL values, ignoring
					// any empty values that might exists.
					$count = count($value);
					for($j=0; $j <= $count; $j++) {
					  if($value[$j])
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
	// }}}

	// {{{ Chase referals
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
	// }}}

	// {{{ Cache this/these object
//	for($i=0; $entry[$i]; $i++)
//	  pql_cache_userentry_add($dn, $entry[$i]);
	// }}}

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
// Support SEARCH API functions
// ------------------------------------------------

// {{{ pql_get_dn(linkid, dn, filter, level)
// This function returns the DN of each object if finds.
// Basically a simplified version of pql_search() - one that only returns
// the DN's of the matches...
function pql_get_dn($linkid, $dn, $filter, $level = 'SUBTREE') {
	unset($entry);

	$objects = pql_search($linkid, $dn, $filter, $level, 1);
	for($i=0; $objects[$i]; $i++)
	  $entry[] = $objects[$i]['dn'];

	if(is_array($entry))
	  return($entry);
	else
	  return false;
}
// }}}

// {{{ pql_get_domains(ldap)
// Get all domains listed in ldap-tree (ou|dc-records)
function pql_get_domains($ldap) {
	$domains = array();

	foreach($_SESSION["BASE_DN"] as $TOP_DN)  {
		if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
			// There's a subtree defined - look if it's there
			$dn     = pql_get_define("PQL_CONF_SUBTREE_USERS").",$TOP_DN";

			$filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=*';
			$result = pql_get_dn($ldap->ldap_linkid, $dn, $filter, 'BASE');
			if(is_array($result)) {
				// ------------------- BUG: THIS IS YET UNTESTED AFTER API (2.1.1-2.1.2) REWRITE!
				// Yes it does - Is there any users just below the ou=People object?
				$filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $TOP_DN)."=*";
				$domains = pql_get_dn($ldap->ldap_linkid, $dn, $filter, 'ONELEVEL');
				if(is_array($domains)) {
					// Just to be safe - is there any branches here?
					$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
					$new = pql_get_dn($ldap->ldap_linkid, $dn, $filter, 'ONELEVEL');
					if(is_array($new))
					  $domains = pql_add2array($domains, $new);
				}
			}
		}

		// -------------------
		// Is there any users just below the top DN then?
		$dn	    = $TOP_DN;

		// BUG: I'd really like to use this, but I can't.
		// The reason is: If I specify (in 'phpQLAdmin config->Branch config')
		// to use the following object classeses when creating branches,
		// AND all those object classes doesn't already exists in the branch
		// object(s), no branches will be found!
		// $ocs = '(objectClass=phpQLAdminBranch)(objectClass=phpQLAdminInfo)'
		// But only the first (or none of them) exists -> no branches!!
		//$ocs    = pql_setup_branch_objectclasses(1, $TOP_DN);

		// Instead, I'll have to 'fake' it here.
		if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) == 'o')
		  $ocs = '(objectClass=organization)';
		else if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) == 'dc')
		  $ocs = '(objectClass=domain)'; // Could also be 'domainComponent'
		
		if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN))
		  $filter = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN)."=*";
		else
		  // User reference for this branch/domain not specified. Use default...
		  $filter = 'uid=*';
		$new    = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');

		if(is_array($new))
		  $domains = pql_add2array($domains, $new);
		
		// Just to be safe - is there any branches here?
		$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $TOP_DN) . "=*)$ocs)";
		$new    = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');
		if(is_array($new))
		  $domains = pql_add2array($domains, $new);

		// Now, if the base/root DN is a 'dc' object and domain/branch reference
		// is 'ou', we MUST use 'organizationalUnit' in the object class filter...
		$filter = '(&('.pql_get_define("PQL_ATTR_OU").'=*)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))';
		$new    = pql_get_dn($ldap->ldap_linkid, $TOP_DN, $filter, 'ONELEVEL');
		if(is_array($new) and !in_array('ou=Templates'.','.$TOP_DN, $new))
		  $domains = pql_add2array($domains, $new);
	}

	if($domains[0]) {
	  // Sort the domain array by extracting the actual domain/branch name
	  // and sort by that, not the DN ('ou=', 'o=', 'dc=' etc).
	  for($i=0; $domains[$i]; $i++) {
		// o=Bayour.COM,c=SE => Get 'Bayour.COM'
		$tmp = split(',', $domains[$i]);
		$tmp = split('=', $tmp[0]);
		$tmp = $tmp[1];

		$dom[$domains[$i]] = lc($tmp);
	  }

	  // The array now looks like:
	  // o=Bayour.COM,c=SE => bayour.com

	  // Sort by _value_ (which is lower cased).
	  if(defined("SORT_LOCALE_STRING"))
	    // Added in PHP 4.4.0 and 5.0.2.
		asort($dom, SORT_LOCALE_STRING);
	  else
	    asort($dom);

	  unset($tmp); // I'd like to reuse this variable...

	  // Move the new array (which is correctly sorted) to the
	  // original array.
	  foreach($dom as $dn => $value) {
		for($i=0; $domains[$i]; $i++) {
		  if(lc($dn) == lc($domains[$i]))
			$tmp[] = $dn;
		}
	  }
	  $domains = $tmp;

	  return pql_uniq($domains);
	} else
	  return false;
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
	foreach($_SESSION["BASE_DN"] as $dn) {
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

	foreach($_SESSION["BASE_DN"] as $dn) {
		$results = pql_get_dn($ldap->ldap_linkid, $dn, $filter);
		if(is_array($results))
		  return true;
	}

	return false;
}
// }}}

// {{{ pql_validate_administrator(linkid, dn, attrib, admin)
function pql_validate_administrator($linkid, $dn, $attrib, $admin) {
	$dn = urldecode($dn);

	$result = pql_get_attribute($linkid, $dn, $attrib);
	if(!$result)
	  return(false);

	if(is_array($result)) {
		foreach($result as $res)
		  if($admin == $res)
			return(true);
	} elseif($result == $admin)
	  return(true);
}
// }}}

// {{{ pql_validate_object(linkid, dn, object, type)
// Verify that the object have what it needs to be added:
// 1. All object classes choosen have their MUST attributes defined.
// 2. All attributes have a object class specified (TODO).
// 3. The ACI attribute is defined in the object
// Returns a new, improved/correct object.
// NOTE: $dn and $type is only used/needed for the ACI's...
function pql_validate_object($linkid, $dn, $object, $type) {
	// {{{ Encode values that needs to encoded and extract the object classes
    foreach($object as $key => $value) {
		// Check if we have a MUST in the objectclasses choosen
		// for creation for this attribute...
		if(is_array($value)) {
			foreach($value as $val) {
				if(eregi(pql_get_define("PQL_ATTR_OBJECTCLASS"), $key))
				  // Remember the object class for later
				  $ocs[] = $val;
				else
				  $entry[$key][] = pql_maybe_encode(trim($val), $key, $linkid);
			}
		} else {
			if(eregi(pql_get_define("PQL_ATTR_OBJECTCLASS"), $key))
			  // Remember the object class for later
			  $ocs[] = $value;
			else			
			  $entry[$key] = pql_maybe_encode(trim($value), $key, $linkid);
		}
    }
	// }}}

	// {{{ Retreive all objectclasses the LDAP server knows about
	$ldap = pql_get_subschema($linkid, "objectclasses");
	// }}}

	// {{{ Go through the WANTED objectclasses, checking for a MUST attribute we don't have
	if(is_array($ocs)) {
	  foreach($ocs as $oc) {
		$oc = lc($oc);
		
		if(!$ldap[$oc]['MUST']['count'])
		  // An objectclass with no MUST, add this directly
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
		else {
		  // This objectclass have one or more MUST attributes
		  // Do we have any of those MUST attributes defined?
		  $MUST = $ldap[$oc]['MUST'];
		  
		  foreach($object as $attrib => $value) {
			$attrib = lc($attrib);
			
			// Go through the MUST attributes for this objctclass,
			// looking for an attribute we have NOT defined
			for($i=0; isset($MUST[$i]); $i++) {
			  $must = lc($MUST[$i]);
			  
			  if($attrib == $must)
				unset($MUST[$i]);
			}
			
			// Before we rearrange the array, remove the count
			// entry (othervise it gets renamed to a number).
			unset($MUST['count']);
			
			// Rearrange the array
			$MUST = pql_uniq($MUST);
			
			// Readd the count entry
			$MUST['count'] = count($MUST);
		  }
		  
		  if(!$MUST['count'])
			// This objectclass don't contain MUST attributes which
			// isn't defined in the user object. Add this to the list
			// of objectclasses to add
			$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
		  else {
			// Objectclass which have been removed from the list because
			// there's an MUST attribute missing
			echo "<b>Objectclass with missing attribute in MUST:</b> <u>$oc</u><br>";
			echo "You're missing the following attributes: ";
			for($i=0; $i < $MUST['count']; $i++) {
			  if(!$MUST[$i+1] and ($i != 0))
				echo "and ";
			  
			  echo "<i>".$MUST[$i]."</i>";
			  
			  if($MUST[$i+1])
				echo ", ";
			}
			echo "<p>";
		  }
		}
	  }
	}
	// }}}

	// {{{ Go through each attribute, verifying that we have an object class for this.
	foreach($object as $attrib => $value) {
		$attrib = lc($attrib);

		if(!eregi(pql_get_define("PQL_ATTR_OBJECTCLASS"), $attrib)) {
			// This is a non-object class attribute

			// TODO: How do we check that this attribute have a object class specified in $entry!?
		}
	}
	// }}}

	// {{{ Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci")) {
		if(eregi("ou=Templates,", $dn)) {
			// {{{ We're adding a template - special circumstances
			// Get all 'administrator's and add them to the READ list.
			global $_SESSION;
			foreach($_SESSION["BASE_DN"] as $base_dn)  {
				$admin_dns = pql_get_dn($linkid, urldecode($base_dn), pql_get_define("PQL_ATTR_ADMINISTRATOR").'=*');
				foreach($admin_dns as $admin_dn) {
					$admins = pql_get_attribute($linkid, urldecode($admin_dn), pql_get_define("PQL_ATTR_ADMINISTRATOR"));
					if(is_array($admins)) {
						for($j=0; $admins[$j]; $j++) {
							$admin = lc($admins[$j]);
							$acis[] = "#entry#grant;w,r,s,c,x;[all]#access-id#$admin";
						}
					} elseif($admins) {
						$admin = lc($admins);
						$acis[] = "#entry#grant;w,r,s,c,x;[all]#access-id#$admin";
					}
				}
			}

			// Sort and 'uniquify' the array of MAY attributes
			$acis = pql_uniq($acis);


			// Generate the REAL ACI list.
			$i = 1;

			// Add the 'normal' (public) ACI'.
			$entry[pql_get_define("PQL_ATTR_LDAPACI")][$i] = "$i#entry#grant;r,s,c;objectClass,[entry]#public#"; $i++;

			// Add the ACI's generated above, with the OID (aci counter) value.
			foreach($acis as $aci) {
				$entry[pql_get_define("PQL_ATTR_LDAPACI")][$i] = "$i$aci";
				$i++;
			}
			// }}}
		} else
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $dn, $type);
	}
	// }}}

	return($entry);
}
// }}}

// TODO: Verify that these functions are ok (using new API).

// {{{ pql_get_subschemas(ldap_linkid, type, match)
function pql_get_subschemas($ldap_linkid, $type = "", $match = "") {
    $attribs = array("ldapsyntaxes", "matchingrules", "attributetypes", "objectclasses");
    foreach($attribs as $attrib) {
		$result = pql_get_subschema($ldap_linkid, $attrib);
		if($result)
		  $entry[$attrib] = $result;
    }
	
	if($type and $match) {
		// We're looking for something special, look for it. If it doesn't
		// exists in the array -> return FALSE.
		if(! $entry[lc($type)][lc($match)])
		  return 0;
	}

	// Return 4 arrays with:
	//	ldapsyntaxes
	//	matchingrules
	//	attributetypes
	//	objectclasses
    return $entry;
}
// }}}

// {{{ pql_get_subschema(ldap_linkid, attrib)
function pql_get_subschema($ldap_linkid, $attrib) {
    $attrib = lc($attrib);
	
    // Get the DN for the subSchema
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' 'objectClass=*' subschemaSubentry
	//
	// TODO: Resue the $this->_find_base_option() function here!
    $sr    = ldap_read($ldap_linkid, NULL, '(objectClass=*)', array('subschemaSubentry'));
    $entry = ldap_get_entries($ldap_linkid, $sr);
    if($entry[0][pql_get_define("PQL_ATTR_SUBSCHEMASUBENTRY")][0]) {
		// Get the subSchemaAttributes from the subSchema
		// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b 'cn=Subschema' 'objectClass=subSchema' $attrib
		$sr     = ldap_read($ldap_linkid,
							$entry[0][pql_get_define("PQL_ATTR_SUBSCHEMASUBENTRY")][0],
							pql_get_define("PQL_ATTR_OBJECTCLASS").'=subSchema', array($attrib));
		$entry  = ldap_get_entries($ldap_linkid, $sr);
		for($i=0; $i < $entry[0][$attrib]["count"]; $i++) {
			// ------------------------
			// Retreive NAME
			$words = split("NAME", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				if(ereg("^ \(", $words[1])) {
					$words = split("\)", $words[1]);
					$words = ereg_replace("\'", "", $words[0]);
					$words = split(" ", $words);
					$name  = lc($words[2]);
					
					$VALUE[$name]["NAME"] = $words[2];
					for($j=3; $words[$j]; $j++) {
						$VALUE[$name]["ALIAS"][] = $words[$j];
					}
				} else {
					$words = split("\' ", $words[1]);
					$words = split(" \'", $words[0]);
					$name  = lc($words[1]);
					
					$VALUE[$name]["NAME"] = $words[1];
				}
			}
			
			// ------------------------
			// Get the OID number
			$words = split(" ", $entry[0][$attrib][$i]);
			$VALUE[$name]["OID"] = $words[1];
			
			// ------------------------
			// Retreive DESC
			$words = split("DESC", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				$words = split("\' ", $words[1]);
				$words = split(" \'", $words[0]);
				
				$VALUE[$name]["DESC"] = $words[1];
			}
			
			// ------------------------
			// Retreive MAY's
			$words = split("MAY", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				if(ereg("\(", $words[1])) {
					$words = split("\(", $words[1]);
					$words = split("\)", $words[1]);
				} else {
					$words = split("\)", $words[1]);
				}
				
				// Remove spaces
				$words = ereg_replace(" ", "", $words[0]);
				$words = split('\$', $words);
				
				$j = 0;
				foreach($words as $may) {
					$VALUE[$name]["MAY"][] = $may;
					$j++;
				}
				$VALUE[$name]["MAY"]["count"] = $j;
			}
			
			// ------------------------
			// Retreive MUST's
			$words = split("MUST", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				if(ereg("^ \(", $words[1])) {
					$words = split("\(", $words[1]);
					$words = split("\)", $words[1]);
					$word  = $words[0];
				} else {
					$words = split(" ", $words[1]);
					$word  = $words[1];
				}
				
				// Remove spaces
				$words = ereg_replace(" ", "", $word);
				$words = split('\$', $words);
				
				$j = 0;
				foreach($words as $must) {
					$VALUE[$name]["MUST"][] = $must;
					$j++;
				}
				$VALUE[$name]["MUST"]["count"] = $j;
			}
			
			// ------------------------
			// Retreive EQUALITY
			$words = split("EQUALITY", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				$words = split(" ", $words[1]);
				$VALUE[$name]["EQUALITY"] = $words[1];
			}
			
			// ------------------------
			// Retreive SYNTAX
			$words = split("SYNTAX", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				$words = split(" ", $words[1]);
				$VALUE[$name]["SYNTAX"] = $words[1];
			}
			
			// ------------------------
			// Retreive USAGE
			$words = split("USAGE", $entry[0][$attrib][$i]);
			if(! empty($words[1])) {
				$words = split(" ", $words[1]);
				$VALUE[$name]["USAGE"] = $words[1];
			}
			
			// ------------------------
			// Retreive SUP
			if(eregi("SUP", $entry[0][$attrib][$i])) {
				$words = split("SUP", $entry[0][$attrib][$i]);
				if(! empty($words[1])) {
					if(ereg("^ \(", $words[1])) {
						$words = split("\(", $words[1]);
						$words = split("\)", $words[1]);

						// Remove spaces
						$words = ereg_replace(" ", "", $words[0]);
						$words = split('\$', $words);

						for($j = 0; isset($words[$j]); $j++)
						  $VALUE[$name]["SUP"][] = $words[$j];
					} else {
						$words = split(" ",  $words[1]);

						// Remove spaces
						$VALUE[$name]["SUP"] = ereg_replace(" ", "", $words[1]);
					}
				}
			}

			// ------------------------
			// Retreive SINGLE-VALUE
			if(eregi("SINGLE-VALUE", $entry[0][$attrib][$i]))
			  $VALUE[$name]["SINGLE-VALUE"] = 'TRUE';
			
			// ------------------------
			// Retreive NO-USER-MODIFICATION
			if(eregi("NO-USER-MODIFICATION", $entry[0][$attrib][$i]))
			  $VALUE[$name]["NO-USER-MODIFICATION"] = 'TRUE';
			
			// ------------------------
			// Retreive X-BINARY-TRANSFER-REQUIRED
			if(eregi("X-BINARY-TRANSFER-REQUIRED", $entry[0][$attrib][$i]))
			  $VALUE[$name]["X-BINARY-TRANSFER-REQUIRED"] = 'TRUE';
			
			// ------------------------
			// Retreive X-NOT-HUMAN-READABLE
			if(eregi("X-NOT-HUMAN-READABLE", $entry[0][$attrib][$i]))
			  $VALUE[$name]["X-NOT-HUMAN-READABLE"] = 'TRUE';
		}

		/* If there's no anonymous read access to the 'cn=Subschema',
		 * phpQLAdmin will fail here...  */
		if(is_array($VALUE))
		  ksort($VALUE);

		return $VALUE;
    } else
	  return false;
}
// }}}

// {{{ pql_get_rootdn(dn)
function pql_get_rootdn($dn, $function = '') {
	global $_pql, $_SESSION;
	$tracker = 'Please report this at the <a href="http://bugs.bayour.com/" target="_new">bugtracker</a>.<br>';

	if($function)
	  $function = " (from $function)";

	if($dn == '')
	  die("This is weird. We're called$function with an empty DN! $tracker");
	else
	  $dn = urldecode($dn);

	if(is_array($_SESSION["BASE_DN"])) {
		$counts = 0;

		// Get all possible matches. If we're looking for 'uid=turbo,ou=People,o=Fredriksson,c=SE',
		// there is at least FOUR possibilities (one for each and every branch - they can all be
		// separate root DN's!
		foreach($_SESSION["BASE_DN"] as $base) {
			if(eregi("$base\$", $dn))  {
				$counts++;
				$DNs[] = $base;
			}
		}

		if(($counts < 1) and @$_SESSION["MONITOR_BACKEND_ENABLED"]) {
			// We have ONE more shot in finding the root DN. Look in the
			// 'cn=Databases,cn=Monitor:namingContexts' attribute(s).
			require($_SESSION["path"]."/include/pql_status.inc");
			$tmp = pql_get_status($_pql->ldap_linkid, "cn=Databases,cn=Monitor", 'namingContexts');
			if(is_array($tmp)) {
				// We got more than one namingContexts (i.e. an array). Go through each value...
				foreach($tmp as $base) {
					if(eregi("$base\$", $dn))  {
						$counts++;
						$DNs[] = $base;
					}
				}
			} elseif(eregi($tmp, $dn)) {
				// We've only got one namingContexts (i.e. not an array).
				$counts++;
				$DNs[] = $tmp;
			}
		}
		
		if($counts >= 2) {
			// We have more than one hit (there's at least TWO partitions)!!
			// -> Check each DN, looking for the DN with the HIGHEST amount of commas.
			//    That must be the DN that's closest to the DN we're looking for.
			foreach($DNs as $base)  {
				$dn_parts = explode(',', $base);
				$count = count($dn_parts); // Get amount of DN parts.
				
				// Remember this DN. It's above the DN we're looking for.
				$DN_REF[$base] = ($count) ? $count : 0;
			}
			
			// Again we have more than one. Which of these have the highest
			// amount of commas!?
			if($DN_REF) {
				asort($DN_REF); // Sort the DN's left. The LAST one have the most commas!
				
				// Just go through them one by one to get the last one...
				foreach($DN_REF as $base => $count)
				  $found = $base;
				
				// Return this DN. It MUST be the correct one!!
				return(urlencode($found));
			}

			// We just shouldn't end up here! "It can't happen" - famous last words :)
			die("You've ended up in a place where I didn't expect you to get... How exactly did you get here!? $tracker");
		} elseif($counts < 1) {
			// BUG! We don't have a hit!
			echo "This is weird. We couldn't find the root dn for some reason. The DN we're trying to find a root DN for is: ";
			echo "'$dn'.<p>$tracker<p>";
			echo "Note that you <u><b>MUST</b></u> include exact details on how you got this error.";
			echo "Every single mouse click etc - an URL won't suffice!";
			die();
		} else
		  $dn = $DNs[0];
	} else {
		// We don't have any base dn's. Maybe because
		// we're included from a file which doesn't
		// have a connection to the LDAP server.
		//
		// Find the root dn the 'old fasioned' (ie,
		// broken!) way.
		$rootdn = split(',', $dn);

		if($rootdn[1]) {
			unset($rootdn[0]);
			$dn = implode(",", $rootdn);
		} else
		  $dn = $rootdn[count($rootdn)-1];
	}

	return(urlencode($dn));
}
// }}}

// {{{ pql_setup_branch_objectclasses(filter = 0, dn, linkid = '')
// Setup object class entry for a domain/branch with all
// necessary objectclasses
function pql_setup_branch_objectclasses($filter = 0, $dn, $linkid = '') {
	if($dn and $linkid) {
		// Retreive existing objectClasses from the object
		$object = ldap_explode_dn($dn, 0);
		$sr     = @ldap_read($linkid, $dn, $object[0], array(pql_get_define("PQL_ATTR_OBJECTCLASS")));
		$ocs    = @ldap_get_entries($linkid, $sr) or pql_format_error(1);
		for($j=0; $j < $ocs[0][pql_get_define("PQL_ATTR_OBJECTCLASS")]["count"]; $j++) {
			if(eregi('phpQLAdminBranch', $ocs[0][pql_get_define("PQL_ATTR_OBJECTCLASS")][$j]))
				$got_phpqladmin_objectclass_branch = 1;

			$objectclass[] = $ocs[0][pql_get_define("PQL_ATTR_OBJECTCLASS")][$j];
		}
	} else {
		// No previous DN, we're probably creating a new

		// ObjectClasses for this root DN
		if($dn) {
			// What's the Root DN (namingContexts) for this domain
			$rootdn = pql_get_rootdn($dn, 'pql_setup_branch_objectclasses'); $rootdn = urldecode($rootdn);

			$ocs = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $rootdn);
		} elseif($_SESSION["BASE_DN"][0])
		  $ocs = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $_SESSION["BASE_DN"][0]);
		else
		  $ocs = '';

		$ocs = pql_split_oldvalues($ocs);
		foreach($ocs as $oc) {
			if($filter) {
				if(!eregi('dcOrganizationNameForm', $oc))
				  $entry .= "(objectClass=$oc)";
			} else {
				if(eregi('phpQLAdminBranch', $oc))
				  $got_phpqladmin_objectclass_branch = 1;

				$entry[] = $oc;
			}
		}
	}

	if($filter and $entry)
	  return($entry);

	if($dn and $linkid)
	  return($objectclass);

	if(!$got_phpqladmin_objectclass_branch and !$filter) {
		$entry[] = 'phpQLAdminBranch';
		for($i=0; $objectclass[$i]; $i++)
		  $entry[] = $objectclass[$i];
	}

	if(!count($entry))
	  // We have already setup the objectclasses, return NULL
	  return;
	else
	  // Return the objectClasses
	  return($entry);
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
