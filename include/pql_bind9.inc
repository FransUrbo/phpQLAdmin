<?php
// ----------------------------
// pql_bind9.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for bind9 control - DNS zone in LDAP)
//
// $Id: pql_bind9.inc,v 2.1 2003-05-19 12:11:28 turbo Exp $
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
			
			if($info[$i]['arecord'][0]) {
				if($info[$i]['arecord'][1]) {
					// We have multiple A records - Round-Robin!
					for($j=0; $j < $info[$i]['arecord']['count']; $j++)
					  $ZONE[$zone][$host]['A'][] = $info[$i]['arecord'][$j];
				} else
				  $ZONE[$zone][$host]['A'] = $info[$i]['arecord'][0];
			}
			
			if($info[$i]['cnamerecord'][0]) {
				$cname = $info[$i]['cnamerecord'][0];

				if(preg_match("/\\.$/", $cname))
				  // Remove the domain name, so we're left with the hostname.
				  $cname = str_replace(".$zone.", "", $cname);

				$ZONE[$zone][$host]['CNAME'] = $cname;
			}
			
			if($info[$i]['hinforecord'][0])
			  $ZONE[$zone][$host]['HINFO'] = $info[$i]['hinforecord'][0];

			if($info[$i]['srvrecord'][0])
			  $ZONE[$zone][$host]['SRV'] = $info[$i]['srvrecord'][0];

			if($info[$i]['txtrecord'][0])
			  $ZONE[$zone][$host]['TXT'] = $info[$i]['txtrecord'][0];

			// Sort the host array 
			ksort($ZONE[$zone]);
		}

		// Sort the zone array
		ksort($ZONE);

		return($ZONE);
    }
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
