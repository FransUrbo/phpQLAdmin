<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.24.16.2 2004-12-19 16:20:11 turbo Exp $
//

// {{{ pql_bind9_get_zone(linkid, branchname, domainname)
function pql_bind9_get_zone($linkid, $branchname, $domainname = '') {
	$dn = "ou=DNS,".urldecode($branchname);

	if(!empty($domainname))
	  $filter = "(&(zoneName=$domainname)(objectClass=dNSZone))";
	else
	  $filter = 'objectClass=dNSZone';

    $sr = @ldap_search($linkid, $dn, $filter);
    if($sr) {
		// Oki, we have a DNS zone in LDAP.
		$info = ldap_get_entries($linkid, $sr)
		  or pql_format_error(1);
		for($i=0; $i < $info["count"]; $i++) {
			$zone = $info[$i]['zonename'][0];

			if(eregi("in-addr.arpa", $zone) and ($host != '@')) {
				// This is an reverse zone.
				unset($host);

				$tmp = split('\.', $info[$i]['relativedomainname'][0]);
				for($j=count($tmp)-1; $tmp[$j]; $j--) {
					$host .= $tmp[$j];
					if($tmp[$j-1])
					  $host .= '.';
				}
			} else
			  $host = $info[$i]['relativedomainname'][0];
				
			$ZONE[$zone][$host]['RDN']  = pql_maybe_idna_decode($info[$i]['dn']);
			$ZONE[$zone][$host]['ZONE'] = pql_maybe_idna_decode($zone);
			$ZONE[$zone][$host]['TTL']  = $info[$i]['dnsttl'][0];
			
			// PTR Record
			if(eregi("in-addr.arpa", $zone) and ($host != '@')) {
				unset($PTR);

				$tmp = preg_replace('/.in-addr.arpa/', '', $zone);
				$tmp = split('\.', $tmp);
				for($j=count($tmp)-1; $tmp[$j]; $j--)
				  $PTR .= $tmp[$j].'.';
				
				$ZONE[$zone][$host]['PTR']  = $PTR.$host;
				$ZONE[$zone][$host]['HOST'] = $host;
			} else
			  $ZONE[$zone][$host]['HOST']  = pql_maybe_idna_decode($host);

 			// A Record(s)
			if($info[$i]['arecord'][0]) {
				if($info[$i]['arecord'][1]) {
					// We have multiple A records - Round-Robin!
					for($j=0; $j < $info[$i]['arecord']['count']; $j++)
					  $ZONE[$zone][$host]['A'][] = ($info[$i]['arecord'][$j]);
				} else
				  $ZONE[$zone][$host]['A'] = pql_maybe_idna_decode($info[$i]['arecord'][0]);
			}
			
			// CNAME Record
			if($info[$i]['cnamerecord'][0]) {
				$cname = pql_maybe_idna_decode($info[$i]['cnamerecord'][0]);
				
				if(preg_match("/\\.$/", $cname))
				  // Remove the domain name, so we're left with the hostname.
				  $cname = str_replace(".$zone.", "", $cname);
				
				$ZONE[$zone][$host]['CNAME'] = $cname;
			}

			// HINFO Record
			if($info[$i]['hinforecord'][0])
			  $ZONE[$zone][$host]['HINFO'] = $info[$i]['hinforecord'][0];

			// SRV Record
			if($info[$i]['srvrecord'][0])
			  $ZONE[$zone][$host]['SRV'] = $info[$i]['srvrecord'][0];
			
			// TXT Record
			if($info[$i]['txtrecord'][0])
			  $ZONE[$zone][$host]['TXT'] = $info[$i]['txtrecord'][0];

			// NS Record(s)
			if($info[$i]['nsrecord'][0]) {
				if($info[$i]['nsrecord'][1]) {
					// We have multiple records
					for($j=0; $j < $info[$i]['nsrecord']['count']; $j++)
					  $ZONE[$zone][$host]['NS'][] = pql_maybe_idna_decode($info[$i]['nsrecord'][$j]);
				} else
				  $ZONE[$zone][$host]['NS'] = pql_maybe_idna_decode($info[$i]['nsrecord'][0]);
			}

			// MX Record(s)
			if($info[$i]['mxrecord'][0]) {
				if($info[$i]['mxrecord'][1]) {
					// We have multiple records
					for($j=0; $j < $info[$i]['mxrecord']['count']; $j++)
					  $ZONE[$zone][$host]['MX'][] = pql_maybe_idna_decode($info[$i]['mxrecord'][$j]);
				} else
				  $ZONE[$zone][$host]['MX'] = pql_maybe_idna_decode($info[$i]['mxrecord'][0]);
			}

			// SOA Record
			if($info[$i]['soarecord'][0]) {
				$soa = split(' ', $info[$i]['soarecord'][0]);

				$ZONE[$zone][$host]['SOA']['ADMIN'] = pql_maybe_idna_decode($soa[1]);
				$ZONE[$zone][$host]['SOA']['SERIAL'] = $soa[2];
				$ZONE[$zone][$host]['SOA']['REFRESH'] = $soa[3];
				$ZONE[$zone][$host]['SOA']['RETRY'] = $soa[4];
				$ZONE[$zone][$host]['SOA']['EXPIRE'] = $soa[5];
				$ZONE[$zone][$host]['SOA']['TTL'] = $soa[6];
			}

			// Sort the host array 
			ksort($ZONE[$zone]);
		}
		
		// Sort the zone array
		if(is_array($ZONE)) {
			ksort($ZONE);
			return($ZONE);
		} else 
		  return(array());
    }
}
// }}}


