<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 2.41 2003-01-31 06:05:41 turbo Exp $
//

// ----------------------------
// include and check configuration
require("./include/lang.en.inc");
require("./include/pql_cache.inc");

// ----------------------------
// Get release version
$fp = fopen("./.version", "r");
$VERSION = fgets($fp, 20);
fclose($fp);

// ----------------------------
// make a simple configuration check
if(!pql_check_config(&$result)){
	echo "There are some problems with your configuration:<br>";
	echo "<ul><li>" . join("<li>", $result) . "</ul>";
	die();
}

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

	function _find_basedn() {
		$sr = ldap_read($this->ldap_linkid, '', '(objectClass=*)', array('namingContexts'));
		$entry = ldap_get_entries($this->ldap_linkid, $sr);
		
		// Build an array of base dn's. It's possible to have multiple in a database
		for($i=0; $i < $entry["count"]; $i++) {
			$dn[] = $entry[$i]["dn"];
			$success = 1;
		}
		
		// If we got at least one DN, return the array. Else return false
		if($success) {
			return $dn;
		} else {
			return false;
		}
	}
	
	function pql($host, $binddn = "", $bindpw = "", $no_connect = false, $do_die = 1) {
		// constructor, connect to ldap host
		if($no_connect == true){
			return false;
		}

		// Open a connection to the host
		$this->connect($host) or die("<b>could not connect to ldap server</b>");

		// Bind to the host
		if(!$this->bind($binddn, $bindpw)) {
// TODO: Sometimes we get wrongfully logged out here!?
//			session_unset();
//			session_destroy();

			$this->ldap_error = "<b>Could not bind to ldap server</b>: " . ldap_error($this->ldap_linkid) . "<br>" .
			  "Please <a href=\"index.php\" target=\"_top\">relogin</a>.";

			if($do_die) {
				die($this->ldap_error);
			}

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
		
		if($this->ldap_linkid){
			// Set the LDAP protocol version to 3 (LDAPv3).
			//@ldap_set_option($this->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, 3);

			return true;
		}
		
		return false;
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
	$DEBUG = 0;

	if($DEBUG >= 2)
	  echo "pql_user_add($ldap_linkid, $domain, $cn, $attribs, $account_type, $branch)<br>";

    // Setup the objectclass entries
    $entry["objectClass"][] = "top";
    $entry["objectClass"][] = PQL_LDAP_OBJECTCLASS_USERID;
    
    // Add extra objectclasses to object
    if(defined(PQL_LDAP_OBJECTCLASS_USER_EXTRA)) {
		$objectclasses = split(" ", PQL_LDAP_OBJECTCLASS_USER_EXTRA);
		foreach($objectclasses as $oc) {
			$entry["objectClass"][] = $oc;
		}
    }
    
    // additional attributes
    foreach($attribs as $key => $value){
		// We might have supplied some objectClass definitions
		// Don't overwrite those!
		if(! eregi("objectclass", $key)) {
			if(is_array($value)) {
				foreach($value as $val) {
					$entry[$key][] = trim($val);
				}
			} else {
				$entry[$key] = trim($value);
			}
		} else {
			// We have a objectClass
			if(is_array($value)) {
				// ... in an array. Go through each one, creating the LDAP entry
				foreach($value as $oc) {
					$entry["objectClass"][] = $oc;
				}
			} else {
				// Could this happen?! Just incase, this SHOULD work :)
				$entry["objectClass"][] = $value;
			}
		}
    }
    
	$userdn = PQL_LDAP_ATTR_UID . "=" . $entry[PQL_LDAP_ATTR_UID] . ",";
	if($branch)
	  $userdn .= $branch;
	else
	  $userdn .= $domain;

	// --------------------------
	// DEBUG mode: Print the LDIF
	if($DEBUG >= 1) {
		echo "<br>--- DEBUG: pql_user_add() ---<br>";
		echo "dn: $userdn<br>";
		foreach($entry as $key => $e) {
			if(is_array($entry[$key])) {
				// ... in an array. Go through each one, creating the LDAP entry
				foreach($entry[$key] as $x) {
					echo "$key: $x<br>";
				}
			} else {
				echo "$key: $entry[$key]<br>";
			}
		}
		die("--- DEBUG ---");
	}
	// --------------------------
    
    // adds the user
    if(! ldap_add($ldap_linkid, $userdn, $entry)){
		// failed to add user
		return false;
	}
	$dns[]  = $userdn;
    
	// If this is a 'system' account, create the Group object
	if($account_type == "system" and $entry["gidnumber"]) {
		$gidnr = $entry["gidnumber"];
		
		// Create a new LDAP object
		$entry = "";
		$entry["objectClass"][] = "top";
		$entry["objectClass"][] = "posixGroup";
		$entry["cn"] = $attribs["uid"];
		$entry["gidnumber"] = $gidnr;
		
		if(defined("PQL_LDAP_SUBTREE_GROUPS"))
		  $subrdn = "," . PQL_LDAP_SUBTREE_GROUPS . ",";

		$groupdn = PQL_LDAP_ATTR_CN . "=" . $attribs[PQL_LDAP_USER_RDN_ATTRIBUTE] . $subrdn . $domain;

		// --------------------------
		
		if(! ldap_add($ldap_linkid, $groupdn, $entry))
		  return $dns;

		$dns[] = $groupdn;
	}
	
    return $dns;
}

// Delete a user from the LDAP database
function pql_user_del($ldap, $domain, $user, $delete_forwards) {
	$linkid = $ldap->ldap_linkid;

    if(!pql_user_exist($linkid, $user)) {
		// user does not exist
		return false;
    }

	// Remove all administrator entries which contain the user DN
	foreach($ldap->ldap_basedn as $dn) {
		$sr = ldap_search($linkid, $dn, "administrator=$user");
		$info = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
		for($i=0; $i<$info["count"]; $i++) {
			unset($entry); unset($adms);

			// Get administrator attributes for this domain/branch DN
			$admins	= pql_get_domain_value($ldap, $info[$i]["dn"], 'administrator');
			for($j=0; $admins[$j]; $j++) {
				if($admins[$j] != $user)
				  $adms[] = $admins[$j];
			}
				  
			if(is_array($adms)) {
				// Add the administrators that's left to the DN
				$entry["administrator"] = $adms;
				if(! ldap_mod_replace($linkid, $info[$i]["dn"], $entry))
				  pql_errormsg($linkid);
			}
		}
	}

	// Get uidnr of user
	$uidnr = pql_get_userattribute($linkid, $user, PQL_LDAP_ATTR_QMAILUID);
	$uidnr = $uidnr[0];
    
	// Delete the group object if it exists
	$filter = "(&(gidNumber=" . $uidnr . ")(objectclass=posixGroup))";
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
			$email = pql_get_userattribute($linkid, $user, PQL_LDAP_ATTR_MAIL);
			$aliases = pql_get_userattribute($linkid, $user, PQL_LDAP_ATTR_MAILALTERNATE);
			
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
			$fwd_addresses = pql_get_userattribute($linkid, $forward['reference'], PQL_LDAP_ATTR_FORWARDS);
			foreach($addresses as $address) {
				// does this user forward to the removed user or one of his aliases?
				$rem_key = array_search($address, $fwd_addresses);
				if ($rem_key !== false) {
					// we found a forward -> remove it 
					pql_remove_userattribute($linkid, $forward['reference'],
											 PQL_LDAP_ATTR_FORWARDS,
											 $fwd_addresses[$rem_key]);
				}
			}
		} 
    }

    return true;
}

