<?php
// attribute plugin for
// confirmText and moderatorText (Qmail-GROUP object texts)
// $Id: attrib.confirmtexts.inc,v 2.7 2007-02-15 12:07:17 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;

	// Nothing to check - return true
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    global $_pql;
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($_REQUEST["attrib"] == "confirmtext") { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Sender confirmation text')?>
<?php } else { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Moderator confirmation text')?>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td valign="center">
            <textarea name="<?php echo $_REQUEST["attrib"]?>" cols="40" rows="10"><?=$_REQUEST["oldvalue"]?></textarea>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"       value="1">
    <input type="hidden" name="attrib"       value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue"     value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"       value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"       value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"         value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"         value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>

<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	$entry = array();
	$attrib = $_REQUEST["attrib"];

	if(!empty($_REQUEST[$attrib]))
	  $entry[$attrib] = preg_replace('/\\\/', "", $_REQUEST[$attrib]);
	
	if($_REQUEST["attrib"] == "confirmtext") {
		$msg = $LANG->_('Sender confirmation text');
	} else {
		$msg = $LANG->_('Moderator confirmation text');
	}

	if(pql_modify_attribute($_REQUEST["user"], '', '', $entry)) 
	  $msg = pql_complete_constant($LANG->_('successfully changed %what%'),
								   array('what' => $msg));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $msg)) . ": " . pql_format_error(0);
	
	attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
