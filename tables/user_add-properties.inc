<?php /* $Id: user_add-properties.inc,v 1.15 2005-03-06 18:45:04 turbo Exp $ */ ?>
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
            <?php echo pql_complete_constant($LANG->_('Profile \b%type%\B account'), array('type' => $templates[$i]["short"])); ?>: <?=$templates[$i]["long"]?>.
          </table>
        </td>
      </tr>

<?php } ?>

      <tr>
        <td></td>
        <td>
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
	        <?=$LANG->_('An \bAlias object\B is a \ilink\I from one object to the real object - in LDAP terms: this is called a \ureference\U.')?>
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
