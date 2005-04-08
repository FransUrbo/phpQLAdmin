<?php
// control attribute plugin for
// ldapuid (user id of virtual user)
// ldapgid (group id of virtual user)
//
// $Id: attrib.control.ldapuid.inc,v 2.16 2005-01-29 18:46:57 turbo Exp $

// {{{ ldapuid_check(type)
function ldapuid_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["ldapuid"] == "") {
		$error["ldapuid"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["ldapuid"])) {
		$error["ldapuid"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	// missing
	if($_REQUEST["ldapgid"] == "") {
		$error["ldapgid"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check validity
	if(preg_match("/[^0-9]/", $_REQUEST["ldapgid"])) {
		$error["ldapgid"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	if($is_error == false)
	  return true;
}
// }}}

// {{{ ldapuid_init(host)
// init values
function ldapuid_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$attribs = array("ldapuid" => pql_get_define("PQL_ATTR_LDAPUID"),
					 "ldapgid" => pql_get_define("PQL_ATTR_LDAPGID"));
	foreach($attribs as $key => $attrib) {
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											$attrib);
	}
}
// }}}


// {{{ ldapuid_print_view(host)
function ldapuid_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapuid_init($host);

	// set empty field to "not set"
	$attribs = array("ldapuid" => pql_get_define("PQL_ATTR_LDAPUID"),
					 "ldapgid" => pql_get_define("PQL_ATTR_LDAPGID"));
	foreach($attribs as $key => $attrib) {
		if($_REQUEST[$key] == "")
		  $_REQUEST[$key] = $LANG->_('Not set');
	}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">uid, gid (<?=$LANG->_('virtual user settings')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">UID</td>
        <td><?=$_REQUEST["ldapuid"]?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">GID</td>
        <td><?=$_REQUEST["ldapgid"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapuid"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapuid_print_form(void)
function ldapuid_print_form() {
	global $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">uid, gid (<?=$LANG->_('virtual user settings')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">UID</td>
          <td><?=pql_format_error_span($error["ldapuid"])?><input type="text" name="ldapuid" value="<?=$_REQUEST["ldapuid"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title">GID</td>
          <td><?=pql_format_error_span($error["ldapgid"])?><input type="text" name="ldapgid" value="<?=$_REQUEST["ldapgid"]?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('These must be a valid user ID and group ID for the system which is running qmail-ldap. Setting these to 0 (root) is a bad idea')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ ldapuid_save(type, host)
function ldapuid_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		$attribs = array("ldapuid" => pql_get_define("PQL_ATTR_LDAPUID"),
						 "ldapgid" => pql_get_define("PQL_ATTR_LDAPGID"));
		foreach($attribs as $key => $attrib)
		  $return[] = pql_modify_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib, '', $_REQUEST[$key]);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('virtual user options')))
			. ": " . pql_format_error(0);
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
// }}}


// {{{ ldapuid_help(void)
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
// }}}

// {{{ ldapuid_help_cr(void)
function ldapuid_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
