<?php
// attribute plugin for
// uid (user id)
// $Id: attrib.uid.inc,v 2.1 2002-12-17 12:24:35 turbo Exp $

function attribute_check($type){
	global $uid, $oldvalue, $_pql, $error;

	// is typed in ?
	if($uid == ""){
		$error["mail"] = PQL_MISSING;
		return false;
  }

	// valid
	if(preg_match("/[^a-z0-9\.@_-]/i",$uid)){
		$error["uid"] = PQL_INVALID;
		return false;
	}

	// if type is modify, check existance of old value, else the new one
	if($oldvalue != $uid and pql_search_attribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR,"uid",$uid)){
			$error["uid"] = PQL_EXISTS;
			return false;
	}

	return true;
}

function attribute_init(){
	global $uid, $oldvalue;

	$uid = $oldvalue;
}

function attribute_print_form(){
	global $uid, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_UID_CHANGE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_UID_TITLE; ?></td>
		<td><?php echo format_error($error["uid"]); ?><input type="text" name="uid" value="<?php echo $uid; ?>"></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2">
		<img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LDAP_UID_HELP; ?></td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="1">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	<input type="hidden" name="user" value="<?php echo $user ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
	</form>

	<?php
}

function attribute_save($type){
	global $domain, $user, $attrib, $oldvalue, $uid, $_pql;

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
				$msg = PQL_LDAP_UID_UNCHANGED;
				attribute_forward($msg);
				break;
    	}

			if(pql_replace_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_UID, $uid)){
				$msg = pql_complete_constant(PQL_LDAP_UID_CHANGE_OK, array("old" => $oldvalue, "new" => $uid));
			} else {
				$msg = PQL_LDAP_UID_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
			}

			// We use the uid to reference users and it has just changed...
			if (PQL_LDAP_REFERENCE_USERS_WITH == PQL_LDAP_ATTR_UID) {
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