// {{{ pql_bind9_add_zone(linkid, dn, zone)
function pql_bind9_add_zone($linkid, $dn, $zone) {
	$MASTER_DN = urldecode($dn);

	// ----------------
	// See if the sub branch exists. If not, create it.
	if(!pql_unit_exist($linkid, $MASTER_DN, 'DNS')) {
		// Does not exists - create it
		if(pql_unit_add($linkid, $MASTER_DN, 'ou=DNS')) {
			$dn = "ou=DNS,$MASTER_DN";

			$DNs[] = $dn;
		}
	}

	// ----------------
	// Create each of the domain components in the domainname
	$dc = split('\.', $zone);

	unset($entry);
	$entry['objectclass'][] = 'top';
	$entry['objectclass'][] = 'domain';

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"]) 
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid,
																			  $_SESSION["USER_DN"],
																			  'dnszone');

	for($i=count($dc)-1; $dc[$i]; $i--) {
		$dc_tmp = pql_maybe_idna_encode($dc[$i]);

		$entry[pql_get_define("PQL_ATTR_DC")] = $dc_tmp;
		if($dn)
		  $dn = pql_get_define("PQL_ATTR_DC")."=".$dc_tmp.",".$dn;
		else
		  $dn = pql_get_define("PQL_ATTR_DC")."=".$dc_tmp;

		// See if the component exists. If not, create it.
		$sr = @ldap_read($linkid, $dn, 'objectClass=domain');
		if(!$sr) {
			// Create a LDIF object to print in case of error
			$LDIF = pql_create_ldif("pql_bind9_add_zone - dc=".$dc_tmp, $dn, $entry);
			
			if(! ldap_add($linkid, $dn, $entry)) {
				// Can't add domain component
				pql_format_error(1);
				echo "Can't add domain component...<br>\n";
				die($LDIF);
			} else
			  $DNs[] = $dn;
		}
	}

	// ----------------
	// Create a template SOA record
	$zonename = pql_maybe_idna_encode($zone);
	unset($entry);
	$entry['objectClass'][]		 = 'top';
	$entry['objectClass'][]		 = 'dNSZone';

	$entry['relativeDomainName'] = '@';
	$entry['zoneName']			 = $zonename;
	$entry['dNSTTL']			 = 604800;

	$entry['sOARecord']			 = "$zonename. registry.$zonename. 2002082106 172800 86400 2419200 604800";
	$entry['nSRecord'][]		 = "ns1.$zonename.";
	$entry['nSRecord'][]		 = "ns2.$zonename.";
	$entry['mXRecord']			 = "10 mail.$zonename.";
	$entry['tXTRecord']			 = "$zonename";

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($ldap_linkid,
																			  $_SESSION["USER_DN"],
																			  'dnszone');

	$dn = "relativeDomainName=".$entry['relativeDomainName'].",$dn";

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_zone - @", $dn, $entry);

	if(! ldap_add($linkid, $dn, $entry)) {
		// Can't add SOA record
		pql_format_error(1);
		echo "Can't add the 'relativeDomainName=@' entry...<br>\n";
		die($LDIF);
	} else
	  $DNs[] = $dn;

	return true;
}
// }}}

