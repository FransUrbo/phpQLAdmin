<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 2.112 2003-10-02 17:41:12 turbo Exp $
//

// ----------------------------
// include and check configuration
require("./include/pql_cache.inc");

// ----------------------------
// Get release version
$fp = fopen("./.version", "r");
$VERSION = fgets($fp, 20);
fclose($fp);

// ----------------------------
// turn on output-buffering by default
ob_start();

// ----------------------------
// API functions / classes
// ----------------------------

// PQL - main class (connection handling)
class pql {
	var $ldap_linkid = 0;					// ldap connection identifier
	var $ldap_error  = 0;					// error message incase of failiure
	var $ldap_basedn = 0;					// array of base dns for future searches etc
	
	function _find_base_option($attrib) {
	    $attrib = lc($attrib);

	    $sr = @ldap_read($this->ldap_linkid, NULL, '(objectClass=*)', array($attrib));
	    if(! $sr) die("Can't find base dn - ".ldap_error($this->ldap_linkid));
	    $entry = ldap_get_entries($this->ldap_linkid, $sr);
//printr($entry); die();

	    // Build an array of base dn's. It's possible to have multiple in a database
	    for($i=0; $i < $entry["count"]; $i++) {
			if(is_array($entry[$i][$attrib])) {
				for($j=0; $j < $entry[$i][$attrib]["count"]; $j++ )
				  $val[] = urlencode(maybe_encode($entry[$i][$attrib][$j], $attrib, $this->ldap_linkid));
			} elseif($entry[$i]["dn"]) {
				$val[] = urlencode(maybe_encode($entry[$i]["dn"], $attrib, $this->ldap_linkid));
			} else
			  $val[] = urlencode(maybe_encode($entry[$i][$attrib][0], $attrib, $this->ldap_linkid));

			$success = 1;
	    }
	    
	    // If we got at least one DN, return the array. Else return false
	    if($success) {
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
	
	function _find_basedn() {
	    // CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* namingContexts
	    return $this->_find_base_option("namingContexts");
	}

	function pql($host, $binddn = "", $bindpw = "", $no_connect = false, $do_die = 1) {
		// constructor, connect to ldap host
		if($no_connect == true)
		  return false;

		// Open a connection to the host
		$this->connect($host) or die("<b>could not connect to ldap server</b>");

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
		
		ldap_set_option($this->ldap_linkid, LDAP_OPT_DEREF, 3); // Always

		if($this->ldap_linkid) {
			// Start with a LDAPv3 bind
			if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 3)) {
				// Didn't work. Try a LDAPv2 bind...
				echo "Don't support v3 bind, trying to do a v2 bind.<br>";
				
				if(! @ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 2))
				  echo "Neither protocol LDAPv3 nor LDAPv2 worked, strange!<p>LDAP Error: ".ldap_error($this->ldap_linkid);
				
				return false;
			}

			return true;
		}
	}
	
	function bind($binddn = "", $bindpw = "") {
		// If we haven't set the DN/PW -> do a anonymous bind
		if($this->ldap_linkid and @ldap_bind($this->ldap_linkid, $binddn, $bindpw)) {
			return true;
		}
		
		return false;
	}
	
	function close(){
		// close connection to ldap host
		@ldap_unbind($this->ldap_linkid);
	}
	
	function connected(){
		if($this->ldap_linkid != false){
  			return true;
		}
	}
}

// ------------------------------------------------
// PQL - API functions
// ------------------------------------------------

// Add a user to the LDAP database
function pql_user_add($ldap_linkid, $domain, $cn, $attribs, $account_type, $branch) {
	global $account_type;

	// -----
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($domain); $rootdn = urldecode($rootdn);

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
			  $entry[$key][] = maybe_encode(trim($val), $key, $ldap_linkid);
		} else
		  $entry[$key] = maybe_encode(trim($value), $key, $ldap_linkid);
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
					for($i=0; $MUST[$i]; $i++)
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
	$userdn = maybe_encode($userdn);

	// Add the OpenLDAPaci attribute (maybe)
	if(function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid, $GLOBALS["USER_DN"], 'user');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_user_add - user creation", $userdn, $entry);

	// -----
    // Add the user to the database
    if(! ldap_add($ldap_linkid, $userdn, $entry)){
		// failed to add user
		pql_errormsg($ldap_linkid);
		die("$LDIF");
		return false;
	}
	$dns[]  = $userdn;
    
	// -----
	// If this is a 'system' account, create the Group object
	if($account_type == "system" and $entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")]) {
		// Remember this for the NEW entry object...
		$gidnr = $entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")];
		
		// Create a new LDAP object
		$entry = "";
		$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = "top";
		$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = "posixGroup";
		$entry[pql_get_define("PQL_GLOB_ATTR_CN")] = $attribs[pql_get_define("PQL_GLOB_ATTR_UID")];
		$entry[pql_get_define("PQL_GLOB_ATTR_QMAILGID")] = $gidnr;
		
		if(pql_get_define("PQL_GLOB_SUBTREE_GROUPS"))
		  $subrdn = "," . pql_get_define("PQL_GLOB_SUBTREE_GROUPS");

		$groupdn = pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $attribs[pql_get_define("PQL_CONF_USER_RDN_ATTRIBUTE", $rootdn)] . $subrdn . "," . $domain;

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_user_add - group creation", $groupdn, $entry);

		// --------------------------
		
		if(! ldap_add($ldap_linkid, $groupdn, $entry)) {
			// failed to add user
			pql_errormsg($ldap_linkid);
			die("$LDIF");
			return $dns;
		}

		$dns[] = $groupdn;
	}
	
    return $dns;
}

