  <!-- misc MAIL Attributes (mailmessagestore, mailhost, mailquota)-->
<?php if(!is_array($quota)) {
	$quota = $LANG->_('Standard');
      } else {
	  $quota = pql_ldap_mailquota($quota);
      }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Mailbox properties')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Path to mailbox')?></td>
        <td><?=$mailmessagestore?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailmessagestore&user=<?=$userdn?>&oldvalue=<?=$mailmessagestore?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mailserver')?></td>
        <td><?=$mailhost?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailhost&user=<?=$userdn?>&oldvalue=<?=$mailhost?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Default quota')?></td>
        <td><?=$quota?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailquota&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
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
