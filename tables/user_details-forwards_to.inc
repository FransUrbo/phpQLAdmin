<?php $forwarders = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_FORWARDS"]); ?>
  <!-- Forwarders (mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LANG_MAILFORWARDINGADDRESS_TITLE; ?></th>
      <tr>
        <td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
        <td class="title"><?php echo PQL_LANG_OPTIONS; ?></td>
      </tr>

<?php if(empty($forwarders)) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="2"><?php echo PQL_LANG_MAILFORWARDINGADDRESS_NONE; ?></td>
      </tr>
<?php } else { 
	foreach($forwarders as $forwarder) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$forwarder?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailforwardingaddress&user=<?=$user?>&mailforwardingaddress=<?=$forwarder?>&oldvalue=<?=$forwarder?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailforwardingaddress&user=<?=$user?>&oldvalue=<?=$forwarder?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
<?php	} // end of foreach
      } // end of if-else
?>

      <tr class="subtitle">
        <td colspan="2">
          <a href="user_add_attribute.php?attrib=mailforwardingaddress&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LANG_MAILFORWARDINGADDRESS_NEW; ?></a>
        </td>
      </tr>
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
