<?php
// control attribute plugin for
// $ldapdefaultquota (default quota for all mail users)

// $Id: attrib.control.ldapdefaultquota.inc,v 2.4 2003-04-04 06:54:30 turbo Exp $

function ldapdefaultquota_check($type){
	global $options, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $error;

	// empty
	if($maxsize == "" and $options == "user" and $use_maxsize != ""){
		$error["maxsize"] = PQL_LANG_MISSING;
		$failed = true;
	}

	// valid (numeric)
	if($maxsize != "" and preg_match("/[^0-9]/", $maxsize) and $options == "user" and $use_maxsize != ""){
		$error["maxsize"] = PQL_LANG_INVALID;
		$failed = true;
	}

	// bigger than 0
	if($error["maxsize"] == "" and $maxsize <= 0 and $options == "user" and $use_maxsize != ""){
		$error["maxsize"] = PQL_LANG_GREATER_ZERO;
		$failed = true;
	}

	// empty
	if($maxmails == ""  and $options == "user" and $use_maxmails != ""){
		$error["maxmails"] = PQL_LANG_MISSING;
		$failed = true;
	}

	// valid (numeric)
	if($maxmails != "" and preg_match("/[^0-9]/", $maxmails)  and $options == "user" and $use_maxmails != ""){
		$error["maxmails"] = PQL_LANG_INVALID;
		$failed = true;
	}

	// bigger than 0
	if($error["maxmails"] == "" and $maxmails <= 0 and $options == "user" and $use_maxmails != ""){
		$error["maxmails"] = PQL_LANG_GREATER_ZERO;
		$failed = true;
	}

	// userdefined, but no quota selected;
	if($options == "user" and $use_maxsize == "" and $use_maxmails == ""){
		$error["selected"] = PQL_LANG_MAILQUOTA_NONESELECTED . "<br>";
		$failed = true;
	}

	if($failed){
		return false;
	} else {
		return true;
	}
}

function ldapdefaultquota_init($host){
	global $_pql_control, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $options, $quota;
	$quota = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "ldapdefaultquota");

	if(!is_null($quota)){
		$quota = pql_parse_quota($quota[0]);
		
		$maxmails = $quota["maxmails"];
		
		if($maxmails != ""){
			$use_maxmails = "1";
		}
		
		$maxsize = $quota["maxsize"];
		
		if($maxsize != ""){
			$use_maxsize = "1";
		}
		
		$options = "user";
	} else {
		$options = "default";
	}
}

function ldapdefaultquota_print_view($host) {
	// shows information about defaultquota
	global $quota;

	ldapdefaultquota_init($host);

	$ldapdefaultquota = pql_ldap_mailquota($quota);

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldapdefaultquota (Default quota)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $ldapdefaultquota; ?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute?attrib=ldapdefaultquota"><img src="images/edit.png" width="12" height="12" border="0">change default quota</a>
        </td>
      </tr>
    </th>
  </table>
<?php
}

function ldapdefaultquota_print_form($type = "modify"){
	global $use_maxmails, $use_maxsize, $maxmails, $maxsize, $options, $PHP_SELF, $attrib, $error;

?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=PQL_LANG_MAILQUOTA_CHANGE?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=PQL_LANG_OPTIONS?></td>
          <td><input type="radio" name="options" value="default" <?php if($options == "default"){ echo "checked"; }?>> Unlimited <input type="radio" name="options" value="user" <?php if($options == "user"){ echo "checked"; }?>> <?=PQL_LANG_MAILQUOTA_USERDEFINED?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=PQL_LANG_MAILQUOTA_MAXSIZE_TITLE?></td>
          <td><input type="checkbox" name="use_maxsize" value="1" <?php if($use_maxsize == "1"){ echo "checked"; }?>> <?php echo format_error($error["maxsize"]); ?><input type="text" name="maxsize" value="<?=$maxsize?>"> KB</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=PQL_LANG_MAILQUOTA_MAXMAILS_TITLE?></td>
          <td><input type="checkbox" name="use_maxmails" value="1" <?php if($use_maxmails == "1"){ echo "checked"; }?>> <?php echo format_error($error["maxmails"]); ?><input type="text" name="maxmails" value="<?=$maxmails?>"> Mails</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><?php echo format_error($error["selected"]); ?><img src="images/info.png" width="16" height="16" border="0"> Note: This affects to all user which have set quota to standard.<br>It's recommended that you set both, max mails and max size for default quota.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?=PQL_LANG_SAVE?>">
  </form>
<?php
}

function ldapdefaultquota_save($type, $host){
	global $_pql_control, $maxsize, $maxmails, $options, $use_maxmails, $use_maxsize;

	// recontrol values
	if(!ldapdefaultquota_check($type)){
 		return false;
	}

	switch($options){
		case "user":
			if($use_maxsize != ""){
				$maxsize = $maxsize * 1024;
				$mailquota[] = $maxsize . "S";
			} else {
				$maxsize = PQL_LANG_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED;
			}

			if($use_maxmails != ""){
				$mailquota[] = $maxmails . "C";
			} else {
				$maxmails = PQL_LANG_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED;
    	    }

			if(is_array($mailquota)){
				$mailquota = join(",", $mailquota);
			}

			$msg = pql_complete_constant(PQL_LANG_MAILQUOTA_CHANGE_OK, array("quota" =>  pql_ldap_mailquota(array("maxsize" => $maxsize / 1024, "maxmails" => $maxmails))));
			break;

		case "default":
			$mailquota = "";
			$msg = pql_complete_constant(PQL_LANG_MAILQUOTA_CHANGE_OK, array("quota" => PQL_LANG_MAILQUOTA_DEFAULT));
	}

	switch($type){
		case "modify":
			if(!pql_control_replace_attribute($_pql_control->ldap_linkid,
							"cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"], "ldapdefaultquota", $mailquota)){
				$msg = PQL_LANG_MAILQUOTA_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql_control->ldap_linkid);
			}

			attribute_forward($msg);
			break;

		default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function ldapdefaultquota_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
	?>
	The default quota. Overridden by user's attribute mailquota if existant.
	If you are setting a default quota you should always set both size and count.
	<br><br>
	Don't forget to set quotawarning, otherwise no quota warning messages are issued.
	<?php
}

function ldapdefaultquota_help_cr(){
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
?>
