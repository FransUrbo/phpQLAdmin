<?php $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

      require("./include/pql_bind9.inc");
      $zones = pql_bind9_get_zone($_pql_control->ldap_linkid, "ou=DNS,".$domain);
      if(is_array($zones)) {
?>
  <br>
  <table cellspacing="0" border="0" width="100%" cellpadding="0" size="100%">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><?php foreach($zones as $domainname => $zone_data) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=dnszone&domainname=$domainname&view=$view"?>"><img alt="/ Domain <?=$domainname?> \" vspace="0" hspace="0" border="0" src="images/xxx.png"></a><?php } ?></td>
    </tr>
  </table>

  <br>

  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left">Domainname: <?=$domainname?>
      <tr class="title">
        <td class="title">SOA Record</td>
        <td>SOA Value</td>
        <td class="title"></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Admin</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['SOA']['ADMIN']?></td>
        <td></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Serial number</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['SOA']['SERIAL']?></td>
        <td></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Refresh time</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['SOA']['REFRESH']?></td>
        <td></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Retry time</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['SOA']['RETRY']?></td>
        <td></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Expire time</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['SOA']['RETRY']?></td>
        <td></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">A</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=a&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value A"></a>&nbsp;<?=$zones[$domainname]['@']['A']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=a&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">TTL</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ttl&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value TTL"></a>&nbsp;<?=$zones[$domainname]['@']['TTL']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ttl&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>

      <tr></tr>

<?php if(is_array($zones[$domainname]['@']['NS'])) {
	  foreach($zones[$domainname]['@']['NS'] as $ns) {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">NS</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value NS"></a>&nbsp;<?=$ns?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns&value=<?=$ns?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>
<?php	  }
      } else {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">NS</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value NS"></a>&nbsp;<?=$zones[$domainname]['@']['NS']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns&value=<?=$zones[$domainname]['@']['NS']?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>
<?php } ?>

      <tr></tr>

<?php if(is_array($zones[$domainname]['@']['MX'])) {
	  foreach($zones[$domainname]['@']['MX'] as $mx) {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">MX</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&typef=mx&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value MX"></a>&nbsp;<?=$mx?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx&value=<?=$mx?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>
<?php	  }
      } else {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">MX</td>
        <td><a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value MX"></a>&nbsp;<?=$zones[$domainname]['@']['MX']?></td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx&value=<?=$zones[$domainname]['@']['MX']?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br>

  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left">
      <tr class="title">
        <td>Hostname</td>
        <td>TTL</td>
        <td>Type</td>
        <td>Record</td>
        <td class="title"></td>
      </tr>

<?php	  foreach($zones[$domainname] as $data) {
		if($data['HOST'] != '@') {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&attrib=host&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value host"></a>
          <?=$data['HOST']?>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&attrib=ttl&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value TTL"></a>
          <?=$data['TTL']?>
        </td>

<?php			if($data['CNAME']) { ?>
        <td>CNAME</td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&attrib=cname&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value CNAME"></a>
          <?=$data['CNAME']?>
        </td>

<?php			} elseif($data['A']) { ?>
        <td>A</td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&attrib=a&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value A"></a>
          <?=$data['A']?>
        </td>

<?php			} elseif($data['SRV']) { ?>
        <td>SRV</td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&attrib=srv&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value SRV"></a>
          <?=$data['SRV']?>
        </td>

<?php			} elseif($data['TXT']) { ?>
        <td>TXT</td>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=modify&attrib=txt&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value TXT"></a>
          <?=$data['TXT']?>
        </td>
<?php			} ?>
        <td>
          <a href="bind9_edit_attributes.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&action=del&rdn=<?=$data['RDN']?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
      </tr>
<?php		}
	  } // foreach $zones
?>

      <tr class="subtitle">
        <td><a href="bind9_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&domainname=<?=$domainname?>&action=add&type=host&submit=1&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Add host">Add host</a></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
    </th>
  </table>
<?php } else { ?>
  <br>
  No zones configure for this branch. 
<?php } ?>
  <br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_LANG_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="bind9_add.php?domain=<?=$domain?>&action=add&type=domain&view=<?=$view?>">Add DNS zone</a></td>
      </tr>
    </th>
  </table>

