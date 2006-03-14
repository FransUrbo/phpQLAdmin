<?php require($_SESSION["path"]."/libs/pql_bind9.inc");
      $zones = pql_bind9_get_zone($_pql->ldap_linkid, $_REQUEST["domain"]);
      if(is_array($zones)) {
?>
  <table cellspacing="0" border="0" width="100%" cellpadding="0">
<?php	foreach($zones as $dname => $zone_data) {
			if(!@$_REQUEST["dns_domain_name"] and !@$dns_domain_name)
				// Use the first zone as default (ie, if we're not called with a specific zone to show)
				$dns_domain_name = $dname;
			elseif(@$_REQUEST["dns_domain_name"])
				$dns_domain_name = $_REQUEST["dns_domain_name"];
?>
    <a href="<?=$_SERVER["PHP_SELF"]?>?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dname?>"><img alt="/ <?=$dname?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($dname)?>"></a>
<?php	} ?>
  </table>

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
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ttl&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." TTL"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['TTL']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ttl&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>

      <tr></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Domain address')?></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=a&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." A"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['A']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=a&view=<?=$_REQUEST["view"]?>&oldvalue=<?=$zones[$dns_domain_name]['@']['A']?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>

<?php if(@is_array($zones[$dns_domain_name]['@']['NS'])) {
	$new_tr = 1;

	foreach($zones[$dns_domain_name]['@']['NS'] as $ns) {
?>

<?php		if($new_tr) { $new_tr = 0; ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Name servers')?></td>
<?php		} else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&ns=<?=$ns?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." NS"?>"></a>&nbsp;<?=$ns?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&oldvalue=<?=$ns?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php	  }
      } elseif(@$zones[$dns_domain_name]['@']['NS']) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Name server')?></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&ns=<?=$ns?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." NS"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['NS']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&oldvalue=<?=$zones[$dns_domain_name]['@']['NS']?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php } else { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchanger')?></td>
<?php } ?>

<?php if(@$zones[$dns_domain_name]['@']['NS']) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php } ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=add&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&ns=<?=$ns?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." NS"?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." NS"?></a></td>
        <td></td>
<?php if(@$zones[$dns_domain_name]['@']['NS']) { ?>
      </tr>
<?php } ?>

<?php if(@is_array($zones[$dns_domain_name]['@']['MX'])) {
	$new_tr = 1;

	foreach($zones[$dns_domain_name]['@']['MX'] as $mx) {
?>

<?php		if($new_tr) { $new_tr = 0; ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchangers')?></td>
<?php		} else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&mx=<?=$mx?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." MX"?>"></a>&nbsp;<?=$mx?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&oldvalue=<?=$mx?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php	  }
      } elseif(@$zones[$dns_domain_name]['@']['MX']) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchanger')?></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&mx=<?=$mx?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." MX"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['MX']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&oldvalue=<?=$zones[$dns_domain_name]['@']['MX']?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php } else { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchanger')?></td>
<?php } ?>

<?php if(@$zones[$dns_domain_name]['@']['MX']) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php } ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=add&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&mx=<?=$mx?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." MX"?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." MX"?></a></td>
        <td></td>
<?php if(@$zones[$dns_domain_name]['@']['MX']) { ?>
      </tr>
<?php } ?>

<?php if(@is_array($zones[$dns_domain_name]['@']['AFSDB'])) {
	$new_tr = 1;

	foreach($zones[$dns_domain_name]['@']['AFSDB'] as $afsdb) {
?>

<?php		if($new_tr) { $new_tr = 0; ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('AFS Database servers')?></td>
<?php		} else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=afsdb&afsdb=<?=$afsdb?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." AFSDB"?>"></a>&nbsp;<?=$afsdb?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=afsdb&oldvalue=<?=$afsdb?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php	  }
      } elseif(@$zones[$dns_domain_name]['@']['AFSDB']) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('AFS Database Server')?></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=afsdb&afsdb=<?=$afsdb?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." AFSDB"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['AFSDB']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=afsdb&oldvalue=<?=$zones[$dns_domain_name]['@']['AFSDB']?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php } else { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchanger')?></td>
<?php } ?>

<?php if(@$zones[$dns_domain_name]['@']['AFSDB']) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php } ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=add&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=afsdb&afsdb=<?=$afsdb?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." AFSDB"?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." AFSDB"?></a></td>
        <td></td>
<?php if(@$zones[$dns_domain_name]['@']['AFSDB']) { ?>
      </tr>
<?php } ?>
    </th>
  </table>

  <br>

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
						$HREF .= "&dns_domain_name=$dns_domain_name";

						if(@$data[$TYPE]) {
							unset($records);
							if(@!is_array($data[$TYPE])) {
								$records[] = $data[$TYPE];
							} else {
								$records = $data[$TYPE];
							}

							foreach($records as $record) {
								if(($TYPE == 'CNAME') and !eregi('\.$', $record)) {
									$record .= '.'.pql_maybe_idna_decode($dns_domain_name).'.';
								}

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
          <a href="<?=$HREF?>&type=host"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_('Modify %type% record'), array('type' => "host"))?>"></a>
          <?=$data['HOST']?>
        </td>

        <td>
          <a href="<?=$HREF?>&type=ttl"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_('Modify %type% record'), array('type' => "TTL"))?>"></a>
          <?=$data['TTL']?>
        </td>

        <td><?=$LANG->_($TYPE)?></td>
        <td>
          <a href="<?=$HREF?>&type=<?=lc($TYPE)?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_('Modify %type% record'), array('type' => $TYPE))?>"></a>
          <?=$record?>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=del&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
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
        <td><a href="bind9_add.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&domainname=<?=$dns_domain_name?>&action=add&type=host&submit=1&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$dns_domain_name?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?></a></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
    </th>
  </table>
<?php } else { ?>
  <br>
  <?=$LANG->_('No zones configure for this branch')?>.
<?php } ?>
  <br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_add.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&action=add&type=domain&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>
<?php if($dns_domain_name) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_del.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&dns_domain_name=<?=$dns_domain_name?>&action=del&type=domain&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Remove this %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="tools/dnszonetemplate.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&defaultdomain=<?=$dns_domain_name?>&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Create DNS template zone file')?></a></td>
      </tr>
<?php } ?>
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
