<?php
// attribute plugin for
// Samba Home Drive Letter
// 

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["sambahomedrive"])) {
		$error[pql_get_define("PQL_ATTR_CN")] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["sambahomedrive"] = $_GET["sambahomedrive"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Drive Letter')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Drive Letter (Example d:)')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_CN")]); ?><input type="text" name="sambahomedrive" value="<?=$_REQUEST["sambahomedrive"]?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
    
    switch($type) {
      case "add":
		// no add operation possible with sambahomedrive
		break;

      case "modify":
		if($_REQUEST["sambahomedrive"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Drive Letter')));
			attribute_forward($msg);
			break;
    	}
		
		if($_REQUEST["oldvalue"])
		  // Replace specific value.
		  $old = $_REQUEST["oldvalue"];
		else
		  // Completely replace the attribute
		  $old = 1;
		
		// {{{ Replace the specified attribute
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $old, $_REQUEST["sambahomedrive"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('Drive Letter'),
											 'new'  => $_REQUEST["sambahomedrive"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Drive Letter')))
			. ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		// }}}

		attribute_forward($msg, true);
		break;

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
