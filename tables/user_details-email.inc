<?php $email   = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_MAIL")); $email = $email[0];
      $aliases = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_MAILALTERNATE"));

	  $tmp     = split('@', $email);

	  $mail	   = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);
?>
  <!-- Addresses (mail, mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Registred addresses')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Type')?></td>
        <td class="title"><?=$LANG->_('Email')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Main address')?></td>
        <td><a href="mailto:<?=$email?>"><?=$mail?></a></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=mail&user=<?=$url["user"]?>&mail=<?=$email?>&oldvalue=<?=$email?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change main address')?>" border="0"></a>&nbsp;&nbsp;&nbsp;
<?php } ?>
          <a href="user_sendmail.php?email=<?=$email?>&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/mail.png" width="16" height="11" alt="<?=$LANG->_('Send testmail')?>" border="0"></a>
        </td>
      </tr>

<?php
if(is_array($aliases)) {
    asort($aliases);
    foreach($aliases as $alias) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Alias')?></td>
        <td><a href="mailto:<?=$alias?>"><?=pql_maybe_idna_decode($alias)?></a></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=mailalternateaddress&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&mailalternateaddress=<?php echo pql_strip_domain($alias); ?>&oldvalue=<?=$alias?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=mailalternateaddress&user=<?=$url["user"]?>&oldvalue=<?=$alias?>&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?=$alias?>&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/mail.png" width="16" height="11" alt="<?=$LANG->_('Send testmail')?>" border="0"></a></td>
      </tr>
<?php
    }
}
?>
      <tr>
        <td class="subtitle" colspan="3"><a href="user_add_attribute.php?rootdn=<?=$url["rootdn"]?>&attrib=mailalternateaddress&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?=$LANG->_('Create new alias')?></a></td>
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
