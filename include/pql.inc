<?php
// ----------------------------
// pql.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql.inc,v 1.1 2002-12-11 15:09:23 turbo Exp $
//
// ----------------------------
// Note: There are a few " followed after PHP comment
// instructions like // in this code . They are
// here to prevent Quanta+ highlighting all following
// code as a string. A dirty workaround, I know :)
//

// ----------------------------
// include and check configuration
// ----------------------------
require("config.inc");

// make a simple configuration check
if(!pql_check_config(&$result)){
	echo "There are some problems with your configuration:<br>";
	echo "<ul><li>" . join("<li>", $result) . "</ul>";
	die();
}


// phpQLAdmin version
define("PQL_VERSION", "1.2.0");

// turn on output-buffering by default
ob_start();

// global variables
$pql_cache_userdn = array();
$pql_cache_userentry = array();


// ----------------------------
// API functions / classes
// ----------------------------

// PQL - main class (connection handling)
class pql {
	// runtime vars
	var $ldap_host = PQL_LDAP_HOST;			// host to connect to
	var $ldap_port = 389;					// Port of host to connect to,
											// Default is 389, but can be
	// changed in the PQL_LDAP_HOST 
	// define...
	var $ldap_basedn = PQL_LDAP_BASEDN; 	// base dn
	var $ldap_rootdn = PQL_LDAP_ROOTDN; 	// root dn
	var $ldap_rootpw = PQL_LDAP_ROOTPW; 	// password to bind to root dn
	var $ldap_linkid = 0;					// ldap connection identifier

	function pql($no_connect = false){
		// constructor, connect to ldap host
		if($no_connect == true){
			return false;
		}
		
		$this->connect() or die("<b>could not connect to ldap server</b>");
		$this->bind() or die("<b>could not bind to ldap server</b>: " . ldap_error($this->ldap_linkid));
	}
	
	function connect(){
		// Check to see if we have a port defined in the host.
		if(ereg(":", $this->ldap_host)) {
			$ldap = split(":", $this->ldap_host);
			
			$this->ldap_host = $ldap[0];
			$this->ldap_port = $ldap[1];
		}
		
		// connect to ldap host and bind to root_dn
		$this->ldap_linkid = @ldap_connect($this->ldap_host, $this->ldap_port);
		
		if($this->ldap_linkid){
			return true;
		}
		
		return false;
	}
	
