<?php
// control attribute plugin for
// tarpitcount (user id of virtual user)
// tarpitdelay (group id of virtual user)
//
// $Id: attrib.control.tarpitcount.inc,v 2.1 2002-12-17 06:21:05 turbo Exp $

function tarpitcount_check($type) {
	global $error, $tarpitcount, $tarpitdelay;

	// missing
	if($tarpitcount == ""){
		$error["tarpitcount"] = PQL_MISSING;
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $tarpitcount)){
		$error["tarpitcount"] = PQL_INVALID;
		$is_error = true;
	}
	
	// missing
	if($tarpitdelay == ""){
		$error["tarpitdelay"] = PQL_MISSING;
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $tarpitdelay)){
		$error["tarpitdelay"] = PQL_INVALID;
		$is_error = true;
	}
	
	// must be > 0
	if($error["tarpitdelay"] == "" and $tarpitdelay <= 0){
		$error["tarpitdelay"] = PQL_GREATER_ZERO;
		$is_error = true;
	}
	
	if($is_error == false){
		return true;
 	}
}

function tarpitcount_init($host) {
	// init values
	global $_pql_control, $tarpitcount, $tarpitdelay;
	
	// fetch data from ldap server
	$attribs = array("tarpitcount", "tarpitdelay");
	
	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
										   $attrib);
		
		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
}

function tarpitcount_print_view($host) {
	global $_pql_control, $tarpitcount, $tarpitdelay;
	
	// init data
	tarpitcount_init($host);

	//
	if($tarpitcount == "" or $tarpitcount == 0){
		$tarpitcount = "disabled";
	} else {
		$tarpitcount = "Delay submitting after $tarpitcount RCPTO's";
	}
	
	//
	if($tarpitdelay == ""){
		// default value
		$tarpitdelay = 5;
	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">tarpitcount, tarpitdelay (Tarpitting settings)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Tarpitting</td>
        <td><?php echo $tarpitcount; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Tarpit delay</td>
        <td><?php echo $tarpitdelay; ?> sec</td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=tarpitcount"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function tarpitcount_print_form() {
	global $attrib, $tarpitcount, $tarpitdelay, $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">tarpitcount, tarpitdelay (Tarpitting settings)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Tarpit count</td>
          <td><?php echo format_error($error["tarpitcount"]); ?><input type="text" name="tarpitcount" value="<?php echo $tarpitcount; ?>"> RCPTO's</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Tarpit delay</td>
          <td><?php echo format_error($error["tarpitdelay"]); ?><input type="text" name="tarpitdelay" value="<?php echo $tarpitdelay; ?>"> seconds</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set tarpit count to 0 to disable tarpitting</td>
        </tr>
      </th>
    </table>
    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
   </form>
<?php
}

function tarpitcount_save($type){
	global $_pql_control, $attrib, $tarpitcount, $tarpitdelay, $error;

	// recontrol values
	if(!tarpitcount_check($type)){
 		return false;
	}

	switch($type){
		case "modify";
			// save entries

			$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													  "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
													  "tarpitcount", $tarpitcount);
			$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													  "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
													  "tarpitdelay", $tarpitdelay);

			if(in_array(false, $return)){
				$msg = "Error saving tarpitting options: " . pql_ldap_error($_pql_control->ldap_linkid);
			} else {
				$msg = "Successfully saved tarpitting options";
			}

			attribute_forward($msg);
			break;
		default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function tarpitcount_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
	?>
	<b>Tarpit count</b>
	<br>
	The number of RCPT TOs you accept before you start tarpitting. Tarpitting means introducing an delay after each subsequent RCPT TO, therefore sending messages to lots of recipient takes a _lot_ of time and makes life for spammers harder.
	Default: 0 (disabled)
	<br><br>
	<b>Tarpit delay</b>
	<br>
	The number of seconds of delay for tarpitting. Default: 5
	<?php
}

function tarpitcount_help_cr(){
?>
	help text taken from life with qmail-ldap
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
