<?php
// ----------------------------
// pql_units.inc
// Sub units (ou=People, ou=Users, dc=host) etc
//
// $Id: pql_units.inc,v 1.2 2003-12-13 13:45:30 turbo Exp $
//

// {{{ pql_unit_get(linkid, dn, filter)
function pql_unit_get($linkid, $dn, $filter = 1) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);

	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc") {
		$ocb = "domain";
	} else {
		$ocb = "organizationalUnit";
	}
	
	$info = pql_search($linkid, $dn, 'objectclass='.$ocb);
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

// {{{ pql_unit_exist(linkid, dn, unit)
// Returns true if sub unit exists, false if not...
function pql_unit_exist($linkid, $dn, $unit) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($dn); $rootdn = urldecode($rootdn);
	$dn = urldecode($dn);

	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc") {
		$ocb = "domain";
		$ref = 'dc';
	} else {
		$ocb = "organizationalUnit";
		$ref = 'ou';
	}

	$filter = "(&($ref=$unit)(objectclass=".$ocb."))";
	
	$sr   = ldap_list($linkid, $dn, $filter) or pql_format_error_msg($linkid);
	$info = ldap_get_entries($linkid, $sr) or pql_format_error_msg($linkid);
	if($info["count"])
	  return(true);
	return(false);
}
// }}}

// {{{ pql_unit_add(linkid, domain, unit)
// Returns true if successfully added sub unit, false if not
function pql_unit_add($linkid, $domain, $unit) {
	// What's the Root DN (namingContexts) for this domain
	$rootdn = pql_get_rootdn($domain); $rootdn = urldecode($rootdn);
	$domain = urldecode($domain);

	if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc") {
		$ocb = "domain";
		$ref = 'dc';
	} else {
		$ocb = "organizationalUnit";
		$ref = 'ou';
	}

	$dn = "$ref=$unit,$domain";
	$entry[$ref] = $unit;
	$entry['objectClass'] = $ocb;

	// Add the OpenLDAPaci attribute (maybe)
	if($GLOBALS["ACI_SUPPORT_ENABLED"])
	  $entry[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")] = user_generate_aci($linkid, $GLOBALS["USER_DN"], 'unit');

	// Create a LDIF object to print in case of error
	$LDIF = pql_create_ldif("pql_unit_add", $dn, $entry);

    // Add the unit to the database
    if(! ldap_add($linkid, $dn, $entry)) {
		// failed to add unit
		pql_format_error_msg($linkid);
		die("$LDIF");
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
