<?php
// attribute plugin for
// deliverymode (delivery preferences)
// $Id: attrib.deliverymode.inc,v 2.2 2002-12-18 16:28:46 turbo Exp $

function attribute_check($type){
	global $error, $reply, $mailreplytext;

	// if reply is selected, mailreplytext must not be null
	if($reply != "" and $mailreplytext == ""){
		$error["mailreplytext"] = PQL_MISSING;
		return false;
	}

	return true;
}

function attribute_init(){
	global $reply, $mailreplytext, $deliverymode, $domain, $user, $_pql;
	// initialize values directly from ldap

	$mode = pql_get_userattribute($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user, PQL_LDAP_ATTR_MODE);
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

	$mailreplytext = pql_get_userattribute($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user, PQL_LDAP_ATTR_REPLYTEXT);
	$mailreplytext = $mailreplytext[0];

}


function attribute_print_form($type = "modify"){
	global $reply, $mailreplytext, $deliverymode, $domain, $user, $PHP_SELF, $attrib, $error

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_DELIVERYMODE_TITLE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_DELIVERYMODE_PROFILE; ?></td>
		<td><?php echo format_error($error["deliverymode"]); ?>
			<select name="deliverymode">
				<option value="local" <?php if($deliverymode == "local"){ echo "SELECTED";} ?>><?php echo PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL; ?>
				<option value="forward" <?php if($deliverymode == "forward"){ echo "SELECTED";} ?>><?php echo PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD; ?>
			</select>
		</td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_DELIVERYMODE_REPLY; ?></td>
		<td><input type="checkbox" name="reply" value="ok" <?php if($reply=="ok"){ echo "checked";} ?>> <?php echo PQL_LDAP_DELIVERYMODE_REPLY_SEND; ?></td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_DELIVERYMODE_REPLY_TEXT; ?></td>
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
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
	</form>

	<?php
}

function attribute_save($type){
	global $domain, $user, $attrib, $reply, $mailreplytext, $deliverymode, $_pql;

	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}

	switch($deliverymode){
		case "forward":
			$entry[PQL_LDAP_ATTR_MODE][] = "forwardonly";
			$entry[PQL_LDAP_ATTR_MODE][] = "nombox";
			break;
		case "local":
			$entry[PQL_LDAP_ATTR_MODE][] = "localdelivery";
	}

	if($reply == "ok"){
		$entry[PQL_LDAP_ATTR_MODE][] = "reply";
	}
	if (!empty($mailreplytext)) {
		$entry[PQL_LDAP_ATTR_REPLYTEXT] = $mailreplytext;
	}
	
	switch($type){
		case "fulldomain":
		
			$users = pql_get_user($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain);

			if(is_array($users)){
				foreach($users as $user){
					$return[] = pql_replace_userattributes($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user,$entry);
				}

				if(in_array(false, $return)){
					$msg = PQL_LDAP_DELIVERYMODE_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
				} else {
					$msg = PQL_LDAP_DELIVERYMODE_CHANGE_OK;
				}

				attribute_forward($msg);

			}
			break;

		case "modify":

			if(pql_replace_userattributes($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user,$entry)){
				$msg = PQL_LDAP_DELIVERYMODE_CHANGE_OK;
			} else {
				$msg = PQL_LDAP_DELIVERYMODE_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
			}

			attribute_forward($msg);

			break;
    default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}
