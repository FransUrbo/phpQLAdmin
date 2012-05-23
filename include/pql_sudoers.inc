<?php
// ----------------------------
// pql_formating.inc
// Output formating etc
//
// $Id: pql_sudoers.inc,v 2.1 2007-02-26 09:45:05 turbo Exp $

// {{{ pql_sudo_add_role(domain, rolename, user, command, runas, host)
function pql_sudo_add_role($domain, $rolename, $user, $command, $runas, $host) {
  global $_pql;
  
  if(pql_get_define("PQL_CONF_SUBTREE_SUDOERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_SUDOERS") . ",";

  if(!$_pql->get_dn($subrdn.$domain, 'objectClass=*', 'BASE')) {
	// Organization unit don't exist - create it
	$ou = explode('=', pql_get_define("PQL_CONF_SUBTREE_SUDOERS")); $ou=$ou[1];
	if(!pql_unit_add($domain, $ou)) {
	  pql_format_error(1);
	  die("pql_sudo_add_role(): Failed to create 'ou=$ou,$domain'");
	}
  }
  
  $dn = "cn=" . $rolename . "," . $subrdn . $domain;
  
  $user = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_UID"));
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'top';
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'sudoRole';
  $entry[pql_get_define("PQL_ATTR_CN")] = $rolename;
  
  if(isset($user) && $user != 'none')
	$entry[pql_get_define("PQL_ATTR_SUDOUSER")] = $user;
  
  if(isset($command) && $command != '')
	$entry[pql_get_define("PQL_ATTR_SUDOCOMMAND")] = $command;
  
  if(isset($runas) && $runas != '')
	$entry[pql_get_define("PQL_ATTR_SUDORUNAS")] = $runas;
  
  if(isset($host) && $host != 'none')
	$entry[pql_get_define("PQL_ATTR_SUDOHOST")] = $host;
  
  if(!$_pql->add($dn, $entry, $type, 'pql_sudo_add_role')) {
	// Failed to add user
	pql_format_error(1);
	die("pql_sudo_add_role(): Failed to create '$dn'");
  }
  
  return(true);
}
// }}}

// {{{ pql_sudo_list_expand(role, alist, attribute, extraURL)
// TODO: This is seriosly broken! See tables/domain_details-sudoers.inc.
function pql_sudo_list_expand($role, $alist, $attribute, $extraURL) {
  global $LANG;

  $mouse["over"]  = 'onmouseover="if (isDOM || isIE4) {hilightBase(\''.$role.$attribute.'\', \'#CCFFCC\')}"';
  $mouse["out"]   = 'onmouseout="if (isDOM || isIE4) {hilightBase(\''.$role.$attribute.'\', \'#D0DCE0\')}"';
  
  $url = $_SERVER['REQUEST_URI'];
  if(is_array($alist)) {
?>
  <div id='<?php echo $role.$attribute?>Parent' class='parent' <?=$mouse["over"]." ".$mouse["out"]?>>
    <nobr>
      <a class="item" onClick="if (capable) {expandBase('<?php echo $role.$attribute?>', true); return false;}">
        <img name='imEx' src='images/plus.png' border='0' alt='+' width='9' height='9' id='<?php echo $role.$attribute; ?>Img'>
      </a>

      <a class="item" onClick="if (capable) {expandBase('<?php echo $role.$attribute?>', false)}">
        <span class="heada"><?php echo $alist[0]?></span>
      </a>
    </nobr>
  </div>

  <div id='<?php echo $role.$attribute?>Child' class='child' style="margin-bottom: 5px" <?=$mouse["over"]." ".$mouse["out"]?>>
<?php
	for($w = 1; $w < count($alist); $w++) {
	  $alt_msg = pql_complete_constant($LANG->_('Delete %who% from Sudo role'), array('who' => $alist[$w]));
?>
    <nobr>
      <a class="tblItem" title="<?php echo $alist[$w]?>" href="<?=$_SERVER['REQUEST_URI'].$extraURL?>" target="pqlmain"><?=$alist[$w]?></a>
    </nobr>
<?php
	  //print "&attribute=" . $attribute . "&" . $attribute . "=" . $alist[$w] . "\">";
	  //print "<img src='images/del.png' width='12' height='12' border='0' alt=\"$alt_msg\"></a><br>\n";
	}
  } else {
    if(isset($alist)) {
      $alt_msg = pql_complete_constant($LANG->_('Delete %who% from Sudo role'), array('who' => $alist));
?>
    <nobr>
      <a class="tblItem" title="<?php echo $alist?>" href="<?=$_SERVER['REQUEST_URI'].$extraURL?>" target="pqlmain"><?=$alist?></a>
    </nobr>
<?php
	  //print "\t\t" . $alist;
	  //print " <a href=\"" . $_SERVER['REQUEST_URI'] . $extraURL;
	  //print "&attribute=" . $attribute . "&" . $attribute . "=" . $alist . "\">";
	  //print "<img src='images/del.png' width='12' height='12' border='0' alt=\"$alt_msg\"></a><br>\n";
    } else {
      print "&nbsp;";
    }
  }

  print "  </div>\n";
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
