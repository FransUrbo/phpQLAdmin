<?php
// attribute plugin for
// uid (user id)
// $Id: attrib.uid.inc,v 2.14 2003-11-18 13:16:38 turbo Exp $

function attribute_check($type){
	global $uid, $oldvalue, $_pql, $error, $domain, $LANG;
	
	// is typed in ?
	if($uid == "") {
		$error[pql_get_define("PQL_GLOB_ATTR_MAIL")] = $LANG->_('Missing');
		return false;
	}
	
	// valid
	if(preg_match("/[^a-z0-9\.@_-]/i",$uid)) {
		$error["uid"] = $LANG->_('Invalid');
		return false;
	}
	
	// if type is modify, check existance of old value, else the new one
	if($oldvalue != $uid and pql_search_attribute($_pql->ldap_linkid, $domain, "uid", $uid)) {
		$error["uid"] = $LANG->_('Already exists');
		return false;
	}
	
	return true;
}

function attribute_init() {
	global $uid, $oldvalue;
	$uid = $oldvalue;
}

function attribute_print_form() {
	global $uid, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change username')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Username')?></td>
          <td><?php echo format_error($error["uid"]); ?><input type="text" name="uid" value="<?=$uid?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('The username can only contain numbers, letters and following special characters @, . (dot), _, -. Additionally, the username should be unique in the system')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}

function attribute_save($type) {
	global $domain, $user, $attrib, $oldvalue, $uid, $_pql, $view, $LANG;
	
	// recontrol values
	if(!attribute_check($type))
	  return false;
	
	switch($type) {
	  case "add":
		// no add operation possible with uid
		break;

	  case "modify";
		if($uid == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Username')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_attribute($_pql->ldap_linkid, $user,
									 pql_get_define("PQL_GLOB_ATTR_UID"),
									 $uid))
		{
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% from %old% to %new%'),
										 array('what' => $LANG->_('username'),
											   'old'  => $oldvalue,
											   'new'  => $uid));
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('username'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		}
		
		// We use the uid to reference users and it has just changed...
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", pql_get_rootdn($user)) == pql_get_define("PQL_GLOB_ATTR_UID")) {
			$user = $uid;
			// Reload the navigation bar (if allowed by config)
			attribute_forward($msg, true);
		} else {
			attribute_forward($msg);
		}
		
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
