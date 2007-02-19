  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left"><?=$LANG->_('Domainname')?>: <?=pql_maybe_idna_decode($dns_domain_name)?>
      <tr class="title">
        <td class="title"><?=$LANG->_('SOA Record')?></td>
        <td><?=$LANG->_('SOA Value')?></td>
        <td class="title"></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Administrator')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['SOA']['ADMIN']?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Serial number')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['SOA']['SERIAL']?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Refresh time')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['SOA']['REFRESH']?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Retry time')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['SOA']['RETRY']?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Expire time')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['SOA']['RETRY']?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Time To Live')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['TTL']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ttl&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?><?=$additional_uri?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." TTL"?>"></a>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ttl&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?><?=$additional_uri?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete SOA value')?>"></a>
        </td>
      </tr>

      <tr></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Text Record')?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$dns_domain_name]['@']['TXT']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=txt&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?><?=$additional_uri?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." TXT"?>"></a>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=txt&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?><?=$additional_uri?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete SOA value')?>"></a>
        </td>
      </tr>

      <tr></tr>

<?php
// {{{ Domain addresses
$class = pql_format_table(0);
$url_tmp  = "bind9_edit_attributes.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"];
$url_tmp .= "&view=".$_REQUEST["view"]."&rdn=".urlencode($zones[$dns_domain_name]['@']['RDN']);
$url_tmp .= "&type=a&view=".$_REQUEST["view"]."&dns_domain_name=$dns_domain_name$additional_uri";
  
$tbl_txt = array('tit' => $LANG->_('Domain address'),
                 'add' => $LANG->_('Add domain address'),
                 'del' => $LANG->_('Delete domain address'),
				 'mod' => $LANG->_('Modify domain address'));
$tbl_url = array('add' => $url_tmp."&action=add",
                 'del' => $url_tmp."&action=del",
				 'mod' => $url_tmp."&action=modify");
pql_format_table_single($tbl_txt, $tbl_url,
						$zones[$dns_domain_name]['@']['A'],
						"a", $class, 4);
// }}}

// {{{ NS record(s)
$class = pql_format_table(0);
$url_tmp  = "bind9_edit_attributes.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"];
$url_tmp .= "&view=".$_REQUEST["view"]."&rdn=".urlencode($zones[$dns_domain_name]['@']['RDN']);
$url_tmp .= "&type=ns&view=".$_REQUEST["view"]."&dns_domain_name=$dns_domain_name$additional_uri";
  
$tbl_txt = array('tit' => $LANG->_('Name servers'),
                 'add' => $LANG->_('Add SOA value NS'),
                 'del' => $LANG->_('Delete NS'),
				 'mod' => $LANG->_('Modify NS'));
$tbl_url = array('add' => $url_tmp."&action=add",
                 'del' => $url_tmp."&action=del",
				 'mod' => $url_tmp."&action=modify");
pql_format_table_single($tbl_txt, $tbl_url,
						$zones[$dns_domain_name]['@']['NS'],
						"ns", $class, 4);
// }}}

// {{{ MX record(s)
$class = pql_format_table(0);
$url_tmp  = "bind9_edit_attributes.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"];
$url_tmp .= "&view=".$_REQUEST["view"]."&rdn=".urlencode($zones[$dns_domain_name]['@']['RDN']);
$url_tmp .= "&type=mx&view=".$_REQUEST["view"]."&dns_domain_name=$dns_domain_name$additional_uri";
  
$tbl_txt = array('tit' => $LANG->_('Mail exchangers'),
                 'add' => $LANG->_('Add SOA value MX'),
                 'del' => $LANG->_('Delete MX'),
				 'mod' => $LANG->_('Modify MX'));
$tbl_url = array('add' => $url_tmp."&action=add",
                 'del' => $url_tmp."&action=del",
				 'mod' => $url_tmp."&action=modify");
pql_format_table_single($tbl_txt, $tbl_url,
						$zones[$dns_domain_name]['@']['MX'],
						"mx", $class, 4);
// }}}

// {{{ AFSDB record(s)
$class = pql_format_table(0);
$url_tmp  = "bind9_edit_attributes.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"];
$url_tmp .= "&view=".$_REQUEST["view"]."&rdn=".urlencode($zones[$dns_domain_name]['@']['RDN']);
$url_tmp .= "&type=afsdb&view=".$_REQUEST["view"]."&dns_domain_name=$dns_domain_name$additional_uri";
  
$tbl_txt = array('tit' => $LANG->_('AFS Database servers'),
                 'add' => $LANG->_('Add SOA value AFSDB'),
                 'del' => $LANG->_('Delete AFSDB'),
				 'mod' => $LANG->_('Modify AFSDB'));
$tbl_url = array('add' => $url_tmp."&action=add",
                 'del' => $url_tmp."&action=del",
				 'mod' => $url_tmp."&action=modify");
pql_format_table_single($tbl_txt, $tbl_url,
						$zones[$dns_domain_name]['@']['AFSDB'],
						"afsdb", $class, 4);
// }}}
?>
    </th>
  </table>
<?php

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
