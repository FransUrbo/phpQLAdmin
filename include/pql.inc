<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 2.160 2004-03-13 08:18:37 turbo Exp $
//

// ----------------------------
// Get release version
$fp = fopen("./.version", "r");
$_SESSION["VERSION"] = fgets($fp, 20);
$_SESSION["VERSION"] = rtrim($_SESSION["VERSION"], "\n");
fclose($fp);

// ----------------------------
// turn on output-buffering by default
ob_start();

// ----------------------------
// API classes
// ----------------------------

// {{{ PQL - main class (connection handling)
class pql {
	var $ldap_linkid = 0;					// ldap connection identifier
	var $ldap_error  = 0;					// error message incase of failiure
	var $ldap_basedn = 0;					// array of base dns for future searches etc
	
	// {{{ string _find_base_option(attribute)

	function _find_base_option($attrib) {
	    $attrib = lc($attrib);

	    $sr = @ldap_read($this->ldap_linkid, NULL, '(objectClass=*)', array($attrib));
	    if(! $sr) die("Can't find base dn - ".ldap_error($this->ldap_linkid));
	    $entry = ldap_get_entries($this->ldap_linkid, $sr);
//printr($entry); die();

	    // Build an array of base dn's. It's possible to have multiple in a database
	    for($i=0; $i < $entry["count"]; $i++) {
			if(is_array(@$entry[$i][$attrib])) {
				for($j=0; $j < $entry[$i][$attrib]["count"]; $j++ )
				  $val[] = urlencode(pql_maybe_encode($entry[$i][$attrib][$j], $attrib, $this->ldap_linkid));
			} elseif($entry[$i]["dn"]) {
				$val[] = urlencode(pql_maybe_encode($entry[$i]["dn"], $attrib, $this->ldap_linkid));
			} else
			  $val[] = urlencode(pql_maybe_encode($entry[$i][$attrib][0], $attrib, $this->ldap_linkid));

			$success = 1;
	    }
	    
	    // If we got at least one DN, return the array. Else return false
	    if($success) {
			$new = NULL;

			// Remove any spaces after the RDN separator
			foreach($val as $dn) {
				while(eregi("%2C\+", $dn)) {
					// Replace ', ' with ','
					$new  = eregi_replace("%2C\+", "%2C", $dn);
				}
			}

			if(is_array($new))
			  return $new;
			else
			  return $val;
		} else
		  return false;
	}

	// }}}

	// {{{ string _find_basedn(void)
	function _find_basedn() {
	    // CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* namingContexts
	    return $this->_find_base_option("namingContexts");
	}
	// }}}

	// {{{ bool pql(hostname, binddn, bindpw, noconnect, dodie)
	function pql($host, $binddn = "", $bindpw = "", $no_connect = false, $do_die = 1) {
		// constructor, connect to ldap host
		if($no_connect == true)
		  return false;

		// Open a connection to the host
		$this->connect($host) or die("<b>could not connect to ldap server</b>");

		// Setup connection (set options etc)
		$this->set_options($this->ldap_linkid);

		// Bind to the host
		if(!$this->bind($binddn, $bindpw)) {
			$this->ldap_error = "<b>Could not bind to ldap server</b>: ";
			$this->ldap_error = $this->ldap_error . ldap_error($this->ldap_linkid) . "<br>" .
			  "Please <a href=\"index.php\" target=\"_top\">relogin</a>.";

			if($do_die)
			  die($this->ldap_error);

			return false;
		}

		// Find the base DN(s)
		$this->ldap_basedn = $this->_find_basedn();

		return true;
	}
	// }}}

	// {{{ bool set_options() {
	function set_options() {
		// LDAP_OPT_REFERRALS
		//	Automatically follow referrals returned by LDAP server?
		//	LDAP_OPT_OFF			 0
		//	LDAP_OPT_ON				!0 (default)
		ldap_set_option($this->ldap_linkid, LDAP_OPT_REFERRALS,	0);

		// LDAP_OPT_DEREF
		//	Deterime how aliases are handled during search.
		//	LDAP_DEREF_NEVER		 0 (default)
		//	LDAP_DEREF_SEARCHING	 1
		//	LDAP_DEREF_FINDING		 2
		//	LDAP_DEREF_ALWAYS		 3
		ldap_set_option($this->ldap_linkid, LDAP_OPT_DEREF,     0);

		if($this->ldap_linkid) {
			// Start with a LDAPv3 bind
			if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 3)) {
				// Didn't work. Try a LDAPv2 bind...
				echo "Don't support v3 bind, trying to do a v2 bind.<br>";
				
				if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 2))
				  echo "Neither protocol LDAPv3 nor LDAPv2 worked, strange!<p>LDAP Error: ".ldap_error($this->ldap_linkid);
				
				return false;
			}

			// We're using LDAPv3, so try a TLS initialization.
			// Only availible in PHP v4.2.0, hence tripple check
			// that that function is availible before trying to
			// use it.
			if(function_exists("ldap_start_tls")) {
				if(! ldap_start_tls($this->ldap_linkid))
				  echo "TLS initialization failed: ".ldap_error($this->ldap_linkid);
			}

			return true;
		}
	}
	// }}}

	// {{{ bool connect(hostname)
	function connect($host) {
		// Check to see if we have a port defined in the host.
		if(ereg(";", $host)) {
			$ldap = split(";", $host);
			
			$host = $ldap[0];
			$port = $ldap[1];
		} else {
			$host = $host;
			$port = 389;
		}

		// connect to ldap host and bind to root_dn
		$this->ldap_linkid = @ldap_connect($host, $port);

		return true;
	}
	// }}}

	// {{{ bool bind(binddn, bindpw)
	function bind($binddn = "", $bindpw = "") {
		// If we haven't set the DN/PW -> do a anonymous bind
		if($this->ldap_linkid and @ldap_bind($this->ldap_linkid, $binddn, $bindpw)) {
			return true;
		}
		
		return false;
	}
	// }}}

	// {{{ int close(void)
	function close(){
		// close connection to ldap host
		@ldap_unbind($this->ldap_linkid);
	}
	// }}}
	
	// {{{ bool connected(void)
	function connected() {
		if($this->ldap_linkid != false){
  			return true;
		}
	}
	// }}}
}
// }}}

// ------------------------------------------------
// API functions - core
// ------------------------------------------------

// {{{ pql_user_get(ldap_linkid, dn)

