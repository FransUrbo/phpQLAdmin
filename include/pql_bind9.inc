<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.27 2004-11-05 13:34:46 turbo Exp $
//

// {{{ pql_bind9_get_zone(linkid, branchname, domainname)
function pql_bind9_get_zone($linkid, $branchname, $domainname = '') {
	$dn = "ou=DNS,".urldecode($branchname);

	if(!empty($domainname))
	  $filter = "(&(zoneName=$domainname)(objectClass=dNSZone))";
	else
	  $filter = 'objectClass=dNSZone';

	$objects = pql_search($linkid, $dn, $filter);
	for($i=0; $objects[$i]['zonename']; $i++) {
		$zone = $objects[$i]['zonename'];
		$host = $objects[$i]['relativedomainname'];
		
		$ZONE[$zone][$host]['RDN']  = pql_maybe_idna_decode($objects[$i]['dn']);
		$ZONE[$zone][$host]['ZONE'] = pql_maybe_idna_decode($zone);
		$ZONE[$zone][$host]['TTL']  = $objects[$i]['dnsttl'];

		if(eregi("in-addr.arpa", $zone) and ($host != '@')) {
			$tmp = preg_replace('/.in-addr.arpa/', '', $zone);
			$tmp = split('\.', $tmp);
			
			unset($PTR);
			for($j=count($tmp)-1; $tmp[$j]; $j--)
			  $PTR .= $tmp[$j].'.';
			
			$ZONE[$zone][$host]['PTR'] = $PTR.$host;
		} else
		  $ZONE[$zone][$host]['HOST'] = pql_maybe_idna_decode($host);
		
		// A Record(s)
		if($objects[$i]['arecord']) {
			if(is_array($objects[$i]['arecord'])) {
				foreach($objects[$i]['arecord'] as $record)
				  $ZONE[$zone][$host]['A'][] = pql_maybe_idna_decode($record);
			} else
			  $ZONE[$zone][$host]['A'] = pql_maybe_idna_decode($objects[$i]['arecord']);
		}
		
		// CNAME Record
		if($objects[$i]['cnamerecord']) {
			if(is_array($objects[$i]['cnamerecord'])) {
				foreach($objects[$i]['cnamerecord'] as $record) {
					$cname = pql_maybe_idna_decode($record);
					
					if(preg_match("/\\.$/", $cname))
					  // Remove the domain name, so we're left with the hostname.
					  $cname = str_replace(".$zone.", "", $cname);
					
					$ZONE[$zone][$host]['CNAME'] = $cname;
				}
			} else {
				$cname = pql_maybe_idna_decode($objects[$i]['cnamerecord']);
				
				if(preg_match("/\\.$/", $cname))
				  // Remove the domain name, so we're left with the hostname.
				  $cname = str_replace(".$zone.", "", $cname);
				
				$ZONE[$zone][$host]['CNAME'] = $cname;
			}
		}
		
		// HINFO Record
		if($objects[$i]['hinforecord'])
		  $ZONE[$zone][$host]['HINFO'] = $objects[$i]['hinforecord'];
		
		// SRV Record
		if($objects[$i]['srvrecord']) {
			if(is_array($objects[$i]['srvrecord'])) {
				foreach($objects[$i]['srvrecord'] as $record)
				  $ZONE[$zone][$host]['SRV'] = $record;
			} else
			  $ZONE[$zone][$host]['SRV'] = $objects[$i]['srvrecord'];
		}
		
		// TXT Record
		if($objects[$i]['txtrecord'])
		  $ZONE[$zone][$host]['TXT'] = $objects[$i]['txtrecord'];
		
		// NS Record(s)
		if($objects[$i]['nsrecord']) {
			if(is_array($objects[$i]['nsrecord'])) {
				// We have multiple records
				foreach($objects[$i]['nsrecord'] as $record)
				  $ZONE[$zone][$host]['NS'][] = pql_maybe_idna_decode($record);
			} else
			  $ZONE[$zone][$host]['NS'] = pql_maybe_idna_decode($objects[$i]['nsrecord']);
		}
		
		// MX Record(s)
		if($objects[$i]['mxrecord']) {
			if(is_array($objects[$i]['mxrecord'])) {
				// We have multiple records
				foreach($objects[$i]['mxrecord'] as $record)
				  $ZONE[$zone][$host]['MX'][] = pql_maybe_idna_decode($record);
			} else
			  $ZONE[$zone][$host]['MX'] = pql_maybe_idna_decode($objects[$i]['mxrecord']);
		}
		
		// SOA Record
		if($objects[$i]['soarecord']) {
			$soa = split(' ', $objects[$i]['soarecord']);

			$tmp   = split('\.', $soa[1]);
			$admin = $tmp[0]."@";
			for($j=1; $tmp[$j]; $j++) {
				$admin .= $tmp[$j];
				if($tmp[$j+1])
				  $admin .= ".";
			}
			$ZONE[$zone][$host]['SOA']['ADMIN']   = pql_maybe_idna_decode($admin);

			$ZONE[$zone][$host]['SOA']['SERIAL']  = $soa[2];
			$ZONE[$zone][$host]['SOA']['REFRESH'] = $soa[3];
			$ZONE[$zone][$host]['SOA']['RETRY']   = $soa[4];
			$ZONE[$zone][$host]['SOA']['EXPIRE']  = $soa[5];
			$ZONE[$zone][$host]['SOA']['TTL']     = $soa[6];
		}
		
		// Sort the host array 
		ksort($ZONE[$zone]);
	}
	
	// Sort the zone array
	if(is_array($ZONE)) {
		ksort($ZONE);
		return($ZONE);
	}
}
// }}}


