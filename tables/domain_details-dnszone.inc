<?php $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

      require("./include/pql_bind9.inc");
      $zones = pql_bind9_get_zone($_pql_control->ldap_linkid, "ou=DNS,".$domain);
      if(is_array($zones)) {
?>
  <br>
  <table cellspacing="0" border="0" width="100%" cellpadding="0" size="100%">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><?php foreach($zones as $domainname => $zone_data) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=dnszone&domainname=$domainname"?>"><img alt="/ Domain <?=$domainname?> \" vspace="0" hspace="0" border="0" src="images/xxx.png"></a><?php } ?></td>
    </tr>
  </table>

  <br>

  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left">Domainname: <?=$domainname?>
      <tr class="title">
        <td></td>
        <td>SOA Record</td>
        <td>SOA Value</td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td></td>
        <td>SOA DN</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['RDN']?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td></td>
        <td>SOA</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;<?=$zones[$domainname]['@']['SOA']?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=a"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
        <td>A</td>
        <td><a href="bind9_edit_attributes.php?action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=a"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value"></a>&nbsp;<?=$zones[$domainname]['@']['A']?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ttl"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
        <td>TTL</td>
        <td><a href="bind9_edit_attributes.php?action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ttl"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value"></a>&nbsp;<?=$zones[$domainname]['@']['TTL']?></td>
      </tr>

      <tr></tr>

<?php if(is_array($zones[$domainname]['@']['NS'])) {
	  foreach($zones[$domainname]['@']['NS'] as $ns) {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns&value=<?=$ns?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
        <td>NS</td>
        <td><a href="bind9_edit_attributes.php?action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value"></a>&nbsp;<?=$ns?></td>
      </tr>
<?php	  }
      } else {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns&value=<?=$zones[$domainname]['@']['NS']?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
        <td>NS</td>
        <td><a href="bind9_edit_attributes.php?action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=ns"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value"></a>&nbsp;<?=$zones[$domainname]['@']['NS']?></td>
      </tr>
<?php } ?>

      <tr></tr>

<?php if(is_array($zones[$domainname]['@']['MX'])) {
	  foreach($zones[$domainname]['@']['MX'] as $mx) {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx&value=<?=$mx?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
        <td>MX</td>
        <td><a href="bind9_edit_attributes.php?action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value"></a>&nbsp;<?=$mx?></td>
      </tr>
<?php	  }
      } else {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx&value=<?=$zones[$domainname]['@']['MX']?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>
        <td>MX</td>
        <td><a href="bind9_edit_attributes.php?action=modify&rdn=<?=$zones[$domainname]['@']['RDN']?>&type=mx"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify SOA value"></a>&nbsp;<?=$zones[$domainname]['@']['MX']?></td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br>

  <table cellspacing="0" cellpadding="3" border="0" size="100%">
    <th colspan="3" align="left">
      <tr class="title">
        <td></td>
        <td>Hostname</td>
        <td>TTL</td>
        <td>Type</td>
        <td>Record</td>
      </tr>

<?php	  foreach($zones[$domainname] as $data) {
		if($data['HOST'] != '@') {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>
          <a href="bind9_edit_attributes.php?action=del&rdn=<?=$data['RDN']?>"><img src="images/del.png" width="12" height="12" border="0" alt="Delete host"></a>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?action=modify&attrib=host&rdn=<?=$data['RDN']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value"></a>
          <?=$data['HOST']?>
        </td>

        <td>
          <a href="bind9_edit_attributes.php?action=modify&attrib=ttl&rdn=<?=$data['RDN']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value"></a>
          <?=$data['TTL']?>
        </td>

<?php		if($data['CNAME']) { ?>
        <td>CNAME</td>
        <td>
          <a href="bind9_edit_attributes.php?action=modify&attrib=cname&rdn=<?=$data['RDN']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value"></a>
          <?=$data['CNAME']?>
        </td>

<?php		} elseif($data['A']) { ?>
        <td>A</td>
        <td>
          <a href="bind9_edit_attributes.php?action=modify&attrib=a&rdn=<?=$data['RDN']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value"></a>
          <?=$data['A']?>
        </td>

<?php		} elseif($data['SRV']) { ?>
        <td>SRV</td>
        <td>
          <a href="bind9_edit_attributes.php?action=modify&attrib=srv&rdn=<?=$data['RDN']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value"></a>
          <?=$data['SRV']?>
        </td>

<?php		} elseif($data['TXT']) { ?>
        <td>TXT</td>
        <td>
          <a href="bind9_edit_attributes.php?action=modify&attrib=txt&rdn=<?=$data['RDN']?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify host value"></a>
          <?=$data['TXT']?>
        </td>
<?php		} ?>
      </tr>
<?php		}
	  } // foreach $zones
?>

      <tr class="subtitle">
        <td></td>
        <td><a href="bind9_add.php?action=add&type=host&domain=<?=$domain?>&domainname=<?=$domainname?>&submit=1"><img src="images/edit.png" width="12" height="12" border="0" alt="Add host">Add host</a></td>
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
  <a href="bind9_add.php?action=add&type=domain&domain=<?=$domain?>">Add DNS zone</a>
