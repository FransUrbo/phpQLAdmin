<?php /* $Id: user_add-properties.inc,v 1.13 2004-05-06 17:45:33 turbo Exp $ */ ?>
<form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Account properties')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Type')?></td>

        <td>
          <select name="account_type">
            <option value="mail" SELECTED><?=$LANG->_('Mail account')?></option>
<?php if($_SESSION["NEW_STYLE_QUOTA"]) { ?>
            <option value="group"><?=$LANG->_('Group account')?></option>
<?php } ?>
            <option value="system"><?=$LANG->_('System account')?></option>
            <option value="forward"><?=$LANG->_('Forward account')?></option>
            <option value="shell"><?=$LANG->_('Shell account')?></option>
            <option value="alias"><?=$LANG->_('Alias object')?></option>
          </select>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('mail'))); ?>: <?=$LANG->_('local mailbox, POP account')?>.
          </table>
        </td>
      </tr>

<?php if($_SESSION["NEW_STYLE_QUOTA"]) { ?>
      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('group'))); ?>: <?=$LANG->_('group delivery with verification (~ mailinglist)')?>.
          </table>
        </td>
      </tr>

<?php } ?>
      <tr>
        <td></td>
        <td colspan="2">
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('system'))); ?>: <?=$LANG->_('loginshell, homedirectory')?>, <?=$LANG->_('local mailbox, POP account')?>.
          </table>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('forward'))); ?>: <?=$LANG->_('forward only, no local mailbox')?>.
          </table>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account includes'), array('type' => $LANG->_('shell'))); ?>: <?=$LANG->_('loginshell, homedirectory, no mail etc')?>.
          </table>
        </td>
      </tr>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            An alias object is a <i>link</i> from one object to the real object. In LDAP terms,
            this is called a <u>reference</u>.
          </table>
        </td>
      </tr>
    </th>
  </table>

  <input type="hidden" name="page_next" value="one">
  <input type="hidden" name="page_curr" value="">
  <input type="hidden" name="domain"    value="<?=$url["domain"]?>">
  <input type="hidden" name="rootdn"    value="<?=$url["rootdn"]?>">
<?php if($_REQUEST["subbranch"]) { ?>
  <input type="hidden" name="subbranch" value="<?=urlencode(pql_get_define("PQL_ATTR_OU")."=".$_REQUEST["subbranch"].",".$_REQUEST["domain"])?>">
<?php } ?>

  <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
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