// {{{ pql_bind9_add_zone(linkid, dn, zone)
function pql_bind9_add_zone($linkid, $dn, $zone) {
	$MASTER_DN = urldecode($dn);

	// ----------------
	// See if the sub branch exists. If not, create it.
	$filter = "(&(ou=DNS)(objectclass=organizationalUnit))";

    if(!pql_get_dn($linkid, $_REQUEST["domain"], $filter, 'ONELEVEL')) {
		// Does not exists - create it
		if(!pql_unit_add($linkid, $MASTER_DN, 'ou=DNS'))
		  die("Could not create 'ou=DNS,$MASTER_DN'");
	}
	$dn = "ou=DNS,$MASTER_DN";
	$DNs[] = $dn;

	// ----------------
	// Create each of the domain components in the domainname
	$dc = split('\.', $zone);

	unset($entry);
	$entry['objectclass'][] = 'top';
	$entry['objectclass'][] = 'domain';

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"]) 
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'dnszone');

	for($i=count($dc)-1; $dc[$i]; $i--) {
		$dc_tmp = pql_maybe_idna_encode($dc[$i]);

		$entry[pql_get_define("PQL_ATTR_DC")] = $dc_tmp;
		if($dn)
		  $dn = pql_get_define("PQL_ATTR_DC")."=".$dc_tmp.",".$dn;
		else
		  $dn = pql_get_define("PQL_ATTR_DC")."=".$dc_tmp;

		// See if the component exists. If not, create it.
		if(!pql_get_dn($linkid, $dn, 'objectClass=domain', 'BASE')) {
			// Doesn't already exists. Create a LDIF object to print in case of error
			$LDIF = pql_create_ldif("pql_bind9_add_zone - dc=".$dc_tmp, $dn, $entry);
			if(file_exists("./.DEBUG_ME"))
			  echo $LDIF;
			else {
				if(!ldap_add($linkid, $dn, $entry)) {
					// Can't add domain component
					pql_format_error(1);
					echo "Can't add domain component...<br>\n";
					die($LDIF);
				} else
				  $DNs[] = $dn;
			}
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
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'dnszone');

	$dn = "relativeDomainName=".$entry['relativeDomainName'].",$dn";

	if(!pql_get_dn($linkid, $dn, 'objectClass=dNSZone', 'BASE')) {
		// Doesn't already exists. Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("pql_bind9_add_zone - @", $dn, $entry);
		if(file_exists("./.DEBUG_ME"))
		  echo $LDIF;
		else {
			if(!ldap_add($linkid, $dn, $entry)) {
				// Can't add SOA record
				pql_format_error(1);
				echo "Can't add the 'relativeDomainName=@' entry...<br>\n";
				die($LDIF);
			} else
			  $DNs[] = $dn;
		}
	}
	
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
	if($_SESSION["ACI_SUPPORT_ENABLED"]) 
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'dnszone');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_bind9_add_host", $dn, $entry);
	if(file_exists("./.DEBUG_ME")) {
		echo $LDIF;
	} else {
		if(!ldap_add($linkid, $dn, $entry)) {
			pql_format_error(1);
			die($LDIF);
			return false;
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
