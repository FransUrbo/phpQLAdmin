<?php
// attribute plugin for the Webserver Configuration editor
//
// $Id: attrib.websrv.inc,v 1.6.4.1 2004-11-19 08:50:57 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
    global $_pql, $LANG;

    if($_REQUEST["attrib"] && ($_REQUEST[$_REQUEST["attrib"]] != $_REQUEST["oldvalue"]))
      return true;

    return false;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $_pql, $LANG;

	$url["domain"] = pql_format_urls($_REQUEST["domain"]);
	$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
	$url["server"] = pql_format_urls($_REQUEST["server"]);

	// Slightly ugly, but what the heck! :)
	$attr_val	   = $_REQUEST[$_REQUEST["attrib"]];
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_('Change webserver configuration')?></th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Server configuration DN')?>:</td>
            <td><?=$_REQUEST["server"]?></td>
          </tr>

<?php if($_REQUEST["oldvalue"]) { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Old value')?>:</td>
            <td><?=$_REQUEST["oldvalue"]?></td>
          </tr>

<?php } ?>
          <tr class="<?php pql_format_table(); ?>">
<?php if($_REQUEST["oldvalue"]) { ?>
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $_REQUEST["attrib"])); ?></td>
<?php } else { ?>
            <td class="title"><?php echo pql_complete_constant($LANG->_('Set attribute %attribute%'), array('attribute' => $_REQUEST["attrib"])); ?></td>
<?php } ?>
            <td><input type="text" name="<?=$_REQUEST["attrib"]?>" value="<?=$attr_val?>" size="50"></td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
      <input type="hidden" name="rootdn"   value="<?=$url["rootdn"]?>">
      <input type="hidden" name="domain"   value="<?=$url["domain"]?>">
      <input type="hidden" name="server"   value="<?=$url["server"]?>">
      <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
      <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
      <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
      <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
<?php
}
// }}}

// {{{ attribute_save(action)
function attribute_save($action) {
    global $_pql, $LANG;

	// Just so that we can avoid using '$_REQUEST[$_REQUEST["attrib"]]'.
	$attrib = $_REQUEST["attrib"];

    switch($action) {
	  case "add":
		// Fetch the old values from the DB
		$filter = "(&(objectclass=".pql_get_define("PQL_ATTR_WEBSRV_OC").")($attrib=*))";
		$sr   = ldap_read($_pql->ldap_linkid, $_REQUEST["server"], $filter, array($attrib));
		$info = ldap_get_entries($_pql->ldap_linkid, $sr)
		  or pql_format_error(0);
		for($i=0; $i < $info[0][$attrib]["count"]; $i++) {
			// Add the old value(s) to the array
			$entry[$attrib][] = $info[0][$attrib][$i];
		}

		// Add the new value to the array
		$entry[$attrib][] = $_REQUEST[$attrib];

		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["server"], '', '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $_REQUEST[$attrib])).".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $_REQUEST[$attrib])) . ": "
										 . ldap_error($_pql->ldap_linkid);
		break;

	  case "mod":
		if(($attrib == pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")) and eregi('^ldapi', $_REQUEST[$attrib])) {
			// It is an LDAP URI - urlencode.
			
			// Extract the path 
			$uri = eregi_replace('ldapi://', '', $_REQUEST[$attrib]);
			$_REQUEST[$attrib] = 'ldapi://' . urlencode($uri);
		}

		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["server"], $attrib, '', $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "del":
		$entry[$attrib] = array();

		if(ldap_mod_del($_pql->ldap_linkid, $_REQUEST["server"], $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
									   array('value' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
									   array('value' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
	}

    attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
