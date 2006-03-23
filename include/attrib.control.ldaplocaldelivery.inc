<?php
// control attribute plugin for
// ldaplocaldelivery (search for local account, if ldap search is not successful)
//
// $Id: attrib.control.ldaplocaldelivery.inc,v 2.18.2.1 2006-03-23 09:13:15 turbo Exp $

// {{{ ldaplocaldelivery_check(type)
function ldaplocaldelivery_check($type) {
	return true;
}
// }}}

// {{{ ldaplocaldelivery_init(host)
// init values
function ldaplocaldelivery_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["ldaplocaldelivery"] = pql_get_attribute($_pql_control->ldap_linkid,
													   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													   pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY"));

	if(@is_null($_REQUEST["ldaplocaldelivery"] ))
	  return true;
}
// }}}


// {{{ ldaplocaldelivery_print_view(host)
function ldaplocaldelivery_print_view($host) {
	global $LANG;

	// init data
	ldaplocaldelivery_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldaplocaldelivery (<?=$LANG->_('Deliver locally if ldap didn\'t match a user')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Local deliveries')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["ldaplocaldelivery"]) { echo $LANG->_('Yes'); } else { echo $LANG->_('No'); }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY")?>&<?=pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY")?>=<?php if(@$_REQUEST["ldaplocaldelivery"]) { echo "0"; } else { echo "1"; }?>&type=modify&submit=1"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldaplocaldelivery_print_form(void)
function ldaplocaldelivery_print_form() {
	// don't print form, save directly
	ldaplocaldelivery_save("modify", $_REQUEST["mxhost"]);
}
// }}}

// {{{ ldaplocaldelivery_save(type, host)
function ldaplocaldelivery_save($type, $host) {
	global $_pql_control, $LANG;
	$attrib = $_REQUEST["attrib"];

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY"), 1, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldaplocaldelivery options')));
		else {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldaplocaldelivery options')));
		}

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
