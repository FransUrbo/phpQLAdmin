<?php
// attribute for phpQLAdmin/Password Policies - user entries
// $Id: attrib.ppolicy.inc,v 2.2 2007-03-09 17:24:23 turbo Exp $

// {{{ attribute_check_ppolicy(void)
function attribute_check_ppolicy() {
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
  $ppolicies = pql_get_ppolicies();
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
  global $ppolicies, $LANG, $error;

  $attrib = $_REQUEST["attrib"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change password policy')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password policy')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
<?php if(is_array($ppolicies)) {
		for($i=0; $ppolicies[$i]; $i++) {
?>
            <input type="radio" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>">
<?php	} ?>
<?php } else { ?>
            <?=$LANG->_('No password policies defined')?>.
<?php } ?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
<?php if(is_array($ppolicies)) { ?>
    <input type="submit" value="<?=$LANG->_('Save')?>">
<?php } ?>
  </form>
<?php
}
// }}}

// {{{ attribute_save(void)
function attribute_save() {
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
