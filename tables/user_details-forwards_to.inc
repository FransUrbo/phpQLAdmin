<?php $forwarders = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_FORWARDS")); ?>
  <!-- Forwarders (mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Forward')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Email')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>

<?php if(empty($forwarders)) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="2"><?=$LANG->_('None')?></td>
      </tr>
<?php } else { 
	foreach($forwarders as $forwarder) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=pql_maybe_idna_decode($forwarder)?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailforwardingaddress&user=<?=$userdn?>&mailforwardingaddress=<?=$forwarder?>&oldvalue=<?=$forwarder?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailforwardingaddress&user=<?=$userdn?>&oldvalue=<?=$forwarder?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
<?php	} // end of foreach
      } // end of if-else
?>

      <tr class="subtitle">
        <td colspan="2">
          <a href="user_add_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailforwardingaddress&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?=$LANG->_('Create new forwarding address')?></a>
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
