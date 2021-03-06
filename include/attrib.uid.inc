<?php
// attribute plugin for
// uid (user id)
// $Id: attrib.uid.inc,v 2.29 2007-09-29 21:15:13 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type){
    global $error, $LANG, $_pql;
	
	// is typed in ?
	if(empty($_REQUEST["uid"])) {
		$error[pql_get_define("PQL_ATTR_MAIL")] = $LANG->_('Missing');
		return false;
	}

	// valid
	if(preg_match("/[^a-z0-9\.@_-]/i",$_REQUEST["uid"])) {
		$error["uid"] = $LANG->_('Invalid');
		return false;
	}

	// if type is modify, check existance of old value, else the new one
	$filter = "uid=".pql_maybe_encode($_REQUEST["uid"], "uid", $_pql->ldap_linkid);
	if($_REQUEST["oldvalue"] != $_REQUEST["uid"] and $_pql->search($_REQUEST["domain"], $filter)) {
		$error["uid"] = $LANG->_('Already exists');
		return false;
	}
	
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["uid"] = (!empty($_GET["oldvalue"]) ? $_GET["oldvalue"] : "");
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $LANG, $error;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Change username')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Username')?></td>
          <td><?php echo pql_format_error_span($error["uid"]); ?><input type="text" name="uid" value="<?php echo $_REQUEST["uid"]?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo $LANG->_('The username can only contain numbers, letters and following special characters @, . (dot), _, -. Additionally, the username should be unique in the system')?>
          </td>
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
		// no add operation possible with uid
		break;

	  case "modify":
		if($_REQUEST["uid"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Username')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_UID"), '', $_REQUEST["uid"])) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% from %old% to %new%'),
										 array('what' => $LANG->_('username'),
											   'old'  => $_REQUEST["oldvalue"],
											   'new'  => $_REQUEST["uid"]));
			if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", pql_get_rootdn($_REQUEST["user"], 'attrib.uid.inc:attribute_save')) == sprintf("%s", pql_get_define("PQL_ATTR_UID"))) {
				// TODO: We use the uid to reference users and it has just changed...

				$_REQUEST["user"] = $_REQUEST["uid"];
				// Reload the navigation bar (if allowed by config)
				attribute_forward($msg, true);
			}
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('username'))) . ": " . pql_format_error(0);
		
			attribute_forward($msg);
		}
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
