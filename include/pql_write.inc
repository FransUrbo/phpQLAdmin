<?php
// ----------------------------
// pql_write.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_write.inc,v 2.6 2004-11-12 09:49:49 turbo Exp $
//

// ------------------------------------------------
// Core WRITE API functions
// ------------------------------------------------

// {{{ pql_write_mod(linkid, dn, entry, caller)
// MODIFY an object - called with an 'ldif' for modification.
function pql_write_mod($linkid, $dn, $entry, $caller = NULL, $count = 0) {
	$count++;

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif($caller, $dn, $entry);
	if(file_exists("./.DEBUG_ME"))
	  echo $LDIF;
	else {
		// Modify this object
		if(!ldap_mod_replace($linkid, $dn, $entry)) {
			// Modify failed. Why? Can't support every single
			// failure, but some I CAN rectify (I think :).

			// TODO: In some cases I've ldap_mod_del() the attribute
			//       and then ldap_mod_add() it...

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
				
				// Recursion... Only try five times...
				if($count >= 6) { // The very first time $count=1, so...
					$msg = "We're looping in pql_write.inc:pql_write_mod()";
					if($caller)
					  $msg .= " (from $caller)";
					$msg .= ". No point in trying again. I've got errno=65 (Object class violation) every time...<br>";
					$msg .= "This is the LDIF I'm trying to write to the LDAP server. Please report this problem at the ";
					$msg .= '<a href="http://bugs.bayour.com/" target="_new">bugtracker</a>.';

					die("$msg<p>".$LDIF);
				} else
				  pql_write_mod($linkid, $dn, $entry, $caller, $count);
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
// (ezmlmAdministrator, controlsAdministrator and seeAlso) value(s)
// when a user is deleted or renamed.
function pql_replace_values($ldap, $attribs, $old, $new = NULL) {
	// Setup the search filter
	$filter = '(|';
	if(is_array($attribs)) {
		foreach($attribs as $attrib)
		  $filter .= "($attrib=$old)";
	} else
	  $filter .= "($attribs=$old)";
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
			  $object[$attrib][] = pql_maybe_encode($new[$i], $attrib, $linkid);
		} else
		  $object = $new;
	} else {
		// Get all the old attribute values first.
		$entry = pql_get_attribute($linkid, $dn, $attrib);
		if($entry) {
			if(!$old and $new) {
				// Add old values to the new array.
				if(!is_array($entry))
				  $object[$attrib][] = pql_maybe_encode($entry, $attrib, $linkid);
				else {
					for($i=0; $entry[$i]; $i++)
					  $object[$attrib][] = pql_maybe_encode($entry[$i], $attrib, $linkid);
				}
				
				// Add the $new value to attribute
				$object[$attrib][] = pql_maybe_encode($new, $attrib, $linkid);
			} else {
				if($old == 1) {
					// Replace the existing attribute value
					$object[$attrib][] = pql_maybe_encode($new, $attrib, $linkid);
				} elseif(!$old) {
					// We're called without both $old and $new - delete whole attribute
					$object[$attrib] = array();
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
						  $object[$attrib][] = pql_maybe_encode($value, $attrib, $linkid);
					}
				}
			}
		} elseif($new)
		  // No previous attribute value, add $new
		  $object[$attrib][] = pql_maybe_encode($new, $attrib, $linkid);
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

// ------------------------------------------------
// These functions should REALLY be revised for rewrite.
// They're WAY to complicated!
// ------------------------------------------------

// {{{ pql_user_add(ldap_linkid, domain, cn, attribs, account_type, branch)
// Add a user to the LDAP database
function pql_user_add($ldap_linkid, $domain, $cn, $attribs, $account_type, $branch) {
	global $account_type;

	// -----
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($domain, 'pql_user_add'); $rootdn = urldecode($rootdn);

	// -----
	if($account_type == "group") {
	  $ocs[] = 'person';		// OpenLDAP version >2.2 REQUIRE one structural objectclass.
	  $ocs[] = 'qmailgroup';
	  $ocs[] = 'posixAccount';	// We need the uid attribute (etc);
	} else {
		// Retreive WANTED objectclasses
		if(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn))
		  foreach(pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn)) as $oc)
			$ocs[] = $oc;
		else
		  $ocs[] = '';
	}

	// -----
	// Setup user entry variable
    foreach($attribs as $key => $value) {
		// Check if we have a MUST in the objectclasses choosen
		// for creation for this attribute...
		if(is_array($value)) {
			foreach($value as $val)
			  $entry[$key][] = pql_maybe_encode(trim($val), $key, $ldap_linkid);
		} else
		  $entry[$key] = pql_maybe_encode(trim($value), $key, $ldap_linkid);
    }

	// -----
	// Retreive all EXISTING objectclasses in the LDAP server
	$ldap = pql_get_subschema($ldap_linkid, "objectclasses");

	// Go through the WANTED objectclasses, checking for a
	// MUST attribute we don't have
	foreach($ocs as $oc) {
		if(!$ldap[$oc]['MUST']['count'])
		  // An objectclass with no MUST, add this directly
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
		else {
			// This objectclass have one or more MUST attributes
			// Do we have any of those MUST attributes defined?
			$MUST = $ldap[$oc]['MUST'];

			foreach($entry as $attrib => $value) {
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
				echo "<b>Objectclass with missing attribute in MUST:</b> <u>$oc</u>";
				printr($MUST);
			}
		}
	}

	// -----
	// Create the RDN for the user
	if($entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)])
	  $userdn = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . "=" .
		$entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)] . ",";
	elseif($entry[pql_get_define("PQL_ATTR_UID")])
	  $userdn = pql_get_define("PQL_ATTR_UID")."=".$entry[pql_get_define("PQL_ATTR_UID")].",";
	else {
		echo "I can't figure out how to name the RDN!<br>";
		echo "Both the user reference value (".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn);
		echo ") and the backup reference (".pql_get_define("PQL_ATTR_UID").") is empty...<br>";
		die("I have no idea how to continue from here...");
	}

	if($branch)
	  $userdn .= urldecode($branch);
	else
	  $userdn .= urldecode($domain);
	$userdn = pql_maybe_encode($userdn);

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid, $userdn, 'user');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_user_add - user creation", $userdn, $entry);

	if(file_exists("./.DEBUG_ME"))
	  die($LDIF);

	// -----
    // Add the user to the database
    if(! ldap_add($ldap_linkid, $userdn, $entry)) {
		// failed to add user
		$failed = 1; pql_format_error(1);

		$errno = ldap_errno($ldap_linkid);
		if($errno == 32) {
			// No such object - try adding the ou before it
			if($branch)
			  $ou_rdn .= urldecode($branch);
			else
			  $ou_rdn .= pql_get_define("PQL_CONF_SUBTREE_USERS").",".urldecode($domain);
			$ou_rdn = pql_maybe_encode($ou_rdn);
			
			$reg = split("=", pql_get_define("PQL_CONF_SUBTREE_USERS"));
			$ou_entry[$reg[0]] = $reg[1];
			$ou_entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'organizationalUnit';
			
			// Create a LDIF object to print in case of error
			$ou_LDIF = pql_create_ldif("pql_user_add - user creation/ou", $ou_rdn, $ou_entry);
			if(! ldap_add($ldap_linkid, $ou_rdn, $ou_entry)) {
				echo "I've tried to add the user object, but that failed because of error 32 - No such object. ";
				echo "The idea was that it was missing the organizational unit container preceding it in the path. ";
				echo "That didn't work either. I'm stumped...<br>";

				echo "$ou_LDIF<p>";
				$failed = 1;
			} else {
				// Adding the ou container worked! Try adding the user again.
				$failed = 0;
				if(! ldap_add($ldap_linkid, $userdn, $entry))
				  $failed = 1;
			}
		}
		
		if($failed) {
			pql_format_error(1);
			die("$LDIF");
		}
	}
	$dns[]  = $userdn;
	echo "Added $userdn<br>";
    
	// -----
	// If this is a 'system' account, create the Group object
	if($account_type == "system" and $entry[pql_get_define("PQL_ATTR_QMAILGID")]) {
		// Remember this for the NEW entry object...
		$gidnr = $entry[pql_get_define("PQL_ATTR_QMAILGID")];
		
		// Create a new LDAP object
		$entry = "";
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "top";
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "posixGroup";
		$entry[pql_get_define("PQL_ATTR_CN")] = $attribs[pql_get_define("PQL_ATTR_UID")];
		$entry[pql_get_define("PQL_ATTR_QMAILGID")] = $gidnr;
		
		if(pql_get_define("PQL_CONF_SUBTREE_GROUPS"))
		  $subrdn = "," . pql_get_define("PQL_CONF_SUBTREE_GROUPS");

		$groupdn = pql_get_define("PQL_ATTR_CN") . "=" . $attribs[pql_get_define("PQL_ATTR_USER_RDN_ATTRIBUTE", $rootdn)] . $subrdn . "," . urldecode($domain);

		// Add the OpenLDAPaci attribute (maybe)
		if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci"))
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid, $_SESSION["USER_DN"], 'group');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_user_add - group creation", $groupdn, $entry);

		// --------------------------
		if(! ldap_add($ldap_linkid, $groupdn, $entry)) {
			// failed to add user
			pql_format_error(1);
			die("$LDIF");
			return $dns;
		}
		$dns[] = $groupdn;
		echo "Added $groupdn<br>";
	}
	
    return $dns;
}

