<?php
// control attribute plugin for
// ldapdefaultdotmode (default mode how to interpret .qmail files)
//
// $Id: attrib.control.ldapdefaultdotmode.inc,v 2.1 2002-12-17 06:21:05 turbo Exp $

function ldapdefaultdotmode_check($type) {
	global $error, $ldapdefaultdotmode;
	
	// missing
	if($ldapdefaultdotmode == ""){
		$error["ldapdefaultdotmode"] = PQL_MISSING;
		return false;
	}
	
	switch($ldapdefaultdotmode){
	  case "both":
	  case "dotonly":
	  case "ldaponly":
	  case "ldapwithprog":
	  case "none":
		break;
	  default:
		$error["ldapdefaultdotmode"] = PQL_INVALID;
		return false;
	}
	
	return true;
}

function ldapdefaultdotmode_init($host) {
	// init values
	global $_pql_control, $ldapdefaultdotmode;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
																		 "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
																		 "ldapdefaultdotmode");
	
	if(!is_null($value)){
		$ldapdefaultdotmode = $value[0];
	}
}

function ldapdefaultdotmode_print_view($host) {
	global $_pql_control, $ldapdefaultdotmode;

	// init data
	ldapdefaultdotmode_init($host);

	// set empty field to "not set"
	if($ldapdefaultdotmode == ""){
		$ldapdefaultdotmode = "not set";
 	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapdefaultdotmode (Default dot mode)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Mode</td>
        <td><?php echo $ldapdefaultdotmode; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=ldapdefaultdotmode"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function ldapdefaultdotmode_print_form() {
	global $attrib, $ldapdefaultdotmode, $PHP_SELF, $error;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapdefaultdotmode (Default dot mode)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Mode</td>
          <td><?php echo format_error($error["ldapdefaultdotmode"]); ?>
            <select name="ldapdefaultdotmode">
              <option value="ldaponly" <?php if($ldapdefaultdotmode == "ldaponly"){ echo "SELECTED"; }?>>ldaponly (ldap attribute "deliveryProgramPath" and .qmail files are ignored)</option>
              <option value="dotonly" <?php if($ldapdefaultdotmode == "dotonly"){ echo "SELECTED"; }?>>dotonly (only .qmail)</option>
              <option value="ldapwithprog" <?php if($ldapdefaultdotmode == "ldapwithprog"){ echo "SELECTED"; }?>>ldapwithprog (attribute "deliveryProgramPath is used if existant, .qmail files are ignored))</option>
              <option value="both" <?php if($ldapdefaultdotmode == "both"){ echo "SELECTED"; }?>>both (ldap attribute "deliveryProgramPath" and .qmail files are used)</option>
              <option value="none" <?php if($ldapdefaultdotmode == "none"){ echo "SELECTED"; }?>>none (both ldap attribute and .qmail files are ignored)</option>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">This affects ldap delivery only. It doesn't work for local non-ldap deliveries.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>
<?php
}

function ldapdefaultdotmode_save($type, $host) {
	global $_pql_control, $attrib, $ldapdefaultdotmode, $error;

	// recontrol values
	if(!ldapdefaultdotmode_check($type)){
 		return false;
	}

	switch($type){
		case "modify";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
																			 "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
																			 "ldapdefaultdotmode", $ldapdefaultdotmode)){
      	$msg = "Successfully saved ldapdefaultdotmode options";
			} else {
				$msg = "Failed saving ldapdefaultdotmode: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
