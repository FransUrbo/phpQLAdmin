<?php /* $Id: user_add-additional.inc,v 1.9 2004-03-01 10:46:49 turbo Exp $ */ ?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
    <input type="hidden" name="surname" value="<?=$_REQUEST["surname"]?>">
    <input type="hidden" name="name" value="<?=$_REQUEST["name"]?>">
    <input type="hidden" name="email" value="<?=$_REQUEST["email"]?>">
    <input type="hidden" name="uid" value="<?=$_REQUEST["uid"]?>">
<?php if($account_type != "forward") { ?>
    <input type="hidden" name="password" value="<?=$_REQUEST["password"]?>">
    <input type="hidden" name="pwscheme" value="<?=$_REQUEST["pwscheme"]?>">
    <input type="hidden" name="crypted" value="<?=$_REQUEST["crypted"]?>">
<?php } else { ?>
    <input type="hidden" name="forwardingaddress" value="<?=$_REQUEST["forwardingaddress"]?>">
<?php } ?>
    <input type="hidden" name="subbranch" value="<?=$_REQUEST["subbranch"]?>">
    <input type="hidden" name="email_domain" value="<?=$_REQUEST["email_domain"]?>">
    <input type="hidden" name="include_additional" value="<?=$_REQUEST["include_additional"]?>">

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Additional account properties')?></th>
        <!-- Home directory -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Path to homedirectory')?></td>
<?php if($error_text["homedirectory"]) { ?>
          <td><?php echo pql_format_error_span($error_text["homedirectory"]); ?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td><input type="text" name="homedirectory" size="30" value="/tmp/<?=$_REQUEST["uid"]?>"></td>
<?php } else { ?>
          <td><?=$_REQUEST["homedirectory"]?></td>
          <input type="hidden" name="homedirectory" value="<?=$_REQUEST["homedirectory"]?>">
<?php } ?>
        </tr>

<?php if($_REQUEST["account_type"] == "normal" or $_REQUEST["account_type"] == "system") {
		// display forms for SYSTEM/MAIL account(s)
?>
        <input type="hidden" name="loginshell" value="<?=$_REQUEST["loginshell"]?>">

        <!-- MailMessageStore -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Path to mailbox')?></td>
<?php	if($error_text["maildirectory"]) { ?>
          <td><?php echo pql_format_error_span($error_text["maildirectory"]); ?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td><input type="text" name="maildirectory" size="30" value="/tmp/<?=$_REQUEST["uid"]?>"></td>
<?php	} else { ?>
          <td><?=$_REQUEST["maildirectory"]?></td>
          <input type="hidden" name="maildirectory" value="<?=$_REQUEST["maildirectory"]?>">
<?php	} ?>
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
<?php	if(is_array($_REQUEST["userhost"]) or defined($_REQUEST["userhost"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mail server')?></td>
          <td>
<?php	if(is_array($_REQUEST["userhost"])) { ?>
            <input type="hidden" name="userhost" value="<?=$_REQUEST["userhost"][1]?>">
            <input type="radio" name="host" value="default" <?php if($_REQUEST["userhost"][0] and ($_REQUEST["host"] != "user")){ echo "checked";}?>>
<?php		} elseif(defined($_REQUEST["userhost"])) { ?>
            <input type="hidden" name="userhost" value="<?=$_REQUEST["userhost"]?>">
            <input type="radio" name="host" value="default" <?php if($_REQUEST["userhost"] and ($_REQUEST["host"] != "user")){ echo "checked";}?>>
<?php	}
		echo "            " . $LANG->_('Standard (DNS entry)') . ": <b>";

		if(is_array($_REQUEST["userhost"]))
		  echo $_REQUEST["userhost"][1];
			elseif(defined($_REQUEST["userhost"]))
		  echo $_REQUEST["userhost"];
			else
			  echo pql_format_error_span($error_text["userhost"]);

		echo "</b>\n";?>
          </td>
        </tr>
<?php	}

		if(!$_REQUEST["userhost"][0] and $_REQUEST["userhost"][1]) {
			// It's defined, but it comes from LDAP
?>

        <tr class="<?php pql_format_table(); ?>">
<?php	if(is_array($_REQUEST["userhost"]) or defined($_REQUEST["userhost"])) { ?>
          <td class="title"></td>
<?php	} else { $shown_mailserver_in_controls_object_question=1; ?>
          <td class="title"><?=$LANG->_('Mail server')?></td>
<?php	} ?>
          <td>
            <input type="radio" name="host" value="user" checked><?=$LANG->_('QmailLDAP/Controls object')?>: <b><?=$_REQUEST["userhost"][1]?></b>
          </td>
        </tr>
<?php	} ?>
  
        <tr class="<?php pql_format_table(); ?>">
<?php	if(is_array($_REQUEST["userhost"]) or defined($_REQUEST["userhost"]) or $shown_mailserver_in_controls_object_question) { ?>
          <td class="title"></td>
<?php	} else { ?>
          <td class="title"><?=$LANG->_('Mail server')?></td>
<?php	} ?>
          <td>
            <input type="radio" name="host" value="user" <?php if((!$_REQUEST["userhost"][0] and !$_REQUEST["userhost"][1]) or ($_REQUEST["host"] == "user")){ echo "checked";}?>>
            <?=$LANG->_('User defined')?>&nbsp;<input type="text" name="userhost" size="30"><br>
          </td>
        </tr>
<?php
	} // end of if-else
?>
      </th>
    </table>
  
    <br>

    <input type="hidden" name="account_type" value="<?=$_REQUEST["account_type"]?>">
    <input type="hidden" name="account_status" value="<?=$_REQUEST["account_status"]?>">
    <input type="hidden" name="page_curr" value="two">
    <input type="hidden" name="page_next" value="save">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
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
