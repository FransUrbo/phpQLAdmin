<?php
// ----------------------------
// pql_control.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control)
//
// $Id: pql_control.inc,v 2.30 2003-11-12 09:33:32 turbo Exp $
//

// {{{ control connection class
class pql_control extends pql {
    function pql_control($host, $binddn = "", $bindpw = "", $no_connect = false) {
		if(!pql_get_define("PQL_GLOB_CONTROL_USE"))
		  // control patch is not enabled, leave
		  return false;
		
		// call constructor of parent class
		$this->pql($host, $binddn, $bindpw, $no_connect);
    }
}
// }}}

// ------------------------------------------------
// PQL - Control - API functions
// ------------------------------------------------

// {{{ pql_control_get_attribute(linkid, dn, attribute)
function pql_control_get_attribute($linkid, $dn, $attribute) {
    // get a specified option
    $sr   = ldap_read($linkid, $dn, "(objectClass=qmailControl)", array($attribute));
    $info = ldap_get_entries($linkid, $sr);
    
    for ($i=0; $i < $info[0][$attribute]["count"]; $i++) {
		$attrib[] = pql_maybe_decode($info[0][$attribute][$i]);
    }
    
    return $attrib;
}
// }}}

// {{{ pql_control_get_hosts(ldap_linkid, ldap_basedn)
function pql_control_get_hosts($ldap_linkid, $ldap_basedn) {
	// Find all hosts below the $GLOBALS["USER_SEARCH_DN_CTR"]
	$sr = @ldap_search($ldap_linkid, $ldap_basedn,
					   '(&(cn=*)(objectclass=qmailControl))',
					   array('cn', 'dn'));

    if($sr == false) {
		pql_errormsg($ldap_linkid);
		return false;
    }

	$info = @ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($ldap_linkid);

	for ($i=0; $i<$info["count"]; $i++) {
		$hosts[] = pql_maybe_decode($info[$i]['cn'][0]);
	}

	return $hosts;
}
// }}}

// {{{ pql_control_replace_attribute(ldap_linkid, ldap_control_basedn, attrib, value)
function pql_control_replace_attribute($ldap_linkid, $ldap_control_basedn, $attrib, $value) {
    // replaces an attribute

    // if $value == "" remove the attribute from tree
    if($value == "")
	  return pql_control_remove_whole_attribute($ldap_linkid, $ldap_control_basedn, $attrib);
    
    // if $value is an array and all elements are "", remove attribute from tree
    if(is_array($value)) {
		$remove = true;
		
		foreach($value as $val)
		  if($val != "")
			$remove = false;
		
		if($remove)
		  return pql_control_remove_whole_attribute($ldap_linkid, $ldap_control_basedn, $attrib);
    }
    
    // format entry
	if(is_array($value)) {
		foreach($value as $val)
		  $entry[$attrib][] = pql_maybe_encode($val);
	} else
	  $entry[$attrib] = $value;
    
	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_control_replace_attribute", $ldap_control_basedn, $entry);

    // replace
    if(!ldap_mod_replace($ldap_linkid, $ldap_control_basedn, $entry)) {
		// Try to delete the attribute from the object
		$del[$attrib] = array();
		if(!ldap_mod_del($ldap_linkid, $ldap_control_basedn, $del)) {
			echo "Can't delete attribute $attrib<br>";

			pql_errormsg($ldap_linkid);
			die("$LDIF");
			return false;
		}

		// Now add the attribute again (this time with the new value(s))
		if(!ldap_mod_add($ldap_linkid, $ldap_control_basedn, $entry)) {
			echo "Can't add attribute $attrib<br>";

			pql_errormsg($ldap_linkid);
			die("$LDIF");
			return false;
		}
    }
    
    return true;
}
// }}}

