<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.1 2003-01-14 10:53:21 turbo Exp $
//

// ------------------------------------------------
// PQL_SEARCH - API functions
// ------------------------------------------------

function pql_search($ldap_linkid, $ldap_basedn, $filter){
	// get all users matching filter
	$filter = "(&(" . PQL_LDAP_REFERENCE_USERS_WITH . "=*)($filter))";

	$sr = ldap_search($ldap_linkid, $ldap_basedn, $filter);
	$info = ldap_get_entries($ldap_linkid, $sr);

	for ($i=0; $i<$info["count"]; $i++) {
		$temp_dn = ldap_explode_dn($info[$i]["dn"], 1);

		$dn[$i]["reference"] = $info[$i][PQL_LDAP_REFERENCE_USERS_WITH][0];
		if(PQL_LDAP_ATTR_DOMAIN == "dc") {
			$dn[$i]["domain"] = $temp_dn[2];
		} else {
			$dn[$i]["domain"] = $temp_dn[1];
		}
	}

	return $dn;
}


/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
