<?php
// attribute plugin for
// cn (Full name)
// $Id: attrib.cn.inc,v 2.27 2005-04-15 13:03:57 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["cn"])) {
		$error[pql_get_define("PQL_ATTR_CN")] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
	if(!$_REQUEST["cn"])
	  $_REQUEST["cn"] = (empty($_GET["oldvalue"]) ? "" : $_GET["oldvalue"]);
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;

	if($_REQUEST["cn"] and !$_REQUEST["oldvalue"])
	  $_REQUEST["oldvalue"] = $_REQUEST["cn"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Common name')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Common name')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_CN")]); ?><input type="text" name="cn" value="<?=$_REQUEST["cn"]?>"></td>
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
<?php if(!$_REQUEST["cn"] and !$_REQUEST["oldvalue"] and ($_REQUEST["type"] == 'add')) { ?>
    <input type="hidden" name="type"     value="<?=$_REQUEST["type"]?>">
<?php } ?>
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	if($_REQUEST["type"])
	  $type = $_REQUEST["type"];
    
    switch($type) {
      case "add":
		// {{{ Add a value
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], '', $_REQUEST["cn"]))
		  $msg = pql_complete_constant($LANG->_('Successfully added %what%'),
									   array('what' => $LANG->_('Common name')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to added %what%'),
									   array('what' => $LANG->_('Common name')))
			. ":&nbsp;" . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg, true);
		break;
		// }}}
		break;

      case "modify":
		// {{{ Modify a value
		if($_REQUEST["cn"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Common name')));
			attribute_forward($msg);
			break;
    	}

		if($_REQUEST["oldvalue"])
		  // Replace specific value.
		  $old = $_REQUEST["oldvalue"];
		else
		  // Completley replace the attribute
		  $old = 1;
		
		// Replace the specified attribute
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $old, $_REQUEST["cn"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('Common name'),
											 'new'  => $_REQUEST["cn"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Common name')))
			. ":&nbsp;" . ldap_error($_pql->ldap_linkid);

		if(pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_GECOS")))
		  // The 'gecos' attribute exists in the object, try to change it. Ignore failures...
		  pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_GECOS"), 1, $_REQUEST["cn"]);

		// Get first and last name from the 'CN'.
		$name = split(" ", $_REQUEST["cn"]);

		// Change givenName which is the first name
		pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_GIVENNAME"), $old, $name[0]);

		// Change SN which is the last name
		pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_SN"), $old, $name[1]);

		attribute_forward($msg, true);
		break;
		// }}}

	  case "del":
		// {{{ Delete a value
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $_REQUEST["cn"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully deleted %what%'),
									   array('what' => $LANG->_('Common name')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to delete %what%'),
									   array('what' => $LANG->_('Common name')))
			. ":&nbsp;" . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg, true);
		break;
		// }}}

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
