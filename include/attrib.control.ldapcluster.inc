<?php
// control attribute plugin for
// ldapcluster (clustering support)
//
// $Id: attrib.control.ldapcluster.inc,v 2.10 2004-01-01 12:35:33 turbo Exp $

// check values
function ldapcluster_check($type) {
	global $view;
	return true;
}

// init values
function ldapcluster_init() {
	global $_pql_control, $ldapcluster, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_LDAPCLUSTER"));
	
	if(!is_null($value))
	  $ldapcluster = $value[0];
	else
	  // set to default value
	  $ldapcluster = 0;
}

function ldapcluster_print_view($host) {
	global $ldapcluster, $view, $LANG;

	// init data
	ldapcluster_init();

	if($ldapcluster == 0)
	  $ldapcluster = $LANG->_('Disabled, single ldap server');
	else
	  $ldapcluster = $LANG->_('Enabled, use native clustering');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldapcluster (<?=$LANG->_('Clustering support')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$ldapcluster?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapcluster&set=1"><?=$LANG->_('Enable clustering')?></a> |
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapcluster&set=0"><?=$LANG->_('Disable clustering')?></a>
        </td>
      </tr>

      <tr class="subtitle">
        <td><img src="images/info.png" width="16" height="16" border="0"> Note: Clustering support must be compiled into qmail-ldap</td>
      </tr>
    </th>
  </table>
<?php
}

function ldapcluster_print_form() {
	global $mxhost, $view;

	// don't print form, save directly
	ldapcluster_save("modify", $mxhost);
}

function ldapcluster_save($type, $host) {
	global $_pql_control, $set, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_LDAPCLUSTER"),
										 $set))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapcluster options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapcluster options')))
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
