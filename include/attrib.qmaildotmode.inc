<?php
// attribute plugin for
// qmaildotmode (interpretation of .qmail-files)
// $Id: attrib.qmaildotmode.inc,v 2.2 2002-12-18 16:28:46 turbo Exp $

function attribute_check($type){
	global $qmaildotmode, $domain, $oldvalue, $_pql, $error;

	// is typed in ?
	if($qmaildotmode == ""){
		$error["qmaildotmode"] = PQL_MISSING;
		return false;
  }

	// valid entry ?
	switch($qmaildotmode){
		case "default":
		case "both":
		case "dotonly":
		case "ldaponly":
		case "ldapwithprog":
		case "none":
			break;
		default:
			$error["qmaildotmode"] = PQL_INVALID;
			return false;
  }

	return true;
}

function attribute_init(){
	global $qmaildotmode, $oldvalue;


	$qmaildotmode = $oldvalue;

}

function attribute_print_form(){
	global $qmaildotmode, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error;

	$qmaildotmode_select[$qmaildotmode] = "selected";

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_QMAILDOTMODE_CHANGE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_QMAILDOTMODE_CHANGE; ?></td>
		<td><?php echo format_error($error["qmaildotmode"]); ?>
    <select name="qmaildotmode">
			<option value="default" <?php echo $qmaildotmode_select["default"]; ?>><?php echo PQL_LDAP_QMAILDOTMODE_MODE_DEFAULT; ?></option>
			<option value="both" <?php echo $qmaildotmode_select["both"]; ?>><?php echo PQL_LDAP_QMAILDOTMODE_MODE_BOTH; ?></option>
			<option value="dotonly" <?php echo $qmaildotmode_select["dotonly"]; ?>><?php echo PQL_LDAP_QMAILDOTMODE_MODE_DOTONLY; ?></option>
			<option value="ldaponly" <?php echo $qmaildotmode_select["ldaponly"]; ?>><?php echo PQL_LDAP_QMAILDOTMODE_MODE_LDAPONLY; ?></option>
			<option value="ldapwithprog" <?php echo $qmaildotmode_select["ldapwithprog"]; ?>><?php echo PQL_LDAP_QMAILDOTMODE_MODE_LDAPWITHPROG; ?></option>
			<option value="none" <?php echo $qmaildotmode_select["none"]; ?>><?php echo PQL_LDAP_QMAILDOTMODE_MODE_NONE; ?></option>
    </select>

		</td>
	</tr>
	<tr class="subtitle">
		<td colspan="2">
		<?php echo format_error($error["general"]); ?>
		<img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LDAP_QMAILDOTMODE_HELP; ?></td>
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
	global $domain, $user, $attrib, $oldvalue, $qmaildotmode, $_pql;

	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}

	switch($type){
		case "add":
			// no add operation possible with qmaildotmode
			break;
  	case "modify";

			if($qmaildotmode == $oldvalue){
				// don't change if new value equal to old one
				$msg = PQL_LDAP_QMAILDOTMODE_UNCHANGED;
				attribute_forward($msg);
				break;
    	}

			if($qmaildotmode == "default"){
				$qmaildotmode = "";
    	}

			if(pql_replace_userattribute($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user, PQL_LDAP_ATTR_DOTMODE, $qmaildotmode)){
				$msg = pql_complete_constant(PQL_LDAP_QMAILDOTMODE_CHANGE_OK, array("mode" => $qmaildotmode));
			} else {
				$msg = PQL_LDAP_QMAILDOTMODE_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
			}

			attribute_forward($msg);
			break;

		default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}
