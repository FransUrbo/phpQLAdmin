<?php /* $Id: user_add-properties.inc,v 1.13.8.1 2005-02-12 05:19:17 turbo Exp $ */ ?>
<form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Account properties')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Type')?></td>

        <td>
          <select name="template">
<?php for($i=0; $templates[$i]; $i++) { ?>
            <option value="<?=$templates[$i]["name"]?>"><?=$templates[$i]["short"]?></option>
<?php } ?>
<?php if($_SESSION["NEW_STYLE_QUOTA"]) { ?>
            <option value="group"><?=$LANG->_('Group account')?></option>
<?php } ?>
            <option value="alias"><?=$LANG->_('Alias object')?></option>
          </select>
        </td>
      </tr>

<?php for($i=0; $templates[$i]; $i++) { ?>
      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account'), array('type' => $templates[$i]["name"])); ?>: <?=$templates[$i]["long"]?>.
          </table>
        </td>
      </tr>

<?php } ?>
<?php if($_SESSION["NEW_STYLE_QUOTA"]) { ?>
      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <?php echo pql_complete_constant($LANG->_('Profile %type% account'), array('type' => $LANG->_('group'))); ?>: <?=$LANG->_('Group delivery with verification (~ mailinglist)')?>.
          </table>
        </td>
      </tr>

<?php } ?>

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
