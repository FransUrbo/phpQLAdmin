<?php
// control attribute plugin for
// localiphost (name substituted for local IP address)
//
// $Id: attrib.control.localiphost.inc,v 2.1 2002-12-17 06:21:05 turbo Exp $

function localiphost_check($type) {
	global $error, $localiphost;

	// validity
	if($localiphost != "" and !check_hostaddress($localiphost)){
		$error["localiphost"] = PQL_INVALID;
		return false;
	}
	
	return true;
}

function localiphost_init($host) {
	// init values
	global $_pql_control, $localiphost;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
									   "localiphost");
	
	if(!is_null($value)){
		$localiphost = $value[0];
	}
}

function localiphost_print_view($host) {
	global $_pql_control, $localiphost;
	
	// init data
	localiphost_init($host);

	// set empty field to default value
	if($localiphost == ""){
		$localiphost = $host . ", (default, taken from 'me')";
 	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">localiphost (Substitution for local IP)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Host</td>
        <td><?php echo $localiphost; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=localiphost"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function localiphost_print_form() {
	global $attrib, $localiphost, $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">localiphost (Substitution for local IP)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Host</td>
          <td><?php echo format_error($error["localiphost"]); ?><input type="text" name="localiphost" value="<?php echo $localiphost; ?>"> Default: <?=$host?> (value in 'me')</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0">Leave the fields empty to use default configuration.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function localiphost_save($type, $host) {
	global $_pql_control, $attrib, $localiphost, $error;

	// recontrol values
	if(!localiphost_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
										 "localiphost", $localiphost)){
			$msg = "Successfully saved localiphost options";
		} else {
			$msg = "Failed saving localiphost: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function localiphost_help() {
	?>
	Replacement host name for local IP addresses.  Default:
	me, if that is supplied.  qmail-smtpd is responsible
	for recognizing dotted-decimal addresses for the
	current host.  When it sees a recipient address of the
	form box@[d.d.d.d], where d.d.d.d is a local IP
	address, it replaces [d.d.d.d] with localiphost.  This
	is done before rcpthosts.
	<?php
}

function localiphost_help_cr() {
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
