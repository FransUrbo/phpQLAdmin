<?php
// control attribute plugin for
// ldapcluster (clustering support)
//
// $Id: attrib.control.ldapcluster.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $

function ldapcluster_check($type){
	// check values
	return true;
}

function ldapcluster_init(){
	// init values
  global $_pql_control, $ldapcluster;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "ldapcluster");

	if(!is_null($value)){
		$ldapcluster = $value[0];
	} else {
		// set to default value
		$ldapcluster = 0;
  }

}

function ldapcluster_print_view(){
	global $ldapcluster;

	// init data
	ldapcluster_init();

	//
	if($ldapcluster == 0){
		$ldapcluster = "Disabled, single ldap server";
  } else {
		$ldapcluster = "Enabled, use native clustering";
  }

	?>
	<table cellspacing="0" cellpadding="3" border="0">
		<th colspan="2" align="left">ldapcluster (Clustering support)</th>
		<tr class="<?php table_bgcolor(); ?>">
			<td><?php echo $ldapcluster; ?></td>
		</tr>
		<tr class="subtitle">
			<td><a href="control_edit_attribute?attrib=ldapcluster&set=1">enable clustering</a> |
				<a href="control_edit_attribute?attrib=ldapcluster&set=0">disable clustering</a>
			</td>
		</tr>
		<tr class="subtitle">
			<td><img src="images/info.png" width="16" height="16" border="0"> Note: Clustering support must be compiled into qmail-ldap</td>
		</tr>
	</table>

	<?php
}

function ldapcluster_print_form(){
	// don't print form, save directly
	ldapcluster_save("modify");
}

function ldapcluster_save($type){
	global $_pql_control, $set;

	// recontrol values
	if(!ldapcluster_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "ldapcluster", $set)){
				$msg = "Successfully saved ldapcluster options";
    	} else {
				$msg = "Failed saving ldapcluster options: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}
