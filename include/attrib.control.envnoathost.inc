<?php
// control attribute plugin for
// envnoathost (default prefix for mailbox paths)
//
// $Id: attrib.control.envnoathost.inc,v 2.6 2003-05-05 14:11:12 turbo Exp $

function envnoathost_check($type) {
	global $error, $envnoathost;

	// missing
	if($envnoathost == ""){
		$error["envnoathost"] = PQL_LANG_MISSING;
		return false;
	}

	// validity
	if(!check_hostaddress($envnoathost)){
		// "
		$error["envnoathost"] = PQL_LANG_INVALID;
		return false;
	}

	return true;
}

function envnoathost_init($host) {
	// init values
	global $_pql_control, $envnoathost;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "envnoathost");

	if(!is_null($value)){
		$envnoathost = $value[0];
	}
}

function envnoathost_print_view($host) {
	global $_pql_control, $envnoathost;

	// init data
	envnoathost_init($host);

	// set empty field to default value
	if($envnoathost == ""){
		$envnoathost = $host . ", (default, taken from 'me')";
 	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">envnoathost (Default domain for addresses without @)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Email</td>
        <td><?php echo $envnoathost; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=envnoathost"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function envnoathost_print_form() {
	global $attrib, $envnoathost, $PHP_SELF, $error, $mxhost;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">envnoathost (Default domain for addresses without @)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Address</td>
          <td><?php echo format_error($error["envnoathost"]); ?><input type="text" name="envnoathost" value="<?php echo $envnoathost; ?>"> Default: <?=$mxhost?> (value in 'me')</td>
        </tr>

        <tr>
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Leave the fields empty to use default configuration.</td>
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

function envnoathost_save($type, $host) {
	global $_pql_control, $attrib, $envnoathost, $error;

	// recontrol values
	if(!envnoathost_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "envnoathost", $envnoathost)){
			$msg = "Successfully saved envnotathost options";
		} else {
			$msg = "Failed saving envnoathost: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}
		
		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function envnoathost_help() {
?>
	Presumed domain name for addresses without @ signs.
	Default:  me, if that is supplied; otherwise the
	literal name envnoathost, which is probably not what
	you want.  If qmail-send sees an envelope recipient
	address without an @ sign, it appends @envnoathost.
<?php
}


function envnotathost_help_cr(){
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
