<?php
// attribute plugin for
// mailforwardingaddress (forward all mails to this address)
// $Id: attrib.mailforwardingaddress.inc,v 2.6 2003-04-29 08:37:20 turbo Exp $

function attribute_check($type){
	global $mailforwardingaddress, $error;

	// is typed in ?
	if($mailforwardingaddress == ""){
		$error["mailforwardingaddress"] = PQL_LANG_MISSING;
		return false;
  	}

	// valid
	if(!check_email($mailforwardingaddress)){
		$error["mailforwardingaddress"] = PQL_LANG_INVALID;
		return false;
	}

	// maybe check existence for modifying
	return true;
}

function attribute_init(){
	// dummy function
}

function attribute_print_form(){
	global $mailforwardingaddress, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php if($oldvalue == ""){echo PQL_LANG_MAILFORWARDINGADDRESS_NEW;}else{echo PQL_LANG_MAILFORWARDINGADDRESS_CHANGE;} ?></th>
	<?php
	  // We're changing a MailForwardingAddress. Give the old as reference
	  if($oldvalue != "") {
			echo "<tr class=\"c2\">";
			echo "  <td class=\"title\">Old address</td>";
			echo "  <td>" . $mailforwardingaddress . "</td>";
			echo "</tr>";
	  }
	?>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
		<td><?php echo format_error($error["mailforwardingaddress"]); ?><input type="text" name="mailforwardingaddress" value=""></td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="1">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	<input type="hidden" name="user" value="<?php echo $user ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
	</form>

	<?php
}

function attribute_save($type){
	global $domain, $user, $attrib, $oldvalue;
	global $mailforwardingaddress, $_pql, $config;

	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($type){
	  case "add":
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_FORWARDS"], $mailforwardingaddress)){
			$msg = pql_complete_constant(PQL_LANG_MAILFORWARDINGADDRESS_ADD_OK, array("mail" => $mailforwardingaddress));
		} else {
			$msg = PQL_LANG_MAILFORWARDINGADDRESS_ADD_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
		}
		
		attribute_forward($msg);
		break;
	  case "modify";
		$email = $mailforwardingaddress;
		
		if($email == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LANG_MAILFORWARDINGADDRESS_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_FORWARDS"], $oldvalue, $email)){
			$msg = pql_complete_constant(PQL_LANG_MAILFORWARDINGADDRESS_CHANGE_OK, array("old" => $oldvalue, "new" => $email));
		} else {
			$msg = PQL_LANG_MAILFORWARDINGADDRESS_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
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
