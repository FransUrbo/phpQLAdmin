<?php
// control attribute plugin for
// concurrencylocal (may simultaneous local deliveries)
// concurrencyremote (may simultaneous remote deliveries)
//
// $Id: attrib.control.concurrencylocal.inc,v 2.3 2002-12-18 16:28:45 turbo Exp $

function concurrencylocal_check($type) {
	global $error, $concurrencylocal, $concurrencyremote;

	// check validity
	if(preg_match("/[^0-9]/", $concurrencylocal)){
		$error["concurrencylocal"] = PQL_INVALID;
		$is_error = true;
	}

	// check validity
	if(preg_match("/[^0-9]/", $concurrencyremote)){
		$error["concurrencyremote"] = PQL_INVALID;
		$is_error = true;
	}

	if($is_error == false){
		return true;
 	}
}

function concurrencylocal_init($host) {
	// init values
	global $_pql_control, $concurrencylocal, $concurrencyremote;

	// fetch data from ldap server
	$attribs = array("concurrencylocal", "concurrencyremote");

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
}

function concurrencylocal_print_view($host) {
	global $_pql_control, $concurrencylocal, $concurrencyremote;

	// init data
	concurrencylocal_init($host);

	// set empty fields to their default value
	if($concurrencylocal == ""){
		$concurrencylocal = "10 (default)";
	}
	
	if($concurrencylocal == 0){
		$concurrencylocal = "0 (local delivery holded)";
	}
	
	if($concurrencyremote == ""){
		$concurrencyremote = "20 (default)";
	}
	
	if($concurrencyremote == 0){
		$concurrencyremote = "0 (remote delivery holded)";
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">concurrencylocal, concurrencyremote (Max simultaneous deliveries)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Local</td>
        <td><?php echo $concurrencylocal; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Remote</td>
        <td><?php echo $concurrencyremote; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=concurrencylocal"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function concurrencylocal_print_form(){
	global $attrib, $concurrencylocal, $concurrencyremote, $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">concurrencylocal, concurrencyremote (Max simultaneous deliveries)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Local</td>
          <td><?php echo format_error($error["concurrencylocal"]); ?><input type="text" name="concurrencylocal" value="<?php echo $concurrencylocal; ?>"> Default: 10</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Remote</td>
          <td><?php echo format_error($error["concurrencyremote"]); ?><input type="text" name="concurrencyremote" value="<?php echo $concurrencyremote; ?>"> Default: 20</td>
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

function concurrencylocal_save($type){
	global $_pql_control, $attrib, $concurrencylocal, $concurrencyremote, $error;

	// recontrol values
	if(!concurrencylocal_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "concurrencylocal", $concurrencylocal);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "concurrencyremote", $concurrencyremote);
		
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

function concurrencylocal_help(){
?>

	<b>Local</b>
	<br>
	Maximum number of simultaneous local delivery attempts.
	Default: 10.  If 0, local deliveries will be put on
	hold.  concurrencylocal is limited at compile time to
	120.
	<br><br>
	<b>Remote</b>
	<br>
	Maximum number of simultaneous remote delivery
	attempts.  Default: 20.  If 0, remote deliveries will
	be put on hold.  concurrencyremote is limited at
	compile time to 120.

<?php
}

function concurrencylocal_help_cr(){
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
