<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_search.inc,v 2.5 2003-04-26 13:16:16 turbo Exp $
//

// ------------------------------------------------
// PQL_CONF_SEARCH - API functions
// ------------------------------------------------

function pql_search($ldap_linkid, $ldap_basedn, $filter){
	global $config;

	// get all users matching filter
	$filter = "(&(" . $config["PQL_CONF_REFERENCE_USERS_WITH"][$ldap_basedn] . "=*)($filter))";

	$sr = ldap_search($ldap_linkid, $ldap_basedn, $filter);
	$info = ldap_get_entries($ldap_linkid, $sr);

	for ($i=0; $i<$info["count"]; $i++)
	  $dn[] = $info[$i]["dn"];

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
