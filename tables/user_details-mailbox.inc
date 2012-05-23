  <!-- misc MAIL Attributes (mailmessagestore, mailhost, mailquota)-->
<?php if(!is_array($quota)) {
	$quota = $LANG->_('Standard');
      } else {
	  $quota = pql_ldap_mailquota($quota);
      }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Mailbox properties')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Path to mailbox')?></td>
        <td><?php echo $mailmessagestore?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=mailmessagestore&user=<?=$url["user"]?>&oldvalue=<?=$mailmessagestore?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Mailserver')?></td>
        <td><?php echo $mailhost?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=mailhost&user=<?=$url["user"]?>&oldvalue=<?=$mailhost?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Default quota')?></td>
        <td><?php echo $quota?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=mailquota&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
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
