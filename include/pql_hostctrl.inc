<?php
// ----------------------------
// pql_hostctrl.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_hostctrl.inc,v 1.3 2006-07-06 18:01:07 turbo Exp $

$LANG = new pql_lang($_pql) or die("Couldn't make a new LANG object");
require($_SESSION["path"]."/left-head.html");

// {{{ pql_update_sudo(ldap, dn, type, attribute)
function pql_update_sudo($ldap, $dn, $type, $attribute) {
  global $_REQUEST, $LANG;

  if(isset($_REQUEST[$type]) && (($_REQUEST[$type] == '') or ($_REQUEST[$type] == 'none')))
    return;

  if(isset($_REQUEST[$type]) && ($_REQUEST[$type] != 'none') &&
     pql_modify_attribute($ldap, $dn, $attribute, '', $_REQUEST[$type] ))
    pql_format_status_msg(pql_complete_constant($LANG->_('Added %type%=%value% to sudo role %role%'),
						array('type' => $type, 'value' => $_REQUEST[$type],
						      'role' => $dn)));
  else
    pql_format_status_msg(pql_complete_constant($LANG->_('Failed to add %type%=%value% to sudo role '%role%''),
						array('type' => $type, 'value' => $_REQUEST[$type],
						      'role' => $dn)));
}
// }}}

// {{{ pql_list_expand(role, alist, attribute, extraURL)
function pql_list_expand($role, $alist, $attribute, $extraURL) {
  global $LANG;

  if(is_array($alist)) {
?>
  <div id='<?php echo $role.$attribute; ?>Parent' class='parent'>
    <a href="<?php echo $_SERVER['REQUEST_URI']; ?>"
       onClick="if (capable) {expandBase('<?php echo $role.$attribute; ?>', true); return false;}">
       <img name='imEx' src='images/plus.png' border='0' alt='+' width='9' height='9' id='<?php echo $role.$attribute; ?>Img'></a>
    <?php echo $alist[0]; ?>
    <a href="<?php echo $_SERVER['REQUEST_URI']; ?>&<?php echo $extraURL;?>&<?php echo $attribute; ?>=<?php echo $list[0]; ?>">
      <img src='images/del.png' width='12' height='12' border='0'></a><br>
  </div>

  <div id='<?php echo $role.$attribute; ?>Child' class='child'>
    <nobr>
<?php
  for($w = 1; $w < count($alist); $w++) {
	$alt_msg = pql_complete_constant($LANG->_('Delete %who% from Sudo role'), array('who' => $alist[$w]));

	print "\t\t" . $alist[$w];
	print " <a href=\"" . $_SERVER['REQUEST_URI'] . $extraURL;
	print "&attribute=" . $attribute . "&" . $attribute . "=" . $alist[$w] . "\">";
	print "<img src='images/del.png' width='12' height='12' border='0' alt=\"$alt_msg\"></a><br>\n";
      }
  } else {
    if(isset($alist)) {
      $alt_msg = pql_complete_constant($LANG->_('Delete %who% from Sudo role'), array('who' => $alist));

      print "\t\t" . $alist;
      print " <a href=\"" . $_SERVER['REQUEST_URI'] . $extraURL;
      print "&attribute=" . $attribute . "&" . $attribute . "=" . $alist . "\">";
      print "<img src='images/del.png' width='12' height='12' border='0' alt=\"$alt_msg\"></a><br>\n";
    } else {
      print "&nbsp;";
    }
  }

  print "</nobr></div>";
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

