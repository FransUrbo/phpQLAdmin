<?php /* $Id: user_add-additional.inc,v 1.3 2003-11-15 12:24:31 turbo Exp $ */ ?>
  <form action="<?=$PHP_SELF?>" method="post" accept-charset="UTF-8">
    <input type="hidden" name="surname" value="<?=$surname?>">
    <input type="hidden" name="name" value="<?=$name?>">
    <input type="hidden" name="email" value="<?=$email?>">
    <input type="hidden" name="uid" value="<?=$uid?>">
<?php if($account_type != "forward") { ?>
    <input type="hidden" name="password" value="<?=$password?>">
    <input type="hidden" name="pwscheme" value="<?=$pwscheme?>">
    <input type="hidden" name="crypted" value="<?=$crypted?>">
<?php } ?>
    <input type="hidden" name="subbranch" value="<?=$subbranch?>">
    <input type="hidden" name="email_domain" value="<?=$email_domain?>">
    <input type="hidden" name="include_additional" value="<?=$include_additional?>">

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Additional account properties')?></th>
<?php
	if($account_type == "normal" or $account_type == "system") {
		// display forms for SYSTEM/MAIL account(s)
?>
        <input type="hidden" name="loginshell" value="<?=$loginshell?>">

        <!-- MailMessageStore -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Path to mailbox')?></td>
          <td><?=$maildirectory?></td>
        </tr>

        <tr></tr>

        <!-- Mailhost -->
<?php
		if($error_text["userhost"]) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td><?php echo format_error($error_text["userhost"]); ?></td>
        </tr>
<?php
		}
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Mail server')?></td>
          <td>
<?php	if(is_array($userhost)) { ?>
            <input type="hidden" name="userhost" value="<?=$userhost[1]?>">
            <input type="radio" name="host" value="default" <?php if($userhost[0] and ($host != "user")){ echo "checked";}?>>
<?php	} else { ?>
            <input type="hidden" name="userhost" value="<?=$userhost?>">
            <input type="radio" name="host" value="default" <?php if($userhost and ($host != "user")){ echo "checked";}?>>
<?php	}
		echo "            " . $LANG->_('Standard (DNS entry)') . ": <b>";
		if(is_array($userhost))
		  echo $userhost[1];
		else
		  echo $userhost;

		echo "</b>\n";?>
          </td>
        </tr>
<?php
		if(!$userhost[0] and $userhost[1]) {
			// It's defined, but it comes from LDAP
?>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="host" value="user" checked><?=$LANG->_('QmailLDAP/Controls object')?>: <b><?=$userhost[1]?></b>
          </td>
        </tr>
<?php
		}
?>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <input type="radio" name="host" value="user" <?php if((!$userhost[0] and !$userhost[1]) or ($host == "user")){ echo "checked";}?>>
            <?=$LANG->_('User defined')?>&nbsp;<input type="text" name="userhost" size="30"><br>
          </td>
        </tr>
<?php
	} // end of if-else
?>
      </th>
    </table>
  
    <br>

<?php	if(($account_type == "normal") and $maildirectory) {
			$homedirectory = $maildirectory;
?>
    <input type="hidden" name="homedirectory" value="<?=$homedirectory?>">
<?php	}

		if($account_type != "forward") {
?>
    <input type="hidden" name="maildirectory" value="<?=$maildirectory?>">
<?php	}?>
    <input type="hidden" name="account_type" value="<?=$account_type?>">
    <input type="hidden" name="account_status" value="<?=$account_status?>">
    <input type="hidden" name="page_curr" value="two">
    <input type="hidden" name="page_next" value="save">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
