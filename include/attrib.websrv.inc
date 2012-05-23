<?php
// attribute plugin for the Webserver Configuration editor
//
// $Id: attrib.websrv.inc,v 1.20 2007-09-29 21:15:13 turbo Exp $

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
 	$url["host"]   = pql_format_urls($_REQUEST["host"]);
	$url["server"] = pql_format_urls($_REQUEST["server"]);

	// Slightly ugly, but what the heck! :)
	$attr_val	   = $_REQUEST[$_REQUEST["attrib"]];
?>
    <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?php echo $LANG->_('Change webserver configuration')?></th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Server configuration DN')?>:</td>
            <td><?php echo $_REQUEST["server"]?></td>
          </tr>

<?php if($_REQUEST["oldvalue"]) { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Old value')?>:</td>
            <td><?php echo $_REQUEST["oldvalue"]?></td>
          </tr>

<?php } ?>
          <tr class="<?php pql_format_table(); ?>">
<?php if($_REQUEST["oldvalue"]) { ?>
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $_REQUEST["attrib"])); ?></td>
<?php } else { ?>
            <td class="title"><?php echo pql_complete_constant($LANG->_('Set attribute %attribute%'), array('attribute' => $_REQUEST["attrib"])); ?></td>
<?php } ?>
            <td><input type="text" name="<?php echo $_REQUEST["attrib"]?>" value="<?php echo $attr_val?>" size="50"></td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
      <input type="hidden" name="rootdn"   value="<?php echo $url["rootdn"]?>">
      <input type="hidden" name="domain"   value="<?php echo $url["domain"]?>">
      <input type="hidden" name="host"     value="<?php echo $url["host"]?>">
      <input type="hidden" name="server"   value="<?php echo $url["server"]?>">
<?php if($_REQUEST["virthost"]) { ?>
      <input type="hidden" name="virthost" value="<?php echo $_REQUEST["virthost"]?>">
<?php } ?>
<?php if($_REQUEST["hostdir"]) { ?>
      <input type="hidden" name="hostdir"  value="<?php echo $_REQUEST["hostdir"]?>">
<?php } ?>
      <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
      <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
      <input type="hidden" name="action"   value="<?php echo $_REQUEST["action"]?>">
      <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
      <br>
      <input type="submit" value="<?php echo $LANG->_('Save')?>">
    </form>
<?php
}
// }}}

// {{{ attribute_save(action)
function attribute_save($action) {
    global $_pql, $LANG;

	// {{{ Setup the resulting DN
	// ... depending on if it's a server, virtual host or a location object
	$dn = pql_format_normalize_dn($_REQUEST["server"]);
	if($_REQUEST["virthost"])
	  $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$_REQUEST["virthost"].",".$dn;
	if($_REQUEST["hostdir"])
	  $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")."=".$_REQUEST["hostdir"].",".$dn;
	// }}}

	// Just so that we can avoid using '$_REQUEST[$_REQUEST["attrib"]]'.
	$attrib = $_REQUEST["attrib"];

    switch($action) {
	  case "add":
		// {{{ Add a value
		// {{{ Setup the object class filter
		$ocs = pql_split_oldvalues(pql_get_define("PQL_CONF_WEBSRV_OC"));
		$filter = "(&($attrib=*)(|";
		for($i=0; $i < count($ocs); $i++) {
		  $filter .= '(objectClass='.$ocs[$i].')';
		}
		$filter .= "))";
		// }}}

		// {{{ Fetch the old values from the DB
		$sr   = ldap_read($_pql->ldap_linkid, $dn, $filter, array($attrib));
		$info = ldap_get_entries($_pql->ldap_linkid, $sr)
		  or pql_format_error(0);
		for($i=0; $i < $info[0][$attrib]["count"]; $i++) {
			// Add the old value(s) to the array
			$entry[$attrib][] = $info[0][$attrib][$i];
		}
		// }}}

		// Add the new value to the array
		$entry[$attrib][] = $_REQUEST[$attrib];

		// Create a LDIF object to print in case of error
		$LDIF = pql_create_ldif("attrib.websrv.inc", $dn, $entry);
		if(pql_modify_attribute($dn, '', '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully added %what%'),
									   array('what' => $_REQUEST[$attrib])).".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to add %what%'),
									   array('what' => $_REQUEST[$attrib])) . ": "
										 . ldap_error($_pql->ldap_linkid);
		break;
		// }}}

	  case "mod":
		// {{{ Modify a avalue
		if(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER"))) and preg_match('/^ldapi/i', $_REQUEST[$attrib])) {
			// It is an LDAP URI - urlencode.
			
			// Extract the path 
			$uri = eregi_replace('ldapi://', '', $_REQUEST[$attrib]);
			$_REQUEST[$attrib] = 'ldapi://' . urlencode($uri);
		}

 		if(pql_modify_attribute($dn, $attrib, 1, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
		// }}}

	  case "del":
		// {{{ Delete a value
		$entry[$attrib] = array();

		if(ldap_mod_del($_pql->ldap_linkid, $dn, $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
									   array('value' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
									   array('value' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
		// }}}

	  case "toggle":
		// {{{ A togglable value.
		$oldvalue = $_REQUEST[$attrib];
		if(!$oldvalue) {
		  settype($oldvalue, "boolean");
		  $oldvalue = 'off';
		}
		
		// Reverse state
		if($oldvalue == 'on')
		  $newvalue = 'off';
		elseif($oldvalue == 'off')
		  $newvalue = 'on';

		if(pql_modify_attribute($dn, $attrib, $oldvalue, $newvalue))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
		// }}}
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
