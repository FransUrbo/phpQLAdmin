<?php
// attribute plugin for
// Samba Home UNC Path
// 

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["sambahomepath"])) {
		$error[pql_get_define("PQL_ATTR_CN")] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["sambahomepath"] = (empty($_GET["oldvalue"]) ? "" : $_GET["oldvalue"]);
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('UNC Path')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('(Example //alii/%USERNAME/profile)')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_CN")]); ?><input type="text" name="sambahomepath" value="<?php echo $_REQUEST["sambahomepath"]?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
    
    switch($type) {
      case "add":
		// no add operation possible with sambahomepath
		break;

      case "modify":
		if($_REQUEST["sambahomepath"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('UNC Path')));
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
		if(pql_modify_attribute($_REQUEST["user"], $_REQUEST["attrib"], $old, $_REQUEST["sambahomepath"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('UNC Path'),
											 'new'  => $_REQUEST["sambahomepath"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('UNC Path')))
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
