  <!-- misc MAIL Attributes (mailmessagestore, mailhost, mailquota)-->
<?php if(!is_array($quota)) {
	$quota = PQL_LANG_MAILQUOTA_DEFAULT;
      } else {
	  $quota = pql_ldap_mailquota($quota);
      }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_LANG_USER_MAILBOXPROPERTIES; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_MAILMESSAGESTORE_TITLE; ?></td>
        <td><?=$mailmessagestore?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailmessagestore&user=<?=$user?>&oldvalue=<?=$mailmessagestore?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_MAILHOST_TITLE; ?></td>
        <td><?=$mailhost?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailhost&user=<?=$user?>&oldvalue=<?=$mailhost?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_MAILQUOTA_TITLE; ?></td>
        <td><?=$quota?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailquota&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
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
