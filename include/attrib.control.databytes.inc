<?php
// control attribute plugin for
// databytes (max number of bytes in message)
//
// $Id: attrib.control.databytes.inc,v 2.2 2002-12-17 12:24:35 turbo Exp $

function databytes_check($type) {
	global $error, $databytes;

	// number ?
	if($databytes != "" and preg_match ("/[^0-9]/", $databytes)){
		$error["databytes"] = PQL_INVALID;
		return false;
	}

	return true;
}

function databytes_init($host) {
	// init values
	global $_pql_control, $databytes;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
									   "databytes");
	
	if(!is_null($value)){
		$databytes = $value[0];
	}
}

function databytes_print_view($host) {
	global $_pql_control, $databytes;

	// init data
	databytes_init($host);

	// set empty field to "not set"
	if($databytes == "" or $databytes == 0){
		$databytes = "unlimited";
 	} else {
		$hr_databytes = "(" . pql_print_filesize($databytes) . ")";
		$databytes .= " Bytes";
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">databytes (Maximum number databytes in message)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Max</td>
        <td><?php echo "$databytes $hr_databytes"; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=databytes"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function databytes_print_form() {
	global $attrib, $databytes, $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">databytes (Maximum number databytes in message)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Max</td>
          <td><?php echo format_error($error["databytes"]); ?><input type="text" name="databytes" value="<?php echo $databytes; ?>"> Bytes</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set to 0 to allow unlimited message size</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function databytes_save($type, $host) {
	global $_pql_control, $attrib, $databytes, $error;

	// recontrol values
	if(!databytes_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
										 "databytes", $databytes)){
			$msg = "Successfully saved databytes options";
		} else {
			$msg = "Failed saving databytes: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function databytes_help() {
?>
	Maximum number of bytes allowed in a message, or 0 for
	no limit.  Default: 0.  If a message exceeds this
	limit, qmail-smtpd returns a permanent error code to
	the client; in contrast, if the disk is full or qmail-
	smtpd hits a resource limit, qmail-smtpd returns a
	temporary error code.
	<br><br>
	databytes counts bytes as stored on disk, not as
	transmitted through the network.  It does not count the
	qmail-smtpd Received line, the qmail-queue Received
	line, or the envelope.
<?php
}

function databytes_help_cr() {
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