// Get all users of a domain
function pql_user_get($ldap_linkid, $dn) {
	$dn = urldecode($dn);

	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn, 'pql_user_get'); $rootdn = urldecode($rootdn);

	// Filter out any object which is a posixGroup.
	// (&(cn=*)(!(objectclass=posixgroup)))
	$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . "=*)(!(objectclass=posixgroup)))";
	
	$sr = ldap_search($ldap_linkid, $dn, $filter) or pql_format_error(1);
	$info = ldap_get_entries($ldap_linkid, $sr) or pql_format_error(1);
	for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i]["dn"];
	}
	
	return $user;
}

// }}}

// {{{ pql_user_add(ldap_linkid, domain, cn, attribs, account_type, branch)

// Add a user to the LDAP database
function pql_user_add($ldap_linkid, $domain, $cn, $attribs, $account_type, $branch) {
	global $account_type;

	// -----
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($domain, 'pql_user_add'); $rootdn = urldecode($rootdn);

	// -----
    // Retreive WANTED objectclasses
	if(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn))
	  foreach(pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn)) as $oc)
		$ocs[] = $oc;
	else
	  $ocs[] = '';

	// -----
	// Retreive all EXISTING objectclasses in the LDAP server
	$ldap = pql_get_subschema($ldap_linkid, "objectclasses");

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
	// Go through the WANTED objectclasses, checking for a
	// MUST attribute we don't have
	foreach($ocs as $oc) {
		if(!$ldap[$oc]['MUST']['count'])
		  // An objectclass with no MUST, add this directly
		  $entry['objectclass'][] = $oc;
		else {
			// This objectclass have one or more MUST attributes
			// Do we have any of those MUST attributes defined?
			$MUST = $ldap[$oc]['MUST'];

			foreach($entry as $attrib => $value) {
				if(!is_array($value)) {
					// Go through the MUST attributes for this objctclass,
					// looking for an attribute we have NOT defined
					for($i=0; isset($MUST[$i]); $i++)
					  if(lc($MUST[$i]) == lc($attrib))
						unset($MUST[$i]);
					
					// Before we rearrange the array, remove the count
					// entry (othervise it's get renamed to a number).
					unset($MUST['count']);
					
					// Rearrange the array
					$MUST = array_values(array_unique($MUST));
					
					// Readd the count entry
					$MUST['count'] = count($MUST);
				}
			}
			
			if(!$MUST['count'])
			  // This objectclass don't contain MUST attributes which
			  // isn't defined in the user object. Add this to the list
			  // of objectclasses to add
			  $entry['objectclass'][] = $oc;
//			else {
//				// Objectclass which have been removed from the list because
//				// there's an MUST attribute missing
//				echo "<b>Objectclass with missing attribute in MUST: $oc</b><br>";
//				printr($MUST);
//			}
		}
	}

	// -----
	// Create the RDN for the user
	$userdn = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . "=" .
	          $entry[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)] . ",";
	if($branch)
	  $userdn .= urldecode($branch);
	else
	  $userdn .= urldecode($domain);
	$userdn = pql_maybe_encode($userdn);

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid, $_SESSION["USER_DN"], 'user');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_user_add - user creation", $userdn, $entry);

	// -----
    // Add the user to the database
    if(! ldap_add($ldap_linkid, $userdn, $entry)){
		// failed to add user
		pql_format_error(1);
		die("$LDIF");
		return false;
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
		if($_SESSION["ACI_SUPPORT_ENABLED"])
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid,
																				  $_SESSION["USER_DN"], 'group');

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

    if(!pql_user_exist($linkid, $domain, $user))
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
			$admins	= pql_domain_get_value($ldap, $info[$i]["dn"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
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
					pql_replace_attribute($linkid, $forward['reference'],
											  pql_get_define("PQL_ATTR_FORWARDS"),
											  $fwd_addresses[$rem_key]);
				}
			}
		} 
    }

    return true;
}
// }}}

// {{{ pql_user_exist(ldap_linkid, user)
function pql_user_exist($linkid, $dn, $user) {
	$dn = urldecode($dn);

    // check if user exists
	$user_parts = ldap_explode_dn($user, 0);
	if($user_parts[1])
	  // It's a DN, only use the first part (it already contain the correct user reference
	  $filter = "(&(objectclass=*)(".$user_parts[0]."))";
	else
	  // It's not a DN, construct part of it
	  $filter = "(&(objectclass=*)(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn)."=$user))";

    if(!pql_search($linkid, $dn, $filter))
	  // User does not exist in LDAP tree
	  return(false); 

	return(true);
}
// }}}

// {{{ pql_user_get_quota(ldap_linkid, user)

// Get formated quota of a user
function pql_user_get_quota($ldap_linkid, $user) {
	if($_SESSION["NEW_STYLE_QUOTA"]) {
	  $quota_size  = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_SIZE"));
	  $quota_count = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_COUNT"));

	  return pql_parse_quota(array($quota_size[0], $quota_count[0]));
	} else {
	  $quota = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));

	  if(!is_array($quota))
		return false;
	  else    
		return pql_parse_quota($quota[0]);
  }
}

// }}}

// {{{ pql_user_get_letter(linkid, domain, letra)

// Based in PQL_CONF_GET_USER to search by the cn first letter
function pql_user_get_letter($linkid, $domain, $letra) {
	$filter = "(".pql_get_define("PQL_ATTR_UID")."=".$letra."*)";
	$user = pql_search($linkid, $domain, $filter);
    return $user;
}

// }}}

// {{{ pql_user_get_number(ldap_linkid, domain)
// Based in PQL_CONF_GET_USER to search by an initial number at cn attribute
function pql_user_get_number($linkid, $domain) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn, 'pql_user_get_number'); $rootdn = urldecode($rootdn);

    for($numero=0; $numero <= 9; $numero++) {
		$filter = "(".pql_get_define("PQL_ATTR_UID")."=".$numero."*)";
		$tmp = pql_search($linkid, $domain, $filter);
		
		if($user)
		  $user = $user + $tmp;
    }
    
    return $user;
}
// }}}

// ------------------------------------------------

// {{{ pql_domain_get(ldap)

