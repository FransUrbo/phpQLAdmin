<?php
// control attribute plugin for
// ldaplocaldelivery (search for local account, if ldap search is not successful)
//
// $Id: attrib.control.ldaplocaldelivery.inc,v 2.12 2004-02-14 14:25:35 turbo Exp $

// check values
function ldaplocaldelivery_check($type) {
	global $view;
	return true;
}

// init values
function ldaplocaldelivery_init($host) {
	global $_pql_control, $ldaplocaldelivery, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_LDAPLOCALDELIVERY"));

	if(!is_null($value))
	  $ldaplocaldelivery = $value[0];
	else
	  // set to default value
	  $ldaplocaldelivery = 1;
}

function ldaplocaldelivery_print_view($host) {
	global $ldaplocaldelivery, $view, $LANG;

	// init data
	ldaplocaldelivery_init($host);
	
	if($ldaplocaldelivery == 0)
	  $ldaplocaldelivery = $LANG->_('Disabled. Don\'t lookup for local users if ldap lookup didn\'t match');
	else
	  $ldaplocaldelivery = $LANG->_('Enabled. Lookup for local users if ldap lookup didn\'t match');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldaplocaldelivery (<?=$LANG->_('Deliver locally if ldap didn\'t match a user')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$ldaplocaldelivery?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaplocaldelivery&set=1"><?=$LANG->_('Enable local lookup')?></a> |
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaplocaldelivery&set=0"><?=$LANG->_('Disable local lookup')?></a>
        </td>
      </tr>
    </th>
  </table>
<?php
}

function ldaplocaldelivery_print_form() {
	global $mxhost, $view;

	// don't print form, save directly
	ldaplocaldelivery_save("modify", $mxhost);
}

function ldaplocaldelivery_save($type, $host) {
	global $_pql_control, $set, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_LDAPLOCALDELIVERY"),
										 $set))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldaplocaldelivery options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldaplocaldelivery options')))
			. ": " . pql_format_error(0);

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
