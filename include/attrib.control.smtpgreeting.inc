<?php
// control attribute plugin for
// smtpgreeting (SMTP greeting message)
//
// $Id: attrib.control.smtpgreeting.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $

function smtpgreeting_check($type){
	global $error, $smtpgreeting;

	// no check necessary

	return true;

}


function smtpgreeting_init(){
	// init values
  global $_pql_control, $smtpgreeting;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "smtpgreeting");

	if(!is_null($value)){
		$smtpgreeting = $value[0];
	}

}


function smtpgreeting_print_view(){
	global $_pql_control, $smtpgreeting;

	// init data
	smtpgreeting_init();

	// set empty field to default value
	if($smtpgreeting == ""){
		$smtpgreeting = PQL_LDAP_CONTROL_ME . ", (default, taken from 'me')";
 	}

	?>
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">smtpgreeting (SMTP greeting message)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Message</td>
		<td><?php echo $smtpgreeting; ?></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><a href="control_edit_attribute.php?attrib=smtpgreeting"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
	</tr>
	</table>

	<?php
}

function smtpgreeting_print_form(){
	global $attrib, $smtpgreeting, $PHP_SELF, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">smtpgreeting (SMTP greeting message)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Message</td>
		<td><?php echo format_error($error["smtpgreeting"]); ?><input type="text" name="smtpgreeting" value="<?php echo $smtpgreeting; ?>"> Default: <?php echo PQL_LDAP_CONTROL_ME; ?> (value in 'me')</td>
	</tr>
	<tr>
		<td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0">Leave the fields empty to use default configuration.</td>
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

function smtpgreeting_save($type){
	global $_pql_control, $attrib, $smtpgreeting, $error;

	// recontrol values
	if(!smtpgreeting_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "smtpgreeting", $smtpgreeting)){
      	$msg = "Successfully saved smtpgreeting options";
			} else {
				$msg = "Failed saving smtpgreeting: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}

function smtpgreeting_help(){
	?>
	SMTP greeting message.  Default:  me, if that is
  supplied; otherwise qmail-smtpd will refuse to run.
	The first word of smtpgreeting should be the current
	host's name.
 	<?php
}

function smtpgreeting_help_cr(){
?>
	help text taken from qmail man pages
<?php
}