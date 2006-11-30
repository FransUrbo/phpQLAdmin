<?php
// $Id: host_details-websrv_access.inc,v 1.1.2.1 2006-11-30 20:04:56 turbo Exp $

// {{{ Get container access
$members = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["server"], pql_get_define("PQL_ATTR_UNIQUE_GROUP"));
if(!@$members) {
  $members = array();
} elseif(@$members and !is_array($members)) {
  // Flat - make it an array
  $members = array($members);
}
// }}}

// {{{ Webserver container access control
if(isset($members)) {
  asort($members);
?>
    <th colspan="2" align="left"><?=$LANG->_('Access control')?>
<?php
  $class = pql_format_table(0);
  $url_tmp  = "host_edit_attribute.php?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"]);
  $url_tmp .= "&server=".urlencode($_REQUEST["server"])."&virthost=".$_REQUEST["virthost"];
  $url_tmp .= "&ref=".$_REQUEST["ref"]."&attrib=".pql_get_define("PQL_ATTR_UNIQUE_GROUP");

  $tbl_txt = array('tit' => $LANG->_('Users with access to webserver'),
				   'add' => $LANG->_('Grant user administrator access to webserver'),
				   'del' => $LANG->_('Disallow user administrator access to webserver'));
  $tbl_url = array('add' => $url_tmp."&action=add",
				   'del' => $url_tmp."&action=del");
  pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $members,
						  pql_get_define("PQL_ATTR_UNIQUE_GROUP"), $class);
?>
    </th>
<?php
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
