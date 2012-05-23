<?php
// ----------------------------
// Retreive, modify and delete ACIs (per-object access control lists)
//
// $Id: pql_aci.inc,v 2.18 2007-07-13 10:12:09 turbo Exp $

// {{{ pql_aci_get(dn, number)
// Get ACI entries from object
function pql_aci_get($dn, $number = 0) {
  global $_pql;

  if($number)
	// We're looking for a specific ACI number
	$wildcard = "$number#*";
  else
	$wildcard = "*";
  
  // TODO: See 'include/pql_bind9.inc:pql_bind9_get_acis()'
  $result = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_LDAPACI"), 1);
  if(is_array($result)) {
	// An openLDAPaci attribute might look something like (see doc/README.aci):
	//	3#entry#grant;r,s,c;macAddress#group#cn=customer,ou=groups,dc=planetsky,dc=com
	// or (with more than one permission/target
	//	3#entry#grant;r,s,c;macAddress;s,c;[all]#group#cn=customer,ou=groups,dc=planetsky,dc=com
	$n = 0; 
	
	for($j=0; $j < count($result); $j++) {
	  $ACI[$n]["LINE"]   = $result[$j];
	  
	  $got_more_rights = 0;
	  // Split up the attribute value into it's (five) parts
	  $tmp1 = explode("#", $result[$j]);
	  
	  $ACI[$n]["OID"]		= $tmp1[0]; // Irrelevant: Counter or dummy OID (Ex: 1.2.3)
	  $ACI[$n]["SCOPE"]	= $tmp1[1]; // Irrelevant: Always 'entry'
	  
	  // --------------
	  $tmp2 = explode(";", $tmp1[2]); // The third part (RIGHTS) is further sub-devided (in _at least_ tree parts)
	  $ACI[$n]["RIGHTS"]["ACTION"]		= $tmp2[0]; // 'grant' or 'deny'
	  
	  $tmp3 = explode(",", $tmp2[1]); // Divide up the PERMISSIONS (w,r,s,c and/or x) into an array
	  for($k=0; $k < count($tmp3); $k++)
		$ACI[$n]["RIGHTS"]["PERMISSIONS"][$k] = $tmp3[$k]; // w,r,s,c or x
	  
	  $tmp3 = explode(",", $tmp2[2]); // Divide up the TARGET (attributes) into an array
	  for($k=0; $k < count($tmp3); $k++)
		$ACI[$n]["RIGHTS"]["TARGET"][$k]	= $tmp3[$k];
	  
	  if(!@empty($tmp2[3]))
		// We have more permission/target combos. Deal with them later.
		$got_more_rights = 1;
	  // --------------
	  
	  $ACI[$n]["TYPE"]	= $tmp1[3];
	  $ACI[$n]["SUBJECT"]	= $tmp1[4];
	  
	  $n++;
	  
	  if($got_more_rights) {
		// Deal with the additional permission/target - separate array.
		for($k=3; $k < count($tmp2);) {
		  // Duplicate the majority of the information from the previous array.
		  $ACI[$n]["OID"]				 = $ACI[$n-1]["OID"];
		  $ACI[$n]["SCOPE"]			 = $ACI[$n-1]["SCOPE"];
		  $ACI[$n]["RIGHTS"]["ACTION"] = $ACI[$n-1]["RIGHTS"]["ACTION"];
		  
		  // --------------
		  $tmp3 = explode(",", $tmp2[$k]); // Divide up the PERMISSIONS (w,r,s,c and/or x) into an array
		  for($l=0; $l < count($tmp3); $l++)
			$ACI[$n]["RIGHTS"]["PERMISSIONS"][$l] = $tmp3[$l];
		  
		  $tmp3 = explode(",", $tmp2[$k+1]); // Divide up the TARGET (attributes) into an array
		  for($l=0; $l < count($tmp3); $l++)
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
// }}}

// {{{ pql_validate_acis(acis)
// In OpenLDAP 2.3, the ACI's changed quite drastically compared
// to version 2.2 (which I am/was used to - I never upgraded to 2.3).
// So try to massage 2.2 ACI's to something that will work in 2.3.
//
// Note however that WHEN I desided to try 2.3, I quickly discovered
// a bug which I got fixed in >2.2.32 (at the time of this writing,
// 2.2.33 isn't yet released, so I ran OPENLDAP_REL_ENG_2_3 from CVS
// which have this bug fixed). This means that ACI's won't work with
// OpenLDAP 2.3 prior to 2.3.33!
//
// The problem was with the [entry] and/or entry match rule. There
// was a dead-loop in it's check. See the mailinglist in February
// 13-15 2007.
function pql_validate_acis($acis) {
  // Save the original block.
  $original = $acis;

  // Zero out the acis, so it can be recreated
  // into the same array.
  unset($acis);

  // Go through the original ACI's, line by line
  $OID = 0;
  foreach($original as $aci) {
	$aci_parts = explode('#', $aci); // oid#scope#access#match#target
	$access_parts = explode(';', $aci_parts[2]); // rule;perm;what

	// It is the 'what' and 'match' that have changed
	// Also, the target (if set) must be normalized!

	if(preg_match('/,/', $access_parts[2])) {
	  // OpenLDAPaci: 1#entry#grant;c,x;userPassword,krb5PrincipalName,cn,mail,mailAlternateAddress#public#
	  // =>
	  // OpenLDAPaci: 1#entry#grant;c,x;userpassword#public#
	  // OpenLDAPaci: 2#entry#grant;c,x;krb5principalname#public#
	  // OpenLDAPaci: 3#entry#grant;c,x;cn#public#
	  // OpenLDAPaci: 4#entry#grant;c,x;mail#public#
	  // OpenLDAPaci: 5#entry#grant;c,x;mailalternateaddress#public#
	  $attrs = explode(',', $access_parts[2]);
	  foreach($attrs as $attr) {
		$acis[$OID]  = "$OID#".$aci_parts[1].'#';
		$acis[$OID] .= $access_parts[0].';'.$access_parts[1].';';
		$acis[$OID] .= lc($attr).'#'.$aci_parts[3]."#";
		if($aci_parts[4])
		  $acis[$OID] .= lc($aci_parts[4]);

		$OID++;
	  }
	} elseif(preg_match('/\[children\]/', $aci_parts[2])) {
	  // OpenLDAPaci: 1#entry#grant;w,r,s,c;[children]#access-id#uid=turbo,ou=People,o=Fredriksson,c=SE
	  // =>
	  // OpenLDAPaci: 1#children#grant;w,r,s,c;[all]#access-id#uid=turbo,ou=people,o=fredriksson,c=se
	  $access = preg_replace('/\[children\]/', '[all]', $aci_parts[2]);

	  $acis[$OID]  = "$OID#children#$access#".$aci_parts[3]."#";
	  if($aci_parts[4])
		$acis[$OID] .= lc($aci_parts[4]);
	  
	  $OID++;
	} elseif(preg_match('/\[entry\]/', $aci_parts[2])) {
	  // OpenLDAPaci: 81#entry#grant;w,r,s,c;[entry]#access-id#uid=fernanda,ou=People,o=Bortheiry,c=SE
	  // =>
	  // OpenLDAPaci: 81#entry#grant;w,r,s,c;entry#access-id#uid=fernanda,ou=People,o=Bortheiry,c=SE
	  $access = preg_replace('/\[entry\]/', 'entry', $aci_parts[2]);

	  $acis[$OID]  = "$OID#entry#$access".$aci_parts[3]."#";
	  if($aci_parts[4])
		$acis[$OID] .= lc($aci_parts[4]); if($aci_parts[4]);
	  
	  $OID++;
	} else {
	  // This line is fine as-is...
	  $acis[$OID]  = "$OID#".$aci_parts[1]."#".$aci_parts[2];
	  $acis[$OID] .= '#'.$aci_parts[3].'#';
	  if($aci_parts[4])
		$acis[$OID] .= lc($aci_parts[4]);
	  
	  $OID++;
	}
  }

  return($acis);
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
