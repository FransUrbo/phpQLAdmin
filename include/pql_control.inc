<?php
// ----------------------------
// pql_control.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control)
//
// $Id: pql_control.inc,v 2.8 2003-03-29 15:15:29 turbo Exp $
//
// version history:
// $Log: pql_control.inc,v $
// Revision 2.8  2003-03-29 15:15:29  turbo
// If the LDAP base DN in searching for control attribute is empty, return. This could happen if there is no (correct) Control DN search base configured etc.
//
// Revision 1.2  2001/09/06 11:36:24  mb
// added no_connect option to pql_control-class
//
// Revision 1.1  2001/05/31 06:49:18  mb
// merged differences from branch phpQLAdmin_1_0release
//
// Revision 1.0.2.3  2001/05/22 14:52:30  mb
// fixed error reporting in some functions
//
// Revision 1.0.2.2  2001/05/21 14:42:32  mb
// fixed bug in pql_control_replace_attribute
//
// Revision 1.0.2.1  2001/05/21 13:49:45  mb
// bug in pql_control_delete_whole_attribute fixed
//
// Revision 1.0  2001/05/21 09:52:17  mb
// update version info to 1.0
//
// Revision 0.13  2001/05/21 08:40:46  mb
// added remove_whole_attribute function
//
// Revision 0.12  2001/05/18 12:49:19  mb
// many things changed
//
// Revision 0.11  2001/05/11 07:42:24  mb
// added pql_control_update_domains function
//
// Revision 0.10  2001/05/01 15:14:08  mb
// added check for unknown attributes
//
// Revision 0.9  2001/04/25 11:54:20  mb
// initial add
//
//
// ----------------------------
// Note: There are a few " followed after PHP comment
// instructions like // in this code . They are
// here to prevent Quanta+ highlighting all following
// code as a string. A dirty workaround, I know :)
//

// ----------------------------
// control connection class
// ----------------------------

class pql_control extends pql {
    function pql_control($host, $binddn = "", $bindpw = "", $no_connect = false){
		// call constructor of parent class
	
		if(!defined("PQL_LDAP_CONTROL_USE")) {
			// control patch is not enabled, leave
			return false;
		}
		
		$this->pql($host, $binddn, $bindpw, $no_connect);
    }
}

// ------------------------------------------------
// PQL - Control - API functions
// ------------------------------------------------

function pql_control_get_attribute($ldap_linkid, $ldap_control_basedn, $attribute) {
    // get a specified option
    $sr   = @ldap_read($ldap_linkid, $ldap_control_basedn, "(objectClass=qmailControl)", array($attribute));
    $info = @ldap_get_entries($ldap_linkid, $sr);
    
    for ($i=0; $i < $info[0][$attribute]["count"]; $i++) {
		$attrib[] = $info[0][$attribute][$i];
    }
    
    return $attrib;
}

// ------------------------------------------------

