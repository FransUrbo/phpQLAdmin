<?php
// attribute plugin for
// mailhost (host address which resides the mailhost)
// $Id: attrib.mailhost.inc,v 2.5 2003-04-04 16:37:50 turbo Exp $

function attribute_check($type){
	global $options, $mailhost, $domain, $error;
	
	// clear attribute ?
	if($options == "none"){
		$mailhost = "";
	}
	
	// is typed in ?
	if($options == "user" and $mailhost == ""){
		$error["mailhost"] = PQL_LANG_MISSING;
		return false;
	}
	
	// valid
	if($options == "user" and !preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$mailhost)){
		$error["mailhost"] = PQL_LANG_INVALID;
		return false;
	}
	
	// check dns if dns option is set
	if($options  == "dns"){
		$res = getmxrr($domain, $rec, $weight);
		if(count($rec) == 0){
			$error["dns"] = PQL_LANG_DNS_NONE . "<br>";
			return false;
		} else {
			$mailhost = $rec[0];
		}
	}
	
 	return true;
}

function attribute_init(){
	global $options, $mailhost, $oldvalue, $domain;
	
	// control dns to set options
	$mailhost = $oldvalue;
	
	if($mailhost == PQL_LANG_MAILHOST_NONE){
		$options = "none";
		$oldvalue = "";
		$mailhost = "";
	}
}

function attribute_print_form(){
	global $mailhost, $options, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error

?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LANG_MAILHOST_TITLE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_OPTIONS; ?></td>
		<td><input type="radio" name="options" value="dns" <?php if($options=="dns"){ echo "checked"; }?>> <?php echo PQL_LANG_MAILHOST_DEFAULT; ?> <input type="radio" name="options" value="user" <?php if($options=="user"){ echo "checked"; }?>> <?php echo PQL_LANG_MAILHOST_USERDEFINED; ?> <input type="radio" name="options" value="none" <?php if($options=="none"){ echo "checked"; }?>> <?php echo PQL_LANG_MAILHOST_NONE; ?></td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_MAILHOST_FQDN; ?></td>
		<td><?php echo format_error($error["mailhost"]); ?><input type="text" name="mailhost" value="<?php echo $mailhost; ?>"></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2">
		<?php echo format_error($error["dns"]); ?>
		<img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_MAILHOST_HELP; ?></td>
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
	global $mailhost, $domain, $user, $attrib, $oldvalue;
	global $_pql, $config;
	
	// recontrol values
	if(!attribute_check($type)){
 		return false;
	}
	
	switch($type){
	  case "add":
		// no add operation possible with mail
		break;

	  case "modify";
		if($mailhost == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LANG_MAILHOST_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MAILHOST"], $mailhost)){
			if($mailhost == ""){
				$mailhost = PQL_LANG_MAILHOST_NONE;
     		}
			$msg = pql_complete_constant(PQL_LANG_MAILHOST_CHANGE_OK, array("mailhost" => $mailhost));
		} else {
			$msg = PQL_LANG_MAILHOST_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
		}
		
		attribute_forward($msg);
		break;
		
	  case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		
		if(is_array($users)){
			foreach($users as $user){
				$return[] = pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MAILHOST"], $mailhost);
			}
			
			if(in_array(false, $return)){
				$msg = PQL_LANG_MAILHOST_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
			} else {
				$msg = pql_complete_constant(PQL_LANG_MAILHOST_CHANGE_OK, array("mailhost" => $mailhost));
      		}
			
			attribute_forward($msg);
		}
		
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
