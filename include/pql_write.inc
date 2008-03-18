<?php
// ----------------------------
// pql_write.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_write.inc,v 2.54 2008-03-18 22:38:52 turbo Exp $
//

// ------------------------------------------------
// WRITE API functions - Support
// ------------------------------------------------

// {{{ pql_replace_values(attribs, old, new)
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
function pql_replace_values($attribs, $old, $new = NULL) {
  global $_pql;
  
  // Setup the search filter
  $filter = '(|';
  if(is_array($attribs)) {
	foreach($attribs as $attrib)
	  $filter .= "($attrib=$old)";
  } else
	$filter .= "($attribs=$old)";
  $filter .= ')';
  
  // Go through the namingContexts one by one
  $error = 0;
  foreach($_SESSION["BASE_DN"] as $dn) {
	// Get all objects that have this object ($old) in any of the $attribs attributes.
	$result = $_pql->search($dn, $filter);
	if(is_array($result)) {
	  for($i=0; $i < count($result); $i++) {
		unset($dn); unset($entry); unset($LDIF);
		
		// Go through the attributes we're interested in, looking for
		// the (old) object DN
		foreach($attribs as $attrib) {
		  if(is_array($result[$i][$attrib])) {
			for($j=0; $j < count($result[$attrib]); $j++) {
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
		
		if(!$_pql->modify($result[$i]["dn"], $entry, "pql_replace_values"))
		  $error = 1;
	  }
	}
  }
  
  if($error)
	die();
  else
	return true;
}

// }}}

// {{{ pql_modify_attribute(dn, attrib, old, new)
// Delete, Replace or Add a value to an attribute list
// * If $old != '' && $new != '' -> replace $old with $new
// * If $old != '' && $new == '' -> delete $old
// * If $old == '' && $new == '' -> delete whole attribute
// * If $old == '' && $new != '' -> add $new
// * If $old == 1  && $new != '' -> replace existing value with $new
// * If $old == 1  && $new == '' -> delete whole attribute
// * If $new == array            -> called with 'ldif', replace
//   + In this case, it is _a must_ (!) with an empty '$attrib' value.
function pql_modify_attribute($dn, $attrib, $old, $new) {
  global $_pql, $pql_cache;
  
  if(!is_array($new)) {
	$attrib = lc($attrib);
	$object[$attrib] = array();
  }
  
  // {{{ Recreate an 'ldif' we can send to the LDAP server
  if(is_array($new)) {
	if($attrib) {
	  for($i=0; $i < count($new); $i++)
		$object[$attrib][] = pql_maybe_encode($new[$i], $attrib, $linkid);
	} else
	  $object = $new;
  } else {
	// Get all the old attribute values first.
	$entry = $_pql->get_attribute($dn, $attrib);
	if($entry) {
	  if(!$old and $new) {
		// Add old values to the new array.
		if(!is_array($entry))
		  $object[$attrib][] = pql_maybe_encode($entry, $attrib, $linkid);
		else {
		  for($i=0; $i < count($entry); $i++)
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
  if($_pql->modify($dn, $object, 'pql_modify_attribute')) {
	if((lc($dn) == lc($_SESSION["USER_DN"])) and ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PASSWD")))) {
	  // Object has changed && we've changed our own password -> update the password in the session!
	  $_SESSION["USER_PASS"] = $_SESSION["NEW_PASS"];
	  unset($_SESSION["NEW_PASS"]);
	}

	return true;
  }
  return false;
  // }}}
}
// }}}

// {{{ pql_user_add(domain, dn, entry, type, branch)
// Add a user to the LDAP database
function pql_user_add($domain, $dn, $entry, $type, $branch) {
  global $_pql;

  if(pql_get_define("PQL_CONF_DEBUG_ME"))
	echo "<b>pql_user_add('$domain', '$dn', '$entry', '$type', '$branch')</b><br>";

  // What's the Root DN (namingContexts) for this domain
  $rootdn = pql_get_rootdn($domain, 'pql_user_add');
  $rootdn = urldecode($rootdn);

  // {{{ Setup the objectclasses
  if(empty($entry[pql_get_define("PQL_ATTR_OBJECTCLASS")])) {
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
		  // BUG 2: Even worse: I don't even remember why I did this in the first place!
		  if((($type == 'mail') and ($oc == 'krb5principal')) or
			 (($type == 'mail') and ($oc == 'trustaccount')))
			next;
		  else
			$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = $oc;
		}
	  }
	}
  }
  // }}}
  
  // {{{ Add the user to the database
  if(!$_pql->add($dn, $entry, $type, 'pql_user_add - user creation/1')) {
	// failed to add user
	$failed = 1; pql_format_error(1);
	
	$errno = ldap_errno($_pql->ldap_linkid);
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
	  if(!$_pql->add($ou_rdn, $ou_entry, $type, 'pql_user_add - user creation/ou')) {
		echo "I've tried to add the user object, but that failed because of error 32 - No such object. ";
		echo "The idea was that it was missing the organizational unit container preceding it in the path. ";
		echo "That didn't work either. I'm stumped...<br>";
		
		$failed = 1;
	  } else {
		// Adding the ou container worked! Try adding the user again.
		$failed = 0;
		if(!$_pql->add($dn, $entry, $type, 'pql_user_add - user creation/2'))
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
  
  // {{{ If this is a 'posixAccount' object, create the Group object
  if(in_array('posixaccount', $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")]) and 
	 $entry[pql_get_define("PQL_ATTR_QMAILGID")]) 
	{
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
	  
	  if($branch)
		$groupdn = pql_get_define("PQL_ATTR_CN")."=$rdnattr".$subrdn.','.$branch;
	  else
		$groupdn = pql_get_define("PQL_ATTR_CN")."=$rdnattr".$subrdn.','.urldecode($domain);
	  // --------------------------
	  if(!$_pql->add($groupdn, $entry, 'group', 'pql_user_add - group creation')) {
		// failed to add user
		pql_format_error(1);
		die();
	  }
	  $dns[] = $groupdn;
	  echo "Added $groupdn<br>";
	}
  // }}}
  
  return $dns;
}

// }}}

// {{{ pql_add_computer(hostname, ip)
function pql_add_computer($hostname, $ip) {
  global $_pql;

  $objectclasses = pql_get_subschema('objectclasses');
  
  $dn = pql_get_define("PQL_ATTR_CN") . "=" . $hostname . "," . $_SESSION["USER_SEARCH_DN_CTR"];
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'ipHost';
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'groupOfUniqueNames';
  if(array_key_exists('dhcpserver', $objectclasses))
	// Don't have any MUST, so it's safe to add if it's availible
	$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'dhcpServer';
  $entry[pql_get_define("PQL_ATTR_CN")] = $hostname;
  $entry[pql_get_define("PQL_ATTR_IPHOSTNUMBER")] = $ip;
  
  //The Current admin adding this entry is auto added as the first
  //uniqueMember to satisfy the must requirements of groupofuniquenames
  $entry[pql_get_define("PQL_ATTR_UNIQUE_GROUP")] = $_SESSION['USER_DN'];
  
  if(!$_pql->add($dn, $entry, 'host', 'pql_add_computer')) {
	// Failed to add user
	$failed = 1;
	pql_format_error(1);
	
	$errno = ldap_errno($ldap_linkid);
	
	if($failed)
	  pql_format_error(1);
  }
  
  return(true);
}
// }}}

// ------------------------------------------------
// These functions should REALLY be revised for rewrite.
// They're WAY to complicated!
// ------------------------------------------------

// {{{ pql_user_del(domain, user, delete_forwards)
// Delete a user from the LDAP database
function pql_user_del($domain, $user, $delete_forwards) {
  global $_pql, $pql_cache;
  
  // Normalize the User and Domain DN
  $user = pql_format_normalize_dn($user);
  $domain = pql_format_normalize_dn($domain);
  
  if(!$_pql->get_dn($user, '(objectclass=*)', 'BASE'))
	// user does not exist
	return false;
  
  // Remove all administrator entries which contain the user DN
  foreach($_SESSION["BASE_DN"] as $dn) {
	$sr = ldap_search($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR")."=$user");
	$info = ldap_get_entries($_pql->ldap_linkid, $sr) or pql_format_error(1);
	for($i=0; $i < $info["count"]; $i++) {
	  unset($entry); unset($adms);
	  
	  // Get administrator attributes for this domain/branch DN
	  $admins	= $_pql->get_attribute($info[$i]["dn"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
	  for($j=0; $j < count($admins); $j++) {
		if($admins[$j] != $user)
		  $adms[] = $admins[$j];
	  }
	  
	  if(is_array($adms)) {
		// Add the administrators that's left to the DN
		$entry[pql_get_define("PQL_ATTR_ADMINISTRATOR")] = $adms;
		if(! ldap_mod_replace($_pql->ldap_linkid, $info[$i]["dn"], $entry))
		  pql_format_error(1);
	  }
	}
  }
  
  // Get uidnr of user
  $uidnr = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_QMAILUID"));
  $uidnr = $uidnr[0];
  
  // Delete the group object if it exists
  // TODO: Replace with '$_pql->search()'
  $filter = "(&(".pql_get_define("PQL_ATTR_QMAILGID")."=$uidnr)(objectclass=posixGroup))";
  $sr = ldap_search($_pql->ldap_linkid, $domain, $filter);
  if(ldap_count_entries($_pql->ldap_linkid, $sr) > 0) {
	$ed = ldap_first_entry($_pql->ldap_linkid, $sr);
	$dn = ldap_get_dn($_pql->ldap_linkid, $ed);
	
	// delete the group
	if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	  echo "delete (group): '$dn'<br>";
	  return TRUE;
	} else {
	  if(!$_pql->delete($dn))
		return false;
	}
  }
	
  // We delete the forwards to this user as they don't really make sense anymore
  if($delete_forwards) {
	// does another account forward to this one?
	$forwarders = pql_search_forwarders($user);
	if ($forwarders) {
	  // someone forwards to this user. Now we need to know which addresses we're removing
	  $email = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAIL"));
	  $aliases = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAILALTERNATE"));
	  
	  $addresses[] = $email[0];
	  if(is_array($aliases)){
		$addresses = array_merge($addresses, $aliases);
	  }
	}
  }
  
  // Delete the user
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo "delete (user): '$user'<br>";
	return TRUE;
  } else {
	if(!$_pql->delete($user))
	  return false;
  }
  
  // user entry has been removed -> remove the cached version
  $pql_cache->del($user);
  
  // delete forwards to this account?
  if($delete_forwards and $forwarders) {
	foreach($forwarders as $forward) {
	  // get the forwarding addresses of this user
	  $fwd_addresses = $_pql->get_attribute($forward['reference'], pql_get_define("PQL_ATTR_FORWARDS"));
	  if(@$fwd_addresses and is_array($fwd_addresses)) {
		foreach($addresses as $address) {
		  // does this user forward to the removed user or one of his aliases?
		  $rem_key = array_search($address, $fwd_addresses);
		  if ($rem_key !== false) {
			// we found a forward -> remove it 
			pql_modify_attribute($forward['reference'], pql_get_define("PQL_ATTR_FORWARDS"), '',
								 $fwd_addresses[$rem_key]);
		  }
		}
	  }
	} 
  }
  
  return true;
}
// }}}

// {{{ pql_domain_del(domain, delete_forwards)
// Removes a domain with all listed users
function pql_domain_del($domain, $delete_forwards) {
  global $_pql;
  
  // Normalize the Domain DN
  $domain = pql_format_normalize_dn($domain);
  
  // {{{ Convert a DNS TTL RDN to something I (phpQLAdmin) can understand
  if(eregi("^dNSTTL=", $domain))
	// 'dNSTTL=3600+relativeDomainName=@,dc=domain,dc=tld,PQL_CONF_SUBTREE_BIND9,o=Branch,c=SE'
	$domain = pql_recreate_dnsttl($domain);
  // }}}
  
  // {{{ Make sure that the logged in user isn't located under the domain/branch being deleted...
  if(eregi($domain, $_SESSION["USER_DN"])) {
	$msg = urlencode(pql_format_error_span("Sorry, I can't allow you to delete the branch under which you yourself is situated!"));
	pql_header("domain_detail.php?domain=$domain&msg=$msg");
  }
  // }}}
  
  // {{{ Searching for sub entries
  // TODO: Replace with '$_pql->search()'
  $sr = ldap_list($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_OBJECTCLASS")."=*");
  if($sr) {
	$info = ldap_get_entries($_pql->ldap_linkid, $sr);
	for($i=0; $i < $info['count']; $i++){
	  // Deleting recursively this sub entry
	  $result = pql_domain_del($info[$i]['dn'], $delete_forwards);
	  if(!$result)
		// Return result code if delete fails
		return($result);
	}
	
	if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	  echo "delete (domain): '$domain'<br>";
	  return TRUE;
	} else
	  return($_pql->delete($domain));
  }
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
