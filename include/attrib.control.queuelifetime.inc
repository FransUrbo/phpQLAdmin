<?php
// control attribute plugin for
// queuelifetime (maximum number of RCPT TO's)
//
// $Id: attrib.control.queuelifetime.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $

function queuelifetime_check($type){
	global $error, $queuelifetime;

	// number ?
	if(preg_match ("/[^0-9]/", $queuelifetime)){
		$error["queuelifetime"] = PQL_INVALID;
		return false;
	}

	// must be > 0
	if($queuelifetime != "" and $error["queuelifetime"] == "" and $queuelifetime <= 0){
		$error["queuelifetime"] = PQL_GREATER_ZERO;
		return false;
	}
	return true;

}

function queuelifetime_init(){
	// init values
  global $_pql_control, $queuelifetime;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "queuelifetime");

	if(!is_null($value)){
		$queuelifetime = $value[0];
	}

}

function queuelifetime_print_view(){
	global $_pql_control, $queuelifetime;

	// init data
	queuelifetime_init();

	// set empty field to "not set"
	if($queuelifetime == ""){
		$queuelifetime = "604800 s (default)";
 	} else {
		$queuelifetime .= " s";
  }

	?>
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">queuelifetime (Max lifetime of a message in queue)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Remain</td>
		<td><?php echo $queuelifetime; ?></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><a href="control_edit_attribute.php?attrib=queuelifetime"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
	</tr>
	</table>

	<?php
}

function queuelifetime_print_form(){
	global $attrib, $queuelifetime, $PHP_SELF, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">queuelifetime (Max lifetime of a message in queue)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Remain</td>
		<td><?php echo format_error($error["queuelifetime"]); ?><input type="text" name="queuelifetime" value="<?php echo $queuelifetime; ?>"> seconds</td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Empty field to set to the default value (604800 seconds)</td>
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

function queuelifetime_save($type){
	global $_pql_control, $attrib, $queuelifetime, $error;

	// recontrol values
	if(!queuelifetime_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "queuelifetime", $queuelifetime)){
      	$msg = "Successfully saved queuelifetime options";
			} else {
				$msg = "Failed saving queuelifetime: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}


function queuelifetime_help(){
?>
Number of seconds a message can stay in the queue.
Default: 604800 (one week).  After this time expires,
qmail-send will try the message once more, but it will
treat any temporary delivery failures as permanent
failures.
<?php
}

function queuelifetime_help_cr(){
?>
	help text taken from qmail man pages
<?php
}