// Delete a user from the LDAP database
function pql_user_del($ldap, $domain, $user, $delete_forwards) {
	$linkid = $ldap->ldap_linkid;

    if(!pql_user_exist($linkid, $user))
	  // user does not exist
	  return false;

	// Remove all administrator entries which contain the user DN
	foreach($ldap->ldap_basedn as $dn) {
		$dn = urldecode($dn);

		$sr = ldap_search($linkid, $dn, pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")."=$user");
		$info = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
		for($i=0; $i<$info["count"]; $i++) {
			unset($entry); unset($adms);

			// Get administrator attributes for this domain/branch DN
			$admins	= pql_get_domain_value($ldap, $info[$i]["dn"], pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"));
			for($j=0; $admins[$j]; $j++) {
				if($admins[$j] != $user)
				  $adms[] = $admins[$j];
			}
				  
			if(is_array($adms)) {
				// Add the administrators that's left to the DN
				$entry[pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")] = $adms;
				if(! ldap_mod_replace($linkid, $info[$i]["dn"], $entry))
				  pql_errormsg($linkid);
			}
		}
	}

	// Get uidnr of user
	$uidnr = pql_get_userattribute($linkid, $user, pql_get_define("PQL_GLOB_ATTR_QMAILUID"));
	$uidnr = $uidnr[0];
    
	// Delete the group object if it exists
	$filter = "(&(".pql_get_define("PQL_GLOB_ATTR_QMAILGID")."=$uidnr)(objectclass=posixGroup))";
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
			$email = pql_get_userattribute($linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAIL"));
			$aliases = pql_get_userattribute($linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"));
			
			$addresses[] = $email[0];
			if(is_array($aliases)){
				$addresses = array_merge($addresses, $aliases);
			}
		}
    }
    
    // delete the user
    if(!ldap_delete($linkid, $user)){
		pql_errormsg($linkid);
		return false;
    }
    
    // user entry has been removed -> remove the cached version
    pql_cache_userentry_remove($user);
    
    // delete forwards to this account?
    if ($delete_forwards and $forwarders) {
		foreach($forwarders as $forward) {
			// get the forwarding addresses of this user
			$fwd_addresses = pql_get_userattribute($linkid, $forward['reference'], pql_get_define("PQL_GLOB_ATTR_FORWARDS"));
			foreach($addresses as $address) {
				// does this user forward to the removed user or one of his aliases?
				$rem_key = array_search($address, $fwd_addresses);
				if ($rem_key !== false) {
					// we found a forward -> remove it 
					pql_replace_userattribute($linkid, $forward['reference'],
											  pql_get_define("PQL_GLOB_ATTR_FORWARDS"),
											  $fwd_addresses[$rem_key]);
				}
			}
		} 
    }

    return true;
}

// adds a domain into ldap-tree (o|ou|dc-record)
function pql_domain_add($ldap_linkid, $basedn, $branch_name) {
	// Maybe UTF-8 encode the branch name - to allow international characters
	$branch_name = maybe_encode($branch_name, pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn), $ldap_linkid);

	$entry[pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)] = $branch_name;
	$dn = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) . "=" . $branch_name . "," . $basedn;
	
	// Setup objectclasses
	$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $basedn);
	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "o")
	  $entry[pql_get_define("PQL_GLOB_ATTR_O")] = maybe_encode($branch_name);
	else
	  $entry[pql_get_define("PQL_GLOB_ATTR_O")] = 0;

	// Add the OpenLDAPaci attribute (maybe)
	if(function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid, $GLOBALS["USER_DN"], 'branch');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_domain_add - create domain", $dn, $entry);

    // adds the user
    if(! @ldap_add($ldap_linkid, $dn, $entry)) {
		// failed to add user
		pql_errormsg($ldap_linkid);
		die("$LDIF");
		return false;
    }
	$dns[] = $dn;

    // Add the USER subtree if defined
    if(pql_get_define("PQL_GLOB_SUBTREE_USERS")) {
		$entry = "";
		$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = "top";
		$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = "organizationalUnit";
		
		$ou = eregi_replace(",$", "", pql_get_define("PQL_GLOB_SUBTREE_USERS") . ",");
		$ou = eregi_replace("^".pql_get_define("PQL_GLOB_ATTR_OU")."=", "", $ou); // Remove the 'ou=' from the attribute
		$entry[pql_get_define("PQL_GLOB_ATTR_OU")] = $ou;
		
		$dn = pql_get_define("PQL_GLOB_SUBTREE_USERS") . "," .
			  pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) .
			  "=" . $branch_name . "," . $basedn;

		// Add the OpenLDAPaci attribute (maybe)
		if(function_exists("user_generate_aci"))
		  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid, $GLOBALS["USER_DN"], 'branch');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_domain_add - create subtree users", $dn, $entry);

		// adds the user
		if(!@ldap_add($ldap_linkid, $dn, $entry)){
			// failed to add user subtree
			pql_errormsg($ldap_linkid);
			die($LDIF);
			return $dns;
		}
		$dns[] = $dn;
    }
    
    // Add the GROUPS subtree if defined
    if(pql_get_define("PQL_GLOB_SUBTREE_GROUPS")) {
		$entry = "";
		$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = "top";
		$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = "organizationalUnit";

		$ou = eregi_replace(",$", "", pql_get_define("PQL_GLOB_SUBTREE_GROUPS") . ",");
		$ou = eregi_replace("^".pql_get_define("PQL_GLOB_ATTR_OU")."=", "", $ou); // Remove the 'ou=' from the attribute
		$entry[pql_get_define("PQL_GLOB_ATTR_OU")] = $ou;
		
		$dn = pql_get_define("PQL_GLOB_SUBTREE_GROUPS") . "," .
			  pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) .
			  "=" . $branch_name . "," . $basedn;

		// Add the OpenLDAPaci attribute (maybe)
		if(function_exists("user_generate_aci"))
		  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid, $GLOBALS["USER_DN"], 'branch');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_domain_add - create subtree groups", $dn, $entry);

		if(!@ldap_add($ldap_linkid, $dn, $entry)){
			pql_errormsg($ldap_linkid);
			die($LDIF);
			return $dns;
		}
		$dns[] = $dn;
    }

    return $dns;
}

