<?php
// attribute plugin for
// accountstatus (status of mailaccount)
// $Id: attrib.accountstatus.inc,v 2.5 2003-04-04 16:37:50 turbo Exp $

function attribute_check($type){
	// attribute should be
	// - active
	// - nopop
	// - disabled
	
	global $set, $error;
	
	switch($set){
	  case "active":
	  case "nopop":
	  case "disabled":
		return true;
	  default:
		$error["set"] = "unknown attribute value '" . $set . "' for accountstatus";
		return false;
	}
}

function attribute_init(){
	// dummy function
}

function attribute_print_form($type = "set"){
	// no form neccessary, directly save
	global $error;
	
	if(!attribute_check($type)){
		$msg = $error["set"];
		attribute_forward($msg);
	}
	
	attribute_save($type);
}

function attribute_save($type){
	global $domain, $user, $set, $attrib, $_pql, $config;
	
	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($type){
	  case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		
		if(is_array($users)){
			foreach($users as $user){
				$return[] = pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_ISACTIVE"], $set);
			}
			
			if(in_array(false, $return)){
				$msg = PQL_LANG_ACCOUNTSTATUS_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
      		} else {
				$msg = pql_complete_constant(PQL_LANG_ACCOUNTSTATUS_CHANGE_OK, array("status" => pql_ldap_accountstatus($set)));
      		}
			
			attribute_forward($msg);
		}
		
		break;

	  case "set":
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_ISACTIVE"], $set)){
			$msg = pql_complete_constant(PQL_LANG_ACCOUNTSTATUS_CHANGE_OK, array("status" => pql_ldap_accountstatus($set)));
		} else {
			$msg = PQL_LANG_ACCOUNTSTATUS_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
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
