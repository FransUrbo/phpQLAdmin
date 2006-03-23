<?php
$base_url  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"];

// ----------------------
// Additional domain name
$class = pql_format_table(0);
$url_tmp = "$base_url&attrib=".pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME");
$tbl_txt = array('tit' => $LANG->_('Additional domain name'),
		 'add' => pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))),
		 'del' => $LANG->_('Delete additional domain name'));
$tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		 'del' => $url_tmp."&submit=4&action=delete");
pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $additionaldomainname,
			pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"), $class);

// ----------------------
// Domain administrators
$class = pql_format_table(0);
$url_tmp = "$base_url&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR");
$tbl_txt = array('tit' => $LANG->_('Domain administrators'),
		 'add' => $LANG->_('Add domain administrator for domain '),
		 'del' => $LANG->_('Remove domain administrator'));
$tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		 'del' => $url_tmp."&submit=4&action=delete");
pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $domain_admins,
			pql_get_define("PQL_ATTR_ADMINISTRATOR"), $class);

// ----------------------
// Mailinglist administrators
$class = pql_format_table(0);
$url_tmp = "$base_url&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM");

$tbl_txt = array('tit' => $LANG->_('Mailinglists administrator'),
		 'add' => $LANG->_('Add list administrator for domain '),
		 'del' => $LANG->_('Remove list administrator'));
$tbl_url = array('add' => $url_tmp."&submit=3&action=add",
		 'del' => $url_tmp."&submit=4&action=delete");
pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $mailinglist_admins,
			pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $class);
?>
    </th>
  </table>