// {{{ pql_control_search_attribute(ldap_linkid, ldap_basedn, attribute, value, multiple = 0)
function pql_control_search_attribute($ldap_linkid, $ldap_basedn, $attribute, $value, $multiple = 0) {
	if(!$ldap_basedn)
	  // Just incase there is no QmailLDAP/Controls object/DN
	  return false;

    // search $attribute with $value in whole ldap tree, returns dn or false
    $filter = "(" . $attribute . "=" . $value . ")";
    $sr = @ldap_search($ldap_linkid, $ldap_basedn, $filter, array($attribute));
    
    if($sr == false) {
		pql_errormsg($ldap_linkid);
		return false;
    }

    if(ldap_count_entries($ldap_linkid, $sr) > 0) {
		if(!$multiple) {
			$ed = ldap_first_entry($ldap_linkid, $sr);
			return ldap_get_dn($ldap_linkid,$ed);
		} else {
			$info = ldap_get_entries($ldap_linkid, $sr) or pql_errormsg($linkid);
			for($i=0; $i < $info["count"]; $i++)
			  $retval[] = $info[$i]["dn"];

			return $retval;
		}
    } else {
		return false;
    }
}
// }}}

// {{{ pql_control_add_attribute(ldap_linkid, ldap_control_basedn, attrib, value)
function pql_control_add_attribute($ldap_linkid, $ldap_control_basedn, $attrib, $value) {
    // adds an attribute of a control property
    $entry[$attrib] = pql_maybe_encode($value);

    if(!@ldap_mod_add($ldap_linkid, $ldap_control_basedn, $entry)) {
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}
// }}}

// {{{ pql_control_remove_attribute(ldap_linkid, dn, attrib, value)
// Removes an attribute of a QmailLDAP object
function pql_control_remove_attribute($ldap_linkid, $dn, $attrib, $value) {
    $entry[$attrib] = pql_maybe_encode($value);
    
    if(!ldap_mod_del($ldap_linkid, $dn, $entry)) {
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}
// }}}

// {{{ pql_control_remove_whole_attribute(ldap_linkid, ldap_control_basedn, attrib)
// Removes whole attribute from control tree
function pql_control_remove_whole_attribute($ldap_linkid, $ldap_control_basedn, $attrib) {
	// Delete all attributes of this type, regardless of value
	$entry[$attrib] = array();

    if(!@ldap_mod_replace($ldap_linkid, $ldap_control_basedn, $entry)) {
		pql_errormsg($ldap_linkid);
		return false;
    }
    
    return true;
}
// }}}

// {{{ pql_control_update_domains(ldap, dn, host, domain = "", type = "")
// Replicate all registred domains to locals
//
// domain is array as: 'oldvalue', 'newvalue'
function pql_control_update_domains($ldap, $dn, $host, $domain = "", $type = "") {
    if(!pql_get_define("PQL_GLOB_CONTROL_USE"))
	  // control patch is not enabled
	  return false;

	// ----------------
	if(($host != '*') and !$domain) {
		$domains = pql_get_domains($ldap);
		foreach($domains as $domain) {
			// Get default domain name for this domain
			$domainname = pql_get_domain_value($ldap, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));
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

			// Get additional domain names
			$additional = pql_get_domain_value($ldap, $domain, pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"));
			if(is_array($additional)) {
				for($i=0; $i < count($additional); $i++) 
				  $entry[] = $additional[$i];
			}
		}    
	}

    // Get the hostname
    if($host and ($host != '*') and pql_get_define("PQL_GLOB_CONTROL_AUTOADDHOSTNAME")) {
		$hostname = split("\.", $host);
		$hostname = pql_maybe_encode($hostname[0]);
	
		foreach($temp as $tmp)
		  $entry[] = $hostname . "." . $tmp;
	}
	
    if(pql_get_define("PQL_GLOB_CONTROL_AUTOADDLOCALS"))
	  $entry[] = "localhost";

	// ----------------
	if(($host != '*') and is_array($entry)) {
		if(($type == pql_get_define("PQL_GLOB_ATTR_LOCALS")) or !$type) {
			if(pql_control_replace_attribute($ldap->ldap_linkid, $dn,
											 pql_get_define("PQL_GLOB_ATTR_LOCALS"),
											 $entry))
			  return true;
		} elseif($type == pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS")) {
			if(pql_control_replace_attribute($ldap->ldap_linkid, $dn,
											 pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"),
											 $entry))
			  return true;
		}
	} else {
		// $host=* && $domain => Modify ALL servers
		// domain[0] => oldvalue
		// domain[1] => newvalue

		$hosts = pql_control_get_hosts($ldap->ldap_linkid, $dn);
		if(is_array($hosts)) {
			// Go through each QmailLDAP control object
			foreach($hosts as $h) {
				if(!eregi('^cn', $dn))
				  $hostdn = "cn=$h,$dn";
				else
				  $hostdn = $dn;

				// --------------------------------
				if(($type == pql_get_define("PQL_GLOB_ATTR_LOCALS")) or !$type) {
					$dont_add = 0; unset($entry);

					// Get current locals for this host
					$locals = pql_control_get_attribute($ldap->ldap_linkid, $hostdn,
														pql_get_define("PQL_GLOB_ATTR_LOCALS"));
					if(!is_null($locals)) {
						asort($locals); reset($locals);
						
						// Rearrange the array
						$locals = array_values(array_unique($locals));

						// Do not set locals for hosts which HAVE no locals (ie, slaves)
						foreach($locals as $local) {
							// Make sure the domain don't already exists...
							if(lc($domain[1]) == lc($local))
							  $dont_add = 1;
							else {
								// As long as local isn't equal to domain to add, create the LDIF entry
								if($domain[0]) {
									if(!eregi($domain[0], $local))
									  $entry[] = $local;
								} else
								  $entry[] = $local;
							}
						}
						
						// Add the domain to the LDIF entry
						if($domain[1] && !$dont_add)
						  $entry[] = $domain[1];
						
						// Add 'hostname.newdomain' to LDIF entry
						if($h and $domain[1] and pql_get_define("PQL_GLOB_CONTROL_AUTOADDHOSTNAME")) {
							$hostname = split("\.", $h);
							$entry[] = $hostname[0] . "." . $domain[1];
						}

						if(is_array($entry))
						  pql_control_replace_attribute($ldap->ldap_linkid, $hostdn,
														pql_get_define("PQL_GLOB_ATTR_LOCALS"),
														$entry);
					} else
					  // No previous values, add domain
					  pql_control_add_attribute($ldap->ldap_linkid, $hostdn,
												pql_get_define("PQL_GLOB_ATTR_LOCALS"),
												$domain[1]);
				}

				// --------------------------------
				if($type == pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS") or !$type) {
					$dont_add = 0; unset($entry);

					// Get current rcpthosts for this host
					$rcpthosts = pql_control_get_attribute($ldap->ldap_linkid, $hostdn,
														   pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"));
					if(!is_null($rcpthosts)) {
						asort($rcpthosts); reset($rcpthosts);

						// Rearrange the array
						$rcpthosts = array_values(array_unique($rcpthosts));
						
						// Do not set rcpthosts for hosts which HAVE no rcpthosts (ie, masters)
						foreach($rcpthosts as $rcpt) {
							// Make sure the domain don't already exists...
							if($domain[1] == $rcpt)
							  $dont_add = 1;
							else {
								// As long as local isn't equal to domain to add, create the LDIF entry
								if($domain[0]) {
									if(!eregi($domain[0], $rcpt))
									  $entry[] = $rcpt;
								} else
								  $entry[] = $rcpt;
							}
						}
						
						// Add the domain to the LDIF entry
						if($domain[1] && !$dont_add)
						  $entry[] = $domain[1];
						
						// Add 'hostname.newdomain' to LDIF entry
						if($h and $domain[1] and pql_get_define("PQL_GLOB_CONTROL_AUTOADDHOSTNAME")) {
							$hostname = split("\.", $h);
							$entry[] = $hostname[0] . "." . $domain[1];
						}
						
						if(is_array($entry))
						  pql_control_replace_attribute($ldap->ldap_linkid, $hostdn,
														pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"),
														$entry);
					} else
					  // No previous values, add domain
					  pql_control_add_attribute($ldap->ldap_linkid, $hostdn,
												pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"),
												$domain[1]);
				}
			}
		}
	}

    return true;
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
