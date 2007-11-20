  <!-- Access list -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Branch access')?>
<?php
// {{{ Domain administrators
// Get domain administrators
$domain_admins   = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
if($domain_admins and !is_array($domain_admins)) {
  // It's defined, but it's not an array. Convert it so we don't get into trouble below.
  $domain_admins = array($domain_admins);
} elseif(!$domain_admins) {
  $domain_admins = array();
}
  
$class = pql_format_table(0);
$url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
$url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR");

$tbl_txt = array('tit' => $LANG->_('Branch administrators'),
		 'add' => $LANG->_('Add domain administrator for domain '),
		 'del' => $LANG->_('Remove domain administrator'));
$tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		 'del' => $url_tmp."&submit=4&action=delete");
pql_format_table_single($tbl_txt, $tbl_url, $domain_admins,
			pql_get_define("PQL_ATTR_ADMINISTRATOR"), $class);
// }}}

// {{{ Mailinglist administrators
if(pql_get_define("PQL_CONF_EZMLM_USE")) {
  // Get mailinglist administrators
  $mailinglist_admins = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"));
  if($mailinglist_admins and !is_array($mailinglist_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$mailinglist_admins = array($mailinglist_admins);
  } elseif(!$mailinglist_admins)
	$mailinglist_admins = array();

  $class = pql_format_table(0);
  $url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
  $url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM");
  
  $tbl_txt = array('tit' => $LANG->_('Mailinglists administrator'),
		   'add' => $LANG->_('Add list administrator for domain '),
		   'del' => $LANG->_('Remove list administrator'));
  $tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		   'del' => $url_tmp."&submit=4&action=delete");
  pql_format_table_single($tbl_txt, $tbl_url, $mailinglist_admins,
			  pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $class);
}
// }}}

// {{{ Webserver administrators
// Get webserver administrators
if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
  $websrv_admins = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"));
  if($websrv_admins and !is_array($websrv_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$websrv_admins = array($websrv_admins);
  } elseif(!$websrv_admins)
	$websrv_admins = array();
}

if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
  $class = pql_format_table(0);
  $url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
  $url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV");
  
  $tbl_txt = array('tit' => $LANG->_('Webserver administrator'),
		   'add' => $LANG->_('Add webserver administrator for domain '),
		   'del' => $LANG->_('Remove webserver administrator'));
  $tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		   'del' => $url_tmp."&submit=4&action=delete");
  pql_format_table_single($tbl_txt, $tbl_url, $websrv_admins,
			  pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"), $class);
}
// }}}

// {{{ DNS administrators
// Get DNS administrators
if(pql_get_define("PQL_CONF_BIND9_USE")) {
  $bind9_admins = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"));
  if($bind9_admins and !is_array($bind9_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$bind9_admins = array($bind9_admins);
  } elseif(!$bind9_admins) {
	$bind9_admins = array();
  }

  $class = pql_format_table(0);
  $url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
  $url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9");
  
  $tbl_txt = array('tit' => $LANG->_('DNS administrator'),
		   'add' => $LANG->_('Add DNS administrator for domain '),
		   'del' => $LANG->_('Remove DNS administrator'));
  $tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		   'del' => $url_tmp."&submit=4&action=delete");
  pql_format_table_single($tbl_txt, $tbl_url, $bind9_admins,
			  pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"), $class);
}
// }}}
?>
    </th>
  </table>

  <br><br>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
