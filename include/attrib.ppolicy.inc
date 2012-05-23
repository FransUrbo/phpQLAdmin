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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Change password policy')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Password policy')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
<?php if(is_array($ppolicies)) {
		for($i=0; $ppolicies[$i]; $i++) {
?>
            <input type="radio" name="<?php echo $attrib?>" value="<?php echo $_REQUEST[$attrib]?>">
<?php	} ?>
<?php } else { ?>
            <?php echo $LANG->_('No password policies defined')?>.
<?php } ?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?php echo $attrib?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <br>
<?php if(is_array($ppolicies)) { ?>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
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
