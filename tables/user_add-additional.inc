<?php /* $Id: user_add-additional.inc,v 1.7 2004-02-14 14:25:41 turbo Exp $ */ ?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
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
<?php if($homedirectory) { ?>
        <!-- Home directory -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Home directory')?></td>
          <td><?=$homedirectory?></td>
        </tr>

<?php }
	if($account_type == "normal" or $account_type == "system") {
		// display forms for SYSTEM/MAIL account(s)
?>
        <input type="hidden" name="loginshell" value="<?=$loginshell?>">

        <!-- MailMessageStore -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Path to mailbox')?></td>
          <td><?=$maildirectory?></td>
        </tr>

        <tr></tr>

        <!-- Mailhost -->
<?php
		if($error_text["userhost"]) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td><?php echo pql_format_error_span($error_text["userhost"]); ?></td>
        </tr>
<?php
		}
?>
<?php	if(is_array($userhost) or defined($userhost)) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mail server')?></td>
          <td>
<?php	if(is_array($userhost)) { ?>
            <input type="hidden" name="userhost" value="<?=$userhost[1]?>">
            <input type="radio" name="host" value="default" <?php if($userhost[0] and ($host != "user")){ echo "checked";}?>>
<?php		} elseif(defined($userhost)) { ?>
            <input type="hidden" name="userhost" value="<?=$userhost?>">
            <input type="radio" name="host" value="default" <?php if($userhost and ($host != "user")){ echo "checked";}?>>
<?php	}
		echo "            " . $LANG->_('Standard (DNS entry)') . ": <b>";

		if(is_array($userhost))
		  echo $userhost[1];
			elseif(defined($userhost))
		  echo $userhost;
			else
			  echo pql_format_error_span($error_text["userhost"]);

		echo "</b>\n";?>
          </td>
        </tr>
<?php	}

		if(!$userhost[0] and $userhost[1]) {
			// It's defined, but it comes from LDAP
?>

        <tr class="<?php pql_format_table(); ?>">
<?php	if(is_array($userhost) or defined($userhost)) { ?>
          <td class="title"></td>
<?php	} else { $shown_mailserver_in_controls_object_question=1; ?>
          <td class="title"><?=$LANG->_('Mail server')?></td>
<?php	} ?>
          <td>
            <input type="radio" name="host" value="user" checked><?=$LANG->_('QmailLDAP/Controls object')?>: <b><?=$userhost[1]?></b>
          </td>
        </tr>
<?php	} ?>
  
        <tr class="<?php pql_format_table(); ?>">
<?php	if(is_array($userhost) or defined($userhost) or $shown_mailserver_in_controls_object_question) { ?>
          <td class="title"></td>
<?php	} else { ?>
          <td class="title"><?=$LANG->_('Mail server')?></td>
<?php	} ?>
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
