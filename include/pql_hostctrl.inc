<?php
// ----------------------------
// pql_hostctrl.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_hostctrl.inc,v 1.8 2007-02-26 09:45:06 turbo Exp $

// {{{ pql_array_push_associative(&arr)
function pql_array_push_associative(&$arr) {
  $args = func_get_args();
  foreach ($args as $arg) {
    if (is_array($arg)) {
      foreach ($arg as $key => $value) {
	$arr[$key] = $value;
	$ret++;
      }
    } else
      $arr[$arg] = "";
  }

  return $ret;
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
