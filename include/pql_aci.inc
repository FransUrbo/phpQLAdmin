<?php
// ----------------------------
// Retreive, modify and delete ACIs (per-object access control lists)
//
// $Id: pql_aci.inc,v 2.3 2003-08-18 12:19:02 turbo Exp $

// Get ACI entries from object
function pql_aci_get($linkid, $dn, $number = 0) {
    if($number)
      // We're looking for a specific ACI number
      $wildcard = "$number#*";
    else
      $wildcard = "*";
	
    $filter = "OpenLDAPaci=$wildcard";
    $sr     = ldap_read($linkid, $dn, $filter, array('OpenLDAPaci')) or pql_errormsg($linkid);
    $info   = ldap_get_entries($linkid, $sr) or pql_errormsg($linkid);

    if($info[0][pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")][0]) {
		// An openLDAPaci attribute might look something like (see README.aci):
		//	3#entry#grant;r,s,c;macAddress#group#cn=customer,ou=groups,dc=planetsky,dc=com
		// or (with more than one permission/target
		//	3#entry#grant;r,s,c;macAddress;s,c;[all]#group#cn=customer,ou=groups,dc=planetsky,dc=com
		$n = 0; 

		for($j=0; $j < $info[0][pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")]["count"]; $j++) {
			$got_more_rights = 0;
			
			$tmp1 = split("#", $info[0][pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")][$j]); // Split up the attribute value into it's (five) parts
			$ACI[$n]["OID"]		= $tmp1[0]; // Irrelevant: Counter or dummy OID (Ex: 1.2.3)
			$ACI[$n]["SCOPE"]	= $tmp1[1]; // Irrelevant: Always 'entry'
			
			// --------------
			$tmp2 = split(";", $tmp1[2]); // The third part (RIGHTS) is further sub-devided (in _at least_ tree parts)
			$ACI[$n]["RIGHTS"]["ACTION"]		= $tmp2[0]; // 'grant' or 'deny'
			
			$tmp3 = split(",", $tmp2[1]); // Divide up the PERMISSIONS (w,r,s,c and/or x) into an array
			for($k=0; $tmp3[$k]; $k++)
			  $ACI[$n]["RIGHTS"]["PERMISSIONS"][$k] = $tmp3[$k]; // w,r,s,c or x
			
			$tmp3 = split(",", $tmp2[2]); // Divide up the TARGET (attributes) into an array
			for($k=0; $tmp3[$k]; $k++)
			  $ACI[$n]["RIGHTS"]["TARGET"][$k]	= $tmp3[$k];
			
			if($tmp2[3])
			  // We have more permission/target combos. Deal with them later.
			  $got_more_rights = 1;
			// --------------
			
			$ACI[$n]["TYPE"]	= $tmp1[3];
			$ACI[$n]["SUBJECT"]	= $tmp1[4];
			
			$n++;
			
			if($got_more_rights) {
				// Deal with the additional permission/target - separate array.
				for($k=3; $tmp2[$k];) {
					// Duplicate the majority of the information from the previous array.
					$ACI[$n]["OID"]				 = $ACI[$n-1]["OID"];
					$ACI[$n]["SCOPE"]			 = $ACI[$n-1]["SCOPE"];
					$ACI[$n]["RIGHTS"]["ACTION"] = $ACI[$n-1]["RIGHTS"]["ACTION"];
					
					// --------------
					$tmp3 = split(",", $tmp2[$k]); // Divide up the PERMISSIONS (w,r,s,c and/or x) into an array
					for($l=0; $tmp3[$l]; $l++)
					  $ACI[$n]["RIGHTS"]["PERMISSIONS"][$l] = $tmp3[$l];
					
					$tmp3 = split(",", $tmp2[$k+1]); // Divide up the TARGET (attributes) into an array
					for($l=0; $tmp3[$l]; $l++)
					  $ACI[$n]["RIGHTS"]["TARGET"][$l]	= $tmp3[$l];
					// --------------
					
					$ACI[$n]["TYPE"]	= $ACI[$n-1]["TYPE"];
					$ACI[$n]["SUBJECT"]	= $ACI[$n-1]["SUBJECT"];
					
					$k = $k + 2; $n++;
				}
			}
		}
		
		$ACI["count"] = $n;	// How many ACIs do we have?
		$ACI["dn"] = $dn;	// Remember the DN, just in case!

		return $ACI;
    }
	
    return false;
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
