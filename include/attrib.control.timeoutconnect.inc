<?php
// control attribute plugin for
// timeoutconnect (how long to wait for smtp connection)
// timeoutremote (how long to wait for remote server)
//
// $Id: attrib.control.timeoutconnect.inc,v 2.3 2002-12-18 16:28:46 turbo Exp $

function timeoutconnect_check($type) {
	global $error, $timeoutconnect, $timeoutremote;

	// check validity
	if(preg_match("/[^0-9]/", $timeoutconnect)){
		$error["timeoutconnect"] = PQL_INVALID;
		$is_error = true;
	}

	// must be > 0
	if($error["timeoutconnect"] == "" and $timeoutconnect != "" and $timeoutconnect <= 0){
		$error["timeoutconnect"] = PQL_GREATER_ZERO;
		$is_error = true;
	}

	// check validity
	if(preg_match("/[^0-9]/", $timeoutremote)){
		$error["timeoutremote"] = PQL_INVALID;
		$is_error = true;
	}

	// must be > 0
	if($error["timeoutremote"] == "" and $timeoutremote != "" and $timeoutremote <= 0){
		$error["timeoutremote"] = PQL_GREATER_ZERO;
		$is_error = true;
	}

	if($is_error == false){
		return true;
 	}
}

function timeoutconnect_init($host) {
	// init values
	global $_pql_control, $timeoutconnect, $timeoutremote;

	// fetch data from ldap server
	$attribs = array("timeoutconnect", "timeoutremote");

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);

		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
}

function timeoutconnect_print_view($host) {
	global $_pql_control, $timeoutconnect, $timeoutremote;

	// init data
	timeoutconnect_init($host);

	// set empty fields to their default value
	if($timeoutconnect == ""){
		$timeoutconnect = "60 s (default)";
	} else {
		$timeoutconnect .= " s";
	}
	
	if($timeoutremote == ""){
		$timeoutremote = "1200 s (default)";
	} else {
		$timeoutremote .= " s";
	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">timeoutconnect, timeoutremote (Timeouts)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">SMTP connection</td>
        <td><?php echo $timeoutconnect; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Remote server</td>
        <td><?php echo $timeoutremote; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=timeoutconnect"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function timeoutconnect_print_form(){
	global $attrib, $timeoutconnect, $timeoutremote, $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">timeoutconnect, timeoutremote (Timeouts)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">SMTP connection</td>
          <td><?php echo format_error($error["timeoutconnect"]); ?><input type="text" name="timeoutconnect" value="<?php echo $timeoutconnect; ?>"> seconds. Default: 60</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Remote server</td>
          <td><?php echo format_error($error["timeoutremote"]); ?><input type="text" name="timeoutremote" value="<?php echo $timeoutremote; ?>"> seconds. Default: 1200</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"> Leave the fields empty to set the default values</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function timeoutconnect_save($type){
	global $_pql_control, $attrib, $timeoutconnect, $timeoutremote, $error;

	// recontrol values
	if(!timeoutconnect_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "timeoutconnect", $timeoutconnect);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "timeoutremote", $timeoutremote);
		
		if(in_array(false, $return)){
			$msg = "Error saving remote timeout options: " . pql_ldap_error($_pql_control->ldap_linkid);
		} else {
    		$msg = "Successfully remote timeout options";
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function timeoutconnect_help(){
?>
<b>SMTP connection</b><br>
Number of seconds qmail-remote will wait for the remote
SMTP server to accept a connection.  Default: 60.  The
kernel normally imposes a 75-second upper limit.
<br><br>
<b>Remote server</b><br>
Number of seconds qmail-remote will wait for each
response from the remote SMTP server.  Default: 1200.
<?php
}

function timeoutconnect_help_cr(){
?>
	help text taken from qmail man pages
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
