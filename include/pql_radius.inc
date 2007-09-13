<?php
// ----------------------------
// pql_radius.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_radius.inc,v 1.1 2007-09-13 18:25:14 turbo Exp $

// {{{ pql_get_radprofiles()
// NOTE: This is almost identical to pql_ppolicy.inc:pql_get_ppolicies().
function pql_get_radprofiles() {
  global $_pql;
  $profiles = array();
  
  if(!pql_get_define("PQL_CONF_SUBTREE_PPOLICY")) {
	echo "<font color=\"red\">PQL_CONF_SUBTREE_PPOLICY is not set - canceling.</font><br>";
	return(false);
  }
  
  for($i=0; $_SESSION["BASE_DN"][$i]; $i++) {
	$tmp = $_pql->search(pql_get_define("PQL_CONF_SUBTREE_PPOLICY").','.$_SESSION["BASE_DN"][$i],
						 pql_get_define("PQL_ATTR_OBJECTCLASS").'=radiusProfile', 'ONELEVEL');
	if($tmp) {
	  if(!is_array($tmp))
		$tmp = array($tmp);
	  
	  $profiles[] = $tmp;
	}
  }
  
  return($profiles);
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
