<?php
// attribute plugin for
// accountstatus (status of mailaccount)
// $Id: attrib.accountstatus.inc,v 2.9 2003-10-13 15:58:57 turbo Exp $

function attribute_check($type) {
    // attribute should b, $LANGe
    // - active
    // - nopop
    // - disabled
    global $set, $error;
    
    switch($set) {
      case "active":
      case "nopop":
      case "disabled":
		return true;
		
      default:
		$error["set"] = pql_complete_constant($LANG->_('Unknown attribute value %value% for %attribute%'),
											  array('value' => $set, 'attribute' => $LANG->_('accountstatus')));
		return false;
    }
}

function attribute_init() {
    // dummy function
}

function attribute_print_form($type = "set") {
    // no form neccessary, directly save
    global $error, $view;
    
    if(!attribute_check($type)) {
		$msg = $error["set"];
		attribute_forward($msg);
    }
    
    attribute_save($type);
}

function attribute_save($type) {
    global $domain, $user, $set, $attrib, $_pql, $view, $LANG;
    
    switch($type) {
      case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_userattribute($_pql->ldap_linkid, $user,
													pql_get_define("PQL_GLOB_ATTR_ISACTIVE"),
													$set);
			
			if(in_array(false, $return)) {
				$msg  = pql_complete_constant($LANG->_('Failed to change %what%'),
											 array('what' => $LANG->_('Status')));
				$msg .= ": " . pql_ldap_error($_pql->ldap_linkid);
			} else
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										   array('what' => $LANG->_('Status'),
												 'new'  => pql_ldap_accountstatus($set)));
			
			attribute_forward($msg);
		}
		break;
		
      case "set":
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_ISACTIVE"), $set)) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('Status'),
											   'new'  => pql_ldap_accountstatus($set)));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Status'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
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
