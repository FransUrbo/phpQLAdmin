<?php
// control attribute plugin for
// custombouncetext (additional bounce message)
//
// $Id: attrib.control.custombouncetext.inc,v 2.0 2002-12-13 14:34:57 turbo Exp $

function custombouncetext_check($type){
	global $error, $custombouncetext, $use_custombouncetext;

	// missing
	if($use_custombouncetext == 1 and $custombouncetext == ""){
		$error["custombouncetext"] = PQL_MISSING;
		return false;
	}

	return true;

}

function custombouncetext_init(){
	// init values
  global $_pql_control, $custombouncetext, $use_custombouncetext;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "custombouncetext");

	if(!is_null($value)){
		$custombouncetext = $value[0];
	}

	// initialize radio button
	if($custombouncetext == ""){
		$use_custombouncetext = 0;
 	} else {
		$use_custombouncetext = 1;
  }

}

function custombouncetext_print_view(){
	global $_pql_control, $custombouncetext, $use_custombouncetext;

	// init data
	custombouncetext_init();

	if($use_custombouncetext == "0"){
		$custombouncetext = "disabled";
  }

	?>
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">custombouncetext (Additional bounce text)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Message</td>
		<td><?php echo nl2br($custombouncetext); ?></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><a href="control_edit_attribute.php?attrib=custombouncetext"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
	</tr>
	</table>

	<?php
}

function custombouncetext_print_form(){
	global $attrib, $custombouncetext, $use_custombouncetext,  $PHP_SELF, $error;

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">custombouncetext (Additional bounce text)</th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Use additional message</td>
		<td><input type="radio" name="use_custombouncetext" value="0" <?php if($use_custombouncetext == "0"){ echo "checked"; } //" ?>> no
		<input type="radio" name="use_custombouncetext" value="1" <?php if($use_custombouncetext == "1"){ echo "checked"; } //" ?>> yes</td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title">Message</td>
		<td><?php echo format_error($error["custombouncetext"]); ?><textarea cols="40" rows="10" name="custombouncetext"><?php echo $custombouncetext; ?></textarea></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2"><img src="images/info.png" width="16" height="16" border="0">
		Additional custom text in bounce messages, eg. for providing contact information.
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

function custombouncetext_save($type){
	global $_pql_control, $attrib, $custombouncetext, $use_custombouncetext, $error;

	// recontrol values
	if(!custombouncetext_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if($use_custombouncetext == "0"){
				$custombouncetext = "";
			}

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "custombouncetext", $custombouncetext)){
      	$msg = "Successfully saved custombouncetext message";
			} else {
				$msg = "Failed saving custombouncetext message: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}
