<?php
// ----------------------------
// pql_units.inc
// Sub units (ou=People, ou=Users, dc=host) etc
//
// $Id: pql_units.inc,v 1.16 2006-07-19 21:03:36 turbo Exp $
//

// {{{ pql_unit_get(linkid, dn, filter)
function pql_unit_get($linkid, $dn, $filter = 1) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn, 'pql_unit_get'); $rootdn = urldecode($rootdn);
	$subd = array(); // Just to keep PHP quiet!

	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc")
	  // Look for sub-domains below this domain branch.
	  $subd = pql_get_dn($linkid, $dn, 'objectclass=domain', 'ONELEVEL');

	// Either we couldn't find any sub-domains below the domain branch, OR
	// we have the database layout organized with organization.
	// -> Either way, look for organizational unit below the DN as well.
	$ocs = pql_get_dn($linkid, $dn, 'objectclass=organizationalUnit', 'ONELEVEL');

	// Join the two values
	$subd = pql_add2array($subd, $ocs);
	for($i=0; $i < count($subd); $i++) {
		if($filter) {
			// Filter out the most obvious subbranches we don't put users in...
			if(!eregi('qmail',  		$subd[$i]) and
			   !eregi('auto\.', 		$subd[$i]) and
			   !eregi('mailinglists',	$subd[$i]) and
			   (pql_get_define("PQL_CONF_SUBTREE_GROUPS")    and !eregi(pql_get_define("PQL_CONF_SUBTREE_GROUPS"), 		$subd[$i])) and
			   (pql_get_define("PQL_CONF_SUBTREE_APACHE")    and !eregi(pql_get_define("PQL_CONF_SUBTREE_APACHE"),		$subd[$i])) and
			   (pql_get_define("PQL_CONF_SUBTREE_BIND9")     and !eregi(pql_get_define("PQL_CONF_SUBTREE_BIND9"),		$subd[$i])) and
			   (pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") and !eregi(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"),	$subd[$i])) and
			   (pql_get_define("PQL_CONF_SUBTREE_SUDOERS")   and !eregi(pql_get_define("PQL_CONF_SUBTREE_SUDOERS"),		$subd[$i])))
			  $branches[] = $subd[$i];
		} else
		  $branches[] = $info[$i];
	}

	if(isset($branches))
	  return $branches;
	else
	  return false;
}
// }}}

// {{{ pql_unit_add(linkid, domain, unit)
// Returns true if successfully added sub unit, false if not
function pql_unit_add($linkid, $domain, $unit) {
	// What's the Root DN (namingContexts) for this domain
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

	// Add the unit to the database
	if(!pql_write_add($linkid, $dn, $entry, 'unit', 'pql_unit_add')) {
		// failed to add unit
		pql_format_error(1);
		return false;
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
