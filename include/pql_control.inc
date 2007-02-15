<?php
// ----------------------------
// pql_control.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control)
//
// $Id: pql_control.inc,v 2.57 2007-02-15 12:08:12 turbo Exp $
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

// {{{ pql_control_update_domains(rootdn, dn, host, domain = "", type = "")
// Replicate all registred domains to locals
// domain is array as: 'oldvalue', 'newvalue'
function pql_control_update_domains($rootdn, $dn, $host, $domain = "", $type = "") {
  global $_pql;

  if(!pql_get_define("PQL_CONF_CONTROL_USE"))
	// control patch is not enabled
	return false;
  
  // {{{ Get the hostname (if we should add the hostname to each domain)
  if($host and ($host != "*") and pql_get_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", $rootdn)) {
	$hostname = split("\.", $host);
	$hostname = pql_maybe_encode($hostname[0]);
  } else
	$hostname = '';
  // }}}

  if($host != '*') {
	if(!$domain) {
	  // {{{ Do a MANUAL replication for ONE specific QLC host

	  // Get all default and additinal domain name(s)
	  $entry = pql_get_branch_domains();

	  // Get all email address domains for users which have this host as MX
	  $tmp = pql_get_email_domains($host);
	  pql_add2array($entry, $tmp);
	  
	  // {{{ If we're replicating rcptHosts, get all non-primary MX host value(s) in this MX/QLC object
	  if($type == pql_get_define("PQL_ATTR_RCPTHOSTS")) {
		$nonprimmx = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"));
		if($nonprimmx) {
		  if(!is_array($nonprimmx))
			$nonprimmx = array($nonprimmx);
		  
		  // Add the list of domain names to the entry to add
		  pql_add2array($entry, $nonprimmx);
		}
	  }
	  // }}}
	  
	  // {{{ Extra precaution: Uniqify and IDNA encode all domain names
	  // Uniqify the list of domain names.
	  $entry = pql_uniq($entry);
	  
	  // IDNA encode the domain name(s)
	  $count = count($entry);
	  for($i=0; $i <= $count; $i++) {
		$entry[$i] = pql_maybe_idna_encode($entry[$i]);

		// Add the hostname to this domainname
		if($hostname)
		  $entry[] = $hostname . "." . $entry[$i];
	  }
	  // }}}

	  // Send modifications to the LDAP server
	  if(pql_modify_attribute($dn, $type, 1, $entry))
		return true;
	  // }}}
	} else {
	  // {{{ Add {locals,rcpthosts} to ONE specific QLC object
	  if(!$type)
		$type = pql_get_define("PQL_ATTR_LOCALS");
	  
	  // {{{ Add the hostname to each domainname
	  if(pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $rootdn)) {
		foreach($entry as $domain_name)
		  $entry[] = $hostname . "." . $domain_name;
		
		$entry[] = "localhost";
	  }
	  // }}}
	  
	  if(pql_modify_attribute($dn, $type, '', $entry))
		return true;
	  // }}}
	}
  } elseif(($host == '*') and ($domain[0] or $domain[1])) {
	// {{{ Add, delete or replace {locals,rcpthosts} in _all_ QLC objects
	// '$host=* && $domain!=NULL' => Modify ALL servers
	//		domain[0] => oldvalue
	//		domain[1] => newvalue
	//
	//	If domain[0]!=NULL && domain[1]!=NULL	=> REPLACE domain[0] w/ domain[1]
	//	If domain[0]!=NULL && domain[1]==NULL	=> REMOVE domain[0]
	//	If domain[0]==NULL && domain[1]!=NULL	=> ADD domain[1]
	//	NOTE: BOTH can be set but not NULL!
	
	// {{{ Retreive and go through each QmailLDAP control object
	$hosts = $_pql->get_dn($dn, '(&(cn=*)(objectclass=qmailControl))');
	if($hosts) {
	  if(!is_array($hosts))
		$hosts = array($hosts);
	  
	  foreach($hosts as $host) {
		if(!$domain[0] and $domain[1])
		  // Make sure we don't modify a host unnecessary...
		  $domain_to_add_already_exists = 0;

		$attribs = array('locals'    => pql_get_define("PQL_ATTR_LOCALS"),
						 'rcpthosts' => pql_get_define("PQL_ATTR_RCPTHOSTS"));
		foreach($attribs as $key => $attrib) {
		  if(($type == $attrib) or !$type) {
			// Get current 'attrib' value for this QLC object
			$values = $_pql->get_attribute($host, $attrib);
			if($values) {
			  if(!is_array($values))
				$values = array($values);
			  $$key = $values;
			  
			  // {{{ Add, delete or replace a domain name depending on domain array content
			  foreach($values as $value) {
				if($domain[0] and $domain[1]) {
				  // {{{ We're REPLACING a domainname
				  if(lc($value) == lc($domain[0])) {
					if(pql_get_define("PQL_CONF_DEBUG_ME"))
					  echo "pql_control_update_domains(): Replacing '<b>$value</b>' with '<b>".$domain[1]."</b>' in QLC '$host:$key'<br>";

					$entry[$key][] = $domain[1];
				  } else
					$entry[$key][] = $value;
				  // }}}
				} elseif($domain[0] and !$domain[1]) {
				  // {{{ We're REMOVING a domainname
				  if(lc($domain[0]) != lc($value))
					$entry[$key][] = $value;
				  else
					if(pql_get_define("PQL_CONF_DEBUG_ME"))
					  echo "pql_control_update_domains(): Removing domain '<b>".$domain[0]."</b>' from QLC '$host:$key'<br>";
				  // }}}
				} elseif(!$domain[0] and $domain[1]) {
				  // {{{ We're ADDING a domainname. Add (all) the old one(s)
				  if(eregi($value, $domain[1]))
					$domain_to_add_already_exists = 1;

				  $entry[$key][] = $value;
				  // }}}
				}
			  }
			  // }}}
			}

			if((!$domain[0] and $domain[1]) and !$domain_to_add_already_exists) {
			  // Add the new domain
			  if(pql_get_define("PQL_CONF_DEBUG_ME"))
				echo "pql_control_update_domains(): Adding domain '<b>".$domain[1]."</b>' to QLC '$host:$key'<br>";
			  
			  $entry[$key][] = $domain[1];
			} else {
			  if(pql_get_define("PQL_CONF_DEBUG_ME"))
				echo "pql_control_update_domains(): Domain '<b>".$domain[1]."</b>' already exists in QLC '$host:$key'<br>";
			}

			// NOTE: If there is no previous values and we want to REMOVE a domain, then we
			//       don't need to do anything (can't delete something that isn't there! :).
		  }

		  // {{{ Extra precaution: IDNA encode and uniqify all domain names 
		  // IDNA encode the domain name(s)
		  $count = count($entry[$key]);
		  for($i=0; $i < $count; $i++) {
			$entry[$key][$i] = pql_maybe_idna_encode($entry[$key][$i]);
			
			// Add the hostname to this domainname
			if($hostname)
			  $entry[$key][] = $hostname . "." . $entry[$key][$i];
		  }
		  
		  // Uniqify the list of domain names.
		  $tmp = $entry[$key]; unset($entry[$key]);
		  $entry[$key] = pql_uniq($tmp);
		  // }}}
		}

		// {{{ Do modification
		if(($entry["locals"][0] or $entry["rcpthosts"][0]) and !$domain_to_add_already_exists)
		  pql_modify_attribute($host, '', '', $entry);
		// }}}
	  }
	}
	// }}}
	
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
