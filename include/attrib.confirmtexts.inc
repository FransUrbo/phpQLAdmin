<?php
// attribute plugin for
// confirmText and moderatorText (Qmail-GROUP object texts)
// $Id: attrib.confirmtexts.inc,v 2.1 2004-05-07 04:43:17 turbo Exp $

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
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($_REQUEST["attrib"] == "confirmtext") { ?>
      <th colspan="3" align="left"><?=$LANG->_('Sender confirmation text')?>
<?php } else { ?>
      <th colspan="3" align="left"><?=$LANG->_('Moderator confirmation text')?>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td valign="center">
            <textarea name="<?=$_REQUEST["attrib"]?>" cols="40" rows="10"><?=$oldvalue?></textarea>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"       value="1">
    <input type="hidden" name="attrib"       value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue"     value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"       value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"       value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"         value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"         value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
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
//	else {
//		// Check if the entry in database is empty. 
//		// If not, we have choosen to DELETE the value
//		$mrt_db = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], $attrib);
//		if($mrt_db[0])
//		  pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], $attrib);
//	}

	if($_REQUEST["attrib"] == "confirmtext") {
		$msg = $LANG->_('Sender confirmation text');
	} else {
		$msg = $LANG->_('Moderator confirmation text');
	}

	if(pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], '', $entry)) 
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
