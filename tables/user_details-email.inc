<?php $email   = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MAIL"]); $email = $email[0];
      $aliases = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MAILALTERNATE"]);
?>
  <!-- Addresses (mail, mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_LANG_ADDRESS_REGISTRED; ?></th>
      <tr>
        <td class="title"><?php echo PQL_LANG_ADDRESS_TYPE; ?></td>
        <td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
        <td class="title"><?php echo PQL_LANG_OPTIONS; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo PQL_LANG_MAIL_TITLE; ?></td>
        <td><a href="mailto:<?=$email?>"><?=$email?></a></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mail&user=<?=$user?>&mail=<?=$email?>&oldvalue=<?=$email?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_MAIL_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;&nbsp;
<?php } ?>
          <a href="user_sendmail.php?email=<?=$email?>&user=<?=$user?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_LANG_SENDMAIL; ?>" border="0"></a>
        </td>
      </tr>

<?php
if(is_array($aliases)){
    asort($aliases);
    foreach($aliases as $alias){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo PQL_LANG_MAILALTERNATEADDRESS_TITLE; ?></td>
        <td><a href="mailto:<?=$alias?>"><?=$alias?></a></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailalternateaddress&domain=<?=$domain?>&user=<?=$user?>&mailalternateaddress=<?php echo pql_strip_domain($alias); ?>&oldvalue=<?=$alias?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_MAILALTERNATEADDRESS_CHANGE; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=mailalternateaddress&user=<?=$user?>&oldvalue=<?=$alias?>"><img src="images/del.png" width="12" height="12" alt="<?php echo PQL_LANG_MAILALTERNATEADDRESS_DEL; ?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?=$alias?>&user=<?=$user?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo PQL_LANG_SENDMAIL; ?>" border="0"></a></td>
      </tr>
<?php
    }
}
?>
      <tr>
        <td class="subtitle" colspan="3"><a href="user_add_attribute.php?attrib=mailalternateaddress&domain=<?=$domain?>&user=<?=$user?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?php echo PQL_LANG_MAILALTERNATEADDRESS_NEW; ?></a></td>
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
