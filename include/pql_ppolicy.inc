<?php
// ----------------------------
// pql_ppolicies.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ppolicy.inc,v 2.1 2007-03-05 10:12:25 turbo Exp $

// {{{ pql_get_ppolicies()
function pql_get_ppolicies() {
  global $_pql;

  if(!pql_get_define("PQL_CONF_SUBTREE_PPOLICIES"))
	return(false);

  for($i=0; $_SESSION["BASE_DN"][$i]; $i++) {
	$tmp = $_pql->search(pql_get_define("PQL_CONF_SUBTREE_PPOLICIES").','.$_SESSION["BASE_DN"][$i],
						 pql_get_define("PQL_ATTR_OBJECTCLASS").'=pwdPolicy', 'ONELEVEL');
	if($tmp)
	  pql_add2array($policies, $tmp);
  }

  return($policies);
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