// Get all domains listed in ldap-tree (ou|dc-records)
function pql_domain_get($ldap) {
	foreach($ldap->ldap_basedn as $dn)  {
		$dn = urldecode($dn); unset($info);

		$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $dn) .
                  "=*)" . pql_setup_branch_objectclasses(1, $dn) . ")";

		// Does this top DN contain the ou=People object?
		$sr1 = @ldap_read($ldap->ldap_linkid, pql_get_define("PQL_CONF_SUBTREE_USERS").",$dn", "objectclass=*");
		
		// Is there any users just below the top DN?
		$sr2 = @ldap_list($ldap->ldap_linkid, $dn, pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn)."=*");
		
		if((@ldap_count_entries($ldap->ldap_linkid, $sr1) > 0) or
		   (@ldap_count_entries($ldap->ldap_linkid, $sr2) > 0))
		  {
			  // We got 'ou=People' AND/OR user(s) below the top DN. Remember the branch.
			  $domains[] = $dn;
		} else {
			// No entries. Look for branches with a ONE_LEVEL search
			$sr   = @ldap_list($ldap->ldap_linkid, $dn, $filter) or pql_format_error(1);
			$info = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_format_error(1);
			for ($i=0; $i<$info["count"]; $i++) {
				$domains[] = $info[$i]["dn"];
			}
		}
	}
	
	return $domains;
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
	if($_SESSION["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid, $_SESSION["USER_DN"], 'branch');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_domain_add - create domain", $dn, $entry);

    // adds the user
    if(! @ldap_add($ldap_linkid, $dn, $entry)) {
		// failed to add user
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
	$sr   = ldap_list($linkid, $domain, "objectClass=*");
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

// {{{ pql_domain_exist(ldap, domain, rootdn = '')
// Check if domain exists
function pql_domain_exist($ldap, $domain, $rootdn = '') {
	$domain = urldecode($domain); $domain = pql_maybe_encode($domain);

	if(!$rootdn) {
		$filter = '(objectClass=phpQLAdminBranch)';
		$sr = ldap_read($ldap->ldap_linkid, $domain, $filter);
		if(ldap_count_entries($ldap->ldap_linkid, $sr) > 0)
		  return true;
	} else {
		$filter = '('.pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn).'='.$domain.')';
		$sr = ldap_search($ldap->ldap_linkid, $rootdn, $filter);
		if(ldap_count_entries($ldap->ldap_linkid, $sr) > 0)
		  return true;
	}
    
    return false;
}
// }}}

// {{{ pql_domain_get_value(ldap, dn, value, match, all)
function pql_domain_get_value($ldap, $dn, $value, $match = '', $all = 0) {
	$value = lc($value); $dn = urldecode($dn);

	$recursive = 1; $success = 0; $results = '';

	if(!$match) {
		$match = '*';
		$recursive = 0;
	}

	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn, 'pql_domain_get_value'); $rootdn = urldecode($rootdn);

	// Setup a filter to look for DOMAINS (branches)
//	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "o")
	  $filter = "$value=$match";
//	else
//	  $filter  = "(&($value=$match)(".pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn)."=*))";
	//echo "pql_domain_get_value(): '$dn' '$filter' $value<br>"; // DEBUG

	// Search from the DN
	if(!$recursive)
	  // ... only
	  $sr = ldap_read($ldap->ldap_linkid, $dn, $filter, array($value));
	else
	  // ... and below
	  $sr = ldap_search($ldap->ldap_linkid, $dn, $filter, array($value));

	// Get the entries
	$result = @ldap_get_entries($ldap->ldap_linkid, $sr)
	  or pql_format_error(1);

	// Go through the result, if there is any
	for($i=0; $i < $result["count"]; $i++) {
		// Extract all the descriptor fields
		if(($value == pql_get_define("PQL_ATTR_ADMINISTRATOR")) or
		   ($value == pql_get_define("PQL_ATTR_SEEALSO")) or
		   ($value == pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")) or
		   ($value == pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")) or
		   ($value == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")))
		  {
			if($match != "*") {
				// Return multiple values. If we're looking for 'administrator', return the DN
				if(!$all) {
					for($j=0; $j < $result[$i][$value]["count"]; $j++) {
						// Filter out the top dn, we don't want that in the domain/branch list...
						if(($result[$i][$value][$j] == $match) and
						   (($result["count"] < 2) or ($result[$i]["dn"] != $dn)))
						  {
							  $results[] = $result[$i]["dn"];
							  $success = 1;
						  }
					}
				} else {
					$results[] = $result[$i]["dn"];
					$success = 1;
				}
			} else {
				for($j=0; $j < $result[0][$value]["count"]; $j++) {
					$results[] = pql_maybe_decode($result[0][$value][$j]);
					$success = 1;
				}
			}
		} elseif(($value == 'passwordscheme') or
				 ($value == 'userobjectclass') or
				 ($value == 'branchobjectclass')) {
			// We're requesting a normal multivalue attribute
			for($j=0; $j < $result[$i][$value]["count"]; $j++) {
				$results[] = pql_maybe_decode($result[$i][$value][$j]);
				$success = 1;
			}
		} else {
			// Return the value we're looking for (single valued).
			$results = pql_maybe_decode($result[0][$value][0]);
			$success = 1;
		}
	} // end for($i)

	if(!$success and (($value != pql_get_define("PQL_ATTR_ADMINISTRATOR")) and
					  ($value != pql_get_define("PQL_ATTR_SEEALSO")) and
					  ($value != pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")) or
					  ($value == pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"))))
	  return;

	if($success and (($value == pql_get_define("PQL_ATTR_ADMINISTRATOR")) or
					 ($value == pql_get_define("PQL_ATTR_SEEALSO")) or
					 ($value == pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")) or
					 ($value == pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")) or
					 ($value == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")))
	   and !is_array($results))
	  {
		  // Convert a flat variable to a array
		  $tmp = $results; unset($results);
		  $results[] = pql_maybe_decode($tmp);
	  }

	return $results;
}
// }}}

// {{{ pql_domain_set_value(linkid, dn, type, value)
// Set a embedded domain default value
function pql_domain_set_value($linkid, $dn, $type, $value) {
	$DEBUG = 0;
	$dn = urldecode($dn);

	$entry = array();
	if($type) {
		$type = lc($type);
		
		// Setup the value array
		if(is_array($value)) {
			// We're supplied with an array, add those values
			foreach($value as $attr => $val) {
				$attr = lc($attr);
				if(is_array($val)) {
					foreach($val as $data)
					  $entry[$attr][] = pql_maybe_encode($data, $attr, $linkid);
				} else
				  $entry[$attr][] = pql_maybe_encode($val, $attr, $linkid);
			}
		} else {
			$entry[$type] = pql_maybe_encode($value, $type, $linkid);
		}
	} else {
		// INTERNATIONALIZE
		if(is_array($value)) {
			foreach($value as $attrib => $val) {
			    $entry[$attrib] = array(); // DLW:
				for($i=0; !empty($val[$i]); $i++)
				  $entry[$attrib][] = pql_maybe_encode($val[$i], $attrib, $linkid);
			}
		} else {
			$entry = $value;
		}
	}

	if($value and !$entry["objectclass"]) {
		// Setup objectclasses
		if(!($entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $dn, $linkid)))
		  unset($entry[pql_get_define("PQL_ATTR_OBJECTCLASS")]);

		// There's TWO config object classes. A BRANCH and a GLOBAL.
		// If this is a GLOBAL attribute, pql_setup_branch_objectclasses()
		// doesn't catch this. Do it manually here instead...
		global $PQL_ATTRIBUTE;
		foreach($value as $attrib => $val) {
			if(in_array($attrib, $PQL_ATTRIBUTE["GLOBAL"]))
			  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "phpQLAdminGlobal";
		}
	}
		
	// Create a LDIF object to print in case of error
	$account_type = $type;
	$LDIF = pql_create_ldif("pql_domain_set_value", $dn, $entry);

	// --------------------------
	// DEBUG mode: Print the LDIF
	if($DEBUG >= 1)
	  die("$LDIF");

	// Do the LDAP modification
	if($value != '') {
		// Replace the current attribute value
		if(! @ldap_mod_replace($linkid, $dn, $entry)) {
			if(ldap_errno($linkid) == 65) {
				// Object class violation - try to add the missing objectclass
				$old_ocs = $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")];

				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_missing_objectclasses($linkid, $dn, $entry);
				if(empty($entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][0])) {
					global $LANG;
					echo "pql_domain_set_value() - ".$LANG->_('Could not figure out what objectclasses to add')."<br>";

					$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = $old_ocs;

					$LDIF = pql_create_ldif("pql_domain_set_value", $dn, $entry);
					die($LDIF);
				}

				// Recursion... Hopfully there's a limit on how many (max_execution_time in php.ini)!
				pql_domain_set_value($linkid, $dn, $type, $entry);
			} else {
				// Other error...
				pql_format_error(1);
				die($LDIF);
			}
		}
	} else {
		// Delete all attributes of this type, regardless of value
		$entry[$type] = array();

		if(! ldap_mod_del($linkid, $dn, $entry)) {
			pql_format_error(1);
			die($LDIF);
			return false;
		}
	}
	
    return true;
}
// }}}

// {{{ pql_domain_replace_admins(ldap, old, new)
// Look for 'administrator/seealso = old value',
// replace with 'administrator/seealso = new value'.
function pql_domain_replace_admins($ldap, $old, $new) {
	// Setup the search filter
	$filter  = '(|';
	$filter .= '('.pql_get_define("PQL_ATTR_ADMINISTRATOR")."=$old".')';
	$filter .= '('.pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")."=$old".')';
	$filter .= '('.pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")."=$old".')';
	$filter .= '('.pql_get_define("PQL_ATTR_SEEALSO")."=$old".')';
	$filter .= ')';

	// We're only interested in these attributes
	$attribs = array(pql_get_define("PQL_ATTR_ADMINISTRATOR"),
					 pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
					 pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"),
					 pql_get_define("PQL_ATTR_SEEALSO"));

	// Go through the namingContexts one by one
	foreach($ldap->ldap_basedn as $base) {
		$base = urldecode($base);
		
		$sr   = ldap_search($ldap->ldap_linkid, $base, $filter, $attribs);
		$info = ldap_get_entries($ldap->ldap_linkid, $sr) or pql_format_error(1);
		for($i=0; $i < $info["count"]; $i++) {
			unset($dn); unset($entry); unset($LDIF);
			
			// Go through the attributes we're interested in, looking for
			// the (old) user DN
			foreach($attribs as $attrib) {
				for($j=0; $j < $info[$i][$attrib]["count"]; $j++) {
					// TODO: Only replace the attribute(s) if it have changed
					if($old == $info[$i][$attrib][$j]) {
						// Got a match ...
						if($new)
						  // ... we have something to replace with - replace this attribute
						  $entry[$attrib][] = $new;
					} else
					  // Remember the old value
					  $entry[$attrib][] = $info[$i][$attrib][$j];
				}
			}
			
			// Create a LDIF object to print in case of error
			$LDIF = pql_create_ldif("pql_domain_replace_admins", $info[$i]["dn"], $entry);
			
			// Modify this object
			if(!ldap_mod_replace($ldap->ldap_linkid, $info[$i]["dn"], $entry)) {
				pql_format_error(1);
				die($LDIF);
			}
		}
	}
}
// }}}

// ------------------------------------------------
// TODO: Questionable functons - remove?
// ------------------------------------------------

// {{{ pql_modify_user_addressbook(ldap_linkid, user, attribs)
function pql_modify_user_addressbook($ldap_linkid, $user, $attribs) {
	// What's the Root DN (namingContexts) of this user
	$rootdn = pql_get_rootdn($user, 'pql_modify_user_addressbook'); $rootdn = urldecode($rootdn);

    // check if addressbook is disabled
    if(!pql_get_define("PQL_CONF_USE_ADDRESSBOOK", $rootdn))
	  return false;
    
    // object-class dependencies for addressbook entries
    //
    // ------------------------------------------------------------------------------------
    // | OBJECTCLASS          | Must-Attr. (OpenLDAP 1.2.x) | Must-Attr. (OpenLDAP 2.0.x) |
    // ------------------------------------------------------------------------------------
    // | InetOrgPerson        | none                        | none                        |
    // ------------------------------------------------------------------------------------
    // | organizationalPerson | sn, cn                      | none                        |
    // ------------------------------------------------------------------------------------
    // | country              | c                           | c                           |
    // ------------------------------------------------------------------------------------
    // | organization         | o                           | o                           |
    // ------------------------------------------------------------------------------------
    // | pilotObject          | none                        | none                        |
    // ------------------------------------------------------------------------------------
    //
    // the 'organization' and 'country' objectclass can only be set when an 'o' or a 'c'
    // attribute is given. The other objectclasses will be added by default because the
    // haven't a mandatory attribute
    
    // fetch current registred objectclasses of user
    $oc_user = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_OBJECTCLASS"));
    
    // set all objectclasses to lowercase
    foreach($oc_user as $oc1){
		$oc[] = lc($oc1);
    }
    
    // add addressbook's default object classes if they are not present
    $default_oc = array("inetorgperson", "organizationalperson", "pilotobject");
    
    foreach($default_oc as $oc1){
		if(!in_array($oc1, $oc)){
			//$oc[] = $oc1;
		}
    }
    
    // check for object class dependencies
    echo var_dump($oc);
}
// }}}

// {{{ pql_search_forwarders(ldap, user)
// Search all accounts with forwarders to $user@$domain
function pql_search_forwarders($ldap, $user) {
	$return = '';
	$linkid = $ldap->ldap_linkid;

    // get all email addresses of a user
    $email = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_MAIL"));
    $aliases = pql_get_attribute($linkid, $user, pql_get_define("PQL_ATTR_MAILALTERNATE"));
    
    $addresses[] = $email[0];
    if(is_array($aliases)){
		$addresses = array_merge($addresses, $aliases);
    }
    
    // create filter
    $filter = "(|";
    foreach($addresses as $add){
		$filter .= "(" . pql_get_define("PQL_ATTR_FORWARDS") ."=" . $add . ")";
    }
    $filter .= ")";

    // Go through each base DN in the database, looking
	// for forwarders to this user
	foreach($ldap->ldap_basedn as $dn) {
		$dn = urldecode($dn);

		$sr = ldap_search($linkid, $dn, $filter);
		if(ldap_count_entries($linkid, $sr) > 0) {
			// Get the entries found
			$results = ldap_get_entries($linkid, $sr);

			// Go through the entries
			foreach($results as $key => $result){
				if((string)$key != "count"){
					$return[] = array("reference" => $result["dn"],
									  pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn) => $result[pql_get_define("PQL_ATTR_CN")][0],
									  "email" => $result[pql_get_define("PQL_ATTR_MAIL")][0]);
					
					if(is_array($result[pql_get_define("PQL_ATTR_MAILALTERNATE")])){
						foreach($result[pql_get_define("PQL_ATTR_MAILALTERNATE")] as $key => $mailalternateaddress){
							if((string)$key != "count"){
								$return[] = array("reference" => $result["dn"],
												  pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn) => $result[pql_get_define("PQL_ATTR_CN")][0],
												  "email" => $mailalternateaddress);
							}
						} // end foreach results
					} // end if is_array
				} // end if key != count
			} // end foreach results
		}
	} // end foreach basedn
    
    return $return;
}
// }}}

// {{{ pql_modify_userattribute(ldap_linkid, user, attrib, oldvalue, newvalue)
// Change an attribute of a user
function pql_modify_userattribute($ldap_linkid, $user, $attrib, $oldvalue, $newvalue) {
    // get all attributes first
    $entry = pql_get_attribute($ldap_linkid, $user, $attrib);

    // If we found an entry, make sure it isn't
    // not overwritten, otherwise ADD the attribute.
    if($entry) {
		// array_search workaround
		foreach($entry as $value) {
			if($value == $oldvalue) {
				if($newvalue)
				  // Replace this value with a new one
				  $value = $newvalue;
				else
				  // We want to delete this value
				  unset($value);
			}

			if($value)
			  $entry_new[$attrib][] = $value;
		}

		// If we're not called with a OLD value, then we
		// want to add a new value to the attribute
		if(!$oldvalue and $newvalue)
		  $entry_new[$attrib][] = $newvalue;
    } else
	  $entry_new[$attrib][] = $newvalue;

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_modify_userattribute", $user, $entry_new);

    if(!ldap_mod_replace($ldap_linkid, $user, $entry_new)) {
	    // if the value is empty (""), this causes an error in OpenLDAP 2.x, try to ldap_mod_del
	    if($newvalue == "") {
			unset($entry_new); $entry_new[$attrib] = array();

			if(ldap_mod_del($ldap_linkid, $user, $entry_new))
			  return true;
		}

		pql_format_error(1);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($user);

	// What's the Root DN (namingContexts) of this user
	$rootdn = pql_get_rootdn($user, 'pql_modify_userattribute'); $rootdn = urldecode($rootdn);

    if ($attrib == pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)) {
		pql_cache_userdn_remove($user);
    }
    
    return true;
}
// }}}

// {{{ pql_replace_attribute(ldap_linkid, objectdn, attrib, value)

// Replaces an attribute in a object (only use for single-field attributes)
// Delete an attribute(s) if value is an empty string
function pql_replace_attribute($ldap_linkid, $objectdn, $attrib, $value) {
	$attrib = lc($attrib);

    if(!$value)
	  // Delete all attributes of this type, regardless of value
	  $entry[$attrib] = array();
	elseif(!$attrib)
	  $entry = $value;
	else
	  $entry[$attrib] = pql_maybe_encode($value, $attrib, $ldap_linkid);

	// replace the attribute
	if(!@ldap_mod_replace($ldap_linkid, $objectdn, $entry)) {
	    // if the value is empty (""), this causes an error in OpenLDAP 2.x, try to ldap_mod_del
	    if($value == "") {
			$oldvalue = pql_get_attribute($ldap_linkid, $objectdn, $attrib);
			$oldvalue = $oldvalue[0];
			$entry[$attrib] = $oldvalue;

			if(ldap_mod_del($ldap_linkid, $objectdn, $entry))
			  return true;
			else {
				pql_format_error(1);
				return false;
			}
	    } else {
			if(ldap_errno($ldap_linkid) == 65) {
				// Object class violation - try to add the missing objectclass
				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_missing_objectclasses($linkid, $objectdn, $entry);
			}
		}
	    
	    pql_format_error(1);
	    return false;
	}
	
	// user entry has changed -> remove the cached version
	pql_cache_userentry_remove($objectdn);
	
	return true;
}

// }}}

// ------------------------------------------------
// API functions - support
// ------------------------------------------------

// {{{ pql_email_exists(ldap, email)
// Check if any mail or mailalternateaddress record with this email exists in the ldap tree
function pql_email_exists($ldap, $email) {
	$filter = "(|(" . pql_get_define("PQL_ATTR_MAIL") . "=" . $email . ")(" . pql_get_define("PQL_ATTR_MAILALTERNATE") . "=" . $email . "))";

	foreach($ldap->ldap_basedn as $dn) {
		$dn = urldecode($dn);

		$sr = ldap_search($ldap->ldap_linkid, $dn, $filter);
		if(ldap_count_entries($ldap->ldap_linkid, $sr) > 0) {
			$ed = ldap_first_entry($ldap->ldap_linkid, $sr);
			return ldap_get_dn($ldap->ldap_linkid, $ed);
		} else {
			return false;
		}
	}
}
// }}}

// {{{ pql_get_next_ugidnumber(ldap, type)
// Returns the next free UID/GID number
function pql_get_next_ugidnumber($ldap, $type) {
	if($type == 'uid') {
		$attrib  = pql_get_define("PQL_ATTR_QMAILUID");
		$minimum = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER");
	} else {
		$attrib = pql_get_define("PQL_ATTR_QMAILGID");
		$minimum = pql_get_define("PQL_CONF_MINIMUM_GIDNUMBER");
	}

    $filter = "(&($attrib=*)(objectclass=posixaccount))";

	foreach($ldap->ldap_basedn as $dn) {
		$results = pql_search($ldap->ldap_linkid, $dn, $filter, '');
	
		// Find the highest availible [UG]IDNUMBER
		$highest = 0;
		for($i=0; $i < $results["count"]; $i++) {
			if(($results[$i][$attrib][0] > $highest) &&
			   ($results[$i][$attrib][0] < 64000))
			  {
				  $highest = $results[$i][$attrib][0];
			  }
		}
	}

	if($highest)
	  $highest++; // Increase with one!
	else
	  if($minimum)
		$highest = $minimum;
	  else
		$highest = 1000;
	
    return $highest;
}
// }}}

// {{{ pql_get_next_username(ldap, domain)
// Returns the next free UID/GID number
function pql_get_next_username($ldap, $domain) {
	// Get the prefix from the branch
	$prefix = pql_domain_get_value($ldap, $domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX"));

	$suffix_length = pql_domain_get_value($ldap, $domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"));
	if(!$suffix_length)
	  $suffix_length = "%03d";
	else
	  $suffix_length = "%0".$suffix_length."d";

	if($prefix) {
		// Under what namingContexts is this domain/branch located?
		$rootdn = pql_get_rootdn($domain, 'pql_get_next_username'); $rootdn = urldecode($rootdn);

		// Setup the LDAP search filter
		$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . '=' . $prefix . '*';

		// Search the whole database for all users with this prefix
		foreach($ldap->ldap_basedn as $dn) {
			$dn  = urldecode($dn);
			$sr  = ldap_search($ldap->ldap_linkid, $dn, $filter, array(pql_get_define("PQL_ATTR_UID")));
			$tmp = ldap_get_entries($ldap->ldap_linkid, $sr) or pql_format_error(1);
			for($i=0; $i < $tmp["count"]; $i++)
			  $USERS[] = $tmp[$i][pql_get_define("PQL_ATTR_UID")][0];
		}

		if(!$USERS) {
			$suffix = sprintf("$suffix_length", 1);
		} else {
			// We have previous values in the DB. 
			sort($USERS);
			$nr  = eregi_replace($prefix, "", $USERS[count($USERS)-1]);

			$suffix = sprintf("$suffix_length", $nr + 1);
		}

		return($prefix.$suffix);
	}
}
// }}}

// {{{ pql_get_valid_shells()
// Load list of allowed shells from /etc/shells
function pql_get_valid_shells() {
	$fp = fopen("/etc/shells", "r");
	while (!feof ($fp)) {
		$buffer = fgets($fp, 4096);
		$shell = split(" ", $buffer);
		
		if(!eregi("^#", $shell[0]) and !eregi("^$", $shell[0])) {
			$shells[] = rtrim($shell[0]);
		}
	}
	fclose ($fp);
	asort($shells);

	return $shells;
}
// }}}

// {{{ pql_get_mx(domainname)
function pql_get_mx($domainname) {
	$res = getmxrr($domainname, $rec, $weight);
	if(count($rec) == 0) {
		// Not found in the DNS, 

		if(pql_get_define("PQL_CONF_CONTROL_USE")) {
			// Look for a qmailControl object which lists this domain...
		
			// Initiate a connection to the QmailLDAP/Controls DN
			$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
			if($_pql_control->ldap_linkid) {
				$filter = "(&(objectclass=qmailControl)(".pql_get_define("PQL_ATTR_LOCALS")."=$domainname))";
				$info = pql_search($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter, '');
				for($i=0; $i<$info["count"]; $i++) {
					for($j=0; $j<$info[$i][pql_get_define("PQL_ATTR_LOCALS")]["count"]; $j++) {
						if($info[$i][pql_get_define("PQL_ATTR_LOCALS")][$j] == $domainname)
						  return($info[$i][pql_get_define("PQL_ATTR_CN")][0]);
					}
				}
			}
		}
				
		return(0);
	} else {
		// Take the MX with _LOWEST_ priority/weight.
		asort($weight); $old_prio = 65555;
		foreach($weight as $key => $prio){
			if($prio < $old_prio) {
				$old_prio = $prio; $prio_key = $key;
			}
		}
		
		return($rec[$prio_key]);
	}
}
// }}}

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
		// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b 'cn=Subschema' 'objectClass=*' $attrib
		$sr     = ldap_read($ldap_linkid,
							$entry[0][pql_get_define("PQL_ATTR_SUBSCHEMASUBENTRY")][0],
							'objectClass=*', array($attrib));
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

						for($j = 0; $words[$j]; $j++)
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

		ksort($VALUE);
		return $VALUE;
    } else
	  return false;
}
// }}}

// {{{ pql_get_rootdn(dn)
function pql_get_rootdn($dn, $function = '') {
	global $_pql;
	$tracker = 'Please report this at the <a href="http://apache.bayour.com/anthill/" target="_new">bugtracker</a>.<br>';

	if($dn == '') {
		if($function)
		  echo "This is weird. We're called (from $function) with an empty DN! $tracker";
		else
		  echo "This is weird. We're called with an empty DN! $tracker";
		die();
	} else
	  $dn = urldecode($dn);

	if(is_array($_pql->ldap_basedn)) {
		$counts = 0;
		foreach($_pql->ldap_basedn as $base) {
			$base = urldecode($base);

			if(eregi("$base\$", $dn))  {
				$counts++;
				$DNs[] = $base;
			}
		}
		
		if($counts >= 2) {
			// We have more than one hit!!
			// TODO: What do do with more than one hit!?
			echo "This is weird. We have more than one hit ($counts to be exact) in our pql_get_rootdn() check!<br>";
			echo "<br>";
			echo "These are the DNs that matches the <u>$dn</u> DN. $tracker";
			printr($DNs);
			die();
		} elseif($counts < 1) {
			// BUG! We don't have a hit!
			echo "This is weird. We couldn't find the root dn for some reason. The DN we're trying to find a root DN for is: ";
			echo "'$dn'.<p>$tracker";
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

// {{{ pql_password_hash(password, hash)
function pql_password_hash($password, $hash) {
    // creates a hash of a password
    // - {SSHA}askdfjklödjfnddnu
    // - {SHA}kldfaldkjadsfdsad=
    // - {MD5}lkasjfdndiasdfee=
    // - {crypt}cryptedpassword
    // - {KERBEROS}principal@realm
    // - cleartextpasswort
    switch($hash){
      case "{SSHA}":
		// SHA1 hashing (salted)
		if(function_exists("mhash"))
		  return "{SSHA}" . base64_encode(mhash(MHASH_SHA1, $password, uniqid(microtime())));

      case "{SHA}":
		// SHA1 hashing
		if(function_exists("mhash"))
		  return "{SHA}" . base64_encode(mhash(MHASH_SHA1, $password));

      case "{MD5}":
		// MD5 hashing
		if(function_exists("mhash"))
		  return "{MD5}" . base64_encode(mhash(MHASH_MD5, $password));

      case "{CLEAR}":
		// Password in the clear (JUCK! :)
		return $password;

      case "{KERBEROS}":
		// Kerberos V mapping
		return $hash . $password;

      default:
		$salt_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"; 
		$max_idx = strlen($salt_chars) - 1; 
		$salt = $salt_chars[rand(0, $max_idx)] . $salt_chars[rand(0, $max_idx)]; 

		return "{crypt}" . crypt($password, $salt);
    }
}
// }}}

// {{{ pql_parse_quota(quota_string)
function pql_parse_quota($quota_string) {
	$quota = '';

	if($_SESSION["NEW_STYLE_QUOTA"]) {
		$quota_string[0] = ereg_replace("S$", "", $quota_string[0]);
		$quota_string[1] = ereg_replace("C$", "", $quota_string[1]);

		$quota["maxsize"]  = $quota_string[0];
		$quota["maxmails"] = $quota_string[1];

		return($quota);
	} else { 
		// get max bytes
		preg_match("/([0-9]*)S/", $quota_string, $matches);
		if(isset($matches[1])) {
			$quota["maxsize"] = intval($matches[1] / 1024);
			
			// get max mails
			preg_match("/([0-9]*)C/", $quota_string, $matches);
			$quota["maxmails"] =  $matches[1];
		} else
		  // The quota value isn't correct. Whing it!
		  // Asume that the value in there is SIZE, and set AMOUNT to 0
		  // ('should mean' unlimited if I remember correctly).
		  return(array($quota_string, "0"));
		
		return $quota;
	}
}
// }}}

// {{{ pql_check_email(email)
function pql_check_email($email) {
    $split = split("@", $email);
    
    if(count($split) != 2)
	  // does not contain @ or has to much of it
	  return false;
    
    // check user part
    if(!preg_match("/^[a-z0-9-+=#]+([\._a-z0-9-+=#]?[a-z0-9]+)*$/i", $split[0]))
	  // user part is not valid:
	  // - must start with a valid character (a-Z, 0-9)
	  // - may contain a special char (. _ + = # -), but not at start and end
	  //   and following another special char
	  // - must end with a valid character (a-Z, 0-9)
	  // - must contain at least 1 character
	  return false;
    
    // check host part
	return pql_check_hostaddress($split[1]);
}
// }}}

// {{{ pql_check_hostaddress(host, force_dot)
function pql_check_hostaddress($host, $force_dot = 0) {
    // checks an fqdn - if force_dot is true, it must contain a dot
    if($force_dot)
	  return preg_match("/^([0-9a-z-]+\.[0-9a-z-]+)+$/i", $host);
	else
	  return preg_match("/[^0-9a-z]/i", $host);
}
// }}}

// {{{ pql_print_filesize(bytes)
function pql_print_filesize($bytes) {
    // print out human readable filesizes
    
    // at which byte/kb/mb count switch to the next upper level
    $switch_bytes = "750";
    
    if($bytes >= ($switch_bytes * 1048576)){
		$gb = number_format($bytes / 1073741824, 2, ".", "'");
		return $gb . " GB";
    }
    
    if($bytes >= ($switch_bytes * 1024)){
		$mb = number_format($bytes / 1048576, 2, ".", "'");
		return $mb . " MB";
    }
    
    if($bytes >= $switch_bytes){
		$mb =  number_format($bytes / 1024, 2, ".", "'");
		return $mb . " KB";
    }
    
    return $bytes . " Bytes";
}
// }}}

// {{{ pql_strip_domain(email)
// Get the username of an email address
function pql_strip_domain($email) {
    $email = split("@", $email);
    
    if(count($email) == 2)
	  return $email[0];
    
    return false;
}
// }}}

// {{{ pql_strip_username(email)
// Get the domainname of an email address
function pql_strip_username($email) {
    $email = split("@", $email);
    
    if(count($email) == 2)
	  return $email[1];
    
    return false;
}
// }}}

// {{{ pql_execute(command, hide=true)
function pql_execute($command, $hide=true) {
	$command = "(" . escapeshellcmd($command) . ") 2>&1";

	if(!$hide)
	  echo "\n<pre>\n----\n";

	flush(passthru($command, $ret));

	if($ret)
	  $code = '=> <b><u>FAILED!!</u></b>';
	else
	  $code = '=> <b>SUCCESS</b>';

	if(!$hide)
	  echo "----\nreturn value: $ret $code\n</pre>";

	return $ret;
}
// }}}

// {{{ pql_generate_button(button array)

function pql_generate_button($buttons, $link = '') {
    global $LANG;

	$url = $_SERVER["PHP_SELF"] . "?";

	// It's not sure we have a rootdn and/or domain, so only add those
	// it's set
	if(!empty($_REQUEST["rootdn"])) {
		$url .= "rootdn=" . pql_format_urls($_REQUEST["rootdn"]) . "&";
	}

	if(!empty($_REQUEST["domain"])) {
		$url .= "domain=" . pql_format_urls($_REQUEST["domain"]) . "&";
	}
?>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
<?php
	  $i=0; // A button counter.
	  foreach($buttons as $view => $text) {
		  // Generate the button link etc
?>
    <a href="<?=$url?>view=<?=$view?><?php if($link) { echo "&$link"; } ?>"><img alt="/ <?=$LANG->_($text)?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?php echo urlencode($LANG->_($text)); ?>"></a>
<?php
		  // Increase button counter"
		  $i++;
	  }
?>
  </table>

<?php
}

// }}}

// {{{ pql_ldap_mailquota(quota)
// Returns human readable version of users quota
function pql_ldap_mailquota($quota) {
    global $LANG;

    if($quota["maxmails"] == "")
	  $txt = $LANG->_('Unlimited amount');
	else
	  $txt = pql_complete_constant($LANG->_('Max %maxmails% mails'), array("maxmails" => $quota["maxmails"]));
    
    if($quota["maxsize"] == "")
	  $txt .= $LANG->_('Unlimited size');
	else
	  $txt .= pql_complete_constant($LANG->_('Max %maxsize% KB'), array("maxsize" => $quota["maxsize"]));
    
    return $txt;
}
// }}}

// {{{ pql_ldap_accountstatus(status)
// Returns text of account-status
function pql_ldap_accountstatus($status) {
    global $LANG;

    // active [default]
    // nopop
    // disabled
    
    switch($status) {
      case "nopop":
		return $LANG->_('POP locked');

      case "disabled":
		return $LANG->_('Locked');

      default:
		return $LANG->_('Active');
    }
}
// }}}

// {{{ pql_ldap_deliverymode(mode)
// Returns text of delivery mode
function pql_ldap_deliverymode($mode) {
    global $LANG;

	if($_SESSION["NEW_STYLE_QUOTA"]) {
		// multi field entries of these keywords
		// - (normal)	put message into maildir/mbox, plus forward and program delivery
		// - noforward	do not forward (ignores forwarding entries in ldap and .qmail)
		// - nolocal	do not put message into maildir/mbox (ignores also .qmail)
		// - noprogram	do not do program deliveries (ignores deliveryprogrampath, .qmail)
		// - reply		send an auto_reply mail with text from mailReplyText
		switch($mode) {
		  case "normal":
			return $LANG->_('Save in local mailbox, allow forwarding and program delivery');

		  case "noforward":
			return $LANG->_('Save in local mailbox only (no forwarding)');

		  case "nolocal":
			return $LANG->_('Do not save in local mailbox, allow forwarding and program delivery');

		  case "noprogram":
			return $LANG->_('Do not allow program delivery (ignores deliveryProgramPath and .qmail)');

		  case "reply":
			return $LANG->_('Send an automatic reply mail');
		}
	} else {
		// normal [default]
		// forwardonly
		// nombox
		// localdelivery
		// reply
		// echo
		switch($mode) {
		  case "forwardonly":
			return $LANG->_('Only forward');
			
		  case "nombox":
			return $LANG->_('No local mailbox');
			
		  case "localdelivery":
			return $LANG->_('Save in local mailbox');
			
		  case "reply":
			return $LANG->_('Send an automatic reply mail');
			
		  case "echo":
			return $LANG->_('Echo to console (tricky)');
			
		  default:
			return $LANG->_('Normal');
		}
	}
}

// }}}

// {{{ pql_setup_branch_objectclasses(filter = 0, dn, linkid = '')
// Setup object class entry for a domain/branch with all
// necessary objectclasses
function pql_setup_branch_objectclasses($filter = 0, $dn, $linkid = '') {
	if($dn and $linkid) {
		// Retreive existing objectClasses from the object
		$object = ldap_explode_dn($dn, 0);
		$sr     = @ldap_read($linkid, $dn, $object[0], array('objectclass'));
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
		} elseif($linkid->ldap_basedn[0])
		  $ocs = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $linkid->ldap_basedn[0]);
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

// {{{ pql_validate_administrator(linkid, dn, admin)

function pql_validate_administrator($linkid, $dn, $admin) {
	$dn = urldecode($dn);

	$sr = @ldap_search($linkid, $dn, '('.pql_get_define("PQL_ATTR_ADMINISTRATOR").'=*)',
					   array(pql_get_define("PQL_ATTR_ADMINISTRATOR")));
	if(!$sr)
	  return false;

	$info = @ldap_get_entries($linkid, $sr) or pql_format_error(1);
	for($i=0; $i<$info["count"]; $i++) {
		for($j=0; $j<$info[$i][pql_get_define("PQL_ATTR_ADMINISTRATOR")]["count"]; $j++)
		  // TODO: We shouldn't do a regexp but a 'case insensitive equal'
		  if(eregi($info[$i][pql_get_define("PQL_ATTR_ADMINISTRATOR")][$j], $admin) and ($info[$i]['dn'] == $dn))
			return true;
	}

	return false;
}

// }}}

// {{{ pql_split_oldvalues(value)
function pql_split_oldvalues($value) {
	// (Possibly) split the old value array
	if(eregi(" ", $value)) {
		$values = split(" ", $value);
	} elseif(eregi(",", $value)) {
		$values = split(",", $value);
	} else {
		$values[] = $value;
	}

	asort($values);
	return($values);
}
// }}}

// {{{ pql_check_attribute(ldap_ocs, incl_ocs, attrib)
// Check wanted objectclasses if any of them require/allows
// the specific attribute
//
// Input:
//	1. Objectclasses the LDAP server understands
//	2. Objectclasses we have choosen to use when creating object
//	3. Attribute we like to check
//
// Output:
//	A two dimensional array.
//	First dimension:
//		0. No match (attrib is neither a MUST or a MAY - we're missing
//		   an objectclass for this attribute!)
//		1. This attribute is a MUST
//		2. This attribute is a MAY
//
//	Second dimension:
//		The objectclass we found a match (MUST or MAY) in
function pql_check_attribute($ldap_ocs, $incl_ocs, $attrib) {
	for($i=0; $incl_ocs[$i]; $i++) {
		// Check if there is a MUST for this attribute in one of
		// the included objectclasses
		for($j=0; $j < $ldap_ocs[$incl_ocs[$i]]['MUST']['count']; $j++)
		  if(lc($ldap_ocs[$incl_ocs[$i]]['MUST'][$j]) == lc($attrib))
			return array(1, $incl_ocs[$i]);

		// Check if there is a MAY for this attribute in one of
		// the included objectclasses
		for($j=0; $j < $ldap_ocs[$incl_ocs[$i]]['MAY']['count']; $j++)
		  if(lc($ldap_ocs[$incl_ocs[$i]]['MAY'][$j]) == lc($attrib))
			return array(2, $incl_ocs[$i]);
	}

	return(0);
}
// }}}

// {{{ pql_missing_objectclasses(linkid, dn, entry)
function pql_missing_objectclasses($linkid, $dn, $entry) {
	// Get existing objectclasses in the object
	$existing = pql_setup_branch_objectclasses(0, $dn, $ldap_linkid);
	
	// Get objectclasses from the LDAP server, finding the objectclass
	// that MUST (or if non is found, a MAY) have the attribute we're
	// trying to modify.
	$objectclasses = pql_get_subschema($linkid, 'objectclasses');
	if(is_array($objectclasses)) {
		foreach($entry as $attrib => $val) {
			// For each of the attributes (exept the objectclass one)
			// in the object, let's see if we can find it's objectclass.

			if($attrib != "objectclass") {
				foreach($objectclasses as $key => $oc) {
					// Go through the MUST attributes
					for($i=0; $i < $oc[MUST]['count']; $i++)
					  if(lc($oc[MUST][$i]) == lc($attrib))
						$objectclass[] = $key;
					
					if(!is_array($objectclass)) {
						// Didn't find a MUST, try to find a MAY attribute
						for($i=0; $i < $oc[MAY]['count']; $i++)
						  if(lc($oc[MAY][$i]) == lc($attrib))
							$objectclass[] = $key;
					}
				}
			}
		}

		if(is_array($objectclass)) {
			// We've found objectclass(es) that provide the attributes in the object.
			for($i=0; $existing[$i]; $i++)
			  $objectclass[] = $existing[$i];

			return($objectclass);
		}
	} else
	  return false;
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
