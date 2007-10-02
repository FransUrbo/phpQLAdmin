<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.55 2007-10-02 06:17:23 turbo Exp $
//

// {{{ pql_bind9_get_zone(branchname, domainname)
function pql_bind9_get_zone($branchname, $domainname = '') {
  global $_pql;
  
  $dn = pql_get_define("PQL_CONF_SUBTREE_BIND9").",".urldecode($branchname);
  
  if(!empty($domainname))
	$filter = "(&(zoneName=$domainname)(objectClass=dNSZone))";
  else
	$filter = 'objectClass=dNSZone';
  
  $objects = $_pql->search($dn, $filter);
  
  // Catch a flat object - i.e. only ONE domain
  if(!@$objects[0] and @$objects["dn"]) {
	$tmp = $objects;
	unset($objects);
	$objects[0] = $tmp;
  }
  
  for($i=0; @$objects[$i]['zonename']; $i++) {
	$zone = $objects[$i]['zonename'];
	if($objects[$i]['relativedomainname'][0])
	  $host = $objects[$i]['relativedomainname'][0];
	else 
	$host = $objects[$i]['relativedomainname'];
	
	$ZONE[$zone][$host]['RDN']  = pql_maybe_idna_decode($objects[$i]['dn']);
	$ZONE[$zone][$host]['ZONE'] = pql_maybe_idna_decode($zone);
	$ZONE[$zone][$host]['TTL']  = $objects[$i]['dnsttl'];
	
	if(eregi("in-addr.arpa", $zone) and ($host != '@')) {
	  $tmp = preg_replace('/.in-addr.arpa/', '', $zone);
	  $tmp = split('\.', $tmp);
	  
	  unset($PTR);
	  for($j=count($tmp)-1; !empty($tmp[$j]); $j--)
		$PTR .= $tmp[$j].'.';
	  
	  $ZONE[$zone][$host]['PTR'] = $PTR.$host;
	} else
	  $ZONE[$zone][$host]['HOST'] = pql_maybe_idna_decode($host);
	
	// {{{ A Record(s)
	if(@$objects[$i]['arecord']) {
	  if(is_array($objects[$i]['arecord'])) {
		$ZONE[$zone][$host]['A'] = array();
		foreach($objects[$i]['arecord'] as $record)
		  $ZONE[$zone][$host]['A'][] = pql_maybe_idna_decode($record);
	  } else
		$ZONE[$zone][$host]['A'] = pql_maybe_idna_decode($objects[$i]['arecord']);
	}
	// }}}
	
	// {{{ CNAME Record
	if(@$objects[$i]['cnamerecord']) {
	  if(is_array($objects[$i]['cnamerecord'])) {
		foreach($objects[$i]['cnamerecord'] as $record) {
		  $cname = pql_maybe_idna_decode($record);
		  
		  if(preg_match("/\\.$/", $cname))
			// Remove the domain name, so we're left with the hostname.
			$cname = str_replace(".$zone.", "", $cname);
		  
		  $ZONE[$zone][$host]['CNAME'][] = $cname;
		}
	  } else {
		$cname = pql_maybe_idna_decode($objects[$i]['cnamerecord']);
		
		if(preg_match("/\\.$/", $cname))
		  // Remove the domain name, so we're left with the hostname.
		  $cname = str_replace(".$zone.", "", $cname);
		
		$ZONE[$zone][$host]['CNAME'] = $cname;
	  }
	}
	// }}}
	
	// {{{ HINFO Record
	if(@$objects[$i]['hinforecord'])
	  $ZONE[$zone][$host]['HINFO'] = $objects[$i]['hinforecord'];
	// }}}
	
	// {{{ SRV Record
	if(@$objects[$i]['srvrecord']) {
	  if(is_array($objects[$i]['srvrecord'])) {
		$ZONE[$zone][$host]['SRV'] = array();
		foreach($objects[$i]['srvrecord'] as $record)
		  $ZONE[$zone][$host]['SRV'][] = $record;
	  } else
		$ZONE[$zone][$host]['SRV'] = $objects[$i]['srvrecord'];
	}
	// }}}
	
	// {{{ TXT Record
	if(@$objects[$i]['txtrecord'])
	  $ZONE[$zone][$host]['TXT'] = $objects[$i]['txtrecord'];
	// }}}
	
	// {{{ NS Record(s)
	if(@$objects[$i]['nsrecord']) {
	  if(is_array($objects[$i]['nsrecord'])) {
		// We have multiple records
		foreach($objects[$i]['nsrecord'] as $record)
		  $ZONE[$zone][$host]['NS'][] = pql_maybe_idna_decode($record);
	  } else
		$ZONE[$zone][$host]['NS'] = pql_maybe_idna_decode($objects[$i]['nsrecord']);
	}
	// }}}
	
	// {{{ MX Record(s)
	if(@$objects[$i]['mxrecord']) {
	  if(is_array($objects[$i]['mxrecord'])) {
		// We have multiple records
		foreach($objects[$i]['mxrecord'] as $record)
		  $ZONE[$zone][$host]['MX'][] = pql_maybe_idna_decode($record);
	  } else
		$ZONE[$zone][$host]['MX'] = pql_maybe_idna_decode($objects[$i]['mxrecord']);
	}
	// }}}
	
	// {{{ PTR Record
	if(@$objects[$i]['ptrrecord'])
	  $ZONE[$zone][$host]['HOST'] = $objects[$i]['ptrrecord'];
	// }}}
	
	// {{{ SOA Record
	if(@$objects[$i]['soarecord']) {
	  $soa = split(' ', $objects[$i]['soarecord']);
	  
	  $tmp   = split('\.', $soa[1]);
	  $admin = $tmp[0]."@";
	  for($j=1; $j < count($tmp); $j++) {
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
	// }}}
	
	// {{{ AFS Record
	if(@$objects[$i]['afsdbrecord']) {
	  if(is_array($objects[$i]['afsdbrecord'])) {
		// We have multiple records
		foreach($objects[$i]['afsdbrecord'] as $record)
		  $ZONE[$zone][$host]['AFSDB'][] = pql_maybe_idna_decode($record);
	  } else
		$ZONE[$zone][$host]['AFSDB'] = pql_maybe_idna_decode($objects[$i]['afsdbrecord']);
	}
	// }}}
	
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

// {{{ pql_bind9_get_acis(zone)
// This function will retreive the ACI's from the SOA object
// and return them unmodified.
function pql_bind9_get_acis($zone) {
  global $_pql;
  
  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$zone)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
  foreach($_SESSION["BASE_DN"] as $base) {
	// OpenLDAP uses 'OpenLDAPaci', but everyone else uses 'aci'...
	// TODO: This is possible to specify in slapd.conf. Try to detect this.
	$object = $_pql->search($base, $filter, 'SUBTREE', 1, array(pql_get_define("PQL_ATTR_LDAPACI"), 'aci'));
	if(@is_array($object[0]))
	  return $object[0][pql_get_define("PQL_ATTR_LDAPACI")];
	elseif($object["dn"])
	  // Flat object
	  return $object[pql_get_define("PQL_ATTR_LDAPACI")];
  }
  
  return false;
}
// }}}

// {{{ pql_bind9_get_ttl(linkid, zone)
// This function will retreive the default TTL from the SOA object.
function pql_bind9_get_ttl($linkid, $zone) {
  global $_pql;
  
  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$zone)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
  foreach($_SESSION["BASE_DN"] as $base) {
	$object = $_pql->search($base, $filter);
	if($object && empty($object[0])) {
	  $tmp = $object; unset($object);
	  $object[0] = $tmp; unset($tmp);
	}
	
	if(@is_array($object[0]))
	  return $object[0]['dnsttl'];
  }
  
  return false;
}
// }}}


// {{{ pql_bind9_add_zone(dn, zone)
function pql_bind9_add_zone($dn, $zone) {
  global $_pql;
  
  $MASTER_DN = pql_maybe_encode(urldecode($dn));
  
  // {{{ See if the sub branch exists. If not, create it.
  $filter = "(&(".pql_get_define("PQL_CONF_SUBTREE_BIND9").")(objectclass=organizationalUnit))";
  
  if(!$_pql->get_dn($_REQUEST["domain"], $filter, 'ONELEVEL')) {
	// Does not exists - create it
	if(!pql_unit_add($MASTER_DN, pql_get_define("PQL_CONF_SUBTREE_BIND9")))
	  die("Could not create ".pql_get_define("PQL_CONF_SUBTREE_BIND9")."',$MASTER_DN'");
  }
  $dn = pql_get_define("PQL_CONF_SUBTREE_BIND9").",$MASTER_DN";
  $DNs[] = $dn;
  // }}}
  
  // Create each of the domain components in the domainname
  $dc = split('\.', $zone);
  
  unset($entry);
  $entry['objectclass'][] = 'top';
  $entry['objectclass'][] = 'domain';

  // {{{ Create each and every (leading) part of the domain
  for($i=count($dc)-1; !empty($dc[$i]); $i--) {
	$dc_tmp = pql_maybe_idna_encode($dc[$i]);
	
	$entry[pql_get_define("PQL_ATTR_DC")] = $dc_tmp;
	if($dn)
	  $dn = pql_get_define("PQL_ATTR_DC")."=".$dc_tmp.",".$dn;
	else
	  $dn = pql_get_define("PQL_ATTR_DC")."=".$dc_tmp;
	
	// See if the component exists. If not, create it.
	if(!$_pql->get_dn($dn, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE')) {
	  if(!$_pql->add($dn, $entry, 'bind9/zone', 'pql_bind9_add_zone'))
		// Can't add domain component
		die();
	  else
		$DNs[] = $dn;
	}
  }
  // }}}
  
  // {{{ Create a template SOA record
  $zonename = pql_maybe_idna_encode($zone);
  $date     = date("Ymd01", time());
  
  unset($entry);
  $entry['objectClass'][]			= 'top';
  $entry['objectClass'][]			= 'dNSZone';
  
  $entry['relativeDomainName']	= '@';
  $entry['zoneName']				= $zonename;
  $entry['dNSTTL']				= 604800; // Seven days default.
  
  $entry['sOARecord']				= "$zonename. registry.$zonename. $date 172800 86400 2419200 604800";
  $entry['nSRecord'][]			= "ns1.$zonename.";
  $entry['nSRecord'][]			= "ns2.$zonename.";
  $entry['mXRecord']				= "10 mail.$zonename.";
  $entry['tXTRecord']				= "$zonename";
  // }}}
  
  // {{{ Add the OpenLDAPaci attribute (maybe)
  if(@$_SESSION["ACI_SUPPORT_ENABLED"])
	$entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($_SESSION["USER_DN"], 'dnszone');
  // }}}
  
  $dn = "dNSTTL=".$entry['dNSTTL']."+relativeDomainName=".$entry['relativeDomainName'].",$dn";
  
  if(!pql_get_dn($linkid, $dn, 'objectClass=dNSZone', 'BASE')) {
	if(!$_pql->add($dn, $entry, 'bind9/zone', 'pql_bind9_add_zone')) {
	  // Can't add SOA record
	  die();
	} else {
	  $DNs[] = $dn;
	}
  }
  
  return true;
}
// }}}

// {{{ pql_bind9_add_host(basedn, entry)
function pql_bind9_add_host($basedn, $entry) {
  global $_pql;
  
  $basedn = pql_maybe_encode(urldecode($basedn));
  
  // {{{ Create each of the domain components in the domainname
  $dc = split('\.', $entry[pql_get_define("PQL_ATTR_ZONENAME")]);
  for($i=count($dc)-1; !empty($dc[$i]); $i--) {
	$dc_tmp = pql_maybe_idna_encode($dc[$i]);
	
	if($dn)
	  $dn = "dc=".$dc_tmp.",".$dn;
	else
	  $dn = "dc=".$dc_tmp;
  }
  
  // Remember the hostname. Need it when/if a sub-domain 'host' is added...
  $tmp = $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")];
  
  if(ereg('.', $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]) and
	 !eregi('in-addr', $entry[pql_get_define("PQL_ATTR_ZONENAME")]))
  {
	// {{{ This is a sub-zone!
	// Create something like this:
	//	dn: relativeDomainName=test,dc=domain,dc=tld,PQL_CONF_SUBTREE_BIND9,o=Branch,c=SE
	//	dn: relativeDomainName=ns1.test,dc=domain,dc=tld,PQL_CONF_SUBTREE_BIND9,o=Branch,c=SE
	
	// Split it up in dc components and add each one as a separate dc object.
	$dc = split('\.', $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]);
	if(@$dc[1]) {
	  // dc[1] is unset the second time around...
	  
	  // Remember (and overwrite) the record type so it can be restored
	  // after adding the sub-domain.
	  $tmp_hostname = $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")];
	  if(@$entry[pql_get_define("PQL_ATTR_ARECORD")]) {
		$tmp_record = pql_get_define("PQL_ATTR_ARECORD");
		$tmp_value  = $entry[pql_get_define("PQL_ATTR_ARECORD")];
	  } elseif(@$entry[pql_get_define("PQL_ATTR_CNAMERECORD")]) {
		$tmp_record = pql_get_define("PQL_ATTR_CNAMERECORD");
		$tmp_value  = $entry[pql_get_define("PQL_ATTR_CNAMERECORD")];
	  } elseif(@$entry[pql_get_define("PQL_ATTR_MXRECORD")]) {
		$tmp_record = pql_get_define("PQL_ATTR_MXRECORD");
		$tmp_value  = $entry[pql_get_define("PQL_ATTR_MXRECORD")];
	  } elseif(@$entry[pql_get_define("PQL_ATTR_NSRECORD")]) {
		$tmp_record = pql_get_define("PQL_ATTR_NSRECORD");
		$tmp_value  = $entry[pql_get_define("PQL_ATTR_NSRECORD")];
	  }
	  
	  // Remove the original record type from the object.
	  unset($entry[$tmp_record]);
	  
	  // What type should the sub-domain be, CName or A record?
	  if(ereg("^[0-9].*[0-9]", $tmp_value)) {
		// Starts and ends with a number - best guess is that it's an IP address.
		$entry[pql_get_define("PQL_ATTR_ARECORD")] = $tmp_value;
		$record_to_remove = pql_get_define("PQL_ATTR_ARECORD");
	  } else {
		// Not an IP, must be a FQDN..
		$entry[pql_get_define("PQL_ATTR_CNAMERECORD")] = $tmp_value;
		$record_to_remove = pql_get_define("PQL_ATTR_CNAMERECORD");
	  }
	  
	  // Add the sub-domain 'host'.
	  $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")] = $dc[1];
	  pql_bind9_add_host($basedn, $entry);
	  
	  // Remove the sub-domain record and add the original back.
	  unset($entry[$record_to_remove]);
	  $entry[$tmp_record] = $tmp_value;
	  
	  // Restore the original hostname and record type.
	  $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")] = $tmp_hostname;
	}
	// }}}
  }
  
  $dn = "relativeDomainName=$tmp,$dn,".pql_get_define("PQL_CONF_SUBTREE_BIND9").",$basedn";
  // }}}
  
  if($_pql->get_dn($dn, "objectClass=*")) {
	// {{{ The object already exists - merge the new with the old...
	// Try to find out which attribute we're trying to add/modify
	$TYPES = array(pql_get_define("PQL_ATTR_ARECORD"),
				   pql_get_define("PQL_ATTR_CNAMERECORD"), 
				   pql_get_define("PQL_ATTR_HINFORECORD"),
				   pql_get_define("PQL_ATTR_MXRECORD"),
				   pql_get_define("PQL_ATTR_NSRECORD"),
				   pql_get_define("PQL_ATTR_PTRRECORD"),
				   pql_get_define("PQL_ATTR_TXTRECORD"));
	foreach($TYPES as $TYPE) {
	  $type = lc($TYPE);
	  if(@$entry[$type]) {
		// This one is defined. That's the attribute!
		$attrib = $type;
		break;
	  }
	}
	
	// Does the object exists (with this attribute)?
	$value_exists = 0; 
	if($attrs = $_pql->get_attribute($dn, $attrib)) {
	  // It DOES exists!
	  if(!is_array($attrs))
		// Not an array - make it one.
		$attrs = array($attrs);
	
	  // {{{ Go through the existing attributes, make sure that the new value don't already exist
	  for($i=0; $attrs[$i]; $i++) {
		if(is_array($entry[$attrib])) {
		  for($j=0; $entry[$attrib][$j]; $j++) {
			if($attrs[$i] == $entry[$attrib][$j]) {
			  $value_exists = 1;
			  break;
			}
		  }
		} else {
		  if($attrs[$i] == $entry[$attrib]) {
			$value_exists = 1;
			break;
		  }
		}
	  }
	  // }}}
	}
	
	if(!$value_exists) {
	  // {{{ Add the new attribute to the old list
	  $attrs[] = $entry[$attrib];
	  
	  // Recreate the LDIF
	  unset($entry);
	  $entry[$attrib] = $attrs;
	  
	  // Modify object
	  if(!$_pql->modify($dn, $entry, "pql_bind9_add_host"))
		die();
	  // }}}
	}
	
	return $dn;
	// }}}
  } else {
	// {{{ Object doesn't exists - create a new one
	$entry['objectClass'][] = 'top';
	$entry['objectClass'][] = 'dNSZone';
  
	// {{{ Add the OpenLDAPaci attribute (maybe)
	if(@$_SESSION["ACI_SUPPORT_ENABLED"]) {
	  $ACIs = pql_bind9_get_acis($entry[pql_get_define("PQL_ATTR_ZONENAME")]);
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = $ACIs;
	}
	// }}}
  
	// Add the object to the database
	if(!$_pql->add($dn, $entry, 'bind9/host', 'pql_bind9_add_host'))
	  die();
	
	return $dn;
	// }}}
  }
}
// }}}


// {{{ pql_bind9_del_zone(dn, zone)
function pql_bind9_del_zone($dn, $zone) {
  global $_pql;
  
  $dn = pql_format_normalize_dn($dn);
  $dnsparts = split('\.', $zone);
  for($i=0; $i < count($dnsparts); $i++) {
	$dn_to_remove .= "dc=".$dnsparts[$i].",";
  }
  $dn_to_remove .= pql_get_define("PQL_CONF_SUBTREE_BIND9").",".$dn;
  
  $lc_zone = lc($dn_to_remove);
  
  // Retreive ALL objects below this DN
  // TODO: Replace with '$_pql->search()'
  $sr = ldap_search($linkid, $dn_to_remove, 'objectClass=*');
  $info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
  for ($i=0; $i < $info["count"]; $i++) {
	$lc_dn = lc($info[$i]["dn"]);
	
	if($lc_dn != $lc_zone) {
	  // Don't delete the acctuall zone quite yet
	  if(!$_pql->delete($info[$i]["dn"]))
		return false;
	}
  }
  
  // Delete the zone itself
  if(!$_pql->delete($dn_to_remove))
	return false;
  
  // -------------------------------------------------------------
  // If this was the last zone in this TLD, remove the TLD as well
  
  // Get the TLD DN
  $dn_to_remove = pql_format_normalize_dn($dn_to_remove);
  $dnparts = split(',', $dn_to_remove);
  for($i=1; $i < count($dnparts); $i++) {
	$tld .= $dnparts[$i];
	if(@$dnparts[$i+1])
	  $tld .= ",";
  }
  
  // Look in this DN if there's objects.
  // TODO: Replace with '$_pql->search()'
  $sr = ldap_search($linkid, $tld, 'objectClass=*');
  $info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
  if(!@$info[1]) {
	// We don't have more than the DN object, delete it.
	if(!$_pql->delete($tld))
	  return false;
  }
  
  // -------------------------------------------------------------
  // If this was the last TLD in this branch, remove the organiztionalUnit
  // PQL_CONF_SUBTREE_BIND9 as well.
  
  // Get the ou DN
  $dnparts = split(',', $tld);
  for($i=1; $i < count($dnparts); $i++) {
	$ou .= $dnparts[$i];
	if(@$dnparts[$i+1])
	  $ou .= ",";
  }
  
  // Look in this DN if there's objects.
  // TODO: Replace with '$_pql->search()'
  $sr = ldap_search($linkid, $ou, 'objectClass=*');
  $info = ldap_get_entries($linkid, $sr) or pql_format_error(1);
  if(!@$info[1]) {
	// We don't have more than the DN object, delete it.
	if(!$_pql->delete($ou))
	  return false;
  }
  
  return true;
}
// }}}

// {{{ pql_bind9_update_serial(dn)
function pql_bind9_update_serial($dn) {
  global $_pql;
  
  // Get the zone name from the record (so that we can find the SOA)
  $zone = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_ZONENAME"));
  
  // {{{ Get the SOA for this zone.
  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$zone)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
  foreach($_SESSION["BASE_DN"] as $base) {
	$object = $_pql->search($base, $filter);
	
	// "there can be only one" - anything else should (?) be an error.
	if(@is_array($object[0])) {
	  $soa["dn"]  = $object[0]["dn"];
	  $soa["soa"] = $object[0][lc(pql_get_define("PQL_ATTR_SOARECORD"))];
	} elseif(@is_array($object)) {
	  $soa["dn"]  = $object["dn"];
	  $soa["soa"] = $object[lc(pql_get_define("PQL_ATTR_SOARECORD"))];
	}
  }
  // }}}
  
  // {{{ Get the old serial number from the SOA
  // sOARecord: phpqladmin.com. registry.phpqladmin.com. 2005013001 172800 86400 2419200 604800
  $tmp = split(' ', $soa["soa"]); $serial = $tmp[2];
  
  // Extract date and serial number from the SOA serial number.
  // This _SHOULD_ be in the form 'YYYMMDDxx'.
  // TODO: Do this only if the string is longer than ten characters.
  //       Otherwise, just use the value as serial number.
  for($i=0; $i <= 7; $i++)
	$serial_date .= $serial[$i];
  $serial_number = $serial[8].$serial[9];
  // }}}
  
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
  if(!pql_modify_attribute($soa["dn"], pql_get_define("PQL_ATTR_SOARECORD"), 1, $SOA))
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
