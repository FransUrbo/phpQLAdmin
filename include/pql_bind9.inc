<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.3 2003-05-20 10:37:55 turbo Exp $
//

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
			if($info[$i]['soarecord'][0])
			  $ZONE[$zone][$host]['SOA'] = $info[$i]['soarecord'][0];

			// Sort the host array 
			ksort($ZONE[$zone]);
		}
		
		// Sort the zone array
		ksort($ZONE);

		return($ZONE);
    }
}

function pql_bind9_add_zone($linkid, $dn, $zone) {
	$MASTER_DN = $dn;
	$dn = 'ou=DNS,'.$dn;

	// ----------------
	// See if the sub branch exists. If not, create it.
    $sr = @ldap_read($linkid, $dn, 'objectClass=dNSZone');
    if(!$sr) {
		$entry['objectclass'][] = 'top';
		$entry['objectclass'][] = 'organizationalUnit';
		$entry['ou'] = 'DNS';

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_bind9_add_zone - ou", $dn, $entry);

		if(! ldap_add($linkid, $dn, $entry)) {
			// Can't add the organizational unit
			pql_errormsg($linkid);
			return false;
		} else
		  $DNs[] = $dn;
	}

	// ----------------
	// Create each of the domain components in the domainname
	$dc = split('\.', $zone); unset($entry); $entry['objectclass'] = 'top';
	for($i=count($dc)-1; $dc[$i]; $i--) {
		if($dn)
		  $dn = "dc=".$dc[$i].",".$dn;
		else
		  $dn = "dc=".$dc[$i];

		if(! ldap_add($linkid, $dn, $entry)) {
			// Can't add domain component
			pql_errormsg($linkid);
			
			// Delete $DNs[]
			return false;
		} else
		  $DNs[] = $dn;
	}

	// ----------------
	// Create a template SOA record
	unset($entry);
	$entry['objectClass'][]		 = 'top';
	$entry['objectClass'][]		 = 'dNSZone';
	$entry['relativeDomainName'] = '@';
	$entry['sOARecord']			 = "$zone. registry.$zone. 2002082106 172800 86400 2419200 604800";
	$entry['dNSTTL']			 = 604800;
	$entry['zoneName']			 = $zone;
	$entry['nSRecord']			 = "ns1.$zone.";
	$entry['mXRecord']			 = "10 mail.$zone.";
	$entry['tXTRecord']			 = "$zone";
	$dn = "relativeDomainName=@ + dNSTTL=".$entry['dNSTTL'].",$dn";

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_zone - @", $dn, $entry);
	if(! ldap_add($linkid, $dn, $entry)) {
		// Can't add SOA record
		pql_errormsg($linkid);
		
		// Delete $DNs[]
		return false;
	} else
	  $DNs[] = $dn;

	return true;
}

function pql_bind9_add_host($linkid, $basedn, $entry) {
	// Create each of the domain components in the domainname
	$dc = split('\.', $entry["zoneName"]);
	for($i=count($dc)-1; $dc[$i]; $i--) {
		if($dn)
		  $dn = "dc=".$dc[$i].",".$dn;
		else
		  $dn = "dc=".$dc[$i];
	}
	$dn = "relativeDomainName=".$entry["relativeDomainName"].",$dn,ou=DNS,$basedn";

	$entry['objectClass'][] = 'top';
	$entry['objectClass'][] = 'dNSZone';

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_host", $dn, $entry);
	if(! ldap_add($linkid, $dn, $entry)) {
		// Can't add SOA record
		pql_errormsg($linkid);
		
		die($LDIF);
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
