<?php
// ----------------------------
// pql_control.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control)
//
// $Id: pql_control.inc,v 2.45.6.2.2.1 2006-01-23 10:25:48 turbo Exp $
//

// {{{ control connection class

class pql_control extends pql {
    function pql_control($host, $binddn = "", $bindpw = "", $no_connect = false) {
		if(!pql_get_define("PQL_CONF_CONTROL_USE"))
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

// {{{ pql_control_update_domains(ldap, rootdn, dn, host, domain = "", type = "")
// Replicate all registred domains to locals
//
// domain is array as: 'oldvalue', 'newvalue'
function pql_control_update_domains($ldap, $rootdn, $dn, $host, $domain = "", $type = "") {
    if(!pql_get_define("PQL_CONF_CONTROL_USE"))
	  // control patch is not enabled
	  return false;

	// ----------------
	// {{{ host != '*' and !domain
	if(($host != '*') and !$domain) {
		$domains = pql_get_domains($ldap);
		foreach($domains as $domain) {
			// Get default domain name for this domain
			$domainname = pql_get_attribute($ldap->ldap_linkid, $domain, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
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
			$additional = pql_get_attribute($ldap->ldap_linkid, $domain, pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
			if(is_array($additional)) {
				$additional = pql_uniq($additional); // Remove duplicates (which was allowed in OpenLDAP <2.1).
				for($i=0; $i < count($additional); $i++) {
				  // Before we add this domain to the list, make sure it don't already exist
				  $ent_exists = 0;
				  if(is_array($entry)) {
					foreach($entry as $ent)
					  if($ent == $additional[$i])
						$ent_exists = 1;
				  }
				  
				  if(!$ent_exists)
					$entry[] = $additional[$i];
				}
			} elseif($additional) {
			  // Not an array, but defined.

			  // Before we add this domain to the list, make sure it don't already exist
			  $ent_exists = 0;
			  if(is_array($entry)) {
				foreach($entry as $ent)
				  if($ent == $additional)
					$ent_exists = 1;
			  }
			  
			  if(!$ent_exists)
				$entry[] = $additional;
			}
		}    
	}
	// }}}

    // {{{ Get the hostname
    if($host and ($host != '*') and pql_get_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", $rootdn)) {
		$hostname = split("\.", $host);
		$hostname = pql_maybe_encode($hostname[0]);
	
		foreach($temp as $tmp)
		  $entry[] = $hostname . "." . $tmp;
	}
	// }}}

    if(pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $rootdn))
	  $entry[] = "localhost";

	// ----------------
	if(($host != '*') and is_array($entry)) {
		// {{{ host != '*' and is_array(entry)
		if(($type == pql_get_define("PQL_ATTR_LOCALS")) or !$type) {
			if(pql_modify_attribute($ldap->ldap_linkid, $dn, pql_get_define("PQL_ATTR_LOCALS"), '', $entry))
			  return true;
		} elseif($type == pql_get_define("PQL_ATTR_RCPTHOSTS"))	{
			if(pql_modify_attribute($ldap->ldap_linkid, $dn, pql_get_define("PQL_ATTR_RCPTHOSTS"), '', $entry))
			  return true;
		}
		// }}}
	} else {
		// {{{ $host=* && $domain => Modify ALL servers
		// domain[0] => oldvalue
		// domain[1] => newvalue

		// {{{ Maybe IDNA encode the domain name
		if($domain[1])
		  $domain[1] = pql_maybe_idna_encode($domain[1]);
		// }}}

		// {{{ Get all QmailLDAP/Control hosts.
		$result = pql_get_dn($ldap->ldap_linkid, $dn, '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
		for($i=0; $result[$i]; $i++)
		  $hosts[] = pql_get_attribute($ldap->ldap_linkid, $result[$i], pql_get_define("PQL_ATTR_CN"));
		// }}}

		if(is_array($hosts)) {
			// Go through each QmailLDAP control object
			foreach($hosts as $h) {
				if(!eregi('^cn', $dn))
				  $hostdn = "cn=$h,$dn";
				else
				  $hostdn = $dn;

				// {{{ Setup the 'locals' attribute
				if(($type == pql_get_define("PQL_ATTR_LOCALS")) or !$type) {
					$dont_add = 0; unset($entry);

					// Get current locals for this host
					$locals = pql_get_attribute($ldap->ldap_linkid, $hostdn, pql_get_define("PQL_ATTR_LOCALS"));
					if(is_array($locals)) {
						// Sort and rearrange the array
						$locals = pql_uniq($locals);

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
						if($h and $domain[1] and pql_get_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", $rootdn)) {
							$hostname = split("\.", $h);
							$entry[] = $hostname[0] . "." . $domain[1];
						}

						if(is_array($entry)) {
							$entry = pql_uniq($entry);
							pql_modify_attribute($ldap->ldap_linkid, $hostdn, pql_get_define("PQL_ATTR_LOCALS"), '', $entry);
						}
					} else
					  // No previous values, add domain
					  pql_modify_attribute($ldap->ldap_linkid, $hostdn, pql_get_define("PQL_ATTR_LOCALS"), 1, $domain[1]);
				}
				// }}}

				// {{{ Setup the 'rcpthosts' attribute
				if($type == pql_get_define("PQL_ATTR_RCPTHOSTS") or !$type) {
					$dont_add = 0; unset($entry);

					// Get current rcpthosts for this host
					$rcpthosts = pql_get_attribute($ldap->ldap_linkid, $hostdn, pql_get_define("PQL_ATTR_RCPTHOSTS"));
					if(is_array($rcpthosts)) {
						// Sort and rearrange the array
						$rcpthosts = pql_uniq($rcpthosts);
						
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
						if($h and $domain[1] and pql_get_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", $rootdn)) {
							$hostname = split("\.", $h);
							$entry[] = $hostname[0] . "." . $domain[1];
						}
						
						if(is_array($entry)) {
							$entry = pql_uniq($entry);
							pql_modify_attribute($ldap->ldap_linkid, $hostdn, pql_get_define("PQL_ATTR_RCPTHOSTS"), '', $entry);
						}
					} else
					  // No previous values, add domain
					  pql_modify_attribute($ldap->ldap_linkid, $hostdn, pql_get_define("PQL_ATTR_RCPTHOSTS"), 1, $domain[1]);
				}
				// }}}
			}
		}
		// }}}
	}

    return true;
}
// }}}

// {{{ pql_control_print_filesize(bytes)
function pql_control_print_filesize($bytes) {
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

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
