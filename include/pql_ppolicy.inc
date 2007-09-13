<?php
// ----------------------------
// pql_ppolicies.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ppolicy.inc,v 2.3 2007-09-13 18:14:04 turbo Exp $

// {{{ pql_get_ppolicies()
function pql_get_ppolicies() {
  global $_pql;
  $policies = array();

  if(!pql_get_define("PQL_CONF_SUBTREE_PPOLICY")) {
	echo "<font color=\"red\">PQL_CONF_SUBTREE_PPOLICY is not set - canceling.</font><br>";
	return(false);
  }

  for($i=0; $_SESSION["BASE_DN"][$i]; $i++) {
	$tmp = $_pql->search(pql_get_define("PQL_CONF_SUBTREE_PPOLICY").','.$_SESSION["BASE_DN"][$i],
						 pql_get_define("PQL_ATTR_OBJECTCLASS").'=pwdPolicy', 'ONELEVEL');
	if($tmp) {
	  if(!is_array($tmp))
		$tmp = array($tmp);

	  if(!$tmp[0])
		$policies[] = $tmp;
	  else
		$policies = $tmp;
	}
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
