<?php
// control attribute plugin for
// ldaprebind (don't retrieve password, try to bind to dn)
//
// $Id: attrib.control.ldaprebind.inc,v 2.4 2003-05-01 10:18:26 turbo Exp $

function ldaprebind_check($type){
	// check values
	return true;
}

function ldaprebind_init($host){
	// init values
	global $_pql_control, $ldaprebind;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "ldaprebind");

	if(!is_null($value)){
		$ldaprebind = $value[0];
	} else {
		// set to default value
		$ldaprebind = 0;
  }
}

function ldaprebind_print_view($host) {
	global $ldaprebind;
	
	// init data
	ldaprebind_init($host);
	
	//
	if($ldaprebind == 0){
		$ldaprebind = "Disabled, don't rebind, retrieve password (Default)";
	} else {
		$ldaprebind = "Enabled, rebind with retrieved DN, don't retrieve password";
	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldaprebind (Rebind to LDAP server)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $ldaprebind; ?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute?attrib=ldaprebind&set=1">enable rebind</a> |
          <a href="control_edit_attribute?attrib=ldaprebind&set=0">disable rebind</a>
        </td>
	  </tr>
    </th>
  </table>

<?php
}

function ldaprebind_print_form($host) {
	// don't print form, save directly
	ldaprebind_save("modify", $host);
}

function ldaprebind_save($type, $host) {
	global $_pql_control, $set;

	// recontrol values
	if(!ldaprebind_check($type)){
 		return false;
	}

	switch($type){
		case "modify";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 "ldaprebind", $set)){
				$msg = "Successfully saved ldaprebind options";
			} else {
				$msg = "Failed saving ldaprebind options: " . pql_ldap_error($_pql_control->ldap_linkid);
			}
		
		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