	function bind(){
		// bind to ldap server
		if($this->ldap_linkid and @ldap_bind($this->ldap_linkid, $this->ldap_rootdn, $this->ldap_rootpw)){
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

function pql_add_user($ldap_linkid, $ldap_basedn, $domain, $cn, $attribs, $account_type){
    // adds a user in a domain (all necessesary attributes)
    
    // Setup the objectclass entries
    $entry["objectClass"][] = "top";
    $entry["objectClass"][] = "person";
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
    
    // dn of the user
    $dn = PQL_LDAP_USER_RDN_ATTRIBUTE . "=" . $entry[PQL_LDAP_USER_RDN_ATTRIBUTE] . "," . PQL_LDAP_SUBTREE_USERS
      . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . PQL_LDAP_BASEDN;
    
    // adds the user
    if(ldap_add($ldap_linkid,$dn, $entry)){
		return true;
    }
    
    // If this is a 'system' account, create the Group object
    if($account_type == "system" and $entry["gidnumber"]) {
		$gidnr = $entry["gidnumber"];
		
		// Create a new LDAP object
		$entry = "";
		$entry["objectClass"][] = "top";
		$entry["objectClass"][] = "posixGroup";
		$entry["cn"] = $attribs["uid"];
		$entry["gidnumber"] = $gidnr;
		
		$dn = "cn=" . $attribs["uid"] . "," . PQL_LDAP_SUBTREE_GROUPS . ","
		  . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . PQL_LDAP_BASEDN;
		if(! ldap_add($ldap_linkid,$dn, $entry)){
			return false;
		}
    }
	
    return false;
}

// ------------------------------------------------
function pql_modify_user_addressbook($ldap_linkid, $ldap_basedn, $domain, $user, $attribs){
    // check if addressbook is disabled
    if(PQL_USE_ADDRESSBOOK != true){
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
    $oc_user = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, "objectclass");
    
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

function pql_domain_exist($ldap_linkid,$ldap_basedn,$domain){
    // check if domain exists
    
    if(!pql_search_attribute($ldap_linkid,$ldap_basedn,PQL_LDAP_ATTR_DOMAIN,$domain)){
		// domain does not exist in ldap-tree
		return false;
    }
    
    return true;
}

// ------------------------------------------------

function pql_user_exist($ldap_linkid, $ldap_basedn, $domain, $user){
    // check if user exists
    
    if (pql_get_dn($ldap_linkid, $ldap_basedn, $user)) {
		return(true);
    } else {
		return(false); 
    }
}

// ------------------------------------------------

function pql_email_exists($ldap_linkid, $ldap_basedn, $email){
    // check if any mail or mailalternateaddress record with this email exists in the ldap tree
    
    $filter = "(|(" . PQL_LDAP_ATTR_MAIL . "=" . $email . ")(" . PQL_LDAP_ATTR_MAILALTERNATE . "=" . $email . "))";
    $sr = ldap_search($ldap_linkid, $ldap_basedn, $filter);
    
    if(ldap_count_entries($ldap_linkid,$sr) > 0){
		$ed = ldap_first_entry($ldap_linkid,$sr);
		return ldap_get_dn($ldap_linkid,$ed);
    } else {
		return false;
    }
}

// ------------------------------------------------

function pql_add_domain($ldap_linkid,$ldap_basedn,$domain){
    // adds a domain into ldap-tree (ou|dc-record)
    
    if(pql_domain_exist($ldap_linkid,$ldap_basedn,$domain)){
		// domain already exist
		return false;
    }
    
    // add domain
    $entry["objectClass"][] = "top";
    $entry["objectClass"][] = PQL_LDAP_OBJECTCLASS_DOMAIN;
    $entry[PQL_LDAP_ATTR_DOMAIN] = $domain;
    $entry[PQL_LDAP_ATTR_DESCRIPTION] = $domain;
    if(!@ldap_add($ldap_linkid, PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . $ldap_basedn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // Add the USER subtree if defined
    if(PQL_LDAP_SUBTREE_USERS) {
		$entry = "";
		$entry["objectClass"][] = "top";
		$entry["objectClass"][] = "organizationalUnit";
		
		$ou = eregi_replace(",$", "", PQL_LDAP_SUBTREE_USERS);
		$entry["ou"] = $ou;
		
		$dn = PQL_LDAP_SUBTREE_USERS . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . PQL_LDAP_BASEDN;
		if(!@ldap_add($ldap_linkid, $dn, $entry)){
			pql_errormsg($ldap_linkid);
			return false;
		}
    }
    
    // Add the GROUPS subtree if defined
    if(PQL_LDAP_SUBTREE_GROUPS) {
		$entry = "";
		$entry["objectClass"][] = "top";
		$entry["objectClass"][] = "organizationalUnit";
		$entry["ou"] = PQL_LDAP_SUBTREE_GROUPS;
		
		$dn = PQL_LDAP_SUBTREE_GROUPS . "," . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . PQL_LDAP_BASEDN;
		if(!@ldap_add($ldap_linkid, $dn, $entry)){
			pql_errormsg($ldap_linkid);
			return false;
		}
    }
    
    return true;
}

// ------------------------------------------------

function pql_add_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attrib, $value){
    // adds an attribute of a user
    
    $dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user); 
    
    $entry[$attrib] = $value;
    
    if(!ldap_mod_add($ldap_linkid, $dn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($dn);
    
    return true;
}

// ------------------------------------------------

function pql_remove_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attrib, $value){
    // removes an attribute of a user
    
    $dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
    
    $entry[$attrib] = $value;
    
    if(!ldap_mod_del($ldap_linkid, $dn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($dn);
    
    return true;
}

// ------------------------------------------------

function pql_remove_user($ldap_linkid, $ldap_basedn, $domain, $user, $delete_forwards){
    // removes a user
    if(!pql_user_exist($ldap_linkid, $ldap_basedn, $domain, $user)){
		// user does not exist
		return false;
    }
    
    $dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
    
    // we delete the forwards to this user as they don't really make sense anymore
    if ($delete_forwards) {
		// does another account forward to this one?
		$forwarders = pql_search_forwarders($ldap_linkid, $ldap_basedn, $domain, $user);
		if ($forwarders) {
			// someone forwards to this user. Now we need to know which addresses we're removing
			$email = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, PQL_LDAP_ATTR_MAIL);
			$aliases = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, PQL_LDAP_ATTR_MAILALTERNATE);
			
			$addresses[] = $email[0];
			if(is_array($aliases)){
				$addresses = array_merge($addresses, $aliases);
			}
		}
    }
    
    // delete the user
    if(!ldap_delete($ldap_linkid, $dn)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has been removed -> remove the cached version
    pql_cache_userentry_remove($dn);
    pql_cache_userdn_remove($user);
    
    // delete forwards to this account?
    if ($delete_forwards and $forwarders) {
		foreach($forwarders as $forward) {
			// get the forwarding addresses of this user
			$fwd_addresses = pql_get_userattribute($ldap_linkid, $ldap_basedn, $forward['domain'], $forward['reference'], PQL_LDAP_ATTR_FORWARDS);
			foreach($addresses as $address) {
				// does this user forward to the removed user or one of his aliases?
				$rem_key = array_search($address, $fwd_addresses);
				if ($rem_key !== false) {
					// we found a forward -> remove it 
					pql_remove_userattribute($ldap_linkid, $ldap_basedn, $forward['domain'], $forward['reference'], PQL_LDAP_ATTR_FORWARDS, $fwd_addresses[$rem_key]);
				}
			}
		} 
    }
    
    return true;
}

// ------------------------------------------------

function pql_remove_domain($ldap_linkid, $ldap_basedn, $domain, $delete_forwards){
    // removes a domain with all listed users
    
    if(!pql_domain_exist($ldap_linkid, $ldap_basedn, $domain)){
		// domain does not exist
		return false;
    }
    
    // remove all users of domain
    $users = pql_get_user($ldap_linkid, $ldap_basedn, $domain);
    if(is_array($users)) {
		foreach($users as $user){
			pql_remove_user($ldap_linkid, $ldap_basedn, $domain, $user, $delete_forwards);
		}
    }
    
    // Remove the USER subtrees if defined
    if(PQL_LDAP_SUBTREE_USERS) {
		$dn = PQL_LDAP_SUBTREE_USERS . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . PQL_LDAP_BASEDN;
		if(!ldap_delete($ldap_linkid, $dn)){
			pql_errormsg($ldap_linkid);
			return false;
		}
    }
    
    // Remove the GROUP subtrees if defined
    if(PQL_LDAP_SUBTREE_GROUPS) {
		// Remove all the GROUP objects of the domain
		$groups = pql_get_group($ldap_linkid, $ldap_basedn, $domain);
		if(is_array($groups)) {
			foreach($groups as $group){
				$dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
				
				if(!ldap_delete($ldap_linkid, $dn)){
					pql_errormsg($ldap_linkid);
					return false;
				}
			}
		}
		
		// Remove the group object
		$dn = PQL_LDAP_SUBTREE_GROUPS . "," . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . PQL_LDAP_BASEDN;
		if(!ldap_delete($ldap_linkid, $dn)){
			pql_errormsg($ldap_linkid);
			return false;
		}
    }
    
    // remove domain entry itself (ou)
    $dn = PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . $ldap_basedn;
    if(!ldap_delete($ldap_linkid, $dn)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}

// ------------------------------------------------

function pql_modify_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attrib, $oldvalue, $newvalue){
    // change an attribute of a user
    
    $dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
    
    // get all attributes first
    $entry = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attrib);
    
    // If we found an entry, make it's not overwritten, othervise
    // ADD the attribute.
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
    
    if(!ldap_mod_replace($ldap_linkid, $dn, $entry_new)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // user entry has changed -> remove the cached version
    pql_cache_userentry_remove($dn);
    if ($attrib == PQL_LDAP_REFERENCE_USERS_WITH) {
		pql_cache_userdn_remove($user);
    }
    
    return true;
}

// ------------------------------------------------

function pql_replace_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attrib, $value){
	// replaces an attribute of a user
	// (only use for single-field attributes)
	// delete an attribut if value is an empty string
	
	$dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
	
	// create entry array
	$entry[$attrib] = $value;
	
	// replace the attribute
	if(!ldap_mod_replace($ldap_linkid, $dn, $entry)){
	    // if the value is empty (""), this causes an error in OpenLDAP 2.x, try to ldap_mod_del
	    if($value == ""){
			$oldvalue = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attrib);
			$oldvalue = $oldvalue[0];
			$entry[$attrib] = $oldvalue;
			
			//return false;
			
			if(ldap_mod_del($ldap_linkid, $dn, $entry)){
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
	pql_cache_userentry_remove($dn);
	if ($attrib == PQL_LDAP_REFERENCE_USERS_WITH) {
		pql_cache_userdn_remove($user);
	}
	
	return true;
	
}

// ------------------------------------------------

function pql_replace_userattributes($ldap_linkid, $ldap_basedn, $domain, $user, $entry){
	// replaces more than one attribute of a user
	
	$dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
	
	if(!ldap_mod_replace($ldap_linkid, $dn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
	}
	
	// user entry has changed -> remove the cached version
	pql_cache_userentry_remove($dn);
	if (isset($entry[PQL_LDAP_REFERENCE_USERS_WITH])) {
		pql_cache_userdn_remove($user);
	}
	
	return true;
	
}

// ------------------------------------------------

function pql_get_domains($ldap_linkid,$ldap_basedn){
	// get all domains listed in ldap-tree (ou|dc-records)
	
	$sr = @ldap_list($ldap_linkid,$ldap_basedn,
					 "(&(" . PQL_LDAP_ATTR_DOMAIN . "=*)(objectClass=" . PQL_LDAP_OBJECTCLASS_DOMAIN . "))"
					 ) or pql_errormsg($ldap_linkid);
	$info = @ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);
	
	for ($i=0; $i<$info["count"]; $i++) {
		$domains[] = $info[$i][PQL_LDAP_ATTR_DOMAIN][0];
	}
	
	return $domains;
}

// ------------------------------------------------

function pql_get_user($ldap_linkid, $ldap_basedn, $domain){
	// get all users of a domain
	
	$dn = PQL_LDAP_SUBTREE_USERS . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . $ldap_basedn;
	$filter = PQL_LDAP_REFERENCE_USERS_WITH . "=*";
	
	$sr = @ldap_list($ldap_linkid, $dn, $filter) or pql_errormsg($ldap_linkid);
	$info = ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);
	
	for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i][PQL_LDAP_REFERENCE_USERS_WITH][0];
	}
	
	return $user;
}

function pql_get_group($ldap_linkid, $ldap_basedn, $domain){
    // get all groups of a domain
    
    $dn = PQL_LDAP_SUBTREE_GROUPS . "," . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . "," . $ldap_basedn;
    $filter = "(&(cn=*)(objectclass=posixgroup))";
    
    $sr = @ldap_list($ldap_linkid, $dn, $filter) or pql_errormsg($ldap_linkid);
    $info = ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);
    
    for ($i=0; $i<$info["count"]; $i++) {
		$groups[] = $info[$i][cn][0];
    }
    
    return $groups;
}

// ------------------------------------------------
// function based in PQL_GET_USER to search by the cn first letter
function pql_get_user_letter($ldap_linkid, $ldap_basedn, $domain, $letra) {
    $sr = ldap_search($ldap_linkid,
					  PQL_LDAP_ATTR_DOMAIN . "=" . $domain . ", " . $ldap_basedn,
					  "(" . PQL_LDAP_ATTR_UID . "=" . $letra . "*)");
    $info = ldap_get_entries($ldap_linkid, $sr);
    
    for ($i=0; $i<$info["count"]; $i++) {
		$user[] = $info[$i][PQL_LDAP_REFERENCE_USERS_WITH][0];
    }
    
    return $user;
}

// ------------------------------------------------
// function based in PQL_GET_USER to search by an initial number at cn attribute
function pql_get_user_number($ldap_linkid,$ldap_basedn,$domain){
    for ($numero=0; $numero<=9; $numero = $numero + 1){
		$sr = ldap_search($ldap_linkid,
						  PQL_LDAP_ATTR_DOMAIN . "=" . $domain . ", " . $ldap_basedn,
						  "(" . PQL_LDAP_ATTR_UID . "=" . $numero . "*)");
		$info = ldap_get_entries($ldap_linkid, $sr);
		
		for ($i=0; $i<$info["count"]; $i++) {
			$user[] = $info[$i][PQL_LDAP_REFERENCE_USERS_WITH][0];
		}
    }
    
    return $user;
}

// ------------------------------------------------

function pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, $attribute){
    // get one specified attribute about a user
    $dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
    
    // check whether this user is cached
    $value = '';
    
    // the user is cached  -> return the value for the requested attribute  
    if (pql_cache_userentry_get($dn, $attribute, $value)) {
		return($value);
    }
    
    // not yet cached -> get the userentry and cache it
    $entries = pql_get_userentry($ldap_linkid, $ldap_basedn, $domain, $user);
    if ($entries) {
		pql_cache_userentry_add($dn, $entries);
    } else {
		return(false);
    }
    
    return($entries[$attribute]);	
}

// ------------------------------------------------
function pql_get_userentry($ldap_linkid, $ldap_basedn, $domain, $user){
    // get full entry of a user
    $dn = pql_get_dn($ldap_linkid, $ldap_basedn, $user);
    $sr = @ldap_read($ldap_linkid, $dn, "(objectClass=*)");
    
    if($sr == false){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    // build array of all attributes
    $rid = @ldap_first_entry($ldap_linkid, $sr);
    $attribs[] = strtolower(@ldap_first_attribute($ldap_linkid, $rid, $berid));
    
    while($attrib = @ldap_next_attribute($ldap_linkid, $rid, $berid)){
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

function pql_get_userquota($ldap_linkid, $ldap_basedn, $domain, $user){
    // get formated quota of a user
    
    $quota = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, PQL_LDAP_ATTR_QUOTA);
    
    if(!is_array($quota)){
		return false;
    }
    
    return pql_parse_quota($quota[0]);
}

// ------------------------------------------------

function pql_get_dn($ldap_linkid, $ldap_basedn, $reference){
    // retrieve the dn for a given reference
    
    // check if we already have the dn for this user chached
    $dn = '';
    if (pql_cache_userdn_get($reference, $dn)) {
		return($dn);
    }
    
    // dn not in cache -> search for it
    $dn = pql_search_attribute($ldap_linkid, $ldap_basedn, PQL_LDAP_REFERENCE_USERS_WITH, $reference);
    
    if (!$dn) {
		return(false);
    }
    
    // we got the dn -> remember it
    pql_cache_userdn_add($reference, $dn);
    
    return($dn);
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

function pql_cache_userentry_add($dn, $entries){
    // adds an userentry to the cache
    
    global $pql_cache_userentry;
    
    foreach($entries as $attribute => $value) {
		$pql_cache_userentry[$dn][$attribute] = $value;
    }
}

// ------------------------------------------------

function pql_cache_userentry_get($dn, $attribute, &$value){
    // searches the cache for an attribute and returns it if found 
    
    global $pql_cache_userentry;
    
    // we don't have this user yet
    if (!isset($pql_cache_userentry[$dn])) {
		return(false);
    }
    
    // the user is cached, but he doesn't have the requested attribute
    if (!isset($pql_cache_userentry[$dn][$attribute])) {
		$value = NULL;
		return(true);
    }
    
    $value = $pql_cache_userentry[$dn][$attribute]; 
    return(true);
}

// ------------------------------------------------

function pql_cache_userentry_remove($dn){
    // removes an userentry from the cache
    
    global $pql_cache_userentry;
    
    unset($pql_cache_userentry[$dn]);
}

// ------------------------------------------------

function pql_cache_userdn_add($reference, $dn){
    // add the dn of an user to the cache
    
    global $pql_cache_userdn;
    
    $pql_cache_userdn[$reference] = $dn;
}

// ------------------------------------------------

function pql_cache_userdn_get($reference, &$dn){
    // searches the cache for an user and returns his dn if found
    
    global $pql_cache_userdn;
    
    if (isset($pql_cache_userdn[$reference])) {
		$dn = $pql_cache_userdn[$reference];
		return(true);
    } else {
		return(false);
    }
}

// ------------------------------------------------

function pql_cache_userdn_remove($reference){
    // removes an dn from the cache
    
    global $pql_cache_userdn;
    
    unset($pql_cache_userdn[$reference]);
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

function pql_search_attribute($ldap_linkid, $ldap_basedn, $attribute, $value){
    // search $attribute with $value in whole ldap tree, returns dn or false
    
    $filter = "(" . $attribute . "=" . $value . ")";
    $sr = ldap_search($ldap_linkid, $ldap_basedn, $filter, array($attribute));
    
    if(ldap_count_entries($ldap_linkid,$sr) > 0){
		$ed = ldap_first_entry($ldap_linkid,$sr);
		return ldap_get_dn($ldap_linkid,$ed);
    } else {
		return false;
    }
}

// ------------------------------------------------

function pql_search_forwarders($ldap_linkid, $ldap_basedn, $domain, $user){
    // search all accounts with forwarders to $user@$domain
    
    // get all email addresses of a user
    $email = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, PQL_LDAP_ATTR_MAIL);
    $aliases = pql_get_userattribute($ldap_linkid, $ldap_basedn, $domain, $user, PQL_LDAP_ATTR_MAILALTERNATE);
    
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
    
    $sr = ldap_search($ldap_linkid, $ldap_basedn, $filter);
    if(ldap_count_entries($ldap_linkid,$sr) > 0){
		$results = ldap_get_entries($ldap_linkid, $sr);
		
		foreach($results as $key => $result){
			if((string)$key != "count"){
				$ref = $result[PQL_LDAP_REFERENCE_USERS_WITH][0];
				$cn = $result["cn"][0];
				
				// Find the 'domain' of the user by removing some parts of the DN
				// This must be done if the user forwarding to the specified user,
				// is located under another tree/domain.
				// WARNING: CRUDE HACK!
				$replace = PQL_LDAP_REFERENCE_USERS_WITH . "=.*," . PQL_LDAP_SUBTREE_USERS;
				$domain = eregi_replace($replace, "", $result["dn"]);
				$domain = eregi_replace("," . PQL_LDAP_BASEDN, "", $domain);
				$domain = eregi_replace(PQL_LDAP_ATTR_DOMAIN . "=", "", $domain);
				
				$return[] = array("domain" => $domain,
								  "reference" => $ref,
								  PQL_LDAP_REFERENCE_USERS_WITH => $cn,
								  "email" => $result["mail"][0]);
				
				if(is_array($result[PQL_LDAP_ATTR_MAILALTERNATE])){
					foreach($result[PQL_LDAP_ATTR_MAILALTERNATE] as $key => $mailalternateaddress){
						if((string)$key != "count"){
							$return[] = array("domain" => $domain,
											  "reference" => $ref,
											  PQL_LDAP_REFERENCE_USERS_WITH => $cn,
											  "email" => $mailalternateaddress);
						}
					} // end foreach
				} // end if is_array
			} // end if
		} // end foreach
    } else {
		return false;
    }
    
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

function table_bgcolor(){
    global $flag;
    
    if($flag){
		$flag = false;
		echo "c1";
    } else {
		$flag = true;
		echo "c2";
    }
}

// ------------------------------------------------

function format_error($msg){
    return "<span class=\"error\">" . $msg . " </span>";
}

// ------------------------------------------------

function print_status_msg($msg){
	?>
	  <table cellpadding="3" cellspacing="0" border="0" width="100%">
		<tr>
		  <td class="message"><img src="images/info.png" width="16" height="16" border="0"> <?php echo stripslashes($msg); ?></td>
			</tr>
			  </table>
				<br>
				  <br>
					
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
    
    // ldap server configuration
    if(!defined("PQL_LDAP_HOST") or PQL_LDAP_HOST == ""){
		$result[] = "no ldap host defined!!";
    }
    
    if(!defined("PQL_LDAP_BASEDN") or PQL_LDAP_BASEDN == ""){
		$result[] = "no base dn defined";
    }
    
    if(!defined("PQL_LDAP_ROOTDN") or PQL_LDAP_ROOTDN == ""){
		$result[] = "no root dn defined!!";
    }
    
    if(!defined("PQL_LDAP_ROOTPW") or PQL_LDAP_ROOTPW == ""){
		$result[] = "no root pw defined!!";
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

function pql_get_next_uidnumber($ldap_linkid) {
    // Returns the next free UID/GID number
    
    $filter = "(&(uidnumber=*)(objectclass=posixaccount))";
    $sr = ldap_search($ldap_linkid, PQL_LDAP_BASEDN, $filter);
    $results = @ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);
	
    // Find the highest availible UIDNUMBER
    $highest = 0;
    for($i=0; $i < $results["count"]; $i++) {
		if(($results[$i]["uidnumber"][0] > $highest) && ($results[$i]["uidnumber"][0] < 64000)) {
			$highest = $results[$i]["uidnumber"][0];
		}
    }
	
    return $highest;
}

function pql_get_domain_value($linkid, $domain, $value, $match = "") {
    // In the domain object, there's embeded defaults
    //	description: defaultDomain=THE-DEFAULT-DOMAIN-NAME
	
    if($match = "") {
		$match = "*";
    } else {
		$match = $match . "*";
    }
	
    // (&(dc=bayour)(objectclass=domain)(description=defaultdomainMATCH))
    $filter = "(&(" . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . ")(objectclass=" . 
      PQL_LDAP_OBJECTCLASS_DOMAIN . ")(description=" . $value . $match . "))";
    $sr = ldap_search($linkid, PQL_LDAP_BASEDN, $filter);
    $results = @ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
	
    // Extract all the descriptor fields, look 
    for($i=0; $i < $results[0]["description"]["count"]; $i++) {
		if(eregi($value, $results[0]["description"][$i])) {
			$results = split("=", $results[0]["description"][$i]);
		}
    }
	
    // If nothing found, return the domain we where called with
    if($results[1] == "") {
		return $domain;
    }
	
    return $results[1];
}

function pql_set_domain_value($linkid, $domain, $type, $value) {
    // Set a embedded domain default value
	
    $filter = "(&(" . PQL_LDAP_ATTR_DOMAIN . "=" . $domain . ")(objectclass=" .
      PQL_LDAP_OBJECTCLASS_DOMAIN . "))";
    $sr = ldap_search($linkid, PQL_LDAP_BASEDN, $filter);
    $results = @ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);
	
    $dn = $results[0]["dn"];
	
    // Extract all the descriptor fields. We need to add those that
    // _don't_ contain $type (so we don't completly REPLACE the description
    // attribute).
    for($i=0; $i < $results[0]["description"]["count"]; $i++) {
		if(! eregi($type, $results[0]["description"][$i])) {
			$entry["description"][] = $results[0]["description"][$i];
		}
    }
	
    $entry["description"][] = $type . "=" . $value;
	
    if(!ldap_mod_replace($linkid, $dn, $entry)){
		pql_errormsg($linkid);
		return false;
    }
	
    return true;
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
