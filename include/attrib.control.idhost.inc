<?php
// control attribute plugin for
// idhost (host name for message id's)
//
// $Id: attrib.control.idhost.inc,v 2.5 2003-05-05 14:11:12 turbo Exp $

function idhost_check($type){
	global $error, $idhost;

	// validity
	if($idhost != "" and !check_hostaddress($idhost)){
		$error["idhost"] = PQL_LANG_INVALID;
		return false;
	}

	return true;
}


function idhost_init($host) {
	// init values
  global $_pql_control, $idhost;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "idhost");

	if(!is_null($value)){
		$idhost = $value[0];
	}
}


function idhost_print_view($host) {
	global $_pql_control, $idhost;

	// init data
	idhost_init($host);

	// set empty field to default value
	if($idhost == ""){
		$idhost = $host . ", (default, taken from 'me')";
 	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">idhost (host name for Message-ID's)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Host</td>
        <td><?php echo $idhost; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=idhost"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function idhost_print_form(){
	global $attrib, $idhost, $PHP_SELF, $error, $mxhost;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">idhost (host name for Message-ID's)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Host</td>
          <td><?php echo format_error($error["idhost"]); ?><input type="text" name="idhost" value="<?php echo $idhost; ?>"> Default: <?=$mxhost?> (value in 'me')</td>
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

function idhost_save($type, $host) {
	global $_pql_control, $attrib, $idhost, $error;

	// recontrol values
	if(!idhost_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "idhost", $idhost)){
			$msg = "Successfully saved idhost options";
		} else {
			$msg = "Failed saving idhost: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function idhost_help() {
	?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
	<?php
}

function idhost_help_cr(){
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
