<?php
// control attribute plugin for
// doublebouncehost (username of bounce sender)
// doublebounceto (hostname of bounce sender)
//
// $Id: attrib.control.doublebouncehost.inc,v 2.8 2003-05-28 07:23:37 turbo Exp $

function doublebouncehost_check($type) {
	global $error, $doublebouncehost, $doublebounceto, $view;

	// check validity
	if($doublebounceto != "" and !check_email($doublebounceto . "@domain.tld")){
		$error["doublebounceto"] = PQL_LANG_INVALID;
		$is_error = true;
	}

	// check validity
	if($doublebouncehost != "" and !check_hostaddress($doublebouncehost)){
		// "
		$error["doublebouncehost"] = PQL_LANG_INVALID;
		$is_error = true;
	}

	if($is_error == false){
		return true;
 	}
}

// init values
function doublebouncehost_init($host) {
	global $_pql_control, $doublebouncehost, $doublebounceto, $view;

	// fetch data from ldap server
	$attribs = array("doublebouncehost", "doublebounceto");
	
	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
}

function doublebouncehost_print_view($host) {
	global $_pql_control, $doublebouncehost, $doublebounceto, $view;

	// init data
	doublebouncehost_init($host);

	// set empty fields to their default value
	if($doublebouncehost == ""){
		$doublebouncehost = $host . ", (default, taken from 'me')";
	}
	
	if($doublebounceto == ""){
		$doublebounceto = "postmaster (default)";
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">doublebouncehost, doublebounceto (Double bounce)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Host</td>
        <td><?php echo $doublebouncehost; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Receiver</td>
        <td><?php echo $doublebounceto; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=doublebouncehost"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function doublebouncehost_print_form() {
	global $attrib, $doublebouncehost, $doublebounceto, $PHP_SELF, $error, $mxhost, $view;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Double bounce</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Host</td>
          <td><?php echo format_error($error["doublebouncehost"]); ?><input type="text" name="doublebouncehost" value="<?php echo $doublebouncehost; ?>"> Default: <?=$mxhost?> (value in 'me')</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Receiver</td>
          <td><?php echo format_error($error["doublebounceto"]); ?><input type="text" name="doublebounceto" value="<?php echo $doublebounceto; ?>"> Default: postmaster</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">
		Leave the fields empty to use default configuration.
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

<?php
}

function doublebouncehost_save($type, $host) {
	global $_pql_control, $attrib, $doublebouncehost, $doublebounceto, $error, $view;

	// recontrol values
	if(!doublebouncehost_check($type)){
 		return false;
	}
	
	switch($type){
	  case "modify";
	  case "add";
		// save entries
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "doublebouncehost", $doublebouncehost);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "doublebounceto", $doublebounceto);
		
		if(in_array(false, $return)){
			$msg = "Error saving double bouncing options: " . pql_ldap_error($_pql_control->ldap_linkid);
		} else {
    		$msg = "Successfully saved double bouncing options";
    	}
		
		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function doublebouncehost_help() {
	?>
	<b>Host (doublebouncehost)</b><br>
	Double-bounce host.  Default:  me, if that is supplied;
	otherwise the literal name doublebouncehost, which is
	probably not what you want.Host name of double bounce sender.
	<br><br>
	<b>Receiver (doublebounceto)</b>
	<br>
	User to receive double-bounces.  Default:  postmaster.
	If a single-bounce notice is permanently undeliverable,
	qmail-send sends a double-bounce notice to
	doublebounceto@doublebouncehost.  (If that bounces,
	qmail-send gives up.)
	<?php
}

function doublebouncehost_help_cr() {
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