// removes a domain with all listed users
function pql_domain_del($ldap, $domain, $delete_forwards) {
	$linkid = $ldap->ldap_linkid;

	// Make sure that the logged in user isn't located under the
	// domain/branch being deleted...
	if(eregi($domain, $GLOBALS["USER_DN"])) {
		$msg=urlencode(format_error("Sorry, I can't allow you to delete the branch under which you yourself is situated!"));
		header("Location: " . pql_get_define("PQL_GLOB_URI") . "domain_detail.php?domain=$domain&msg=$msg");
	}

    if(!pql_domain_exist($ldap, $domain)) {
		// domain does not exist
		return false;
    }
    
    // remove all users of domain
    $users = pql_get_user($linkid, $domain);
    if(is_array($users)) {
		foreach($users as $user) {
			pql_user_del($ldap, $domain, $user, $delete_forwards);
		}
    }

	// Get subbranches
	$branches = pql_get_subbranch($linkid, $domain, 0);
	if(is_array($branches)) {
		for($i=0; $branches[$i]; $i++) {
			// Find every object under this DN and delete it
			$sr = @ldap_search($linkid, $branches[$i], 'objectclass=*') or pql_errormsg($linkid);
			$info = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
			for ($j=0; $j<$info["count"]; $j++)
			  if($info[$j]["dn"] != $branches[$i])
				// Don't delete the branch quite yet
				if(!ldap_delete($linkid, $info[$j]["dn"]))
				  pql_errormsg($linkid);
			
			// Delete the organizationalUnit itself
			if(!ldap_delete($linkid, $branches[$i])) {
				pql_errormsg($linkid);
				return false;
			}
		}
	}

    // remove domain entry itself (ou)
    if(!ldap_delete($linkid, $domain)) {
		pql_errormsg($linkid);
		return false;
    }

    return true;
}

// ------------------------------------------------

function pql_modify_user_addressbook($ldap_linkid, $user, $attribs) {
	// What's the Root DN (namingContexts) of this user
	$rootdn = pql_get_rootdn($user); $rootdn = urldecode($rootdn);

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
    $oc_user = pql_get_userattribute($ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS"));
    
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

// ------------------------------------------------

// check if domain exists
function pql_domain_exist($ldap, $domain, $rootdn = '') {
	$domain = urldecode($domain); $domain = maybe_encode($domain);

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

// ------------------------------------------------

function pql_user_exist($ldap_linkid, $user) {
    // check if user exists
    if(!pql_search_attribute($ldap_linkid, $user, 'objectclass', '*'))
	  // User does not exist in LDAP tree
	  return(false); 

	return(true);
}

// ------------------------------------------------

// check if any mail or mailalternateaddress record with this email exists in the ldap tree
function pql_email_exists($ldap, $email) {
	$filter = "(|(" . pql_get_define("PQL_GLOB_ATTR_MAIL") . "=" . $email . ")(" . pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE") . "=" . $email . "))";

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

// ------------------------------------------------

// change an attribute of a user
function pql_modify_userattribute($ldap_linkid, $user, $attrib, $oldvalue, $newvalue) {
    // get all attributes first
    $entry = pql_get_userattribute($ldap_linkid, $user, $attrib);

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

		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($user);

	// What's the Root DN (namingContexts) of this user
	$rootdn = pql_get_rootdn($user); $rootdn = urldecode($rootdn);

    if ($attrib == pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)) {
		pql_cache_userdn_remove($user);
    }
    
    return true;
}

// ------------------------------------------------

// replaces an attribute of a user
// (only use for single-field attributes)
// delete an attribut if value is an empty string
function pql_replace_userattribute($ldap_linkid, $user, $attrib, $value) {
	$attrib = lc($attrib);

    if(!$value)
	  // Delete all attributes of this type, regardless of value
	  $entry[$attrib] = array();
	elseif(!$attrib)
	  $entry = $value;
	else
	  $entry[$attrib] = maybe_encode($value, $attrib, $ldap_linkid);

	// replace the attribute
	if(!@ldap_mod_replace($ldap_linkid, $user, $entry)) {
	    // if the value is empty (""), this causes an error in OpenLDAP 2.x, try to ldap_mod_del
	    if($value == "") {
			$oldvalue = pql_get_userattribute($ldap_linkid, $user, $attrib);
			$oldvalue = $oldvalue[0];
			$entry[$attrib] = $oldvalue;

			if(ldap_mod_del($ldap_linkid, $user, $entry))
			  return true;
			else {
				pql_errormsg($ldap_linkid);
				return false;
			}
	    } else {
			if(ldap_errno($ldap_linkid) == 65) {
				// Object class violation - try to add the missing objectclass

				// Get existing objectclasses in the object
				$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $user, $ldap_linkid);

				// Get objectclasses from the LDAP server, finding the objectclass
				// that MUST (or if non is found, a MAY) have the attribute we're
				// trying to modify.
				$objectclasses = pql_get_subschema($ldap_linkid, 'objectclasses');
				if(is_array($objectclasses)) {
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

				if($objectclass[0] and !$objectclass[1]) {
					// There is only one objectclass which allow/require this attribute

					// See if we have the other MUST attributes that the objectclass require
					for($i=0; $i < $objectclasses[$objectclass[0]]['MUST']['count']; $i++) {
						$oldvalue = pql_get_userattribute($ldap_linkid, $user, $objectclasses[$objectclass[0]]['MUST'][$i]);
						if(!oldvalue or !is_array($oldvalue))
						  $missing[] = $objectclasses[$objectclass[0]]['MUST'][$i];
					}

					if(is_array($missing)) {
						// We're missing attribute(s) - create an array to be used by the user_add_missing_attributes.php page.
						global $MISSING;
						$MISSING['attrib'] = $attrib;
						$MISSING['objectclass'] = $objectclass[0];

						for($i=0; $missing[$i]; $i++)
						  $MISSING['attributes'][] = $missing[$i];
					}

					// Add this objectclass to the existing objectclasses
					$entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][] = $objectclass[0];
				} else { 
					// TODO: We have more than one attribute!!
					$msg  = "We retreived more than one objectclass (or possibly non at al!) that MAY or MUST ";
					$msg .= "the attribute <b>$attrib</b>.<br>This is however not yet supported.<br>Objectclasses ";
					$msg .= "retreived is:"; echo $msg; printr($objectclass);
					die();
				}

				$url = "user_add_missing_attributes.php?user=".urlencode($user);
				require("./user_add_missing_attributes.php");
				die();
			}
		}
	    
	    pql_errormsg($ldap_linkid);
	    return false;
	}
	
	// user entry has changed -> remove the cached version
	pql_cache_userentry_remove($user);
	
	return true;
}

