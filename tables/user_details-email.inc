<?php $email   = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_MAIL"));
      $aliases = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_MAILALTERNATE"));

	  $tmp     = split('@', $email);
	  $mail	   = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);
?>
  <!-- Addresses (mail, mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Registred addresses')?></th>
      <tr>
        <td class="title"></td>
        <td class="title"><?php echo $LANG->_('Type')?></td>
        <td class="title"><?php echo $LANG->_('Email')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td></td>
        <td><?php echo $LANG->_('Main address')?></td>
        <td>
<?php if($email and $mail) { ?>
          <a href="mailto:<?php echo $email?>"><?php echo $mail?></a>
<?php } else { ?>
          <i><b>unset</b></i>
<?php } ?>
        </td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mail&user=<?php echo $url["user"]?>&mail=<?php echo $email?>&oldvalue=<?php echo $email?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change main address')?>" border="0"></a>&nbsp;&nbsp;&nbsp;
<?php } ?>
<?php if($email and $mail) { ?>
          <a href="user_sendmail.php?email=<?php echo $email?>&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo $LANG->_('Send testmail')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php
if(is_array($aliases)) {
	$email_number = 1;

    asort($aliases);
    foreach($aliases as $alias) {
		$tmp     = split('@', $alias);
		$mail	 = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td align="right"><?php echo $email_number?></td>
        <td><?php echo $LANG->_('Alias')?></td>
        <td><a href="mailto:<?php echo $alias?>"><?php echo $mail?></a></td>
        <td><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mailalternateaddress&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&mailalternateaddress=<?php echo pql_strip_domain($alias); ?>&oldvalue=<?php echo $alias?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mailalternateaddress&user=<?php echo $url["user"]?>&oldvalue=<?php echo $alias?>&view=<?php echo $_GET["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Delete alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?php echo $alias?>&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo $LANG->_('Send testmail')?>" border="0"></a></td>
      </tr>
<?php
		$email_number++;
    }
} elseif($aliases) {
	// Only one value
	$tmp  = split('@', $aliases);
	$mail = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td align="right">1</td>
        <td><?php echo $LANG->_('Alias')?></td>
        <td><a href="mailto:<?php echo $aliases?>"><?php echo $mail?></a></td>
        <td><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mailalternateaddress&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&mailalternateaddress=<?php echo pql_strip_domain($aliases); ?>&oldvalue=<?php echo $aliases?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mailalternateaddress&user=<?php echo $url["user"]?>&oldvalue=<?php echo $aliases?>&view=<?php echo $_GET["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Delete alias')?>" border="0"></a>&nbsp;&nbsp;<a href="user_sendmail.php?email=<?php echo $aliases?>&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/mail.png" width="16" height="11" alt="<?php echo $LANG->_('Send testmail')?>" border="0"></a></td>
      </tr>
<?php
}
?>
<?php if($email and $mail) { ?>
      <tr>
        <td class="subtitle" colspan="4"><a href="user_add_attribute.php?rootdn=<?php echo $url["rootdn"]?>&attrib=mailalternateaddress&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?php echo $LANG->_('Create new alias')?></a></td>
      </tr>
<?php } ?>
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
