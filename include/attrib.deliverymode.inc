<?php
// attribute plugin for
// deliverymode (delivery preferences)
// $Id: attrib.deliverymode.inc,v 2.7 2003-04-29 08:37:20 turbo Exp $

function attribute_check($type){
	global $error, $reply, $mailreplytext;

	// if reply is selected, mailreplytext must not be null
	if($reply != "" and $mailreplytext == ""){
		$error["mailreplytext"] = PQL_LANG_MISSING;
		return false;
	}

	return true;
}

function attribute_init(){
	global $reply, $mailreplytext, $deliverymode, $domain;
	global $user, $_pql, $config;

	// initialize values directly from ldap
	$mode = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MODE"]);

	// delivery profile
	if(is_array($mode) and in_array("forwardonly",$mode)){
		$deliverymode = "forward";
	} else {
		$deliverymode = "local";
	}
	
	// reply / mailreplytext
	if(is_array($mode) and in_array("reply", $mode)){
		$reply = "ok";
	}
	
	$mailreplytext = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_REPLYTEXT"]);
	$mailreplytext = $mailreplytext[0];
}


function attribute_print_form($type = "modify"){
	global $reply, $mailreplytext, $deliverymode, $domain, $user, $PHP_SELF, $attrib, $error

?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LANG_DELIVERYMODE_TITLE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_DELIVERYMODE_PROFILE; ?></td>
		<td><?php echo format_error($error["deliverymode"]); ?>
			<select name="deliverymode">
				<option value="local" <?php if($deliverymode == "local"){ echo "SELECTED";} ?>><?php echo PQL_LANG_DELIVERYMODE_PROFILE_LOCAL; ?>
				<option value="forward" <?php if($deliverymode == "forward"){ echo "SELECTED";} ?>><?php echo PQL_LANG_DELIVERYMODE_PROFILE_FORWARD; ?>
			</select>
		</td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_DELIVERYMODE_REPLY; ?></td>
		<td><input type="checkbox" name="reply" value="ok" <?php if($reply=="ok"){ echo "checked";} ?>> <?php echo PQL_LANG_DELIVERYMODE_REPLY_SEND; ?></td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_DELIVERYMODE_REPLY_TEXT; ?></td>
		<td valign="center"><?php echo format_error($error["mailreplytext"]); ?> <textarea name="mailreplytext" cols="40" rows="10"><?php echo $mailreplytext; ?></textarea></td>
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
	global $domain, $user, $attrib, $reply, $mailreplytext;
	global $deliverymode, $_pql, $config;
	
	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($deliverymode){
	  case "forward":
		$entry[$config["PQL_GLOB_ATTR_MODE"]][] = "forwardonly";
		$entry[$config["PQL_GLOB_ATTR_MODE"]][] = "nombox";
		break;
	  case "local":
		$entry[$config["PQL_GLOB_ATTR_MODE"]][] = "localdelivery";
	}
	
	if($reply == "ok"){
		$entry[$config["PQL_GLOB_ATTR_MODE"]][] = "reply";
	}
	if (!empty($mailreplytext)) {
		$entry[$config["PQL_GLOB_ATTR_REPLYTEXT"]] = preg_replace('/\\\/', "", $mailreplytext);
	} else {
		// Check if the entry in database is empty. 
		// If not, we have choosen to DELETE the value
		$mrt_db = pql_get_userattribute($_pql->ldap_linkid,
										$user, $config["PQL_GLOB_ATTR_REPLYTEXT"]);
		if($mrt_db[0]) {
			pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_REPLYTEXT"]);
		}
	}
	
	switch($type){
	  case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		if(is_array($users)){
			foreach($users as $user){
				$return[] = pql_replace_userattribute($_pql->ldap_linkid, $user, '', $entry);
			}
			
			if(in_array(false, $return)){
				$msg = PQL_LANG_DELIVERYMODE_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
			} else {
				$msg = PQL_LANG_DELIVERYMODE_CHANGE_OK;
			}
			
			attribute_forward($msg);
			
		}
		break;
		
	  case "modify":
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, '', $entry)) {
			$msg = PQL_LANG_DELIVERYMODE_CHANGE_OK;
		} else {
			$msg = PQL_LANG_DELIVERYMODE_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
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