// }}}

// {{{ pql_user_del(ldap, domain, user, delete_forwards)
// Delete a user from the LDAP database
function pql_user_del($ldap, $domain, $user, $delete_forwards) {
	$linkid = $ldap->ldap_linkid;

	if(!pql_get_dn($ldap->ldap_linkid, $user, '(objectclass=*)', 'BASE'))
	  // user does not exist
	  return false;

	// Remove all administrator entries which contain the user DN
	foreach($ldap->ldap_basedn as $dn) {
		$dn = urldecode($dn);

		$sr = ldap_search($linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR")."=$user");
		$info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
		for($i=0; $i<$info["count"]; $i++) {
			unset($entry); unset($adms);

			// Get administrator attributes for this domain/branch DN
			$admins	= pql_get_attribute($ldap->ldap_linkid, $info[$i]["dn"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
			for($j=0; $admins[$j]; $j++) {
				if($admins[$j] != $user)
				  $adms[] = $admins[$j];
			}
				  
			if(is_array($adms)) {
				// Add the administrators that's left to the DN
				$entry[pql_get_define("PQL_ATTR_ADMINISTRATOR")] = $adms;
				if(! ldap_mod_replace($linkid, $info[$i]["dn"], $entry))
				  pql_format_error(1);
			}
		}
	}

	// Get uidnr of user
	$uidnr = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_QMAILUID"));
	$uidnr = $uidnr[0];
    
	// Delete the group object if it exists
	$filter = "(&(".pql_get_define("PQL_ATTR_QMAILGID")."=$uidnr)(objectclass=posixGroup))";
	$sr = ldap_search($linkid, $domain, $filter);
	if(ldap_count_entries($linkid, $sr) > 0){
		$ed = ldap_first_entry($linkid, $sr);
		$dn = ldap_get_dn($linkid, $ed);
		
		// delete the group
		ldap_delete($linkid, $dn);
	}

    // we delete the forwards to this user as they don't really make sense anymore
    if ($delete_forwards) {
		// does another account forward to this one?
		$forwarders = pql_search_forwarders($ldap, $user);
		if ($forwarders) {
			// someone forwards to this user. Now we need to know which addresses we're removing
			$email = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_MAIL"));
			$aliases = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_MAILALTERNATE"));
			
			$addresses[] = $email[0];
			if(is_array($aliases)){
				$addresses = array_merge($addresses, $aliases);
			}
		}
    }
    
    // delete the user
    if(!ldap_delete($linkid, $user)){
		pql_format_error(1);
		return false;
    }
    
    // user entry has been removed -> remove the cached version
    pql_cache_userentry_remove($user);
    
    // delete forwards to this account?
    if ($delete_forwards and $forwarders) {
		foreach($forwarders as $forward) {
			// get the forwarding addresses of this user
			$fwd_addresses = pql_get_attribute($linkid, $forward['reference'], pql_get_define("PQL_ATTR_FORWARDS"));
			foreach($addresses as $address) {
				// does this user forward to the removed user or one of his aliases?
				$rem_key = array_search($address, $fwd_addresses);
				if ($rem_key !== false) {
					// we found a forward -> remove it 
					pql_modify_attribute($linkid, $forward['reference'], pql_get_define("PQL_ATTR_FORWARDS"), '',
										 $fwd_addresses[$rem_key]);
										 
				}
			}
		} 
    }

    return true;
}

// }}}

// {{{ pql_domain_add(ldap_linkid, basedn, branch_name)
// Adds a domain into ldap-tree (o|ou|dc-record)
function pql_domain_add($ldap_linkid, $basedn, $branch_name) {
	// Maybe UTF-8 encode the branch name - to allow international characters
	$branch_name = pql_maybe_encode($branch_name, pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn), $ldap_linkid);

	$entry[pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)] = $branch_name;
	$dn = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) . "=" . $branch_name . "," . $basedn;
	
	// Setup objectclasses
	$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $basedn);
	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "o")
	  $entry[pql_get_define("PQL_ATTR_O")] = pql_maybe_encode($branch_name);
	else
	  $entry[pql_get_define("PQL_ATTR_O")] = 0;

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid, $_SESSION["USER_DN"], 'branch');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_domain_add - create domain", $dn, $entry);

	if(file_exists("./.DEBUG_ME"))
	  die($LDIF);

    // adds the user
    if(! @ldap_add($ldap_linkid, $dn, $entry)) {
		// Failed to add branch
		pql_format_error(1);
		die("$LDIF");
		return false;
    }
	$dns[] = $dn;

    // Add the USER subtree if defined
    if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
		$ou = split('=', pql_get_define("PQL_CONF_SUBTREE_USERS"));
		if(pql_unit_add($ldap_linkid, $dn, $ou[1]))
		  $dns[] = pql_get_define("PQL_CONF_SUBTREE_USERS").','.$dn;
    }
    
    // Add the GROUPS subtree if defined
    if(pql_get_define("PQL_CONF_SUBTREE_GROUPS")) {
		$ou = split('=', pql_get_define("PQL_CONF_SUBTREE_GROUPS"));
		if(pql_unit_add($ldap_linkid, $dn, $ou[1]))
		  $dns[] = pql_get_define("PQL_CONF_SUBTREE_GROUPS").','.$dn;
    }

    return $dns;
}

// }}}

// {{{ pql_domain_del(ldap, domain, delete_forwards)
// Removes a domain with all listed users
function pql_domain_del($ldap, $domain, $delete_forwards) {
	$linkid = $ldap->ldap_linkid;

	// Make sure that the logged in user isn't located under the
	// domain/branch being deleted...
	if(eregi($domain, $_SESSION["USER_DN"])) {
		$msg=urlencode(pql_format_error_span("Sorry, I can't allow you to delete the branch under which you yourself is situated!"));
		header("Location: " . pql_get_define("PQL_CONF_URI") . "domain_detail.php?domain=$domain&msg=$msg");
	}

	// Searching for sub entries
	$sr   = ldap_list($linkid, $domain, pql_get_define("PQL_ATTR_OBJECTCLASS")."=*");
	$info = ldap_get_entries($linkid, $sr);
	for($i=0; $i < $info['count']; $i++){
		// Deleting recursively this sub entry
		$result = pql_domain_del($ldap, $info[$i]['dn'], $delete_forwards);
		if(!$result) {
			// Return result code if delete fails
			return($result);
		}
	}

	return(ldap_delete($linkid, $domain));
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
