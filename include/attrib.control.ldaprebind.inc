<?php
// control attribute plugin for
// ldaprebind (don't retrieve password, try to bind to dn)
//
// $Id: attrib.control.ldaprebind.inc,v 2.17 2005-06-22 13:58:33 turbo Exp $

// {{{ ldaprebind_check(type)
function ldaprebind_check($type) {
	return true;
}
// }}}

// {{{ ldaprebind_init(host)
// init values
function ldaprebind_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$_REQUEST["ldaprebind"] = pql_get_attribute($_pql_control->ldap_linkid,
												pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												pql_get_define("PQL_ATTR_LDAPREBIND"));
	if(is_null($_REQUEST["ldaprebind"]))
	  return true;
}
// }}}


// {{{ ldaprebind_print_view(host)
function ldaprebind_print_view($host) {
	global $LANG;
	
	// init data
	ldaprebind_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldaprebind (<?=$LANG->_('Rebind to LDAP server')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Do rebind')?></td>
        <td class="<?=$class?>"><?php if($_REQUEST["ldaprebind"]) { echo $LANG->_('Yes'); } else { echo $LANG->_('No'); }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPREBIND")?>&<?=pql_get_define("PQL_ATTR_LDAPREBIND")?>=<?php if($_REQUEST["ldaprebind"]) { echo "0"; } else { echo "1"; }?>&submit=1&type=modify"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_LDAPREBIND")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ ldaprebind_print_form(host)
function ldaprebind_print_form($host) {
	// don't print form, save directly
	ldaprebind_save("modify", $host);
}
// }}}

// {{{ ldaprebind_save(type, host)
function ldaprebind_save($type, $host) {
	global $_pql_control, $LANG;
	$attrib = $_REQUEST["attrib"];

	switch($type) {
		case "modify":
		case "add":
			// save entries
			if(pql_modify_attribute($_pql_control->ldap_linkid,
									pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
									pql_get_define("PQL_ATTR_LDAPREBIND"), 1, $_REQUEST[$attrib]))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('ldaprebind options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('ldaprebind options')))
				. ": " . pql_format_error(0);
		
		attribute_forward($msg);
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
