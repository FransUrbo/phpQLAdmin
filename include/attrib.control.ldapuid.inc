<?php
// control attribute plugin for
// ldapuid (user id of virtual user)
// ldapgid (group id of virtual user)
//
// $Id: attrib.control.ldapuid.inc,v 2.5 2003-05-05 14:11:12 turbo Exp $

function ldapuid_check($type) {
	global $error, $ldapuid, $ldapgid;

	// missing
	if($ldapuid == ""){
		$error["ldapuid"] = PQL_LANG_MISSING;
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $ldapuid)){
		$error["ldapuid"] = PQL_LANG_INVALID;
		$is_error = true;
	}
	
	// missing
	if($ldapgid == ""){
		$error["ldapgid"] = PQL_LANG_MISSING;
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $ldapgid)){
		$error["ldapgid"] = PQL_LANG_INVALID;
		$is_error = true;
	}
	
	if($is_error == false){
		return true;
 	}
}

function ldapuid_init($host) {
	// init values
	global $_pql_control, $ldapuid, $ldapgid;
	
	// fetch data from ldap server
	$attribs = array("ldapuid", "ldapgid");

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
}

function ldapuid_print_view($host) {
	global $_pql_control, $ldapuid, $ldapgid;

	// init data
	ldapuid_init($host);

	// set empty field to "not set"
	$attribs = array("ldapuid", "ldapgid");

	foreach($attribs as $attrib){
		if($$attrib == ""){
			$$attrib = "not set";
		}
	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">uid, gid (virtual user settings)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">UID</td>
        <td><?php echo $ldapuid; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">GID</td>
        <td><?php echo $ldapgid; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapuid"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function ldapuid_print_form() {
	global $attrib, $ldapuid, $ldapgid, $PHP_SELF, $error, $mxhost;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">uid, gid (virtual user settings)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">UID</td>
          <td><?php echo format_error($error["ldapuid"]); ?><input type="text" name="ldapuid" value="<?php echo $ldapuid; ?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">GID</td>
          <td><?php echo format_error($error["ldapgid"]); ?><input type="text" name="ldapgid" value="<?php echo $ldapgid; ?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">These must be a valid user ID and group ID for the system which is running qmail-ldap. Setting these
		to 0 (root) is a bad idea.</td>
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

function ldapuid_save($type, $host) {
	global $_pql_control, $attrib, $ldapuid, $ldapgid, $error;

	// recontrol values
	if(!ldapuid_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "ldapuid", $ldapuid);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "ldapgid", $ldapgid);

		if(in_array(false, $return)){
			$msg = "Error saving virtual user options: " . pql_ldap_error($_pql_control->ldap_linkid);
		} else {
    		$msg = "Successfully saved virtual user options";
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function ldapuid_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
	?>
	<b>UID</b><br>
  The system user id your virtual users are mapped to.
	You can add as much users as you want to you ldap directory without having system
	accounts for them, they are all mapped to a single system user id - this one is defined here.
	<br><br>
	<b>GID</b>
	<br>
	The system group id all your virtual users are mapped to.
  <?php
}

function ldapuid_help_cr(){
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