// ------------------------------------------------

// get all domains listed in ldap-tree (ou|dc-records)
function pql_get_domains($ldap) {
	foreach($ldap->ldap_basedn as $dn)  {
		$dn = urldecode($dn);

		$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $dn) .
                  "=*)" . pql_setup_branch_objectclasses(1, $dn) . ")";

		$sr = @ldap_list($ldap->ldap_linkid, $dn, $filter) or pql_errormsg($ldap->ldap_linkid);
		$info = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
		if($info["count"] == 0) {
			// No entries, try a BASE search
			$sr = @ldap_read($ldap->ldap_linkid, $dn, $filter) or pql_errormsg($ldap->ldap_linkid);
			$info = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
		}
		
		for ($i=0; $i<$info["count"]; $i++) {
			$domains[] = $info[$i]["dn"];
		}
		
	}

	return $domains;
}

// ------------------------------------------------

// get all users of a domain
function pql_get_user($ldap_linkid, $dn) {
	$dn = urldecode($dn);

	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);

	// Filter out any object which is a posixGroup.
	// (&(cn=*)(!(objectclass=posixgroup)))
	$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . "=*)(!(objectclass=posixgroup)))";
	
	$sr = @ldap_search($ldap_linkid, $dn, $filter) or pql_errormsg($ldap_linkid);
	$info = ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);
	for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i]["dn"];
	}
	
	return $user;
}

// ------------------------------------------------
// function based in PQL_CONF_GET_USER to search by the cn first letter
function pql_get_user_letter($ldap_linkid, $domain, $letra) {
    $sr = ldap_search($ldap_linkid, $domain, "(".pql_get_define("PQL_GLOB_ATTR_UID")."=".$letra."*)");
    $info = ldap_get_entries($ldap_linkid, $sr);
    
    for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i]["dn"];
    }
    
    return $user;
}

// ------------------------------------------------
// function based in PQL_CONF_GET_USER to search by an initial number at cn attribute
function pql_get_user_number($ldap_linkid, $domain) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);

    for($numero=0; $numero <= 9; $numero++) {
		$sr = ldap_search($ldap_linkid, $domain, "(".pql_get_define("PQL_GLOB_ATTR_UID")."=".$numero."*)");
		$info = ldap_get_entries($ldap_linkid, $sr);
		
		for ($i=0; $i<$info["count"]; $i++) {
			$user[] = $info[$i][pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)][0];
		}
    }
    
    return $user;
}

// ------------------------------------------------

function pql_get_userattribute($ldap_linkid, $user, $attribute) {
    $attribute = lc($attribute);

    // check whether this user is cached
    $value = '';
    
	if(pql_cache_userentry_get($user, $attribute, $value))
	  // the user is cached  -> return the value for the requested attribute  
	  return($value);

    // not yet cached -> get the userentry and cache it
    $entries = pql_get_userentry($ldap_linkid, $user);

    if($entries) {
		// INTERNATIONALIZE values
		foreach($entries as $attrib => $e) {
			for($i=0; $e[$i]; $i++)
			  $values[$attrib][] = maybe_decode($e[$i]);
		}

		pql_cache_userentry_add($user, $values);
		return($values[$attribute]);	
	} else
	  return(false);
}

// ------------------------------------------------
function pql_get_userentry($ldap_linkid, $user) {
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

    // free result
    //@ldap_free_result($rid):
    
    // return entry
    return $entry;
}

// ------------------------------------------------

// get formated quota of a user
function pql_get_userquota($ldap_linkid, $user) {
    $quota = pql_get_userattribute($ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_QUOTA"));
    if(!is_array($quota))
	  return false;
	else    
	  return pql_parse_quota($quota[0]);
}

// ------------------------------------------------

// retrieve the dn for a given reference
function pql_get_dn($ldap, $reference, $multiples = 0) {
    // check if we already have the dn for this user chached
    $dn = '';
    if(pql_cache_userdn_get($reference, $dn))
	  return($dn);

    // dn not in cache -> search for it
	foreach($ldap->ldap_basedn as $base) {
		$base = urldecode($base);

		$dn = pql_search_attribute($ldap->ldap_linkid, $base,
								   pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $base),
								   $reference);
		if($multiples and $dn)
		  $dns[] = $dn;
		elseif($dn) {
			// we got the dn -> remember it
			pql_cache_userdn_add($reference, $dn);
			
			return($dn);
		}
	}

	if($multiples and is_array($dns))
	  return($dns);
	else
	  return(false);
}

// ------------------------------------------------
function pql_errormsg($ldap_linkid) {
	if($ldap_linkid)
	  echo format_error("LDAP-Error: ") . ldap_error($ldap_linkid) . "<br>";
}

// ------------------------------------------------
function pql_ldap_error($ldap_linkid, $attrib = "") {
    // an error-message wrapper for adding special comments to some
    // error conditions
    
    $errno = ldap_errno($ldap_linkid);
    $error = ldap_error($ldap_linkid);
    
    switch($errno){
      case 18:
		// Inappropriate matching --> invalid qmail.schema
		$comment = "This error is caused by an invalid qmail.schema. Please look at README.OpenLDAP2 in your doc-directory.";
		break;
    }
    
    $msg = $errno . ": " . $error;
    
    if($comment){
		$msg .= " (" . $comment . ")";
    }
    
    return $msg;
}

// ------------------------------------------------

function pql_parse_quota($quota_string) {
	$quota = '';

    // get max bytes
    preg_match("/([0-9]*)S/", $quota_string, $matches);
    
    if(isset($matches[1])) {
		$quota["maxsize"] = intval($matches[1] / 1024);
    
		// get max mails
		preg_match("/([0-9]*)C/", $quota_string, $matches);
		$quota["maxmails"] =  $matches[1];
	}

    return $quota;
}

// ------------------------------------------------

