  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left">
      <tr class="title">
        <td><?=$LANG->_('Hostname')?></td>
        <td><?=$LANG->_('Time To Live')?></td>
        <td><?=$LANG->_('Type')?></td>
        <td><?=$LANG->_('Record')?></td>
        <td class="title"></td>
      </tr>

<?php	if(@is_array($zones[$dns_domain_name])) {
			foreach($zones[$dns_domain_name] as $data) {
				if(@$data['HOST'] != '@') {
					$TYPES = array('CNAME', 'A', 'SRV', 'TXT', 'PTR', 'NS', 'MX');
					foreach($TYPES as $TYPE) {
						$type = lc($TYPE);

						$HREF  = "bind9_edit_attributes.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"];
						$HREF .= "&action=modify&rdn=".urlencode($data['RDN'])."&view=".$_REQUEST["view"];
						$HREF .= "&dns_domain_name=$dns_domain_name$additional_uri";

						if(@$data[$TYPE]) {
							unset($records);
							if(@!is_array($data[$TYPE])) {
								$records[] = $data[$TYPE];
							} else {
								$records = $data[$TYPE];
							}

							foreach($records as $record) {
								if(($TYPE == 'CNAME') and !ereg('\.$', $record) and !ereg('[0-9]$', $record))
									$record .= '.'.pql_maybe_idna_decode($dns_domain_name).'.';

								if($TYPE == 'PTR') {
									unset($rev);

									// '168.192.in-addr.arpa' => Remove the '.in-addr.arpa' part
									$zone = preg_replace('/\.in-addr\.arpa/', '', $data["ZONE"]);

									// Reverse what's left => '192.168'
									$tmp = split('\.', $zone);
									$count = count($tmp);
									for($i=$count-1; $tmp[$i]; $i--) {
										$rev .= $tmp[$i];
										if(@$tmp[$i-1])
											$rev .= ".";
									}

									// Add what's missing in reverse order => '192.168'+'.1.10'
									$tmp = split('\.', $record);
									for($i=3; $i >= $count; $i--) {
										$rev .= '.'.$tmp[$i];
									}

									$record = $rev;
								}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td>
          <a href="<?=$HREF?>&type=host&host=<?=$data['HOST']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_('Modify %type% record'), array('type' => "host"))?>"></a>
          <?=$data['HOST']?>
        </td>

        <td>
          <a href="<?=$HREF?>&type=ttl&ttl=<?=$data['TTL']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_('Modify %type% record'), array('type' => "TTL"))?>"></a>
          <?=$data['TTL']?>
        </td>

        <td><?=$LANG->_($TYPE)?></td>
        <td>
          <a href="<?=$HREF?>&type=<?=lc($TYPE)?>&<?=lc($TYPE)?>=<?=$record?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_('Modify %type% record'), array('type' => $TYPE))?>"></a>
          <?=$record?>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>&type=<?=lc($TYPE)?>&<?=lc($TYPE)?>=<?=$record?><?=$additional_uri?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php					  } // foreach record
					  } // if TYPE
				  } // foreach TYPES
				} // if host != '@'
			} // foreach $zones
		}
?>

      <tr class="subtitle">
        <td><a href="bind9_add.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&domainname=<?=$dns_domain_name?>&action=add&type=host&submit=1&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?><?=$additional_uri?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?></a></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
    </th>
  </table>
<?php
// {{{ xyz
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>