<?php require("./include/pql_bind9.inc");
      $zones = pql_bind9_get_zone($_pql->ldap_linkid, $domain);
      if(is_array($zones)) {
?>
  <table cellspacing="0" border="0" width="100%" cellpadding="0">
<?php
	foreach($zones as $dname => $zone_data) {
	    if(!$dns_domain_name)
	      // Use the first zone as default (ie, if we're not called with a specific zone to show)
	      $dns_domain_name = $dname;
?>
    <a href="<?=$_SERVER["PHP_SELF"]?>?rootdn=<?=$rootdn?>&domain=<?=$domain?>&view=<?=$view?>&dns_domain_name=<?=$dname?>"><img alt="/ <?=$dname?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?php echo urlencode($dname)?>"></a>
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
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ttl&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." TTL"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['TTL']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ttl&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>

      <tr></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">A</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=a&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." A"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['A']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=a&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>

<?php if(is_array($zones[$dns_domain_name]['@']['NS'])) {
	$new_tr = 1;

	foreach($zones[$dns_domain_name]['@']['NS'] as $ns) {
?>

<?php		if($new_tr) { $new_tr = 0; ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Name server')?></td>
<?php		} else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." NS"?>"></a>&nbsp;<?=$ns?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&oldvalue=<?=$ns?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php	  }
      } else {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Name server')?></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." NS"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['NS']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&oldvalue=<?=$zones[$dns_domain_name]['@']['NS']?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=add&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=ns&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." NS"?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." NS"?></a></td>
        <td></td>
      </tr>

<?php if(is_array($zones[$dns_domain_name]['@']['MX'])) {
	$new_tr = 1;

	foreach($zones[$dns_domain_name]['@']['MX'] as $mx) {
?>

<?php		if($new_tr) { $new_tr = 0; ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchanger')?></td>
<?php		} else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&typef=mx&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." MX"?>"></a>&nbsp;<?=$mx?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&oldvalue=<?=$mx?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php	  }
      } else {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mail exchanger')?></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify SOA value')." MX"?>"></a>&nbsp;<?=$zones[$dns_domain_name]['@']['MX']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&oldvalue=<?=$zones[$dns_domain_name]['@']['MX']?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=add&rdn=<?php echo urlencode($zones[$dns_domain_name]['@']['RDN']); ?>&type=mx&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." MX"?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('SOA value')))." MX"?></a></td>
        <td></td>
      </tr>
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

<?php	if(is_array($zones[$dns_domain_name])) {
	  foreach($zones[$dns_domain_name] as $data) {
		if($data['HOST'] != '@') {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&type=host&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify host value')." host"?>"></a>
          <?=$data['HOST']?>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&type=ttl&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify host value')." TTL"?>"></a>
          <?=$data['TTL']?>
        </td>

<?php			if($data['CNAME']) { ?>
        <td><?=$LANG->_('CNAME')?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&type=cname&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify host value')." CNAME"?>"></a>
          <?=$data['CNAME']?>
        </td>

<?php			} elseif($data['A']) { ?>
        <td>A</td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&type=a&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify host value')." A"?>"></a>
          <?=$data['A']?>
        </td>

<?php			} elseif($data['SRV']) { ?>
        <td><?=$LANG->_('SRV')?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&type=srv&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify host value')." SRV"?>"></a>
          <?=$data['SRV']?>
        </td>

<?php			} elseif($data['TXT']) { ?>
        <td>TXT</td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&type=txt&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Modify host value')." TXT"?>"></a>
          <?=$data['TXT']?>
        </td>
<?php			} ?>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?php echo urlencode($data['RDN']); ?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete host')?>"></a>
        </td>
      </tr>
<?php		}
	  } // foreach $zones
	}
?>

      <tr class="subtitle">
        <td><a href="bind9_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&domainname=<?=$dns_domain_name?>&action=add&type=host&submit=1&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('host'))); ?></a></td>
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
        <td><a href="bind9_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=add&type=domain&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>
<?php if($dns_domain_name) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&dns_domain_name=<?=$dns_domain_name?>&action=del&type=domain&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Remove this %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dnszonetemplate.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&defaultdomain=<?=$dns_domain_name?>&view=<?=$view?>"><?=$LANG->_('Create DNS template zone file')?></a></td>
      </tr>
<?php } ?>
    </th>
  </table>
