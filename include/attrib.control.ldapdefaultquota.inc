<?php
// control attribute plugin for
// $ldapdefaultquota (default quota for all mail users)
//
// $Id: attrib.control.ldapdefaultquota.inc,v 2.15 2004-03-11 16:31:54 turbo Exp $

// {{{ ldapdefaultquota_check(type)
function ldapdefaultquota_check($type) {
    global $error, $LANG;
    
    // empty
    if($_REQUEST["maxsize"] == "" and $_REQUEST["options"] == "user" and $_REQUEST["use_maxsize"] != "") {
		$error["maxsize"] = $LANG->_('Missing');
		$failed = true;
    }
    
    // valid (numeric)
    if($_REQUEST["maxsize"] != "" and preg_match("/[^0-9]/", $_REQUEST["maxsize"]) and
	   $_REQUEST["options"] == "user" and $_REQUEST["use_maxsize"] != "") {
		$error["maxsize"] = $LANG->_('Invalid');
		$failed = true;
    }
    
    // bigger than 0
    if($error["maxsize"] == "" and $_REQUEST["maxsize"] <= 0 and
	   $_REQUEST["options"] == "user" and $_REQUEST["use_maxsize"] != "") {
		$error["maxsize"] = $LANG->_('Must be above zero');
		$failed = true;
    }
    
    // empty
    if($_REQUEST["maxmails"] == ""  and $_REQUEST["options"] == "user" and
	   $_REQUEST["use_maxmails"] != "") {
		$error["maxmails"] = $LANG->_('Missing');
		$failed = true;
    }
    
    // valid (numeric)
    if($_REQUEST["maxmails"] != "" and preg_match("/[^0-9]/", $_REQUEST["maxmails"]) and
	   $_REQUEST["options"] == "user" and $_REQUEST["use_maxmails"] != "") {
		$error["maxmails"] = $LANG->_('Invalid');
		$failed = true;
    }
    
    // bigger than 0
    if($error["maxmails"] == "" and $_REQUEST["maxmails"] <= 0 and
	   $_REQUEST["options"] == "user" and $_REQUEST["use_maxmails"] != "") {
		$error["maxmails"] = $LANG->_('Must be above zero');
		$failed = true;
    }
    
    // userdefined, but no quota selected;
    if($_REQUEST["options"] == "user" and
	   $_REQUEST["use_maxsize"] == "" and
	   $_REQUEST["use_maxmails"] == "") {
		$error["selected"] = $LANG->_('You must select at least one type of quota') . "<br>";
		$failed = true;
    }
    
    if($failed)
      return false;
    
    return true;
}
// }}}

// {{{ ldapdefaultquota_init(host)
function ldapdefaultquota_init($host) {
    global $_pql_control;
    
    $quota = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"));
    if(!is_null($quota)) {
		$quota = pql_parse_quota($quota[0]);
		
		$_REQUEST["maxmails"] = $quota["maxmails"];
		if($maxmails != "")
		  $_REQUEST["use_maxmails"] = "1";
		
		$_REQUEST["maxsize"] = $quota["maxsize"];
		if($_REQUEST["maxsize"] != "")
		  $_REQUEST["use_maxsize"] = "1";
		
		$_REQUEST["options"] = "user";
		return($quota);
    } else
      $_REQUEST["options"] = "default";
}
// }}}

// {{{ ldapdefaultquota_print_view(host)
// shows information about defaultquota
function ldapdefaultquota_print_view($host) {
    global $LANG;
    
    $quota = ldapdefaultquota_init($host);
    $_REQUEST["ldapdefaultquota"] = pql_ldap_mailquota($quota);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldapdefaultquota (<?=$LANG->_('Default quota')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$_REQUEST["ldapdefaultquota"]?></td>
      </tr>

      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapdefaultquota"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a>
        </td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapdefaultquota_print_form(type)
function ldapdefaultquota_print_form($type = "modify") {
    global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change quota')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="default" <?php if($_REQUEST["options"] == "default"){ echo "checked"; }?>> Unlimited <input type="radio" name="options" value="user" <?php if($_REQUEST["options"] == "user"){ echo "checked"; }?>> <?=$LANG->_('User defined')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum size of mailbox')?></td>
          <td><input type="checkbox" name="use_maxsize" value="1" <?php if($_REQUEST["use_maxsize"] == "1"){ echo "checked"; }?>> <?php echo pql_format_error_span($error["maxsize"]); ?><input type="text" name="maxsize" value="<?=$_REQUEST["maxsize"]?>"> KB</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum number of mails')?></td>
          <td><input type="checkbox" name="use_maxmails" value="1" <?php if($_REQUEST["use_maxmails"] == "1"){ echo "checked"; }?>> <?php echo pql_format_error_span($error["maxmails"]); ?><input type="text" name="maxmails" value="<?=$_REQUEST["maxmails"]?>"> Mails</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><?php echo pql_format_error_span($error["selected"]); ?><img src="images/info.png" width="16" height="16" border="0"> Note: This affects to all user which have set quota to standard.<br>It's recommended that you set both, max mails and max size for default quota.</td>
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

// {{{ ldapdefaultquota_save(type, host)
function ldapdefaultquota_save($type, $host) {
    global $_pql_control, $LANG;
	
	switch($_REQUEST["options"]) {
      case "user":
		if($_REQUEST["use_maxsize"] != "") {
			$_REQUEST["maxsize"] = $_REQUEST["maxsize"] * 1024;
			$mailquota[] = $_REQUEST["maxsize"] . "S";
		} else
		  $_REQUEST["maxsize"] = $LANG->_('Unlimited size');
		
		if($_REQUEST["use_maxmails"] != "")
		  $mailquota[] = $_REQUEST["maxmails"] . "C";
		else
		  $_REQUEST["maxmails"] = $LANG->_('Unlimited amount');
		
		if(is_array($mailquota))
		  $_REQUEST["mailquota"] = join(",", $_REQUEST["mailquota"]);
		
		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => pql_ldap_mailquota(array('maxsize'  => $_REQUEST["maxsize"] / 1024,
																			  'maxmails' => $_REQUEST["maxmails"]))));
		break;
		
      case "default":
		$_REQUEST["mailquota"] = "";
		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => $LANG->_('Standard')));
    }
	
    switch($type) {
      case "modify":
      case "add";
		if(!pql_control_replace_attribute($_pql_control->ldap_linkid,
										  pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										  pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"),
										  $_REQUEST["mailquota"]))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);
															 
		attribute_forward($msg);
		break;
							    
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}


// {{{ ldapdefaultquota_help(void)
function ldapdefaultquota_help() {
	global $LANG;
?>
	The default quota. Overridden by user's attribute mailquota if existant.
	If you are setting a default quota you should always set both size and count.
	<br><br>
	Don't forget to set quotawarning, otherwise no quota warning messages are issued.
<?php
}
// }}}

// {{{ ldapdefaultquota_help_cr(void)
function ldapdefaultquota_help_cr() {
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
