<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.34 2005-04-19 04:44:32 turbo Exp $
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

		// PTR Record
		if($objects[$i]['ptrrecord'])
		  $ZONE[$zone][$host]['HOST'] = $objects[$i]['ptrrecord'];

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

// {{{ pql_bind9_get_acis(linkid, zone)
// This function will retreive the ACI's from the SOA object
// and return them unmodified.
function pql_bind9_get_acis($linkid, $zone) {
  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$zone)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
  foreach($_SESSION["BASE_DN"] as $base) {
	$object = pql_search($linkid, $base, $filter, 'SUBTREE', 1, 1);
	if(is_array($object[0]))
	  return $object[0][pql_get_define("PQL_ATTR_LDAPACI")];
  }

  return false;
}
// }}}

// {{{ pql_bind9_get_ttl(linkid, zone)
// This function will retreive the default TTL from the SOA object.
function pql_bind9_get_ttl($linkid, $zone) {
  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$zone)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
  foreach($_SESSION["BASE_DN"] as $base) {
	$object = pql_search($linkid, $base, $filter);
	if(is_array($object[0]))
	  return $object[0]['dnsttl'];
  }

  return false;
}
// }}}


// {{{ pql_bind9_add_zone(linkid, dn, zone)
function pql_bind9_add_zone($linkid, $dn, $zone) {
	$MASTER_DN = pql_maybe_encode(urldecode($dn));

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
		  if(!pql_write_add($linkid, $dn, $entry, 'objectclass', 'pql_bind9_add_zone')) {
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
	$date     = date("Ymd01", time());

	unset($entry);
	$entry['objectClass'][]		 = 'top';
	$entry['objectClass'][]		 = 'dNSZone';

	$entry['relativeDomainName'] = '@';
	$entry['zoneName']			 = $zonename;
	$entry['dNSTTL']			 = 604800;

	$entry['sOARecord']			 = "$zonename. registry.$zonename. $date 172800 86400 2419200 604800";
	$entry['nSRecord'][]		 = "ns1.$zonename.";
	$entry['nSRecord'][]		 = "ns2.$zonename.";
	$entry['mXRecord']			 = "10 mail.$zonename.";
	$entry['tXTRecord']			 = "$zonename";

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'dnszone');

	$dn = "relativeDomainName=".$entry['relativeDomainName'].",$dn";

	if(!pql_get_dn($linkid, $dn, 'objectClass=dNSZone', 'BASE')) {
	  if(!pql_write_add($linkid, $dn, $entry, 'bind9/zone', 'pql_bind9_add_zone')) {
		// Can't add SOA record
		pql_format_error(1);
		echo "Can't add the 'relativeDomainName=@' entry...<br>\n";
		die($LDIF);
	  } else
		$DNs[] = $dn;
	}
	
	return true;
}
// }}}

// {{{ pql_bind9_add_host(linkid, basedn, entry)
function pql_bind9_add_host($linkid, $basedn, $entry) {
	$basedn = pql_maybe_encode(urldecode($basedn));

	// {{{ Create each of the domain components in the domainname
	$dc = split('\.', $entry[pql_get_define("PQL_ATTR_ZONENAME")]);
	for($i=count($dc)-1; $dc[$i]; $i--) {
		$dc_tmp = pql_maybe_idna_encode($dc[$i]);

		if($dn)
		  $dn = "dc=".$dc_tmp.",".$dn;
		else
		  $dn = "dc=".$dc_tmp;
	}
	$dn = "relativeDomainName=".$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")].",$dn,ou=DNS,$basedn";
	// }}}

	$entry['objectClass'][] = 'top';
	$entry['objectClass'][] = 'dNSZone';

	// {{{ Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"]) 
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = pql_bind9_get_acis($linkid, $entry[pql_get_define("PQL_ATTR_ZONENAME")]);
																	  
	// }}}

	// {{{ Add the object to the database
	if(!pql_write_add($linkid, $dn, $entry, 'bind9/host', 'pql_bind9_add_host')) {
	  pql_format_error(1);
	  die($LDIF);
	  return false;
	}
	// }}}

	return $dn;
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
			if(!pql_write_del($linkid, $info[$i]["dn"]))
			  pql_format_error(1);
		}
	}

	// Delete the zone itself
	if(!pql_write_del($linkid, $dn_to_remove)) {
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
		if(!pql_write_del($linkid, $tld)) {
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
		if(!pql_write_del($linkid, $ou)) {
			pql_format_error(1);
			return false;
		}
	}

	return true;
}
// }}}

// {{{ pql_bind9_update_serial(linkid, dn)
function pql_bind9_update_serial($linkid, $dn) {
  // Get the zone name from the record (so that we can find the SOA)
  $zone = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_ZONENAME"));

  // Get the SOA for this zone.
  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$zone)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
  foreach($_SESSION["BASE_DN"] as $base) {
	$object = pql_search($linkid, $base, $filter);
	if(is_array($object[0])) {
	  // "there can be only one" - anything else should (?) be an error.
	  $soa["dn"]  = $object[0]["dn"];
	  $soa["soa"] = $object[0][lc(pql_get_define("PQL_ATTR_SOARECORD"))];
	}
  }

  // Get the old serial number from the SOA
  // sOARecord: phpqladmin.com. registry.phpqladmin.com. 2005013001 172800 86400 2419200 604800
  $tmp = split(' ', $soa["soa"]); $serial = $tmp[2];

  // Extract date and serial number from the SOA serial number.
  // This _SHOULD_ be in the form 'YYYMMDDxx'.
  for($i=0; $i <= 7; $i++)
	$serial_date .= $serial[$i];
  $serial_number = $serial[8].$serial[9];

  // Get current date.
  $date = date("Ymd", time());

  // Let's see how the new value should look like.
  if($serial_date == $date) {
	// We've change the zone 'today'. Update the serial number
	$serial_number = sprintf("%02d", ++$serial_number);
  } else {
	// Not changed 'today'. Use todays date with '01' as serial number
	$serial_number = "01";
  }

  // Create the new SOA value from the new serial number added to the current date.
  // sOARecord: phpqladmin.com. registry.phpqladmin.com. 2005013002 172800 86400 2419200 604800
  $SOA = $tmp[0]." ".$tmp[1]." $date$serial_number ".$tmp[3]." ".$tmp[4]." ".$tmp[5]." ".$tmp[6];

  // Update the SOA with this new SOA serial number.
  if(!pql_modify_attribute($linkid, $soa["dn"], pql_get_define("PQL_ATTR_SOARECORD"), 1, $SOA))
	return false;
  else
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
