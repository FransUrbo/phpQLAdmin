<?php
// control attribute plugin for
// helohost (host name used in SMTP HELO command)
//
// $Id: attrib.control.helohost.inc,v 2.6 2003-05-06 07:52:59 turbo Exp $

function helohost_check($type) {
	global $error, $helohost;

	// validity
	if($helohost != "" and !check_hostaddress($helohost)){
		$error["helohost"] = PQL_LANG_INVALID;
		return false;
	}
	
	return true;
}

function helohost_init($host) {
	// init values
	global $_pql_control, $helohost;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "helohost");

	if(!is_null($value)){
		$helohost = $value[0];
	}
}

function helohost_print_view($host) {
	global $_pql_control, $helohost;

	// init data
	helohost_init($host);

	// set empty field to default value
	if($helohost == "")
	  $helohost = $host . ", (default, taken from 'me')";
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">helohost (Helo host)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Host</td>
        <td><?php echo $helohost; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=helohost"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function helohost_print_form() {
	global $attrib, $helohost, $PHP_SELF, $error, $mxhost;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">helohost (Helo host)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Host</td>
          <td><?php echo format_error($error["helohost"]); ?><input type="text" name="helohost" value="<?php echo $helohost; ?>"> Default: <?=$mxhost?> (value in 'me')</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0">Leave the fields empty to use default configuration.</td>
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

function helohost_save($type, $host) {
	global $_pql_control, $attrib, $helohost, $error;

	// recontrol values
	if(!helohost_check($type)){
 		return false;
	}
	
	switch($type){
	  case "modify";
	  case "add";
		// save entries
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "helohost", $helohost)){
			$msg = "Successfully saved helohost options";
		} else {
			$msg = "Failed saving helohost: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function helohost_help() {
	?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
	<?php
}

function helohost_help_cr() {
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
