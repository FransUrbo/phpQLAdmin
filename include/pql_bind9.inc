<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.9 2003-11-11 07:24:42 turbo Exp $
//

// {{{ pql_bind9_get_zone(linkid, dn)
function pql_bind9_get_zone($linkid, $dn) {
    $sr = @ldap_search($linkid, $dn, 'objectClass=dNSZone');
    if($sr) {
		// Oki, we have a DNS zone in LDAP.
		$info = ldap_get_entries($linkid, $sr)
		  or pql_errormsg($linkid);
		for($i=0; $i < $info["count"]; $i++) {
			$zone = $info[$i]['zonename'][0];
			$host = $info[$i]['relativedomainname'][0];
				
			$ZONE[$zone][$host]['RDN']  = $info[$i]['dn'];
			$ZONE[$zone][$host]['ZONE'] = $zone;
			$ZONE[$zone][$host]['HOST'] = $host;
			$ZONE[$zone][$host]['TTL']  = $info[$i]['dnsttl'][0];
			
			// A Record(s)
			if($info[$i]['arecord'][0]) {
				if($info[$i]['arecord'][1]) {
					// We have multiple A records - Round-Robin!
					for($j=0; $j < $info[$i]['arecord']['count']; $j++)
					  $ZONE[$zone][$host]['A'][] = $info[$i]['arecord'][$j];
				} else
				  $ZONE[$zone][$host]['A'] = $info[$i]['arecord'][0];
			}
			
			// CNAME Record
			if($info[$i]['cnamerecord'][0]) {
				$cname = $info[$i]['cnamerecord'][0];
				
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
					  $ZONE[$zone][$host]['NS'][] = $info[$i]['nsrecord'][$j];
				} else
				  $ZONE[$zone][$host]['NS'] = $info[$i]['nsrecord'][0];
			}

			// MX Record(s)
			if($info[$i]['mxrecord'][0]) {
				if($info[$i]['mxrecord'][1]) {
					// We have multiple records
					for($j=0; $j < $info[$i]['mxrecord']['count']; $j++)
					  $ZONE[$zone][$host]['MX'][] = $info[$i]['mxrecord'][$j];
				} else
				  $ZONE[$zone][$host]['MX'] = $info[$i]['mxrecord'][0];
			}

			// SOA Record
			if($info[$i]['soarecord'][0]) {
				$soa = split(' ', $info[$i]['soarecord'][0]);

				$ZONE[$zone][$host]['SOA']['ADMIN'] = $soa[1];
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
		ksort($ZONE);

		return($ZONE);
    }
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

		$sr = @ldap_list($linkid, $dn, $filter, $attrs) or pql_errormsg($linkid);
	} else {
		$filter .= ")";
		$sr = @ldap_read($linkid, $dn, $filter, $attrs) or pql_errormsg($linkid);
	}

	$info = @ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);

	if($info[0][$record]["count"] == 1)
	  $ret[] = $info[0][$record][0];
	else {
		for($i=0; $i < $info[0][$record]["count"]; $i++)
		  $ret[] = $info[0][$record][$i];
	}

	return($ret);
}
// }}}


