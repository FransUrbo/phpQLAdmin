<?php
// control attribute plugin for
// ldaplocaldelivery (search for local account, if ldap search is not successful)
//
// $Id: attrib.control.ldaplocaldelivery.inc,v 2.6 2003-05-28 07:23:37 turbo Exp $

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
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "ldaplocaldelivery");

	if(!is_null($value)){
		$ldaplocaldelivery = $value[0];
	} else {
		// set to default value
		$ldaplocaldelivery = 1;
	}
}

function ldaplocaldelivery_print_view($host) {
	global $ldaplocaldelivery, $view;

	// init data
	ldaplocaldelivery_init($host);
	
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
        <td>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaplocaldelivery&set=1">enable local lookup</a> |
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldaplocaldelivery&set=0">disable local lookup</a>
        </td>
      </tr>
    </th>
  </table>

<?php
}

function ldaplocaldelivery_print_form(){
	global $mxhost, $view;

	// don't print form, save directly
	ldaplocaldelivery_save("modify", $mxhost);
}

function ldaplocaldelivery_save($type, $host) {
	global $_pql_control, $set, $view;

	// recontrol values
	if(!ldaplocaldelivery_check($type)){
 		return false;
	}
	
	switch($type){
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "ldaplocaldelivery", $set)){
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
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
