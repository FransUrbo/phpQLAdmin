<?php
// attribute plugin for
// uid (user id)
// $Id: attrib.uid.inc,v 2.9 2003-06-25 07:07:02 turbo Exp $

function attribute_check($type){
	global $uid, $oldvalue, $_pql, $error, $domain;
	
	// is typed in ?
	if($uid == ""){
		$error["mail"] = PQL_LANG_MISSING;
		return false;
	}
	
	// valid
	if(preg_match("/[^a-z0-9\.@_-]/i",$uid)){
		$error["uid"] = PQL_LANG_INVALID;
		return false;
	}
	
	// if type is modify, check existance of old value, else the new one
	if($oldvalue != $uid and pql_search_attribute($_pql->ldap_linkid, $domain, "uid", $uid)) {
		$error["uid"] = PQL_LANG_EXISTS;
		return false;
	}
	
	return true;
}

function attribute_init(){
	global $uid, $oldvalue;
	
	$uid = $oldvalue;
}

function attribute_print_form(){
	global $uid, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $view;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_UID_CHANGE; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_UID_TITLE; ?></td>
          <td><?php echo format_error($error["uid"]); ?><input type="text" name="uid" value="<?php echo $uid; ?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_UID_HELP; ?>
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
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

<?php
}

function attribute_save($type){
	global $domain, $user, $attrib, $oldvalue, $uid, $_pql, $view;
	
	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($type){
	  case "add":
		// no add operation possible with uid
		break;

	  case "modify";
		if($uid == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LANG_UID_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_UID"), $uid)){
			$msg = pql_complete_constant(PQL_LANG_UID_CHANGE_OK, array("old" => $oldvalue, "new" => $uid));
		} else {
			$msg = PQL_LANG_UID_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
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