// adds a domain into ldap-tree (ou|dc-record)
function pql_domain_add($ldap_linkid, $basedn, $branch_name){
    $entry[PQL_LDAP_REFERENCE_DOMAINS_WITH] = $branch_name;
	$dn = PQL_LDAP_REFERENCE_DOMAINS_WITH . "=" . $branch_name . "," . $basedn;

	// Setup objectclasses
	$entry["objectClasses"] = pql_setup_branch_objectclasses();
	$entry["o"] = 0;

//	// --------------------------
//	// DEBUG mode: Print the LDIF
//	echo "<br>--- DEBUG: pql_domain_add() ---<br>";
//	echo "dn: $dn<br>";
//	foreach($entry as $key => $e) {
//		if(eregi("objectclass", $key)) {
//			// We have a objectClass
//			if(is_array($entry[$key])) {
//				// ... in an array. Go through each one, creating the LDAP entry
//				foreach($entry[$key] as $oc) {
//					echo "$key: $oc<br>";
//				}
//			}
//		} else {
//			echo "$key: $entry[$key]<br>";
//		}
//	}
//	die("--- DEBUG ---");
//	// --------------------------

    if(! @ldap_add($ldap_linkid, $dn, $entry)) {
		pql_errormsg($ldap_linkid);
		return false;
    }
	$dns[] = $dn;

    // Add the USER subtree if defined
    if(defined("PQL_LDAP_SUBTREE_USERS")) {
		$entry = "";
		$entry["objectClass"][] = "top";
		$entry["objectClass"][] = "organizationalUnit";
		
		$ou = eregi_replace(",$", "", PQL_LDAP_SUBTREE_USERS . ",");
		$entry["ou"] = $ou;
		
		$dn = PQL_LDAP_SUBTREE_USERS . "," . PQL_LDAP_REFERENCE_DOMAINS_WITH . "=" . $branch_name . "," . $basedn;
		if(!@ldap_add($ldap_linkid, $dn, $entry)){
			pql_errormsg($ldap_linkid);
			return $dns;
		}
		$dns[] = $dn;
    }
    
    // Add the GROUPS subtree if defined
    if(defined("PQL_LDAP_SUBTREE_GROUPS")) {
		$entry = "";
		$entry["objectClass"][] = "top";
		$entry["objectClass"][] = "organizationalUnit";
		$entry["ou"] = PQL_LDAP_SUBTREE_GROUPS;
		
		$dn = PQL_LDAP_SUBTREE_GROUPS . "," . PQL_LDAP_REFERENCE_DOMAINS_WITH . "=" . $branch_name . "," . $basedn;
		if(!@ldap_add($ldap_linkid, $dn, $entry)){
			pql_errormsg($ldap_linkid);
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
		header("Location: " . PQL_URI . "domain_detail.php?domain=$domain&msg=$msg");
	}

	$dc = ldap_explode_dn($domain, 0); $dc = split('=', $dc[0]);
    if(!pql_domain_exist($ldap, $dc[1])) {
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

function pql_modify_user_addressbook($ldap_linkid, $user, $attribs){
    // check if addressbook is disabled
    if(!defined("PQL_USE_ADDRESSBOOK")) {
		return false;
    }
    
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
    $oc_user = pql_get_userattribute($ldap_linkid, $user, "objectclass");
    
    // set all objectclasses to lowercase
    foreach($oc_user as $oc1){
		$oc[] = strtolower($oc1);
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
function pql_domain_exist($ldap, $domain) {
	$filter = '(&('.PQL_LDAP_REFERENCE_DOMAINS_WITH.'='.$domain.'))';

	foreach($ldap->ldap_basedn as $dn) {
		$sr = ldap_search($ldap->ldap_linkid, $dn, $filter);
		if(ldap_count_entries($ldap->ldap_linkid, $sr) > 0) {
			return true;
		}
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
function pql_email_exists($ldap, $email){
	$filter = "(|(" . PQL_LDAP_ATTR_MAIL . "=" . $email . ")(" . PQL_LDAP_ATTR_MAILALTERNATE . "=" . $email . "))";

	foreach($ldap->ldap_basedn as $dn) {
		$sr = ldap_search($ldap->ldap_linkid, $dn, $filter);
		if(ldap_count_entries($ldap->ldap_linkid,$sr) > 0) {
			$ed = ldap_first_entry($ldap->ldap_linkid, $sr);
			return ldap_get_dn($ldap->ldap_linkid, $ed);
		} else {
			return false;
		}
	}
}

// ------------------------------------------------

// adds an attribute of a user
function pql_add_userattribute($ldap_linkid, $user, $attrib, $value) {
    $entry[$attrib] = $value;
    
    if(!ldap_mod_add($ldap_linkid, $user, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($user);
    
    return true;
}

// ------------------------------------------------

// removes an attribute of a user
function pql_remove_userattribute($ldap_linkid, $user, $attrib, $value){
    if(!$value) {
		// Delete all attributes of this type, regardless of value
		$entry[$attrib] = array();
    } else {
		$entry[$attrib] = $value;
    }
	
    if(!ldap_mod_del($ldap_linkid, $user, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($user);
    
    return true;
}

// ------------------------------------------------

// change an attribute of a user
function pql_modify_userattribute($ldap_linkid, $user, $attrib, $oldvalue, $newvalue) {
    // get all attributes first
    $entry = pql_get_userattribute($ldap_linkid, $user, $attrib);

    // If we found an entry, make it's not overwritten,
    // otherwise ADD the attribute.
    if($entry) {
		// array_search workaround
		foreach($entry as $value){
			if($value == $oldvalue){
				$value = $newvalue;
			}
			
			$entry_new[$attrib][] = $value;
		}
    } else {
		$entry_new[$attrib][] = $newvalue;
    }

    if(!ldap_mod_replace($ldap_linkid, $user, $entry_new)) {
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($user);
    if ($attrib == PQL_LDAP_REFERENCE_USERS_WITH) {
		pql_cache_userdn_remove($user);
    }
    
    return true;
}

// ------------------------------------------------

// replaces an attribute of a user
// (only use for single-field attributes)
// delete an attribut if value is an empty string
function pql_replace_userattribute($ldap_linkid, $user, $attrib, $value) {
	// create entry array
	$entry[$attrib] = $value;
	
	// replace the attribute
	if(!ldap_mod_replace($ldap_linkid, $user, $entry)) {
	    // if the value is empty (""), this causes an error in OpenLDAP 2.x, try to ldap_mod_del
	    if($value == ""){
			$oldvalue = pql_get_userattribute($ldap_linkid, $user, $attrib);
			$oldvalue = $oldvalue[0];
			$entry[$attrib] = $oldvalue;
			
			//return false;
			
			if(ldap_mod_del($ldap_linkid, $user, $entry)){
				return true;
			} else {
				pql_errormsg($ldap_linkid);
				return false;
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

// replaces more than one attribute of a user
function pql_replace_userattributes($ldap_linkid, $user, $entry){
	if(!ldap_mod_replace($ldap_linkid, $user, $entry)) {
		pql_errormsg($ldap_linkid);
		return false;
	}
	
	// user entry has changed -> remove the cached version
	pql_cache_userentry_remove($user);

	return true;
}

// ------------------------------------------------

function pql_get_domains($ldap) {
	// get all domains listed in ldap-tree (ou|dc-records)
	
	$filter = "(&(" . PQL_LDAP_REFERENCE_DOMAINS_WITH . "=*)" . pql_setup_branch_objectclasses(1) . ")";
	foreach($ldap->ldap_basedn as $dn)  {
		$sr = @ldap_list($ldap->ldap_linkid, $dn, $filter) or pql_errormsg($ldap->ldap_linkid);
		$info = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
		
		for ($i=0; $i<$info["count"]; $i++) {
			$domains[] = $info[$i]["dn"];
		}
		
	}

	return $domains;
}

// ------------------------------------------------

// get all users of a domain
function pql_get_user($ldap_linkid, $dn){
	$filter = PQL_LDAP_REFERENCE_USERS_WITH . "=*";
	
	$sr = @ldap_search($ldap_linkid, $dn, $filter) or pql_errormsg($ldap_linkid);
	$info = ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);
	for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i]["dn"];
	}
	
	return $user;
}

// ------------------------------------------------
// function based in PQL_GET_USER to search by the cn first letter
function pql_get_user_letter($ldap_linkid, $domain, $letra) {
    $sr = ldap_search($ldap_linkid, $domain, "(".PQL_LDAP_ATTR_UID."=".$letra."*)");
    $info = ldap_get_entries($ldap_linkid, $sr);
    
    for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i]["dn"];
    }
    
    return $user;
}

// ------------------------------------------------
// function based in PQL_GET_USER to search by an initial number at cn attribute
function pql_get_user_number($ldap_linkid, $domain) {
    for($numero=0; $numero <= 9; $numero++) {
		$sr = ldap_search($ldap_linkid, $domain, "(".PQL_LDAP_ATTR_UID."=".$numero."*)");
		$info = ldap_get_entries($ldap_linkid, $sr);
		
		for ($i=0; $i<$info["count"]; $i++) {
			$user[] = $info[$i][PQL_LDAP_REFERENCE_USERS_WITH][0];
		}
    }
    
    return $user;
}

// ------------------------------------------------

function pql_get_userattribute($ldap_linkid, $user, $attribute) {
    // check whether this user is cached
    $value = '';
    
	if(pql_cache_userentry_get($user, $attribute, $value))
	  // the user is cached  -> return the value for the requested attribute  
	  return($value);

    // not yet cached -> get the userentry and cache it
    $entries = pql_get_userentry($ldap_linkid, $user);
    if($entries)
	  pql_cache_userentry_add($user, $entries);
	else
	  return(false);

    return($entries[$attribute]);	
}

// ------------------------------------------------
function pql_get_userentry($ldap_linkid, $user) {
    $sr = @ldap_read($ldap_linkid, $user, "(objectClass=*)");
    if($sr == false){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // build array of all attributes
    $rid = @ldap_first_entry($ldap_linkid, $sr);
    $attribs[] = strtolower(@ldap_first_attribute($ldap_linkid, $rid, $berid));
    while($attrib = @ldap_next_attribute($ldap_linkid, $rid, $berid)) {
		$attribs[] = strtolower($attrib);
	}
    
    // build entry array with all attributes and values
    foreach($attribs as $attrib){
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

function pql_get_userquota($ldap_linkid, $user){
    // get formated quota of a user
    
    $quota = pql_get_userattribute($ldap_linkid, $user, PQL_LDAP_ATTR_QUOTA);
    
    if(!is_array($quota)){
		return false;
    }
    
    return pql_parse_quota($quota[0]);
}

// ------------------------------------------------

// retrieve the dn for a given reference
function pql_get_dn($ldap, $reference) {
    // check if we already have the dn for this user chached
    $dn = '';
    if(pql_cache_userdn_get($reference, $dn))
	  return($dn);
    
    // dn not in cache -> search for it
	foreach($ldap->ldap_basedn as $base) {
		$dn = pql_search_attribute($ldap->ldap_linkid, $base, PQL_LDAP_REFERENCE_USERS_WITH, $reference);
		if($dn) {
			// we got the dn -> remember it
			pql_cache_userdn_add($reference, $dn);
			
			return($dn);
		}
	}

	return(false);
}

// ------------------------------------------------
function pql_errormsg($ldap_linkid){
    echo format_error("LDAP-Error:") . ldap_error($ldap_linkid) . "<br>";
}

// ------------------------------------------------
function pql_ldap_error($ldap_linkid, $attrib = ""){
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

function pql_parse_quota($quota_string){
    // get max bytes
    preg_match("/([0-9]*)S/", $quota_string, $matches);
    
    if($matches[1] != ""){
		$quota["maxsize"] = intval($matches[1] / 1024);
    }
    
    // get max mails
    preg_match("/([0-9]*)C/", $quota_string, $matches);
    $quota["maxmails"] =  $matches[1];
    
    return $quota;
}

// ------------------------------------------------

// search $attribute with $value in whole ldap tree, returns dn or false
function pql_search_attribute($ldap_linkid, $basedn, $attribute, $value) {
    $filter = "(" . $attribute . "=" . $value . ")";
    $sr = ldap_search($ldap_linkid, $basedn, $filter, array($attribute));
	
    if(ldap_count_entries($ldap_linkid, $sr) > 0) {
		$ed = ldap_first_entry($ldap_linkid, $sr);
		return ldap_get_dn($ldap_linkid, $ed);
	} else {
		return false;
	}
}

// ------------------------------------------------

// search all accounts with forwarders to $user@$domain
function pql_search_forwarders($ldap, $user){
	$linkid = $ldap->ldap_linkid;

    // get all email addresses of a user
    $email = pql_get_userattribute($linkid, $user, PQL_LDAP_ATTR_MAIL);
    $aliases = pql_get_userattribute($linkid, $user, PQL_LDAP_ATTR_MAILALTERNATE);
    
    $addresses[] = $email[0];
    if(is_array($aliases)){
		$addresses = array_merge($addresses, $aliases);
    }
    
    // create filter
    $filter = "(|";
    foreach($addresses as $add){
		$filter .= "(" . PQL_LDAP_ATTR_FORWARDS ."=" . $add . ")";
    }
    $filter .= ")";

    // Go through each base DN in the database, looking
	// for forwarders to this user
	foreach($ldap->ldap_basedn as $dn) {
		$sr = ldap_search($linkid, $dn, $filter);
		if(ldap_count_entries($linkid ,$sr) > 0) {
			// Get the entries found
			$results = ldap_get_entries($linkid, $sr);

			// Go through the entries
			foreach($results as $key => $result){
				if((string)$key != "count"){
					$return[] = array("reference" => $result["dn"],
									  PQL_LDAP_REFERENCE_USERS_WITH => $result["cn"][0],
									  "email" => $result["mail"][0]);
					
					if(is_array($result[PQL_LDAP_ATTR_MAILALTERNATE])){
						foreach($result[PQL_LDAP_ATTR_MAILALTERNATE] as $key => $mailalternateaddress){
							if((string)$key != "count"){
								$return[] = array("reference" => $result["dn"],
												  PQL_LDAP_REFERENCE_USERS_WITH => $result["cn"][0],
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

function pql_password_hash($password, $hash){
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
		if(function_exists("mhash")){
			return "{SSHA}" . base64_encode(mhash(MHASH_SHA1, $password, uniqid(microtime())));
		}
      case "{SHA}":
		// SHA1 hashing
		if(function_exists("mhash")){
			return "{SHA}" . base64_encode(mhash(MHASH_SHA1, $password));
		}
      case "{MD5}":
		// MD5 hashing
		if(function_exists("mhash")){
			return "{MD5}" . base64_encode(mhash(MHASH_MD5, $password));
		}
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

function format_error($msg){
    return "<span class=\"error\">" . $msg . "</span>";
}

// ------------------------------------------------

function print_status_msg($msg){
?>
  <table cellpadding="3" cellspacing="0" border="0" width="100%">
    <tr>
      <td class="message"><img src="images/info.png" width="16" height="16" border="0"> <?php echo stripslashes($msg); ?></td>
    </tr>
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
    if(!preg_match("/^[a-z0-9-+=#]+([\._a-z0-9-+=#]?[a-z0-9]+)*$/i",$split[0])){
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

function check_hostaddress($host, $allow_domain){
    // checks an fqdn
    // if allow_domain is true, it will not be checked for dots
    
    if($allow_domain){
		return !preg_match("/[^0-9a-z-]/", $host);
    } else {
		return preg_match("/^([0-9a-z-]+\.[0-9a-z-]+)+$/i", $host);
    }
}

// ------------------------------------------------

function pql_print_filesize($bytes){
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

function pql_strip_domain($email){
    // get username of an email address
    $email = split("@", $email);
    
    if(count($email) == 2){
		return $email[0];
    }
    
    return false;
}

// ------------------------------------------------

function pql_strip_username($email){
    // get domainname of an email address
    $email = split("@", $email);
    
    if(count($email) == 2){
		return $email[1];
    }
    
    return false;
}

// ------------------------------------------------

function pql_complete_constant($text, $replacement){
	// replaces dynamic vars in constants
	// %key% will be replaced with $replacement["key"]
	
    if(!is_array($replacement)){
		return false;
    }
    
    foreach($replacement as $key => $value){
		// prepare patterns
		$pattern[] = "/(%$key%)/i";
    }
    
    return preg_replace($pattern, $replacement, $text);
}

// ------------------------------------------------
function pql_check_config(&$result, $enhanced = false){
    $result = array();
    
    // language definition
    if(!defined("PQL_LANG")){
		$result[] = "no language definition file included!!";
    }
    
    if($enhanced){
		// enhance configuration check
        // --> to be done
    }
    
    if(count($result) > 0){
		return false;
    }
    
    return true;
}


// --------------------------------------
// format ldap attribute codes to text
// --------------------------------------

function pql_ldap_mailquota($quota){
    // returns human readable version of users quota
    
    if($quota["maxmails"] == ""){
		$txt = PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED;
    } else {
		$txt = pql_complete_constant(PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS, array("maxmails" => $quota["maxmails"]));
    }
    
    $txt .= ", ";
    
    if($quota["maxsize"] == ""){
		$txt .= PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED;
    } else {
		$txt .= pql_complete_constant(PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE, array("maxsize" => $quota["maxsize"]));
    }
    
    return $txt;
}

// ------------------------------------------------

function pql_ldap_accountstatus($status){
    // returns text of account-status
    
    // active [default]
    // nopop
    // disabled
    
    switch($status){
      case "nopop":
		return PQL_LDAP_ACCOUNTSTATUS_NOPOP;
      case "disabled":
		return PQL_LDAP_ACCOUNTSTATUS_DISABLE;
      default:
		return PQL_LDAP_ACCOUNTSTATUS_ACTIVE;
    }
}

// ------------------------------------------------

function pql_ldap_deliverymode($mode){
    // returns text of delivery mode
    
    // normal [default]
    // forwardonly
    // nombox
    // localdelivery
    // reply
    // echo
    
    switch($mode){
      case "forwardonly":
		return PQL_LDAP_DELIVERYMODE_FORWARDONLY;
      case "nombox":
		return PQL_LDAP_DELIVERYMODE_NOMBOX;
      case "localdelivery":
		return PQL_LDAP_DELIVERYMODE_LOCALDELIVERY;
      case "reply":
		return PQL_LDAP_DELIVERYMODE_REPLY;
      case "echo":
		return PQL_LDAP_DELIVERYMODE_ECHO;
      default:
		return PQL_LDAP_DELIVERYMODE_NORMAL;
    }
}

// Returns the next free UID/GID number
function pql_get_next_uidnumber($ldap) {
    $filter = "(&(" . PQL_LDAP_ATTR_QMAILUID . "=*)(objectclass=posixaccount))";

	foreach($ldap->ldap_basedn as $dn) {
		$sr = ldap_search($ldap->ldap_linkid, $dn, $filter);
		$results = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
	
		// Find the highest availible UIDNUMBER
		$highest = 0;
		for($i=0; $i < $results["count"]; $i++) {
			if(($results[$i][PQL_LDAP_ATTR_QMAILUID][0] > $highest) &&
			   ($results[$i][PQL_LDAP_ATTR_QMAILUID][0] < 64000))
			  {
				  $highest = $results[$i][PQL_LDAP_ATTR_QMAILUID][0];
			  }
		}
	}
	$highest++; // Increase with one!
	
    return $highest;
}

function pql_get_domain_value($ldap, $dn, $value, $match = '') {
	$recursive = 1;

	if(!$match) {
		$match = '*';
		$recursive = 0;
	}

	// Setup a filter to look for DOMAINS (branches)
	$filter  = "(&($value=$match)(".PQL_LDAP_REFERENCE_DOMAINS_WITH."=*))";
		
	if(!$recursive)
	  $sr = ldap_read($ldap->ldap_linkid, $dn, $filter, array($value));
	else
	  $sr = ldap_search($ldap->ldap_linkid, $dn, $filter, array($value));

	$result = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);

	for($i=0; $i < $result["count"]; $i++) {
		// Extract all the descriptor fields
		if((($value == 'administrator') or ($value == 'seealso')) and ($match != "*")) {
			// Return multiple values. If we're looking for 'administrator', return the DN
			for($j=0; $j < $result[$i][$value]["count"]; $j++) {
				// Filter out the top dn, we don't want that in the domain/branch list...
				if(($result[$i][$value][$j] == $match) and ($result[$i]["dn"] != $ldap->ldap_basedn[0])) {
					$results[] = $result[$i]["dn"];
					$success = 1;
				}
			}
		} elseif((($value == 'administrator') or ($value == 'seealso')) and ($match == "*")) {
			for($j=0; $j < $result[$i][$value]["count"]; $j++) {
				$results[] = $result[$i][$value][$j];
				$success = 1;
			}
		} else {
			// Return the value we're looking for (single valued).
			$results = $result[0][$value][0];
			$success = 1;
		}
	} // end for($i)

	if(!$success and (($value != 'administrator') and ($value != 'seealso'))) {
		return;
	}

	return $results;
}

// Set a embedded domain default value
function pql_set_domain_value($linkid, $dn, $type, $value) {
	// Lowercase type...
	if($type) {
		$type = strtolower($type);
		
		// Setup the value array
		if(is_array($value)) {
			// We're supplied with an array, add those values
			foreach($value as $val) {
				if(($type != 'administrator') and ($type != 'seealso')) {
					$entry[$type][] = $val;
				} else {
					$entry[$type][] = $val;
				}
			}
		} else {
			// Add the single value
			if(($type != 'administrator') and ($type != 'seealso')) {
				$entry[$type] = $value;
			} else {
				$entry[$type] = $value;
			}
		}
	} else {
		// We're only supplied with an array of entries to add/modify
		$entry = $value;
	}
	
	// Setup objectclasses
	$entry["objectClasses"] = pql_setup_branch_objectclasses(0, $linkid, $dn);

//	// --------------------------
//	// DEBUG mode: Print the LDIF
//	echo "<br>--- DEBUG: pql_set_domain_value() ---<br>";
//	echo "dn: $dn<br>";
//	foreach($entry as $key => $e) {
//		if(is_array($e)) {
//			foreach($e as $v) {
//				echo "$key: $v<br>";
//			}
//		} else {
//			echo "$key: $e<br>";
//		}
//	}
//	die("--- DEBUG ---");
//	// --------------------------

	// Do the LDAP modification
	if($value != '') {
		// Replace the current attribute value
		if(! ldap_mod_replace($linkid, $dn, $entry)) {
			pql_errormsg($linkid);
			return false;
		}
	} else {
		// Delete all attributes of this type, regardless of value
		$entry[$type] = array();

		if(! ldap_mod_del($linkid, $dn, $entry)) {
			pql_errormsg($linkid);
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

		$filter = "(&(objectclass=qmailControl)(|(locals=$domainname)(rcpthosts=$domainname)))";
		foreach($ldap->ldap_basedn as $dn) {
			$sr = @ldap_search($ldap->ldap_linkid, $dn, $filter) or pql_errormsg($ldap->ldap_linkid);
			$info = ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
			for ($i=0; $i<$info["count"]; $i++) {
				return(array(0, $info[$i]["cn"][0]));
			}
		}

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
    $attrib = strtolower($attrib);
	
    // Get the DN for the subSchema
    $sr    = ldap_read($ldap_linkid, '', '(objectClass=*)', array('subschemaSubentry'));
    $entry = ldap_get_entries($ldap_linkid, $sr);
    if($entry["count"]) {
		// Get the subSchemaAttributes from the subSchema
		$sr     = ldap_read($ldap_linkid,
							$entry[0]["subschemasubentry"][0],
							'(objectClass=*)', array($attrib));
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
					$name  = strtolower($words[2]);
					
					$VALUE[$name]["NAME"] = $words[2];
					for($j=3; $words[$j]; $j++) {
						$VALUE[$name]["ALIAS"][] = $words[$j];
					}
				} else {
					$words = split("\' ", $words[1]);
					$words = split(" \'", $words[0]);
					$name  = strtolower($words[1]);
					
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
			// Retreive SINGLE-VALUE
			if(eregi("SINGLE-VALUE", $entry[0][$attrib][$i]))
			  $VALUE[$name]["SINGLE-VALUE"] = 'true';
			
			// ------------------------
			// Retreive NO-USER-MODIFICATION
			if(eregi("NO-USER-MODIFICATION", $entry[0][$attrib][$i]))
			  $VALUE[$name]["NO-USER-MODIFICATION"] = 'true';
			
			// ------------------------
			// Retreive X-BINARY-TRANSFER-REQUIRED
			if(eregi("X-BINARY-TRANSFER-REQUIRED", $entry[0][$attrib][$i]))
			  $VALUE[$name]["X-BINARY-TRANSFER-REQUIRED"] = 'true';
			
			// ------------------------
			// Retreive X-NOT-HUMAN-READABLE
			if(eregi("X-NOT-HUMAN-READABLE", $entry[0][$attrib][$i]))
			  $VALUE[$name]["X-NOT-HUMAN-READABLE"] = 'true';
		}
		
		return $VALUE;
    }
}

function pql_get_subschemas($ldap_linkid, $match = "") {
    $attribs = array("ldapsyntaxes", "matchingrules", "attributetypes", "objectclasses");
    foreach($attribs as $attrib) {
		$result = pql_get_subschema($ldap_linkid, $attrib);
		if($result) {
			$entry[$attrib] = $result;
		}
    }

	if($match) {
		// We're looking for something special, look for it. If it doesn't
		// exists in the array -> return FALSE.
		if(! $entry["objectclasses"][strtolower($match)])
		  return 0;
	}
	
    return $entry;
}

function pql_get_subbranch($linkid, $dn, $filter = 1) {
	if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
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
function pql_setup_branch_objectclasses($filter = 0, $linkid = '', $dn = '') {
	if($dn != '' and $linkid != '') {
		// Retreive existing objectClasses from the object
		$object = ldap_explode_dn($dn, 0);
		$sr     = @ldap_read($linkid, $dn, $object[0], array('objectclass'));
		$ocs    = @ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
		
		for($j=0; $j < $ocs[0]["objectclass"]["count"]; $j++) {
			if(eregi('phpQLAdminBranch', $ocs[0]["objectclass"][$j]))
			  $got_phpqladmin_objectclass_branch = 1;
			
			$entry[] = $ocs[0]["objectclass"][$j];
		}
	} else {
		// No previous DN, we're probably creating a new
		if(eregi(' ', PQL_LDAP_OBJECTCLASS_DOMAIN)) {
			$objectclasses = split(' ', PQL_LDAP_OBJECTCLASS_DOMAIN);
			foreach($objectclasses as $oc) {
				if($filter) {
					$entry .= "(objectClass=$oc)";
				} else {
					$entry[] = $oc;

					if(eregi('phpQLAdminBranch', $oc))
					  $got_phpqladmin_objectclass_branch = 1;
				}
			}
		} else {
			if($filter) {
				$entry = "(objectClass=" . PQL_LDAP_OBJECTCLASS_DOMAIN . ")";
			} else {
				$entry[] = PQL_LDAP_OBJECTCLASS_DOMAIN;
				$entry[] = "dcOrganizationNameForm";
			}
		}
	}

	if(!$got_phpqladmin_objectclass_branch and !filter)
	  $entry[] = 'phpQLAdminBranch';
	else
	  // We have already setup the objectclasses, return NULL
	  $entry = array();

	return($entry);
}

function pql_validate_administrator($linkid, $dn, $admin) {
	$sr   = @ldap_read($linkid, $dn, '(objectClass=*)', array('administrator')) or pql_errormsg($linkid);
	$info = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);

	for($i=0; $i<$info["count"]; $i++) {
		for($j=0; $j<$info[$i]["administrator"]["count"]; $j++) {
			// TODO: We shouldn't do a regexp but a 'case insensitive equal'
			if(eregi($info[$i]["administrator"][$j], $admin)) {
				return true;
			}
		}
	}

	return false;
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
