<?php
$base_url  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"];

// ----------------------
// Contact person
$class = pql_format_table(0);
$url_tmp = "$base_url&attrib=".pql_get_define("PQL_ATTR_SEEALSO");
$tbl_txt = array('tit' => $LANG->_('Contact person'),
				 'add' => pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('contact person for domain'))),
				 'del' => $LANG->_('Remove contact person from ')." ".$o);
$tbl_url = array('add' => $url_tmp."&submit=3&action=add",
				 'del' => $url_tmp."&submit=4&action=delete");
pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $seealso,
						pql_get_define("PQL_ATTR_SEEALSO"), $class);
?>
    </th>
  </table>
