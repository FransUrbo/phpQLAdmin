<?php $email   = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAIL")); $email = $email[0];
      $aliases = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"));
?>
  <!-- Addresses (mail, mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Registred addresses')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Type')?></td>
        <td class="title"><?=$LANG->_('Email')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$LANG->_('Main address')?></td>
        <td><a href="mailto:<?=$email?>"><?=$email?></a></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mail&user=<?=$userdn?>&mail=<?=$email?>&oldvalue=<?=$email?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change main address')?>" border="0"></a>&nbsp;&nbsp;&nbsp;
<?php } ?>
          <a href="user_sendmail.php?email=<?=$email?>&user=<?=$userdn?>&view=<?=$view?>"><img src="images/mail.png" width="16" height="11" alt="<?=$LANG->_('Send testmail')?>" border="0"></a>
        </td>
      </tr>

<?php
if(is_array($aliases)){
    asort($aliases);
    foreach($aliases as $alias){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$LANG->_('Alias')?></td>
        <td><a href="mailto:<?=$alias?>"><?=$alias?></a></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailalternateaddress&domain=<?=$domain?>&user=<?=$userdn?>&mailalternateaddress=<?php echo pql_strip_domain($alias); ?>&oldvalue=<?=$alias?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailalternateaddress&user=<?=$userdn?>&oldvalue=<?=$alias?>&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?=$alias?>&user=<?=$userdn?>&view=<?=$view?>"><img src="images/mail.png" width="16" height="11" alt="<?=$LANG->_('Send testmail')?>" border="0"></a></td>
      </tr>
<?php
    }
}
?>
      <tr>
        <td class="subtitle" colspan="3"><a href="user_add_attribute.php?rootdn=<?=$rootdn?>&attrib=mailalternateaddress&domain=<?=$domain?>&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?=$LANG->_('Create new alias')?></a></td>
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
