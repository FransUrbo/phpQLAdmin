<?php
// ----------------------------
// pql_units.inc
// Sub units (ou=People, ou=Users, dc=host) etc
//
// $Id: pql_units.inc,v 1.13 2004-11-05 13:42:37 turbo Exp $
//

// {{{ pql_unit_get(linkid, dn, filter)
function pql_unit_get($linkid, $dn, $filter = 1) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn, 'pql_unit_get'); $rootdn = urldecode($rootdn);

	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc")
	  // Look for sub-domains below this domain branch.
	  $subd = pql_get_dn($linkid, $dn, 'objectclass=domain', 'ONELEVEL');

	// Either we couldn't find any sub-domains below the domain branch, OR
	// we have the database layout organized with organization.
	// -> Either way, look for organizational unit below the DN as well.
	$ocs = pql_get_dn($linkid, $dn, 'objectclass=organizationalUnit', 'ONELEVEL');

	// Join the two values
	$info = array_merge($subd, $ocs);
	for($i=0; $info[$i]; $i++) {
		if($filter) {
			// Filter out the most obvious subbranches we don't put users in...
			if(!eregi('group',  $info[$i]) and
			   !eregi('qmail',  $info[$i]) and
			   !eregi('auto\.', $info[$i]) and
			   !eregi('web',    $info[$i]) and
			   !eregi('dns',	$info[$i]))
			  $branches[] = $info[$i];
		} else
		  $branches[] = $info[$i];
	}

	return $branches;
}
// }}}

// {{{ pql_unit_add(linkid, domain, unit)
// Returns true if successfully added sub unit, false if not
function pql_unit_add($linkid, $domain, $unit) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($domain, 'pql_unit_add'); $rootdn = urldecode($rootdn);
	$domain = urldecode($domain);

	$unit = pql_maybe_encode($unit);

	if(ereg('=', $unit)) {
		$tmp = split('=', $unit);

		$dn = $tmp[0]."=".$tmp[1].",$domain";
		$entry[$tmp[0]] = $tmp[1];
	} else {
		$dn = "ou=$unit,$domain";
		$entry["ou"] = $unit;
	}

	$entry['objectClass'] = "organizationalUnit";

	// Add the OpenLDAPaci attribute (maybe)
	if($_SESSION["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($linkid, $_SESSION["USER_DN"], 'unit');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_unit_add", $dn, $entry);
	if(file_exists("./.DEBUG_ME"))
	  echo $LDIF;
	else {
		// Add the unit to the database
		if(!ldap_add($linkid, $dn, $entry)) {
			// failed to add unit
			pql_format_error(1);
			die("$LDIF");
			return false;
		}
	}

	return(true);
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