function pql_control_replace_attribute($ldap_linkid, $ldap_control_basedn, $attrib, $value) {
    // replaces an attribute
    
    // if $value == "" remove the attribute from tree
    if($value == ""){
		return pql_control_remove_whole_attribute($ldap_linkid, $ldap_control_basedn, $attrib);
    }
    
    // if $value is an array and all elements are "", remove attribute from tree
    if(is_array($value)){
		$remove = true;
		
		foreach($value as $val){
			if($val != ""){
				$remove = false;
			}
		}
		
		if($remove){
			return pql_control_remove_whole_attribute($ldap_linkid, $ldap_control_basedn, $attrib);
		}
    }
    
    // format entry
    $entry[$attrib] = $value;
    
    // replace
    if(!@ldap_mod_replace($ldap_linkid, $ldap_control_basedn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}

// ------------------------------------------------

function pql_control_search_attribute($ldap_linkid, $ldap_basedn, $attribute, $value) {
	if(!$ldap_basedn) {
		// Just incase there is no QmailLDAP/Controls object/DN
		return false;
	}

    // search $attribute with $value in whole ldap tree, returns dn or false
    $filter = "(" . $attribute . "=" . $value . ")";
    $sr = @ldap_search($ldap_linkid, $ldap_basedn, $filter, array($attribute));
    
    if($sr == false){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    if(ldap_count_entries($ldap_linkid,$sr) > 0){
		$ed = ldap_first_entry($ldap_linkid,$sr);
		return ldap_get_dn($ldap_linkid,$ed);
    } else {
		return false;
    }
}

// ------------------------------------------------

function pql_control_add_attribute($ldap_linkid, $ldap_control_basedn, $attrib, $value){
    // adds an attribute of a control property
    
    $entry[$attrib] = $value;
    
    if(!@ldap_mod_add($ldap_linkid, $ldap_control_basedn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}

// ------------------------------------------------

// removes an attribute of a QmailLDAP object
function pql_control_remove_attribute($ldap_linkid, $dn, $attrib, $value) {
    $entry[$attrib] = $value;
    
    if(!ldap_mod_del($ldap_linkid, $dn, $entry)) {
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}

// ------------------------------------------------

function pql_control_remove_whole_attribute($ldap_linkid, $ldap_control_basedn, $attrib){
    // removes whole attribute from control tree
    
    $attribute = pql_control_get_attribute($ldap_linkid, $ldap_control_basedn, $attrib);
    
    if($attribute == ""){
		// already deleted
		return true;
    }
    
    $entry[$attrib] = $attribute;
    
    if(!@ldap_mod_del($ldap_linkid, $ldap_control_basedn, $entry)){
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}

// ------------------------------------------------

// replicate all registred domains to locals
//
// domain is array as: 'oldvalue', 'newvalue'
function pql_control_update_domains($ldap, $dn, $host, $domain = "") {
    if(!defined("PQL_LDAP_CONTROL_USE")) {
		// control patch is not enabled
		return false;
    }

	if(!PQL_LDAP_CONTROL_AUTOADDLOCALS and $domain) {
		// do not replicate automatically
		return false;
	}

	if(($host != '*') and !$domain) {
		$domains = pql_get_domains($ldap);
		foreach($domains as $domain) {
			// Get default domain name for this domain
			$domainname = pql_get_domain_value($ldap, $domain, "defaultdomain");
			if($domainname) {
				// Before we add this domain to the list, make sure it don't already exist
				$ent_exists = 0;
				if(is_array($entry)) {
					foreach($entry as $ent) {
						if($ent == $domainname) {
							$ent_exists = 1;
						}
					}
				}
				
				if(!$ent_exists) {
					$temp[]  = $domainname;
					$entry[] = $domainname;
				}
			}
		}    
	}

    // Get the hostname
    if($host and ($host != '*')) {
		$hostname = split("\.", $host);
		$hostname = $hostname[0];
	
		foreach($temp as $tmp) {
			$entry[] = $hostname . "." . $tmp;
		}
	}
	
    if(defined("PQL_LDAP_CONTROL_AUTOADDLOCALS")) {
		$entry[] = "localhost";
    }

	if($host != '*') {
		if(pql_control_replace_attribute($ldap->ldap_linkid, $dn, "locals", $entry))
		  return true;
	} else {
		// $host=* && $domain => Modify ALL servers
		$hosts = pql_control_get_hosts($ldap->ldap_linkid, $dn);
		if(is_array($hosts)) {
			// Go through each QmailLDAP control object
			foreach($hosts as $h) {
				$hostdn = "cn=$h,$dn";
				unset($entry);

				// Get current locals for this host
				$locals = pql_control_get_attribute($ldap->ldap_linkid, $hostdn, "locals");
				if(!is_null($locals)) {
					foreach($locals as $local) {
						// As long as local isn't equal to domain to add, create the LDIF entry
						if($domain[0]) {
							if(!eregi($domain[0], $local))
							  $entry[] = $local;
						} else
						  $entry[] = $local;
					}

					// Add the domain to the LDIF entry
					if($domain[1])
					  $entry[] = $domain[1];

					// Add 'hostname.newdomain' to LDIF entry
					if($h and $domain[1]) {
						$hostname = split("\.", $h);
						$entry[] = $hostname[0] . "." . $domain[1];
					}

					pql_control_replace_attribute($ldap->ldap_linkid, $hostdn, "locals", $entry);
				}
			}
		}
	}

    return false;
}

// ------------------------------------------------

function pql_control_get_hosts($ldap_linkid, $ldap_basedn) {
	// Find all hosts below the $GLOBALS["USER_SEARCH_DN_CTR"]
	$sr = @ldap_search($ldap_linkid, $ldap_basedn,
					   '(&(cn=*)(objectclass=qmailControl))',
					   array('cn', 'dn'));

    if($sr == false){
		pql_errormsg($ldap_linkid);
		return false;
    }

	$info = @ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);

	for ($i=0; $i<$info["count"]; $i++) {
		$hosts[] = $info[$i]['cn'][0];
	}

	return $hosts;
}

// ----------------------------
// plugin register class
// ----------------------------

class pql_control_plugin_storage {
    var $plugins = array();
    var $cats = array();
    var $attribs = array();
    
    //function pql_control_plugin_storage(){
    //
    //}
    
    function add_plugin($plugin_name, $plugin_file, $plugin_cat){
		// adds an attribute plugin to the plugin storage
		
		if(isset($this->plugins[$plugin_name])){
			// plugin with this name already exists
			return false;
		}
		
		$this->plugins[$plugin_name]["cat"] = $plugin_cat;
		$this->plugins[$plugin_name]["file"] = $plugin_file;
		
		$this->cats[$plugin_cat] = 1;
		
		return true;
    }
	
    function add_attrib($attrib, $plugin_name){
		// adds and attribute to a registred plugin
		
		if(!isset($this->plugins[$plugin_name])){
			// plugin with this name does not exist
			return false;
		}
		
		$this->attribs[$attrib] = $plugin_name;
		
		return true;
    }
	
    function get_plugin($attrib){
		// returns name of plugin for $attrib
		
		if(!isset($this->attribs[$attrib])){
			// an attribute with this name does not exist
			return false;
		}
		
		$plugin_name = $this->attribs[$attrib];
		return $plugin_name;
    }
	
    function get_plugin_filename($plugin_name){
		// return filename of plugin for specified plugin name
		
		if(!isset($this->plugins[$plugin_name])){
			// plugin with this name does not exist
			return false;
		}
		
		return $this->plugins[$plugin_name]["file"];
    }
	
    function get_plugin_cat($plugin_name){
		// returns category of plugin name
		
		return $this->plugins[$plugin_name]["cat"];
    }
    
    function get_cat_plugins($cat){
		// return all plugin names of category
		
		foreach($this->plugins as $plugin_name => $plugin_data){
			if($plugin_data["cat"] == $cat){
				$plugins[] = $plugin_name;
			}
		}
		
		return $plugins;
    }
	
    function get_cats(){
		// return all registred categories
		foreach($this->cats as $cat => $value){
			$cats[] = $cat;
		}
		
		return $cats;
    }
}

// ----------------------------
// plugin API functions
// ----------------------------

$_pql_control_plugins = new pql_control_plugin_storage();

// ------------------------------------------------

function pql_control_plugin_register($plugin_name, $plugin_file, $plugin_cat, $attribs){
    // register a plugin for specified attribute
    global $_pql_control_plugins;
    
    $_pql_control_plugins->add_plugin($plugin_name, $plugin_file, $plugin_cat);
    
    if(is_array($attribs)){
		foreach($attribs as $attrib){
			$_pql_control_plugins->add_attrib($attrib, $plugin_name);
		}
    } else {
		$_pql_control_plugins->add_attrib($attribs, $plugin_name);
    }
}

// ------------------------------------------------

function pql_control_plugin_get_cats(){
    // get all plugin categories
    global $_pql_control_plugins;
    
    return $_pql_control_plugins->get_cats();
}

// ------------------------------------------------

function pql_control_plugin_get($attrib){
    // get a filename for $attrib
    global $_pql_control_plugins;
    
    return $_pql_control_plugins->get_plugin($attrib);
}

// ------------------------------------------------

function pql_control_plugin_get_catplugins($cat){
    // gets all plugin names for specified category
    global $_pql_control_plugins;
    
    return $_pql_control_plugins->get_cat_plugins($cat);
}

// ------------------------------------------------

function pql_control_plugin_cat($plugin_name){
    // gets the category name of plugin
    global $_pql_control_plugins;
    
    return $_pql_control_plugins->get_plugin_cat($plugin_name);
}

// ------------------------------------------------

function pql_control_plugin_get_filename($plugin_name){
    // gets all plugin names for specified category
    global $_pql_control_plugins;
    
    return $_pql_control_plugins->get_plugin_filename($plugin_name);
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
