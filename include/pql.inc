<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 2.194 2004-10-19 10:40:41 turbo Exp $
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
			$this->ldap_error = $this->ldap_error . ldap_error($this->ldap_linkid) . "<p>" .
			  "Host: $host, Port: $port<br>" .
			  "Please <a href=\"index.php?logout=1\" target=\"_top\">relogin</a>.";

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
		//
		// NOTE: Enabling this if the LDAP server is an AD is a bad idea
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
			if(function_exists("ldap_start_tls") and pql_get_define("PQL_CONF_USE_TLS")) {
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

	if(!pql_get_dn($_pql->ldap_linkid, $user, '(objectclass=*)', 'BASE'))
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

// {{{ pql_user_get_quota(ldap_linkid, user)
// Get formated quota of a user
function pql_user_get_quota($ldap_linkid, $user) {
	if($_SESSION["NEW_STYLE_QUOTA"]) {
		$quota_size  = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_SIZE"));
		$quota_count = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_COUNT"));
		
		if($quota_size and $quota_count)
		  $quota = array($quota_size[0]."S,".$quota_count[0]."C");
		else
		  // Just incase we haven't the new style quota in the DB, get the old type
		  $quota = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));
	} else
	  // We're not using new style quota, get the old type...
	  $quota = pql_get_attribute($ldap_linkid, $user, pql_get_define("PQL_ATTR_QUOTA_VALUE"));

	if(!is_array($quota))
	  return false;
	else
	  return pql_parse_quota($quota[0]);
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

// ------------------------------------------------
// API functions - support
// ------------------------------------------------

// {{{ pql_get_next_ugidnumber(ldap, type)
// Returns the next free UID/GID number
function pql_get_next_ugidnumber($ldap, $type) {
    //: Get the minimum allowable ID.
	if($type == 'uid') {
		$attrib  = pql_get_define("PQL_ATTR_QMAILUID");
		$minimum = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER");
	} else {
		$attrib = pql_get_define("PQL_ATTR_QMAILGID");
		$minimum = pql_get_define("PQL_CONF_MINIMUM_GIDNUMBER");
	}

	// Just incase it's not defined, we set the minimum UID/GID
	// number availible to a safe value.
	if(!is_array($minimum))
	  $minimum = array(100);

	$min_id_val = 100;			// Set an absolute min so no one becomes root.
    foreach ($minimum as $min) {
	  if ($min > $min_id_val) {
		$min_id_val = $min;
	  }
	}

    //: Make a list of all the already used IDs.
    $filter = "(&($attrib=*)(objectclass=posixaccount))";

	$ids = array();
	foreach($ldap->ldap_basedn as $dn) {
		$results = pql_search($ldap->ldap_linkid, $dn, $filter);
		for($i=0; $i < count($results); $i++) {
		  if ($results[$i][$attrib][0] >= $min_id_val) {
		    $ids[] = $results[$i][$attrib][0];
		  }
		}
	}

	//: Look for a hole in the ID list.
	$found_id = null;
	if (empty($ids)) {
	  $found_id = $min_id_val;
	} else {
	  sort($ids, SORT_NUMERIC);
	  $prev_id = $ids[0];
	  foreach ($ids as $id) {
		if ($id > $prev_id + 1) {
		  $found_id = $prev_id + 1;
		  break;
		}
		$prev_id = $id;
	  }

	  if (!isset($found_id)) {
		$found_id = end($ids) + 1;
	  }
	}

    return $found_id;
}

// }}}

