<?php
// ----------------------------
// pql_write.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_write.inc,v 2.18 2005-02-05 13:54:08 turbo Exp $
//

// ------------------------------------------------
// WRITE API functions - Core
// ------------------------------------------------

// {{{ pql_write_add(linkid, dn, entry, type, caller)
// ADD an object - called with an 'ldif' for modification.
function pql_write_add($linkid, $dn, $entry, $type, $caller) {
	// Verify that the object have what it needs to be able to be added
	$entry = pql_validate_object($linkid, $dn, $entry, $type);

	// -----
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif($caller, $dn, $entry);
	if(file_exists("./.DEBUG_ME"))
	  echo $LDIF;
	else {
		// -----
		// Add this object to the database
		if(! ldap_add($linkid, $dn, $entry)) {
			// Failed!
			$errno = ldap_errno($linkid);
			$error = ldap_error($linkid);

			// TODO: Error checking. Some problems might be salvable...
			switch($errno) {
			  case 68:
				// Already exists. Fine, return true (?)
				return(true);
				break;
			}
			
			pql_format_error(1);
			echo "Error: #$errno ($error)<p>";
			die($LDIF);
		}
	}

	return(true);
}

// }}}

// {{{ pql_write_mod(linkid, dn, entry, caller)
// MODIFY an object - called with an 'ldif' for modification.
function pql_write_mod($linkid, $dn, $entry, $caller = NULL, $count = 0) {
	$dn = urldecode($dn);
	if(eregi("^dNSTTL=", $dn))
	  $dn = pql_recreate_dnsttl($dn);

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
					$msg = "<p>We're looping in pql_write.inc:pql_write_mod()";
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

// {{{ pql_write_ren(linkid, old, parent, new, keepold)
function pql_write_ren($linkid, $old, $parent, $new, $keepold = TRUE) {
  if(file_exists("./.DEBUG_ME")) {
	echo "Renaming<br>'<b>$old</b>'<br>to<br>'<b>$new,$parent</b>'<br>";
	return true;
  } else {
	if(ldap_rename($linkid, $old, $new, $parent, $keepold))
	  return true;
  }

  return false;
}
// }}}

// ------------------------------------------------
// WRITE API functions - Support
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
	foreach($_SESSION["BASE_DN"] as $dn) {
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
// * If $old == 1  && $new == '' -> delete whole attribute
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
					if($new)
					  $object[$attrib][] = pql_maybe_encode($new, $attrib, $linkid);
					else
					  // No new attribute. Probably because we're following a delete URL
					  $object[$attrib] = array();
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

// {{{ pql_user_add(ldap_linkid, domain, dn, entry, type, branch)
// Add a user to the LDAP database
function pql_user_add($ldap_linkid, $domain, $dn, $entry, $type, $branch) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($domain, 'pql_user_add'); $rootdn = urldecode($rootdn);

	// {{{ Setup the objectclasses
	if($type == "group") {
		// OpenLDAP version >2.2 REQUIRE one structural objectclass.
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'person';

		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'qmailgroup';

		// We need the uid attribute (etc);
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'posixAccount';
	} else {
		if(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn)) {
			foreach(pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn)) as $oc) {
				// BUG: I really hate this. No hardcoding!!
				if((($type == 'mail') and ($oc == 'krb5principal')) or
				   (($type == 'mail') and ($oc == 'trustaccount')))
				  next;
				else
				  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
			}
		}
	}
	// }}}

    // {{{ Add the user to the database
    if(!pql_write_add($ldap_linkid, $dn, $entry, $type, 'pql_user_add - user creation/1')) {
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
			if(!pql_write_add($ldap_linkid, $ou_rdn, $ou_entry, '', 'pql_user_add - user creation/ou')) {
				echo "I've tried to add the user object, but that failed because of error 32 - No such object. ";
				echo "The idea was that it was missing the organizational unit container preceding it in the path. ";
				echo "That didn't work either. I'm stumped...<br>";

				$failed = 1;
			} else {
				// Adding the ou container worked! Try adding the user again.
				$failed = 0;
				if(!pql_write_add($ldap_linkid, $dn, $entry, $type, 'pql_user_add - user creation/2'))
				  $failed = 1;
			}
		}
		
		if($failed) {
			pql_format_error(1);
			die();
		}
	}
	$dns[]  = $dn;
	echo "Added $dn<br>";
	// }}}
    
	// {{{ If this is a 'system' account, create the Group object
	if($type == "system" and $entry[pql_get_define("PQL_ATTR_QMAILGID")]) {
		// Remember this for the NEW entry object...
		$gidnr   = $entry[pql_get_define("PQL_ATTR_QMAILGID")];
		$rdnattr = $entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)];
		
		// Create a new LDAP object
		$entry = array();
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = "posixGroup";
		$entry[pql_get_define("PQL_ATTR_CN")]          = $rdnattr;
		$entry[pql_get_define("PQL_ATTR_QMAILGID")]    = $gidnr;
		
		if(pql_get_define("PQL_CONF_SUBTREE_GROUPS"))
		  $subrdn = "," . pql_get_define("PQL_CONF_SUBTREE_GROUPS");

		$groupdn = pql_get_define("PQL_ATTR_CN")."=$rdnattr".$subrdn.','.urldecode($domain);

		// --------------------------
		if(!pql_write_add($ldap_linkid, $groupdn, $entry, 'group', 'pql_user_add - group creation')) {
			// failed to add user
			pql_format_error(1);
			die();
		}
		$dns[] = $groupdn;
		echo "Added $groupdn<br>";
	}
	// }}}
	
	if(file_exists("./.DEBUG_ME")) {
		echo "DNs added: ";
		printr($dns);
		die();
	}

    return $dns;
}

// }}}

// ------------------------------------------------
// These functions should REALLY be revised for rewrite.
// They're WAY to complicated!
// ------------------------------------------------

// {{{ pql_user_del(ldap, domain, user, delete_forwards)
// Delete a user from the LDAP database
function pql_user_del($ldap, $domain, $user, $delete_forwards) {
	$linkid = $ldap->ldap_linkid;

	if(!pql_get_dn($ldap->ldap_linkid, $user, '(objectclass=*)', 'BASE'))
	  // user does not exist
	  return false;

	// Remove all administrator entries which contain the user DN
	foreach($_SESSION["BASE_DN"] as $dn) {
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
