<?php
// attribute plugin for
// deliveryprogrampath (path to program running on delivery)
// $Id: attrib.deliveryprogrampath.inc,v 2.3 2003-01-16 14:58:38 turbo Exp $

function attribute_check($type){
	global $deliveryprogrampath, $domain, $oldvalue, $_pql, $error;
	
	// trailing slash ?
	if(!preg_match ("/^(\/)/", $deliveryprogrampath) and $deliveryprogrampath != ""){
		$error["deliveryprogrampath"] = PQL_INVALID;
		return false;
	}
	
	return true;
}

function attribute_init(){
	global $deliveryprogrampath, $oldvalue, $_pql, $domain, $user;
	
	$deliveryprogrampath = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_PROGRAM);
	$deliveryprogrampath = $deliveryprogrampath[0];
	$oldvalue = $deliveryprogrampath;
}

function attribute_print_form(){
	global $deliveryprogrampath, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error

?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE; ?></td>
		<td><?php echo format_error($error["deliveryprogrampath"]); ?><input type="text" name="deliveryprogrampath" value="<?php echo $deliveryprogrampath; ?>"></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2">
		<?php echo format_error($error["general"]); ?>
		<img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LDAP_DELIVERYPROGRAMPATH_HELP; ?></td>
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
	global $domain, $user, $attrib, $oldvalue, $deliveryprogrampath, $_pql;
	
	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($type){
	  case "add":
		// no add operation possible with deliveryprogrampath
		break;

	  case "modify";
		if($deliveryprogrampath == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LDAP_DELIVERYPROGRAMPATH_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_PROGRAM, $deliveryprogrampath)){
			if($deliveryprogrampath == ""){
				$deliveryprogrampath = PQL_LDAP_DELIVERYPROGRAMPATH_NONE;
     		}
			
			$msg = pql_complete_constant(PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_OK, array("path" => $deliveryprogrampath));
		} else {
			$msg = PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
		}
		
		// check if qmaildotmode is set to 'both' or 'ldapwithprog'
		$qmaildotmode = pql_get_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_DOTMODE);
		$qmaildotmode = $qmaildotmode[0];
		
		if($qmaildotmode != "both" and $qmaildotmode != "ldapwithprog"){
			$msg .= "<br>" . pql_complete_constant(PQL_LDAP_DELIVERYPROGRAMPATH_DOTWARNING, array("qmaildotmode" => $qmaildotmode));
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
