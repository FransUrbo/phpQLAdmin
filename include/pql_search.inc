<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.3 2003-04-02 15:51:41 turbo Exp $
//

// ------------------------------------------------
// PQL_SEARCH - API functions
// ------------------------------------------------

function pql_search($ldap_linkid, $ldap_basedn, $filter){
	global $config;

	// get all users matching filter
	$filter = "(&(" . $config["PQL_LDAP_REFERENCE_USERS_WITH"][$ldap_basedn] . "=*)($filter))";

	$sr = ldap_search($ldap_linkid, $ldap_basedn, $filter);
	$info = ldap_get_entries($ldap_linkid, $sr);

	for ($i=0; $i<$info["count"]; $i++) {
		$temp_dn = ldap_explode_dn($info[$i]["dn"], 1);

		$dn[$i]["reference"] = $info[$i][$config["PQL_LDAP_REFERENCE_USERS_WITH"][$ldap_basedn]][0];
		if($config["PQL_LDAP_REFERENCE_DOMAINS_WITH"][$ldap_basedn] == "dc") {
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
