<?php
// control attribute plugin for
// ldapuid (user id of virtual user)
// ldapgid (group id of virtual user)
//
// $Id: attrib.control.ldapuid.inc,v 2.8 2003-07-02 07:59:00 turbo Exp $

function ldapuid_check($type) {
	global $error, $ldapuid, $ldapgid, $view, $LANG;

	// missing
	if($ldapuid == "") {
		$error["ldapuid"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $ldapuid)) {
		$error["ldapuid"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	// missing
	if($ldapgid == "") {
		$error["ldapgid"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $ldapgid)) {
		$error["ldapgid"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	if($is_error == false)
	  return true;
}

// init values
function ldapuid_init($host) {
	global $_pql_control, $ldapuid, $ldapgid, $view;
	
	// fetch data from ldap server
	$attribs = array("ldapuid", "ldapgid");

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function ldapuid_print_view($host) {
	global $_pql_control, $ldapuid, $ldapgid, $view, $LANG;

	// init data
	ldapuid_init($host);

	// set empty field to "not set"
	$attribs = array("ldapuid", "ldapgid");
	foreach($attribs as $attrib) {
		if($$attrib == "")
		  $$attrib = $LANG->_('Not set');
	}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">uid, gid (<?=$LANG->_('virtual user settings')?>)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">UID</td>
        <td><?=$ldapuid?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">GID</td>
        <td><?=$ldapgid?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapuid"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function ldapuid_print_form() {
	global $attrib, $ldapuid, $ldapgid, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">uid, gid (<?=$LANG->_('virtual user settings')?>)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">UID</td>
          <td><?php echo format_error($error["ldapuid"]); ?><input type="text" name="ldapuid" value="<?=$ldapuid?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">GID</td>
          <td><?php echo format_error($error["ldapgid"]); ?><input type="text" name="ldapgid" value="<?=$ldapgid?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('These must be a valid user ID and group ID for the system which is running qmail-ldap. Setting these to 0 (root) is a bad idea')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function ldapuid_save($type, $host) {
	global $_pql_control, $attrib, $ldapuid, $ldapgid, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "ldapuid", $ldapuid);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "ldapgid", $ldapgid);

		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('virtual user options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('virtual user options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function ldapuid_help() {
	global $LANG;
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

function ldapuid_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