// {{{ pql_get_next_username(ldap, domain)
// Returns the next free UID/GID number
function pql_get_next_username($ldap, $domain) {
	// Get the prefix from the branch
	$prefix = pql_get_attribute($ldap->ldap_linkid, $domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX"));

	$suffix_length = pql_get_attribute($ldap->ldap_linkid, $domain, pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"));
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
	$mx = array();

	$res = getmxrr($domainname, $rec, $weight);
	if(count($rec) > 0) {
		// Take the MX with _LOWEST_ priority/weight.
		asort($weight); $old_prio = 65555;
		foreach($weight as $key => $prio) {
			if($prio < $old_prio) {
				$old_prio = $prio; $prio_key = $key;
			}
		}
		$mx["dns"] = $rec[$prio_key];
	}

	// It's possible that we have a QmailLDAP/Controls object as well...
	if(pql_get_define("PQL_CONF_CONTROL_USE")) {
		// Look for a qmailControl object which lists this domain...
		
		// Initiate a connection to the QmailLDAP/Controls DN
		$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		if($_pql_control->ldap_linkid) {
			$filter = "(&(objectclass=qmailControl)(".pql_get_define("PQL_ATTR_LOCALS")."=$domainname))";
			$info = pql_search($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter);
			for($i=0; is_array($info[$i]); $i++) {
				if($info[$i]["cn"][0])
				  $mx["qlc"][] = $info[$i]["cn"][0];
			}

			// Make sure we don't use it any more than we have to...
			ldap_close($_pql_control->ldap_linkid);
		}
	}

	return($mx);
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

	if(!function_exists("mhash") and ($hash = "{MD5}")) {
		// We do not have access to MD5 encryption - force {CRYPT}!
		unset($hash);

		$log = date("M d H:i:s");
		$log .= "MD5 encryption (mhash) not compiled into PHP. Please review the file doc/INSTALL. Falling back to using {CRYPT}";
		@error_log($log, 3, "phpQLadmin.log");
	}

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

// {{{ pql_generate_password(void)
function pql_generate_password() {
	$amount = 8;

	for(;;) {
		$pwd = NULL; $o = NULL;

		for($x=0; $x < $amount; ) {
			$y = rand(1, 1000);
			if(($y > 350) and ($y < 601)) $d = chr(rand(48, 57));
			if($y < 351) $d=chr(rand(65, 90));
			if($y > 600) $d=chr(rand(97, 122));
			if($d != $o) {
				$o = $d;
				$pwd .= $d;
				$x++;
			}
		}

		if(ereg("^[a-zA-Z]{1}([a-zA-Z]+[0-9][a-zA-Z]+)+", $pwd))
		  break;
	}

	return($pwd);
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

// {{{ pql_ldap_accountstatus(status)
// Returns text of account-status
function pql_ldap_accountstatus($status) {
    global $LANG;

    // active [default]
    // nopop
	// noaccess
    // disabled
    
    switch($status) {
      case "nopop":
		return $LANG->_('POP locked');

	  case "noaccess":
		return $LANG->_('Locked/Noaccess');

      case "disabled":
		return $LANG->_('Locked/Disabled');

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
//		3. This attribute is found in either a MUST or a MAY, but from
//		   a SUP object class...
//
//	Second dimension:
//		The objectclass we found the attribute (MUST or MAY) in
function pql_check_attribute($ldap_ocs, $incl_ocs, $attrib) {
	for($i=0; $incl_ocs[$i]; $i++) {
		// -----------------------------------------------------
		// Check if there is a MUST for this attribute in one of
		// the included objectclasses
		for($j=0; $j < $ldap_ocs[$incl_ocs[$i]]['MUST']['count']; $j++) {
			if(lc($attrib) == lc($ldap_ocs[$incl_ocs[$i]]['MUST'][$j])) {
				return array(1, $incl_ocs[$i]);
			}
		}

		// -----------------------------------------------------
		// Check if there is a MAY for this attribute in one of
		// the included objectclasses
		for($j=0; $j < $ldap_ocs[$incl_ocs[$i]]['MAY']['count']; $j++) {
			if(lc($attrib) == lc($ldap_ocs[$incl_ocs[$i]]['MAY'][$j])) {
				return array(2, $incl_ocs[$i]);
			}
		}

		// -----------------------------------------------------
		// We couldn't find a MUST or a MAY. Let's follow SUP's
		if($ldap_ocs[$incl_ocs[$i]]['SUP']) {
			// This objectClass from the LDAP server HAVE a SUP. Call ourself
			// recursivly looking for a MUST/MAY...
			$ret = pql_check_attribute($ldap_ocs, array(lc($ldap_ocs[$incl_ocs[$i]]['SUP'])), $attrib);
			if($ret[0])
			  return array(3, $incl_ocs[$i]);
		}
	}

	return(0);
}
// }}}

// {{{ pql_missing_objectclasses(linkid, dn, entry)
function pql_missing_objectclasses($linkid, $dn, $entry) {
	// Get objectclasses from the LDAP server, finding the objectclass
	// that MUST (or if non is found, a MAY) have the attribute we're
	// trying to modify.
	$objectclasses = pql_get_subschema($linkid, 'objectclasses');
	if(is_array($objectclasses)) {
		foreach($entry as $attrib => $val) {
			// For each of the attributes (exept the objectclass one)
			// in the object, let's see if we can find it's objectclass.

			if($attrib != pql_get_define("PQL_ATTR_OBJECTCLASS")) {
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
