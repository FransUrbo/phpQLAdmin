<?php
// attribute plugin for
// mailmessagestore (path to mailbox)
// $Id: attrib.mailmessagestore.inc,v 2.3 2003-01-16 14:58:38 turbo Exp $

function attribute_check($type){
	global $mailmessagestore, $domain, $oldvalue, $_pql, $error;
	
	// is typed in ?
	if($mailmessagestore == ""){
		$error["mailmessagestore"] = PQL_MISSING;
		return false;
	}
	
	// check for absolute paths
	if(PQL_ALLOW_ABSOLUTE_PATH != true and preg_match("/^\//", $mailmessagestore)){
		$error["general"] = PQL_LDAP_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED . "<br>";
		return false;
	}
	
	// don't use ".."
	if(preg_match("/(\.){2,}/", $mailmessagestore)){
		$error["mailmessagestore"] = PQL_INVALID;
		return false;
	}
	
	return true;
}

function attribute_init(){
	global $mailmessagestore, $oldvalue;
	
	$mailmessagestore = $oldvalue;
}

function attribute_print_form(){
	global $mailmessagestore, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error;

?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_MAILMESSAGESTORE_CHANGE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_MAILMESSAGESTORE_CHANGE; ?></td>
		<td><?php echo format_error($error["mailmessagestore"]); ?><input type="text" name="mailmessagestore" value="<?php echo $mailmessagestore; ?>"></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2">
		<?php echo format_error($error["general"]); ?>
		<img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LDAP_MAILMESSAGESTORE_HELP; ?></td>
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
	global $domain, $user, $attrib, $oldvalue, $mailmessagestore, $_pql;
	
	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($type){
	  case "add":
		// no add operation possible with mailmessagestore
		break;

	  case "modify";
		if($mailmessagestore == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LDAP_MAILMESSAGESTORE_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, PQL_LDAP_ATTR_MAILSTORE, $mailmessagestore)){
			$msg = pql_complete_constant(PQL_LDAP_MAILMESSAGESTORE_CHANGE_OK, array("path" => $mailmessagestore));
		} else {
			$msg = PQL_LDAP_MAILMESSAGESTORE_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
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