// {{{ pql_bind9_add_zone(linkid, dn, zone)
function pql_bind9_add_zone($linkid, $dn, $zone) {
	$MASTER_DN = $dn;
	$dn = 'ou=DNS,'.$dn;

	// ----------------
	// See if the sub branch exists. If not, create it.
    $sr = @ldap_read($linkid, $dn, 'objectClass=organizationalUnit');
    if(!$sr) {
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'DNS';

		// Add the OpenLDAPaci attribute (maybe)
		if(function_exists("user_generate_aci"))
		  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid,
																				  $GLOBALS["USER_DN"],
																				  'dnszone');

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_bind9_add_zone - ou", $dn, $entry);

		if(! ldap_add($linkid, $dn, $entry)) {
			// Can't add the organizational unit
			pql_errormsg($linkid);
			echo "Can't add the organizational unit...<br>\n";
			die($LDIF);
		} else
		  $DNs[] = $dn;
	}

	// ----------------
	// Create each of the domain components in the domainname
	$dc = split('\.', $zone);

	unset($entry);
	$entry['objectclass'][] = 'top';
	$entry['objectclass'][] = 'domain';

	// Add the OpenLDAPaci attribute (maybe)
	if(function_exists("user_generate_aci")) 
	  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid,
																			  $GLOBALS["USER_DN"],
																			  'dnszone');

	for($i=count($dc)-1; $dc[$i]; $i--) {
		$entry[pql_get_define("PQL_GLOB_ATTR_DC")] = $dc[$i];

		if($dn)
		  $dn = pql_get_define("PQL_GLOB_ATTR_DC")."=".$dc[$i].",".$dn;
		else
		  $dn = pql_get_define("PQL_GLOB_ATTR_DC")."=".$dc[$i];

		// See if the component exists. If not, create it.
		$sr = @ldap_read($linkid, $dn, 'objectClass=domain');
		if(!$sr) {
			// Create a LDIF object to print in case of error
			$LDIF = pql_create_ldif("pql_bind9_add_zone - dc=".$dc[$i], $dn, $entry);
			
			if(! ldap_add($linkid, $dn, $entry)) {
				// Can't add domain component
				pql_errormsg($linkid);
				echo "Can't add domain component...<br>\n";
				die($LDIF);
			} else
			  $DNs[] = $dn;
		}
	}

	// ----------------
	// Create a template SOA record
	unset($entry);
	$entry['objectClass'][]		 = 'top';
	$entry['objectClass'][]		 = 'dNSZone';

	$entry['relativeDomainName'] = '@';
	$entry['zoneName']			 = $zone;
	$entry['dNSTTL']			 = 604800;

	$entry['sOARecord']			 = "$zone. registry.$zone. 2002082106 172800 86400 2419200 604800";
	$entry['nSRecord'][]		 = "ns1.$zone.";
	$entry['nSRecord'][]		 = "ns2.$zone.";
	$entry['mXRecord']			 = "10 mail.$zone.";
	$entry['tXTRecord']			 = "$zone";

	// Add the OpenLDAPaci attribute (maybe)
	if(function_exists("user_generate_aci"))
	  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($ldap_linkid,
																			  $GLOBALS["USER_DN"],
																			  'dnszone');

	$dn = "relativeDomainName=".$entry['relativeDomainName'].",$dn";

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_zone - @", $dn, $entry);

	if(! ldap_add($linkid, $dn, $entry)) {
		// Can't add SOA record
		pql_errormsg($linkid);
		echo "Can't add the 'relativeDomainName=@' entry...<br>\n";
		die($LDIF);
	} else
	  $DNs[] = $dn;

	return true;
}
// }}}

// {{{ pql_bind9_add_host(linkid, basedn, entry)
function pql_bind9_add_host($linkid, $basedn, $entry) {
	// Create each of the domain components in the domainname
	$dc = split('\.', $entry[pql_get_define("PQL_GLOB_ATTR_ZONENAME")]);
	for($i=count($dc)-1; $dc[$i]; $i--) {
		if($dn)
		  $dn = "dc=".$dc[$i].",".$dn;
		else
		  $dn = "dc=".$dc[$i];
	}
	$dn = "relativeDomainName=".$entry[pql_get_define("PQL_GLOB_ATTR_RELATIVEDOMAINNAME")].",$dn,ou=DNS,$basedn";

	$entry['objectClass'][] = 'top';
	$entry['objectClass'][] = 'dNSZone';

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_host", $dn, $entry);

	if(! ldap_add($linkid, $dn, $entry)) {
		pql_errormsg($linkid);
		die($LDIF);
		return false;
	}

	return true;
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
		pql_errormsg($linkid);
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
