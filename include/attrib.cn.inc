<?php
// attribute plugin for
// cn (Full name)
// $Id: attrib.cn.inc,v 2.20 2004-03-11 18:13:41 turbo Exp $

function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["cn"])) {
		$error[pql_get_define("PQL_ATTR_CN")] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}

function attribute_init() {
    $_REQUEST["cn"] = (empty($_GET["oldvalue"]) ? "" : $_GET["oldvalue"]);
}

function attribute_print_form() {
    global $error, $LANG;
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
    <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"     value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $_pql, $LANG;
    
    switch($type) {
      case "add":
		// no add operation possible with cn
		break;

      case "modify";
		if($_REQUEST["cn"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Common name')));
			attribute_forward($msg);
			break;
    	}
		
		// Well. Gecos is the same thing as CN, so we should
		// try to change that as well... But only if it
		// already exist in the object.
		$attribs = array($_REQUEST["attrib"], 'gecos');
		foreach($attribs as $attrib) {
			$old = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"]);
			if(is_array($old)) {
				// We have an old value of this array, try to change it...
				if(pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $_REQUEST["cn"]))
				  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
											   array('what' => $LANG->_('Common name'),
													 'new'  => $_REQUEST["cn"]));
				else
				  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
											   array('what' => $LANG->_('Common name')))
					. ":&nbsp;" . ldap_error($_pql->ldap_linkid);
			}
		}

		/* DLW: Bug: This is broken I think.  It's changing parts of the name, but not the CN.
		 *      Does it want to change the CN? */
		// Get first and last name
		$name = split(" ", $_REQUEST["cn"]);

		// Change givenName which is the first name
		$old = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_GIVENNAME"));
		if(is_array($old)) {
			// We have an old value of this array, try to change it...
			pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_GIVENNAME"), $name[0]);
		}

		// Change SN which is the last name
		$old = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_SN"));
		if(is_array($old)) {
			// We have an old value of this array, try to change it...
			pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_SN"), $name[1]);
		}

		attribute_forward($msg, true);
		break;

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
