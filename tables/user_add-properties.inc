<?php /* $Id: user_add-properties.inc,v 1.4 2003-11-20 08:01:46 turbo Exp $ */ ?>
<form action="<?=$PHP_SELF?>" method="post" accept-charset="UTF-8">
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Account properties')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Type')?></td>

        <td>
          <select name="account_type">
            <option value="normal" SELECTED><?=$LANG->_('Mail account')?></option>
            <option value="system"><?=$LANG->_('System account')?></option>
            <option value="forward"><?=$LANG->_('Forward account')?></option>
            <option value="shell"><?=$LANG->_('Shell account')?></option>
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
    </th>
  </table>

  <input type="hidden" name="page_next" value="one">
  <input type="hidden" name="page_curr" value="">
  <input type="hidden" name="domain" value="<?=$domain?>">
  <input type="hidden" name="rootdn" value="<?=$rootdn?>">

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
