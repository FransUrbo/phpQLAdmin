<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $
//

// ------------------------------------------------
// PQL_SEARCH - API functions
// ------------------------------------------------

function pql_search($ldap_linkid,$ldap_basedn, $filter){
	// get all users matching filter

	$sr = ldap_search($ldap_linkid, $ldap_basedn, "(&(" . PQL_LDAP_REFERENCE_USERS_WITH . "=*)($filter))");
	$info = ldap_get_entries($ldap_linkid, $sr);

	for ($i=0; $i<$info["count"]; $i++) {
		$temp_dn = ldap_explode_dn($info[$i]["dn"], 1);

		$dn[$i]["reference"] = $info[$i][PQL_LDAP_REFERENCE_USERS_WITH][0];
		$dn[$i]["ou"] = $temp_dn[1];
	}

	return $dn;
}

?>
