<?php
// control attribute plugin for
// maxrcptcount (maximum number of RCPT TO's)
//
// $Id: attrib.control.maxrcptcount.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $

function maxrcptcount_check($type){
	global $error, $maxrcptcount;

	// missing
	if($maxrcptcount == ""){
		$error["maxrcptcount"] = PQL_MISSING;
		return false;
	}

	// number ?
	if(preg_match ("/[^0-9]/", $maxrcptcount)){
		$error["maxrcptcount"] = PQL_INVALID;
		return false;
	}

	return true;

}

function maxrcptcount_init(){
	// init values
  global $_pql_control, $maxrcptcount;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "maxrcptcount");

	if(!is_null($value)){
		$maxrcptcount = $value[0];
	}

}

function maxrcptcount_print_view(){
	global $_pql_control, $maxrcptcount;

	// init data
	maxrcptcount_init();

	// set empty field to "not set"
	if($maxrcptcount == "" or $maxrcptcount == 0){
		$maxrcptcount = "unlimited";
 	}

	?>
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">maxrcptcount (Maximum number of RCPT TO's)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Max RCPT TO's</td>
		<td><?php echo $maxrcptcount; ?></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><a href="control_edit_attribute.php?attrib=maxrcptcount"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
	</tr>
	</table>

	<?php
}

function maxrcptcount_print_form(){
	global $attrib, $maxrcptcount, $PHP_SELF, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">maxrcptcount (Maximum number of RCPT TO's)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Max RCPT TO's</td>
		<td><?php echo format_error($error["maxrcptcount"]); ?><input type="text" name="maxrcptcount" value="<?php echo $maxrcptcount; ?>"></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set to 0 to allow unlimited RCPT TO's</td>
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

function maxrcptcount_save($type){
	global $_pql_control, $attrib, $maxrcptcount, $error;

	// recontrol values
	if(!maxrcptcount_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "maxrcptcount", $maxrcptcount)){
      	$msg = "Successfully saved maxrcptcount options";
			} else {
				$msg = "Failed saving maxrcptcount: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}
