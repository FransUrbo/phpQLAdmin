<?php
// control attribute plugin for
// timeoutsmtpd (how long to wait for smtp client)
//
// $Id: attrib.control.timeoutsmtpd.inc,v 2.6 2003-05-05 14:11:12 turbo Exp $

function timeoutsmtpd_check($type) {
	global $error, $timeoutsmtpd;

	// number ?
	if(preg_match ("/[^0-9]/", $timeoutsmtpd)){
		$error["timeoutsmtpd"] = PQL_LANG_INVALID;
		return false;
	}

	// must be > 0
	if($error["timeoutsmtpd"] == "" and $timeoutsmtpd <= 0){
		$error["timeoutsmtpd"] = PQL_LANG_GREATER_ZERO;
		return false;
	}

	return true;
}

function timeoutsmtpd_init($host) {
	// init values
	global $_pql_control, $timeoutsmtpd;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "timeoutsmtpd");
	
	if(!is_null($value)){
		$timeoutsmtpd = $value[0];
	}
}

function timeoutsmtpd_print_view($host) {
	global $_pql_control, $timeoutsmtpd;

	// init data
	timeoutsmtpd_init($host);

	// set empty field to "not set"
	if($timeoutsmtpd == ""){
		$timeoutsmtpd = "1200 s (default)";
 	} else {
		$timeoutsmtpd .= " s";
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">timeoutsmtpd (How long to wait for SMTP client)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Wait</td>
        <td><?php echo $timeoutsmtpd; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=timeoutsmtpd"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function timeoutsmtpd_print_form(){
	global $attrib, $timeoutsmtpd, $PHP_SELF, $error, $mxhost;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">timeoutsmtpd (How long to wait for SMTP client)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Wait</td>
          <td><?php echo format_error($error["timeoutsmtpd"]); ?><input type="text" name="timeoutsmtpd" value="<?php echo $timeoutsmtpd; ?>"> seconds</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Empty field to set to the default value (1200 seconds)</td>
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

function timeoutsmtpd_save($type, $host) {
	global $_pql_control, $attrib, $timeoutsmtpd, $error, $mxhost;

	// recontrol values
	if(!timeoutsmtpd_check($type)){
 		return false;
	}
	
	switch($type){
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $mxhost . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "timeoutsmtpd", $timeoutsmtpd)){
			$msg = "Successfully saved timeoutsmtpd options";
		} else {
			$msg = "Failed saving timeoutsmtpd: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function timeoutsmtpd_help() {
?>
	Number of seconds qmail-smtpd will wait for each new
	buffer of data from the remote SMTP client.  Default:
	1200.
<?php
}

function timeoutsmtpd_help_cr(){
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
