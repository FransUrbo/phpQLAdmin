<?php
// control attribute plugin for
// ldaplocaldelivery (search for local account, if ldap search is not successful)
//
// $Id: attrib.control.ldaplocaldelivery.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $

function ldaplocaldelivery_check($type){
	// check values
	return true;
}

function ldaplocaldelivery_init(){
	// init values
  global $_pql_control, $ldaplocaldelivery;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "ldaplocaldelivery");

	if(!is_null($value)){
		$ldaplocaldelivery = $value[0];
	} else {
		// set to default value
		$ldaplocaldelivery = 1;
  }

}

function ldaplocaldelivery_print_view(){
	global $ldaplocaldelivery;

	// init data
	ldaplocaldelivery_init();

	//
	if($ldaplocaldelivery == 0){
		$ldaplocaldelivery = "Disabled, don't lookup for local users if ldap lookup didn't match";
  } else {
		$ldaplocaldelivery = "Enabled, lookup for local users if ldap lookup didn't match";
  }

	?>
	<table cellspacing="0" cellpadding="3" border="0">
		<th colspan="2" align="left">ldaplocaldelivery (Deliver locally if ldap didn't match a user)</th>
		<tr class="<?php table_bgcolor(); ?>">
			<td><?php echo $ldaplocaldelivery; ?></td>
		</tr>
		<tr class="subtitle">
				<td><a href="control_edit_attribute?attrib=ldaplocaldelivery&set=1">enable local lookup</a> |
				<a href="control_edit_attribute?attrib=ldaplocaldelivery&set=0">disable local lookup</a>
			</td>
		</tr>
	</table>

	<?php
}

function ldaplocaldelivery_print_form(){
	// don't print form, save directly
	ldaplocaldelivery_save("modify");
}

function ldaplocaldelivery_save($type){
	global $_pql_control, $set;

	// recontrol values
	if(!ldaplocaldelivery_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "ldaplocaldelivery", $set)){
				$msg = "Successfully saved ldaplocaldelivery options";
    	} else {
				$msg = "Failed saving ldaplocaldelivery options: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}
