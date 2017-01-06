  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left">
      <tr class="title">
        <td><?php echo $LANG->_('Hostname')?></td>
        <td><?php echo $LANG->_('Time To Live')?></td>
        <td><?php echo $LANG->_('Type')?></td>
        <td><?php echo $LANG->_('Record')?></td>
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
								if(($TYPE == 'CNAME') and !preg_match('/\.$/', $record) and !preg_match('/[0-9]$/', $record))
									$record .= '.'.pql_maybe_idna_decode($dns_domain_name).'.';

								if($TYPE == 'PTR') {
									unset($rev);

									// '168.192.in-addr.arpa' => Remove the '.in-addr.arpa' part
									$zone = preg_replace('/\.in-addr\.arpa/', '', $data["ZONE"]);

									// Reverse what's left => '192.168'
									$tmp = explode('\.', $zone);
									$count = count($tmp);
									for($i=$count-1; $tmp[$i]; $i--) {
										$rev .= $tmp[$i];
										if(@$tmp[$i-1])
											$rev .= ".";
									}

									// Add what's missing in reverse order => '192.168'+'.1.10'
									$tmp = explode('\.', $record);
									for($i=3; $i >= $count; $i--) {
										$rev .= '.'.$tmp[$i];
									}

									$record = $rev;
								}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td>
          <a href="<?php echo $HREF?>&type=host&host=<?php echo $data['HOST']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify %type% record'), array('type' => "host"))?>"></a>
          <?php echo $data['HOST']?>
        </td>

        <td>
          <a href="<?php echo $HREF?>&type=ttl&ttl=<?php echo $data['TTL']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify %type% record'), array('type' => "TTL"))?>"></a>
          <?php echo $data['TTL']?>
        </td>

        <td><?php echo $LANG->_($TYPE)?></td>
        <td>
          <a href="<?php echo $HREF?>&type=<?php echo lc($TYPE)?>&<?php echo lc($TYPE)?>=<?php echo $record?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify %type% record'), array('type' => $TYPE))?>"></a>
          <?php echo $record?>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?php echo $_REQUEST["view"]?>&dns_domain_name=<?php echo $dns_domain_name?>&type=<?php echo lc($TYPE)?>&<?php echo lc($TYPE)?>=<?php echo $record?><?php echo $additional_uri?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Delete host')?>"></a>
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
        <td><a href="bind9_add.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&domainname=<?php echo $dns_domain_name?>&action=add&type=host&submit=1&view=<?php echo $_REQUEST["view"]?>&dns_domain_name=<?php echo $dns_domain_name?><?php echo $additional_uri?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?></a></td>
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
