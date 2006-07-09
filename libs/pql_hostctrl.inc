<?php
// ----------------------------
// pql_hostctrl.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_hostctrl.inc,v 1.1.2.3 2006-07-09 11:53:16 turbo Exp $

// {{{ pql_update_sudo(ldap, dn, type, attribute)
function pql_update_sudo($ldap, $dn, $type, $attribute) {
  global $_REQUEST;

  if(isset($_REQUEST[$type]) && (($_REQUEST[$type] == '') or ($_REQUEST[$type] == 'none')))
    return;

  if(isset($_REQUEST[$type]) && ($_REQUEST[$type] != 'none') &&
     pql_modify_attribute($ldap, $dn, $attribute, '', $_REQUEST[$type] ))
    pql_format_status_msg("Added $type=" . $_REQUEST[$type] . " To Sudo Role: " . $dn);
  else
    pql_format_status_msg("Failed To Add $type=" . $_REQUEST[$type] . " To Sudo Role: " . $dn);
}
// }}}

// {{{ pql_list_expand(role, alist, attribute, extraURL)
// TODO: Verify that this works since move to Smarty template!
function pql_list_expand($role, $alist, $attribute, $extraURL) {
  global $_SESSION, $smarty;

  $show = array('role'   => $role,
		'attrib' => $attribute,
		'url'    => $extraURL,
		'alt'    => pql_complete_constant($LANG->_('Delete %what% from Sudo role'), array('what' => $alist[0])),
		'data'   => $alist);
  $smarty->assign('show', $show);
  pql_display_smarty("hostctrl_listexpand.tpl");
}
// }}}

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

