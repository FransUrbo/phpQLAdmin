<?php
// control attribute plugin for
// bouncefrom (username of bounce sender)
// bouncehost (hostname of bounce sender)
//
// $Id: attrib.control.bouncefrom.inc,v 2.0 2002-12-13 14:34:57 turbo Exp $

function bouncefrom_check($type){
	global $error, $bouncefrom, $bouncehost;

	// check validity
	if($bouncefrom != "" and !check_email($bouncefrom . "@domain.tld")){
		$error["bouncefrom"] = PQL_INVALID;
		$is_error = true;
	}

	// check validity
	if($bouncehost != "" and !check_hostaddress($bouncehost)){
		// "
		$error["bouncehost"] = PQL_INVALID;
		$is_error = true;
	}

	if($is_error == false){
		return true;
 	}
}


function bouncefrom_init(){
	// init values
  global $_pql_control, $bouncefrom, $bouncehost;

	// fetch data from ldap server
	$attribs = array("bouncefrom", "bouncehost");

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, $attrib);

		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}

}

function bouncefrom_print_view(){
	global $_pql_control, $bouncefrom, $bouncehost;

	// init data
	bouncefrom_init();

	// set empty fields to their default value
	if($bouncefrom == ""){
		$bouncefrom = "MAILER-DAEMON (default)";
  }

	if($bouncehost == ""){
		$bouncehost = PQL_LDAP_CONTROL_ME . ", (default, taken from 'me')";
  }

	?>
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">bouncefrom, bouncehost (Bouncing source)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">User for bouncing</td>
		<td><?php echo $bouncefrom; ?></td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Host for bouncing</td>
		<td><?php echo $bouncehost; ?></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><a href="control_edit_attribute.php?attrib=bouncefrom"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
	</tr>
	</table>

	<?php
}

function bouncefrom_print_form(){
	global $attrib, $bouncefrom, $bouncehost, $PHP_SELF, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">bouncefrom, bouncehost (Bouncing source)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">User for bouncing</td>
		<td><?php echo format_error($error["bouncefrom"]); ?><input type="text" name="bouncefrom" value="<?php echo $bouncefrom; ?>"> Default: MAILER-DAEMON</td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Host for bouncing</td>
		<td><?php echo format_error($error["bouncehost"]); ?><input type="text" name="bouncehost" value="<?php echo $bouncehost; ?>"> Default: <?php echo PQL_LDAP_CONTROL_ME; ?> (value in 'me')</td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><img src="images/info.png" width="16" height="16" border="0">
		The combination of user and host will return the email address used for bouning messages, e.g. MAILER-DAEMON@mail.host.ch.
		<br>Leave the fields empty to use default configuration.
		</td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="1">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
	</form>

	<?php
}

function bouncefrom_save($type){
	global $_pql_control, $attrib, $bouncefrom, $bouncehost, $error;

	// recontrol values
	if(!bouncefrom_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "bouncefrom", $bouncefrom);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "bouncehost", $bouncehost);

			if(in_array(false, $return)){
				$msg = "Error saving bouncing options: " . pql_ldap_error($_pql_control->ldap_linkid);
			} else {
    		$msg = "Successfully saved bouncing options";
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}

function bouncefrom_help(){
?>
	<b>User (bouncefrom)</b>
	<br>
	Bounce username.  Default:  MAILER-DAEMON.
	<br><br>
 	<b>Host (bouncehost)</b>
	<br>
	Bounce host.  Default:  me, if that is supplied;
	otherwise the literal name bouncehost, which is
	probably not what you want.  If a message is
	permanently undeliverable, qmail-send sends a single-
	bounce notice back to the message's envelope sender.
	The notice is From: bouncefrom@bouncehost, although its
	envelope sender is empty.
<?php
}

function bouncefrom_help_cr(){
?>
	help text taken from qmail man pages
<?php
}