<?php
// control attribute plugin for
// ldaprebind (don't retrieve password, try to bind to dn)
//
// $Id: attrib.control.ldaprebind.inc,v 2.10.2.1 2003-12-09 16:18:25 dlw Exp $

// check values
function ldaprebind_check($type) {
	global $view;
	return true;
}

// init values
function ldaprebind_init($host) {
	global $_pql_control, $ldaprebind, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_LDAPREBIND"));

	if(!is_null($value))
	  $ldaprebind = $value[0];
	else
	  // set to default value
	  $ldaprebind = 0;
}

function ldaprebind_print_view($host) {
	global $ldaprebind, $view, $LANG;
	
	// init data
	ldaprebind_init($host);
	
	if($ldaprebind == 0)
	  $ldaprebind = $LANG->_('Disabled, don\'t rebind, retrieve password (Default)');
	else
	  $ldaprebind = $LANG->_('Enabled, rebind with retrieved DN, don\'t retrieve password');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldaprebind (<?=$LANG->_('Rebind to LDAP server')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$ldaprebind?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaprebind&set=1"><?=$LANG->_('Enable rebind')?></a> |
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaprebind&set=0"><?=$LANG->_('Disable rebind')?></a>
        </td>
	  </tr>
    </th>
  </table>

<?php
}

function ldaprebind_print_form($host) {
	global $view;

	// don't print form, save directly
	ldaprebind_save("modify", $host);
}

function ldaprebind_save($type, $host) {
	global $_pql_control, $set, $view, $LANG;

	switch($type) {
		case "modify";
		case "add";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_GLOB_ATTR_LDAPREBIND"),
											 $set))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('ldaprebind options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('ldaprebind options')))
				. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		
		attribute_forward($msg);
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