// {{{ pql_bind9_del_zone(linkid, dn, zone)
function pql_bind9_del_zone($linkid, $dn, $zone) {
	$dnsparts = split('\.', $zone);
	for($i=0; $dnsparts[$i]; $i++) {
	    $dn_to_remove .= "dc=".$dnsparts[$i].",";
	}
	$dn_to_remove .= "ou=DNS,".$dn;

	$lc_zone = lc($dn_to_remove);
	
	// Retreive ALL objects below this DN
	$sr = ldap_search($linkid, $dn_to_remove, 'objectClass=*');
	$info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
	for ($i=0; $i < $info["count"]; $i++) {
		$lc_dn = lc($info[$i]["dn"]);

		if($lc_dn != $lc_zone) {
			// Don't delete the acctuall zone quite yet
			if(!ldap_delete($linkid, $info[$i]["dn"]))
			  pql_format_error(1);
		}
	}

	// Delete the zone itself
	if(!ldap_delete($linkid, $dn_to_remove)) {
		pql_format_error(1);
		return false;
	}


	// -------------------------------------------------------------
	// If this was the last zone in this TLD, remove the TLD as well

	// Get the TLD DN
	$dnparts = split(',', $dn_to_remove);
	for($i=1; $dnparts[$i]; $i++) {
		$tld .= $dnparts[$i];
		if($dnparts[$i+1])
		  $tld .= ",";
	}

	// Look in this DN if there's objects.
    $sr = ldap_search($linkid, $tld, 'objectClass=*');
    $info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
	if(! $info[1]) {
		// We don't have more than the DN object, delete it.
		if(!ldap_delete($linkid, $tld)) {
			pql_format_error(1);
			return false;
		}
	}


	// -------------------------------------------------------------
	// If this was the last TLD in this branch, remove the organiztionalUnit
	// ou=DNS as well.

	// Get the ou DN
	$dnparts = split(',', $tld);
	for($i=1; $dnparts[$i]; $i++) {
		$ou .= $dnparts[$i];
		if($dnparts[$i+1])
		  $ou .= ",";
	}

	// Look in this DN if there's objects.
    $sr = ldap_search($linkid, $ou, 'objectClass=*');
    $info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
	if(! $info[1]) {
		// We don't have more than the DN object, delete it.
		if(!ldap_delete($linkid, $ou)) {
			pql_format_error(1);
			return false;
		}
	}

	return true;
}
// }}}


// {{{ pql_bind9_add_host(linkid, basedn, entry)
function pql_bind9_add_host($linkid, $basedn, $entry) {
	// Create each of the domain components in the domainname
	$dc = split('\.', $entry[pql_get_define("PQL_ATTR_ZONENAME")]);
	for($i=count($dc)-1; $dc[$i]; $i--) {
		$dc_tmp = pql_maybe_idna_encode($dc[$i]);

		if($dn)
		  $dn = "dc=".$dc_tmp.",".$dn;
		else
		  $dn = "dc=".$dc_tmp;
	}
	$dn = "relativeDomainName=".$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")].",$dn,ou=DNS,$basedn";

	$entry['objectClass'][] = 'top';
	$entry['objectClass'][] = 'dNSZone';

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'dnszone');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_host", $dn, $entry);
	if(file_exists("./.DEBUG_ME"))
	  die($LDIF);

	if(! ldap_add($linkid, $dn, $entry)) {
		pql_format_error(1);
		die($LDIF);
		return false;
	}

	return true;
}
// }}}

// {{{ pql_bind9_del_host(linkid, dn)
function pql_bind9_del_host($linkid, $dn) {
	$dn = urldecode($dn);
	if(file_exists("./.DEBUG_ME"))
	  return true;

	if(!ldap_delete($linkid, $dn))
	  pql_format_error(1);

	return true;
}
// }}}


// {{{ pql_bind9_get_record(linkid, dn, record)

function pql_bind9_get_record($linkid, $dn, $record = '') {
	if($record) {
		$record = lc($record);

		$filter = "(&(objectclass=dNSZone)($record=*)";
		$attrs  = array($record);
	} else {
		$filter = "(&(objectclass=dNSZone)";
		$attrs  = array();
	}

	if(ereg('@', $dn)) {
		// It's a SOA record. This is encoded differently in the DN and
		// inside the object -> Only the @ is INSIDE the object. Search
		// one level above the exact DN.
		$object = ldap_explode_dn($dn, 0);
		$len    = count($object)-2;

		unset($dn);
		for($i=$len; $object[$i]; $i--)
		  if($i > 0)
			$dn = ",".$object[$i].$dn;

		$dn = ereg_replace("^,", "", $dn);

		$filter .= "(relativeDomainName=@))";

		$sr = @ldap_list($linkid, $dn, $filter, $attrs) or pql_format_error(1);
	} else {
		$filter .= ")";
		$sr = @ldap_read($linkid, $dn, $filter, $attrs) or pql_format_error(1);
	}

	$info = @ldap_get_entries($linkid, $sr) or pql_format_error(1);

	if($info[0][$record]["count"] == 1)
	  $ret[] = $info[0][$record][0];
	else {
		for($i=0; $i < $info[0][$record]["count"]; $i++)
		  $ret[] = $info[0][$record][$i];
	}

	return($ret);
}

// }}}

// {{{ pql_bind9_set_record(linkid, dn, record, value)
function pql_bind9_set_record($linkid, $dn, $record, $value = '') {
	if(!is_array($record)) {
		$record = lc($record);
		$entry[$record] = $value;
	} else
	  // We've been called with an already created 'LDIF'...
	  $entry = $record;

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_set_record", $dn, $entry);

    if(!ldap_mod_replace($linkid, $dn, $entry)) {
		pql_format_error(1);
		die($LDIF);
		return false;
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
