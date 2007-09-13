<?php
// ----------------------------
// pql_units.inc
// Sub units (ou=People, ou=Users, dc=host) etc
//
// $Id: pql_units.inc,v 1.26 2007-09-13 18:16:24 turbo Exp $
//

// {{{ pql_unit_get(dn, filter)
function pql_unit_get($dn, $filter = 1) {
  global $_pql;
  
  // What's the Root DN (namingContexts) for this domain
  $rootdn = pql_get_rootdn($dn, 'pql_unit_get');
  $rootdn = urldecode($rootdn);
  $subd   = array(); // Just to keep PHP quiet!
  
  if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc")
	// Look for sub-domains below this domain branch.
	$subd = $_pql->get_dn($dn, 'objectclass=domain', 'ONELEVEL');
  
  // Either we couldn't find any sub-domains below the domain branch, OR
  // we have the database layout organized with organization.
  // -> Either way, look for organizational unit below the DN as well.
  $ocs = $_pql->get_dn($dn, 'objectclass=organizationalUnit', 'ONELEVEL');
  
  // Join the two values
  pql_add2array($subd, $ocs);
  for($i=0; $i < count($subd); $i++) {
	if($filter) {
	  // Filter out the most obvious subbranches we don't put users in...
	  if(!eregi('qmail',  		$subd[$i]) and
		 !eregi('auto\.', 		$subd[$i]) and
		 !eregi('mailinglists',	$subd[$i]) and
		 !eregi('templates',	$subd[$i]) and
		 !(pql_get_define("PQL_CONF_SUBTREE_GROUPS")	and eregi(pql_get_define("PQL_CONF_SUBTREE_GROUPS"),	$subd[$i])) and
		 !(pql_get_define("PQL_CONF_SUBTREE_BIND9")		and eregi(pql_get_define("PQL_CONF_SUBTREE_BIND9"),		$subd[$i])) and
		 !(pql_get_define("PQL_CONF_SUBTREE_SUDOERS")	and eregi(pql_get_define("PQL_CONF_SUBTREE_SUDOERS"),	$subd[$i])) and
		 !(pql_get_define("PQL_CONF_SUBTREE_PPOLICY")	and eregi(pql_get_define("PQL_CONF_SUBTREE_PPOLICY"),	$subd[$i])))
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

// {{{ pql_unit_add(parentdn, unit)
// Returns true if successfully added sub unit, false if not
function pql_unit_add($parentdn, $unit) {
  global $_pql;
  
  // What's the Root DN (namingContexts) for this domain
  $parentdn = urldecode($parentdn);
  $unit   = pql_maybe_encode($unit);

  if(!$_pql->get_dn($parentdn, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE')) {
	echo "The parent DN '$parentdn' don't exists!?<br>";
	return false;
  }
  
  if(ereg('=', $unit)) {
	$tmp = split('=', $unit);
	
	$dn = $tmp[0]."=".$tmp[1].",$parentdn";
	$entry[$tmp[0]] = $tmp[1];
  } else {
	$dn = "ou=$unit,$parentdn";
	$entry["ou"] = $unit;
  }
  
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = "organizationalUnit";

  if(pql_get_define("PQL_ATTR_ACI_USE") and function_exists("user_generate_aci")) {
	$entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($_SESSION["USER_DN"], 'unit');
  }
  
  // Add the unit to the database
  if(!$_pql->get_dn($dn, pql_get_define("PQL_ATTR_OBJECTCLASS")."=*", 'BASE')) {
	// Object don't exist, add it...
	if(!$_pql->add($dn, $entry, 'unit', 'pql_unit_add'))
	  // Failed to add unit
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
