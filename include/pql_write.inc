<?php
// ----------------------------
// pql_write.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_write.inc,v 2.1 2004-10-19 10:40:42 turbo Exp $
//

// ------------------------------------------------
// Core WRITE API functions
// ------------------------------------------------

// {{{ pql_write_mod(linkid, dn, entry, caller)
// MODIFY an object - called with an 'ldif' for modification.
function pql_write_mod($linkid, $dn, $entry, $caller = NULL) {
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif($caller, $dn, $entry);
	if(file_exists("./.DEBUG_ME"))
	  die($LDIF);
	
	// Modify this object
	if(!ldap_mod_replace($linkid, $dn, $entry)) {
		// Modify failed. Why? Can't support every single
		// failure, but some I CAN rectify (I think :).
		$errno = ldap_errno($linkid);
		switch($errno) {
		  case "65":
			// Object class violation - try to add (the) missing objectclass(es).
			
			// Get the current object classes in this object
			$old = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_OBJECTCLASS"));
			
			// Try to find which object classes to ADD.
			$new = pql_missing_objectclasses($linkid, $dn, $entry);
			
			// Combine the old with the new.
			$ocs = $old;
			for($i=0; $new[$i]; $i++)
			  $ocs[] = $new[$i];
			
			$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_uniq($ocs);
			
			// Recursion... Hopfully there's a limit on how many (max_execution_time in php.ini)!
			pql_write_mod($linkid, $dn, $entry, $caller);
			break;

//		  case "17": // Undefined attribute type
//			break;
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
			die($LDIF);
		}
	}

	return(true);
}
// }}}

// ------------------------------------------------
// Support SEARCH API functions
// ------------------------------------------------

// {{{ pql_replace_values(ldap, attribs, old, new)
// Look for attribs=old,
// - replace with attribs=new
// - or (if 'new' is empty/null), remove the value.
//
// This function is a little more powerfull than the others.
// It will do the modification on ALL objects found, in ALL
// the backends!
// It's primary design/goal was to modify the administrator
// value(s) when a user is deleted.
function pql_replace_values($ldap, $attribs, $old, $new = NULL) {
	// Setup the search filter
	$filter = '(|';
	foreach($attribs as $attrib)
	  $filter .= "($attrib=$old)";
	$filter .= ')';

	// Go through the namingContexts one by one
	foreach($ldap->ldap_basedn as $dn) {
		// Get all objects that have this object ($old) in any of the $attribs attributes.
		$result = pql_search($ldap->ldap_linkid, $dn, $filter);
		for($i=0; $result[$i]; $i++) {
			unset($dn); unset($entry); unset($LDIF);
			
			// Go through the attributes we're interested in, looking for
			// the (old) object DN
			foreach($attribs as $attrib) {
				if(is_array($result[$i][$attrib])) {
					for($j=0; $result[$i][$attrib][$j]; $j++) {
						// TODO: Only replace the attribute(s) if it have changed
						if($old == $result[$i][$attrib][$j]) {
							// Got a match ...
							if($new)
							  // ... we have something to replace with - replace this attribute
							  $entry[$attrib][] = $new;
						} else
						  // Remember the old value
						  $entry[$attrib][] = $result[$i][$attrib][$j];
					}
				} elseif($result[$i][$attrib])
				  $entry[$attrib][] = $result[$i][$attrib];
			}
			
			pql_write_mod($ldap->ldap_linkid, $result[$i]["dn"], $entry, "pql_replace_values");
		}
	}
}

// }}}

// {{{ pql_modify_attribute(linkid, dn, attrib, old, new)
// Delete, Replace or Add a value to an attribute list
// * If $old != '' && $new != '' -> replace $old with $new
// * If $old != '' && $new == '' -> delete $old
// * If $old == '' && $new == '' -> delete whole attribute
// * If $old == '' && $new != '' -> add $new
// * If $old == 1  && $new != '' -> replace existing value with $new
// * If $new == array            -> called with 'ldif', replace
//   + In this case, it is ok with an empty $attrib.
function pql_modify_attribute($linkid, $dn, $attrib, $old, $new) {
	if(!is_array($new)) {
		$attrib = lc($attrib);
		$object[$attrib] = array();
	}

	// {{{ Recreate an 'ldif' we can send to the LDAP server
	if(is_array($new)) {
		if($attrib) {
			for($i=0; $new[$i]; $i++)
			  $object[$attrib][] = $new[$i];
		} else
		  $object = $new;
	} else {
		// Get all the old attribute values first.
		$entry = pql_get_attribute($linkid, $dn, $attrib);
		if($entry) {
			if(!$old and $new) {
				// Add old values to the new array.
				if(!is_array($entry))
				  $object[$attrib][] = $entry;
				else {
					for($i=0; $entry[$i]; $i++)
					  $object[$attrib][] = $entry[$i];
				}
				
				// Add the $new value to attribute
				$object[$attrib][] = $new;
			} else {
				if($old == 1) {
					// Replace the existing attribute value
					$object[$attrib][] = $new;
				} else {
					// Replace $old with $new
					if(!is_array($entry)) {
						// Not an array - one value only! Make it simple - convert
						// to array...
						$tmp = $entry;
						unset($entry);
						$entry = array($tmp);
					}
					
					foreach($entry as $value) {
						if($value == $old) {
							if($new)
							  // replace $old with $new
							  $value = $new;
							else
							  // delete $old value from attribute
							  unset($value);
						}
						
						if($value)
						  $object[$attrib][] = $value;
					}
				}
			}
		} elseif($new)
		  // No previous attribute value, add $new
		  $object[$attrib][] = $new;
	}
	// }}}

	// {{{ Do modifications
	if(pql_write_mod($linkid, $dn, $object, 'pql_modify_attribute')) {
		// Object has changed -> remove the cached version
		pql_cache_userentry_remove($dn);

		// What's the Root DN (namingContexts) of this user
		$rootdn = pql_get_rootdn($dn, 'pql_modify_attribute'); $rootdn = urldecode($rootdn);
		if($attrib == pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn))
		  pql_cache_userdn_remove($dn);
		
		return true;
	}
	return false;
	// }}}
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