// search $attribute with $value in whole ldap tree, returns dn or false
function pql_search_attribute($ldap_linkid, $basedn, $attribute, $value) {
	$value = maybe_encode($value, $attribute, $ldap_linkid);

	// URL decode the object DN if it's URL ENcoded
	if(ereg("%3D", $basedn))
	  $basedn = urldecode($basedn);

    $filter = '('.$attribute.'='.$value.')';
//echo "$basedn: $filter ($attribute)<br>\n";
    $sr = @ldap_search($ldap_linkid, $basedn, $filter, array($attribute));
    if(@ldap_count_entries($ldap_linkid, $sr) > 0) {
		$ed = ldap_first_entry($ldap_linkid, $sr);
		return ldap_get_dn($ldap_linkid, $ed);
	} else {
		return false;
	}
}

// ------------------------------------------------

// search all accounts with forwarders to $user@$domain
function pql_search_forwarders($ldap, $user) {
	$return = '';
	$linkid = $ldap->ldap_linkid;

    // get all email addresses of a user
    $email = pql_get_userattribute($linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAIL"));
    $aliases = pql_get_userattribute($linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"));
    
    $addresses[] = $email[0];
    if(is_array($aliases)){
		$addresses = array_merge($addresses, $aliases);
    }
    
    // create filter
    $filter = "(|";
    foreach($addresses as $add){
		$filter .= "(" . pql_get_define("PQL_GLOB_ATTR_FORWARDS") ."=" . $add . ")";
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
									  pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn) => $result[pql_get_define("PQL_GLOB_ATTR_CN")][0],
									  "email" => $result[pql_get_define("PQL_GLOB_ATTR_MAIL")][0]);
					
					if(is_array($result[pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE")])){
						foreach($result[pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE")] as $key => $mailalternateaddress){
							if((string)$key != "count"){
								$return[] = array("reference" => $result["dn"],
												  pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn) => $result[pql_get_define("PQL_GLOB_ATTR_CN")][0],
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

// ------------------------------------------------

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
		return "{crypt}" . crypt($password);
    }
}

// ------------------------------------------------

function table_bgcolor($echo = 1) {
    global $flag;
    
    if($flag) {
		$flag = false;
		$ret="c1";
    } else {
		$flag = true;
		$ret="c2";
    }

	if($echo) {
		echo $ret;
	} else {
		return $ret;
	}
}

// ------------------------------------------------

function format_error($msg) {
    return "<span class=\"error\">" . $msg . "</span>";
}

// ------------------------------------------------

function print_status_msg($msg){
?>
  <table cellpadding="3" cellspacing="0" border="0" width="100%">
    <th colspan="3" align="left">
      <tr class="message">
        <td colspan="2" class="message">
          <img src="images/info.png" width="16" height="16" border="0" align="left">
          <table>
            <?php echo stripslashes($msg) . "\n"; ?>
          </table>
        </td>
      </tr>
    </th>
  </table>

  <br><br>
<?php
}

// ------------------------------------------------

function check_email($email){
    $split = split("@", $email);
    
    if(count($split) != 2){
		// does not contain @ or has to much of it
		return false;
    }
    
    // check user part
    if(!preg_match("/^[a-z0-9-+=#]+([\._a-z0-9-+=#]?[a-z0-9]+)*$/i", $split[0])){
		// user part is not valid:
		// - must start with a valid character (a-Z, 0-9)
		// - may contain a special char (. _ + = # -), but not at start and end
		//   and following another special char
		// - must end with a valid character (a-Z, 0-9)
		// - must contain at least 1 character
		return false;
    }
    
    // check host part
    if(!preg_match("/^[a-z0-9]+([\._-]?[a-z0-9]{2,})*$/i", $split[1])){
		// host part is not valid:
		// - must start with a valid character (a-Z, 0-9)
		// - may contain a special char (. or -), but not at start and end
		//   and following another special char
		// - must end with 2 valid characters (a-Z, 0-9)
		return false;
    }
    
    return true;
}

// ------------------------------------------------

function check_hostaddress($host, $force_dot) {
    // checks an fqdn - if force_dot is true, it must contain a dot
    if($force_dot)
	  return preg_match("/^([0-9a-z-]+\.[0-9a-z-]+)+$/i", $host);
	else
	  return preg_match("/[^0-9a-z]/i", $host);
}

// ------------------------------------------------

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

// ------------------------------------------------

function pql_strip_domain($email) {
    // get username of an email address
    $email = split("@", $email);
    
    if(count($email) == 2)
	  return $email[0];
    
    return false;
}

// ------------------------------------------------

function pql_strip_username($email) {
    // get domainname of an email address
    $email = split("@", $email);
    
    if(count($email) == 2)
	  return $email[1];
    
    return false;
}

// ------------------------------------------------

// replaces dynamic vars in constants
// %key% will be replaced with $replacement["key"]
function pql_complete_constant($text, $replacement) {
    if(!is_array($replacement))
	  return false;
    
	// prepare patterns
    foreach($replacement as $key => $value)
		$pattern[] = "/(%$key%)/i";
    
    return preg_replace($pattern, $replacement, $text);
}

// --------------------------------------
// format ldap attribute codes to text
// --------------------------------------

// returns human readable version of users quota
function pql_ldap_mailquota($quota) {
    global $LANG;

    if($quota["maxmails"] == "")
	  $txt = $LANG->_('Unlimited amount');
	else
	  $txt = pql_complete_constant($LANG->_('Max %maxmails% mails'), array("maxmails" => $quota["maxmails"]));
    
    $txt .= ",<br>";
    
    if($quota["maxsize"] == "")
	  $txt .= $LANG->_('Unlimited size');
	else
	  $txt .= pql_complete_constant($LANG->_('Max %maxsize% KB'), array("maxsize" => $quota["maxsize"]));
    
    return $txt;
}

// ------------------------------------------------

// returns text of account-status
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

// ------------------------------------------------

// returns text of delivery mode
function pql_ldap_deliverymode($mode) {
    global $LANG;

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
		return $LANG->_('Do auto reply');

      case "echo":
		return $LANG->_('Echo to console (tricky)');

      default:
		return $LANG->_('Normal');
    }
}

// Returns the next free UID/GID number
function pql_get_next_uidnumber($ldap) {
    $filter = "(&(" . pql_get_define("PQL_GLOB_ATTR_QMAILUID") . "=*)(objectclass=posixaccount))";

	foreach($ldap->ldap_basedn as $dn) {
		$dn = urldecode($dn);

		$sr = ldap_search($ldap->ldap_linkid, $dn, $filter);
		$results = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
	
		// Find the highest availible UIDNUMBER
		$highest = 0;
		for($i=0; $i < $results["count"]; $i++) {
			if(($results[$i][pql_get_define("PQL_GLOB_ATTR_QMAILUID")][0] > $highest) &&
			   ($results[$i][pql_get_define("PQL_GLOB_ATTR_QMAILUID")][0] < 64000))
			  {
				  $highest = $results[$i][pql_get_define("PQL_GLOB_ATTR_QMAILUID")][0];
			  }
		}
	}

	if($highest)
	$highest++; // Increase with one!
	else
	  if(pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER"))
		$highest = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER");
	  else
		$highest = 1000;
	
    return $highest;
}

// Returns the next free UID/GID number
function pql_get_next_username($ldap, $domain) {
	// Get the prefix from the branch
	$prefix = pql_get_domain_value($ldap, $domain, pql_get_define("PQL_GLOB_ATTR_USERNAMEPREFIX"));

	if($prefix) {
		// Under what namingContexts is this domain/branch located?
		$rootdn = pql_get_rootdn($domain); $rootdn = urldecode($rootdn);

		// Setup the LDAP search filter
//		$filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) . '=' . $prefix . '*';
		$filter = pql_get_define("PQL_GLOB_ATTR_UID")."=$prefix*"; // DEBUG

		// Search the whole database for all users with this prefix
		foreach($ldap->ldap_basedn as $dn) {
			$dn = urldecode($dn); unset($info);

			$sr     = ldap_search($ldap->ldap_linkid, $dn, $filter);
			$info   = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);

			// Rearrange the array, we're only interested in the PQL_CONF_REFERENCE_USERS_WITH value			
			for($i=0; $i < $info["count"]; $i++)
			  $USERS[] = $info[$i][pql_get_define("PQL_GLOB_ATTR_UID")][0];
		}

		if(!$USERS)
		  return($prefix."0001");
		else {
			// We have previous values in the DB. 
			sort($USERS);
			$nr  = eregi_replace($prefix, "", $USERS[count($USERS)-1]);
			return($prefix.sprintf("%04d", $nr + 1));
		}
	}

}

function pql_get_domain_value($ldap, $dn, $value, $match = '', $all = 0) {
	$value = lc($value); $dn = urldecode($dn);

	$recursive = 1; $success = 0; $results = '';

	if(!$match) {
		$match = '*';
		$recursive = 0;
	}

	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);

	// Setup a filter to look for DOMAINS (branches)
//	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "o")
	  $filter = "$value=$match";
//	else
//	  $filter  = "(&($value=$match)(".pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn)."=*))";
	//echo "pql_get_domain_value(): '$dn' '$filter' $value<br>"; // DEBUG

	// Search from the DN
	if(!$recursive)
	  // ... only
	  $sr = ldap_read($ldap->ldap_linkid, $dn, $filter, array($value));
	else
	  // ... and below
	  $sr = ldap_search($ldap->ldap_linkid, $dn, $filter, array($value));

	// Get the entries
	$result = @ldap_get_entries($ldap->ldap_linkid, $sr)
	  or pql_errormsg($ldap->ldap_linkid);

	// Go through the result, if there is any
	for($i=0; $i < $result["count"]; $i++) {
		// Extract all the descriptor fields
		if(($value == pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")) or
		   ($value == pql_get_define("PQL_GLOB_ATTR_SEEALSO")) or
		   ($value == pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME")) or
		   ($value == pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")) or
		   ($value == pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR")))
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
					$results[] = maybe_decode($result[0][$value][$j]);
					$success = 1;
				}
			}
		} elseif(($value == 'passwordscheme') or
				 ($value == 'userobjectclass') or
				 ($value == 'branchobjectclass')) {
			// We're requesting a normal multivalue attribute
			for($j=0; $j < $result[$i][$value]["count"]; $j++) {
				$results[] = maybe_decode($result[$i][$value][$j]);
				$success = 1;
			}
		} else {
			// Return the value we're looking for (single valued).
			$results = maybe_decode($result[0][$value][0]);
			$success = 1;
		}
	} // end for($i)

	if(!$success and (($value != pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")) and
					  ($value != pql_get_define("PQL_GLOB_ATTR_SEEALSO")) and
					  ($value != pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")) or
					  ($value == pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"))))
	  return;

	if($success and (($value == pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")) or
					 ($value == pql_get_define("PQL_GLOB_ATTR_SEEALSO")) or
					 ($value == pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME")) or
					 ($value == pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")) or
					 ($value == pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR")))
	   and !is_array($results))
	  {
		  // Convert a flat variable to a array
		  $tmp = $results; unset($results);
		  $results[] = maybe_decode($tmp);
	  }

	return $results;
}

// Set a embedded domain default value
function pql_set_domain_value($linkid, $dn, $type, $value) {
	$DEBUG = 0;
	$dn = urldecode($dn);

	if($type) {
		$type = lc($type);
		
		// Setup the value array
		if(is_array($value)) {
			// We're supplied with an array, add those values
			foreach($value as $val) {
				$entry[$type][] = maybe_encode($val, $type, $linkid);
			}
		} else {
			$entry[$type] = maybe_encode($value, $type, $linkid);
		}
	} else {
		// INTERNATIONALIZE
		if(is_array($value)) {
			foreach($value as $attrib => $val) {
				for($i=0; $val[$i]; $i++)
				  $entry[$attrib][] = maybe_encode($val[$i], $attrib, $linkid);
			}
		} else {
			$entry = $value;
		}
	}

	if($value) {
		// Setup objectclasses
		if(!($entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $dn, $linkid)))
		  unset($entry[pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")]);
	}
		
	// Create a LDIF object to print in case of error
	$account_type = $type;
	$LDIF = pql_create_ldif("pql_set_domain_value", $dn, $entry);
	
	// --------------------------
	// DEBUG mode: Print the LDIF
	if($DEBUG >= 1)
	  die("$LDIF");

	// Do the LDAP modification
	if($value != '') {
		// Replace the current attribute value
		if(! ldap_mod_replace($linkid, $dn, $entry)) {
			pql_errormsg($linkid);
			die($LDIF);
			return false;
		}
	} else {
		// Delete all attributes of this type, regardless of value
		$entry[$type] = array();

		if(! ldap_mod_del($linkid, $dn, $entry)) {
			pql_errormsg($linkid);
			die($LDIF);
			return false;
		}
	}
	
    return true;
}

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

function pql_get_mx($ldap, $domainname) {
	$res = getmxrr($domainname, $rec, $weight);
	if(count($rec) == 0) {
		// Not found in the DNS, look for a qmailControl object which lists
		// this domain...

		$filter = "(&(objectclass=qmailControl)(".pql_get_define("PQL_GLOB_ATTR_LOCALS")."=$domainname))";
		$sr = @ldap_search($ldap, $GLOBALS["USER_SEARCH_DN_CTR"], $filter)
		  or pql_errormsg($ldap);

		$info = ldap_get_entries($ldap, $sr)
		  or pql_errormsg($ldap);
		for($i=0; $i<$info["count"]; $i++)
		  for($j=0; $j<$info[$i][pql_get_define("PQL_GLOB_ATTR_LOCALS")]["count"]; $j++)
			if($info[$i][pql_get_define("PQL_GLOB_ATTR_LOCALS")][$j] == $domainname)
			  return(array(0, $info[$i][pql_get_define("PQL_GLOB_ATTR_CN")][0]));

		return(array(0,0));
	} else {
		// Take the MX with _LOWEST_ priority/weight.
		asort($weight); $old_prio = 65555;
		foreach($weight as $key => $prio){
			if($prio < $old_prio) {
				$old_prio = $prio; $prio_key = $key;
			}
		}
		
		return(array(1, $rec[$prio_key]));
	}
}

function pql_get_subschema($ldap_linkid, $attrib) {
    $attrib = lc($attrib);
	
    // Get the DN for the subSchema
	// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' 'objectClass=*' subschemaSubentry
	//
	// TODO: Resue the $this->_find_base_option() function here!
    $sr    = ldap_read($ldap_linkid, NULL, '(objectClass=*)', array('subschemaSubentry'));
    $entry = ldap_get_entries($ldap_linkid, $sr);
    if($entry[0][pql_get_define("PQL_GLOB_ATTR_SUBSCHEMASUBENTRY")][0]) {
		// Get the subSchemaAttributes from the subSchema
		// CMD: /usr/bin/ldapsearch -x -LLL -h localhost -s base -b 'cn=Subschema' 'objectClass=*' $attrib
		$sr     = ldap_read($ldap_linkid,
							$entry[0][pql_get_define("PQL_GLOB_ATTR_SUBSCHEMASUBENTRY")][0],
							'objectClass=*', array($attrib));
		$entry  = ldap_get_entries($ldap_linkid, $sr);
		for($i=0; $i < $entry[0][$attrib]["count"]; $i++) {
			// ------------------------
			// Retreive NAME
			$words = split("NAME", $entry[0][$attrib][$i]);
			if($words[1]) {
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
			if($words[1]) {
				$words = split("\' ", $words[1]);
				$words = split(" \'", $words[0]);
				
				$VALUE[$name]["DESC"] = $words[1];
			}
			
			// ------------------------
			// Retreive MAY's
			$words = split("MAY", $entry[0][$attrib][$i]);
			if($words[1]) {
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
			if($words[1]) {
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
			if($words[1]) {
				$words = split(" ", $words[1]);
				$VALUE[$name]["EQUALITY"] = $words[1];
			}
			
			// ------------------------
			// Retreive SYNTAX
			$words = split("SYNTAX", $entry[0][$attrib][$i]);
			if($words[1]) {
				$words = split(" ", $words[1]);
				$VALUE[$name]["SYNTAX"] = $words[1];
			}
			
			// ------------------------
			// Retreive USAGE
			$words = split("USAGE", $entry[0][$attrib][$i]);
			if($words[1]) {
				$words = split(" ", $words[1]);
				$VALUE[$name]["USAGE"] = $words[1];
			}
			
			// ------------------------
			// Retreive SUP
			if(eregi("SUP", $entry[0][$attrib][$i])) {
				$words = split("SUP", $entry[0][$attrib][$i]);
				if($words[1]) {
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

function pql_get_subbranch($linkid, $dn, $filter = 1) {
	$dn = urldecode($dn);

	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);

	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc") {
		$ocb = "domain";
	} else {
		$ocb = "organizationalUnit";
	}
	
	$sr   = ldap_search($linkid, $dn, 'objectclass=organizationalUnit');
	$info = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
	for ($i=0; $i<$info["count"]; $i++) {
		if($filter) {
			// Filter out the most obvious subbranches we don't put users in...
			if(!eregi('group', $info[$i]["dn"]) and
			   !eregi('qmail', $info[$i]["dn"]) and
			   !eregi('auto\.', $info[$i]["dn"]))
			  $branches[] = $info[$i]["dn"];
		} else
		  $branches[] = $info[$i]["dn"];
	}

	return $branches;
}

// Setup object class entry for a domain/branch with all
// necessary objectclasses
function pql_setup_branch_objectclasses($filter = 0, $dn, $linkid = '') {
	if($dn and $linkid) {
		// Retreive existing objectClasses from the object
		$object = ldap_explode_dn($dn, 0);
		$sr     = @ldap_read($linkid, $dn, $object[0], array('objectclass'));
		$ocs    = @ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
		
		for($j=0; $j < $ocs[0][pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")]["count"]; $j++) {
			if(eregi('phpQLAdminBranch', $ocs[0][pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][$j]))
				$got_phpqladmin_objectclass_branch = 1;

			$objectclass[] = $ocs[0][pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][$j];
		}
	} else {
		// No previous DN, we're probably creating a new

		// ObjectClasses for this root DN
		if($dn) {
			// What's the Root DN (namingContexts) for this domain
			$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);

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

function pql_validate_administrator($linkid, $dn, $admin) {
	$dn = urldecode($dn);

	$sr = @ldap_search($linkid, $dn, '('.pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR").'=*)',
					   array(pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")));
	if(!$sr)
	  return false;

	$info = @ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
	for($i=0; $i<$info["count"]; $i++) {
		for($j=0; $j<$info[$i][pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")]["count"]; $j++)
		  // TODO: We shouldn't do a regexp but a 'case insensitive equal'
		  if(eregi($info[$i][pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")][$j], $admin) and ($info[$i]['dn'] == $dn))
			return true;
	}

	return false;
}

// --------------------------
// Look for 'administrator/seealso = old value',
// replace with 'administrator/seealso = new value'.
function pql_replace_admins($ldap, $old, $new) {
	// Setup the search filter
	$filter  = '(|';
	$filter .= '('.pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")."=$old".')';
	$filter .= '('.pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR")."=$old".')';
	$filter .= '('.pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")."=$old".')';
	$filter .= '('.pql_get_define("PQL_GLOB_ATTR_SEEALSO")."=$old".')';
	$filter .= ')';

	// We're only interested in these attributes
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"),
					 pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR"),
					 pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR"),
					 pql_get_define("PQL_GLOB_ATTR_SEEALSO"));

	// Go through the namingContexts one by one
	foreach($ldap->ldap_basedn as $base) {
		$base = urldecode($base);
		
		$sr   = ldap_search($ldap->ldap_linkid, $base, $filter, $attribs);
		$info = ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
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
			$LDIF = pql_create_ldif("pql_replace_admins", $info[$i]["dn"], $entry);
			
			// Modify this object
			if(!ldap_mod_replace($ldap->ldap_linkid, $info[$i]["dn"], $entry)) {
				pql_errormsg($ldap->ldap_linkid);
				die($LDIF);
			}
		}
	}
}

function pql_get_rootdn($dn) {
	global $_pql;
	$tracker = 'Please report this at the <a href="http://papadoc.bayour.com:23000/anthill/" target="_new">bugtracker</a>.<br>';

	if($dn == '') {
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

// --------------------------
// Create a LDIF object to print in case of error
function pql_create_ldif($func, $dn, $entry) {
	global $account_type;

	if($func)
	  $LDIF .= "<br>--- DEBUG: $func($account_type) ---<br>";

	$LDIF .= "dn: $dn<br>";
	foreach($entry as $key => $e) {
		if(is_array($entry[$key])) {
			// ... in an array. Go through each one, creating the LDAP entry
			foreach($entry[$key] as $x)
			  $LDIF .= "$key: $x<br>";
		} else {
			$LDIF .= "$key: $entry[$key]<br>";
		}
	}

	if($func)
	  $LDIF .= "--- DEBUG ---<p>";	

	return($LDIF);
}

// --------------------------
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

// --------------------------
function printr($string) {
	echo "<pre>\n";

	if(is_array($string))
	  print_r($string);
	else
	  echo "$string<br>";

	echo "</pre><p>\n";
}

// --------------------------
// I'm a perlhacker really, does it show!? :)
function lc($string) {
	return(strtolower($string));
}

// --------------------------
// Force a non-internationalization of a string
// (for directories and files for example)
function non_internationalize($string) {
	$new = strtr($string,
				 "ÁÉÍÓÚÑÀÈÌÒÙÅÄËÏÖÜÂÊÎÔÛáéíóúñàèìòùåäëïöüâêîôûç",
				 "AEIOUNAEIOUAAEIOYAEIOUaeiounaeiouaaeioyaeiouc");
	return($new);
}

// --------------------------
// Check if we SHOULD encode the string...
function maybe_encode($string, $attrib = '', $linkid = '') {
	$old = $string;

	$new = non_internationalize($old);
	if($old != $new) {
		// The two strings isn't equal -> something have been replaced

		if(($attrib != '') && ($linkid != '')) {
			// Retreive all EXISTING attributes the LDAP server knows about
			$ldap_attribs = pql_get_subschema($linkid, "attributetypes");
			
			// Find the syntax for the attribute (or it's superior attribute)
			if($ldap_attribs[$attrib]["SYNTAX"]) {
				$syntax = $ldap_attribs[$attrib]["SYNTAX"];
			} else {
				// No syntax, check the SUP if that have a SYNTAX
				// TODO: Be recursive!
				if($ldap_attribs[$attrib]["SUP"] && $ldap_attribs[$ldap_attribs[$attrib]["SUP"]]["SYNTAX"]) {
					$syntax = $ldap_attribs[$ldap_attribs[$attrib]["SUP"]]["SYNTAX"];
				}
			}
		
			// Does this attribute allow a UTF-8 encoded string?
			if(eregi('1\.3\.6\.1\.4\.1\.1466\.115\.121\.1\.15', $syntax))
			  return(utf8_encode($string));
			else
			  // Oups, no! Replace any national character with it's
			  // international (ie US char) counter part...
			  return($new);
		} else
		  return(utf8_encode($string));
	}

	return($string);
}

// --------------------------
// Check if we SHOULD decode the string...
function maybe_decode($string) {
	if(eregi("\=$", $string))
	  // It's a BASE64 string, decode it
	  $decoded = base64_decode($string);
	else
	  // It seems quite safe to UTF8 decode a non UTF8 string
	  $decoded = utf8_decode($string);

	if($decoded)
	  return($decoded);
	else
	  return($string);
}

// --------------------------
function pql_generate_button($link, $text) {
	global $PHP_SELF, $LANG, $rootdn, $domain, $userdn;

	echo '<a href="'.$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=$link".'">';
    echo '<img alt="/ '.$LANG->_($text).' \\" vspace="0" hspace="0" border="0" src="navbutton.php?';
	echo $LANG->_($text).'"></a>';
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
