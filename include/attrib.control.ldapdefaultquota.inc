<?php
// control attribute plugin for
// $ldapdefaultquota (default quota for all mail users)

// $Id: attrib.control.ldapdefaultquota.inc,v 2.11.2.1 2003-12-09 16:18:24 dlw Exp $

function ldapdefaultquota_check($type) {
    global $options, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $error, $view, $LANG;
    
    // empty
    if($maxsize == "" and $options == "user" and $use_maxsize != "") {
		$error["maxsize"] = $LANG->_('Missing');
		$failed = true;
    }
    
    // valid (numeric)
    if($maxsize != "" and preg_match("/[^0-9]/", $maxsize) and $options == "user" and $use_maxsize != "") {
		$error["maxsize"] = $LANG->_('Invalid');
		$failed = true;
    }
    
    // bigger than 0
    if($error["maxsize"] == "" and $maxsize <= 0 and $options == "user" and $use_maxsize != "") {
		$error["maxsize"] = $LANG->_('Must be above zero');
		$failed = true;
    }
    
    // empty
    if($maxmails == ""  and $options == "user" and $use_maxmails != "") {
		$error["maxmails"] = $LANG->_('Missing');
		$failed = true;
    }
    
    // valid (numeric)
    if($maxmails != "" and preg_match("/[^0-9]/", $maxmails)  and $options == "user" and $use_maxmails != "") {
		$error["maxmails"] = $LANG->_('Invalid');
		$failed = true;
    }
    
    // bigger than 0
    if($error["maxmails"] == "" and $maxmails <= 0 and $options == "user" and $use_maxmails != "") {
		$error["maxmails"] = $LANG->_('Must be above zero');
		$failed = true;
    }
    
    // userdefined, but no quota selected;
    if($options == "user" and $use_maxsize == "" and $use_maxmails == "") {
		$error["selected"] = $LANG->_('You must select at least one type of quota') . "<br>";
		$failed = true;
    }
    
    if($failed)
      return false;
    
    return true;
}

function ldapdefaultquota_init($host) {
    global $_pql_control, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $options, $quota, $view;
    
    $quota = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTQUOTA"));
    
    if(!is_null($quota)) {
		$quota = pql_parse_quota($quota[0]);
		
		$maxmails = $quota["maxmails"];
		if($maxmails != "")
		  $use_maxmails = "1";
		
		$maxsize = $quota["maxsize"];
		if($maxsize != "")
		  $use_maxsize = "1";
		
		$options = "user";
    } else
      $options = "default";
}

// shows information about defaultquota
function ldapdefaultquota_print_view($host) {
    global $quota, $LANG;
    
    ldapdefaultquota_init($host);
    
    $ldapdefaultquota = pql_ldap_mailquota($quota);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">ldapdefaultquota (<?=$LANG->_('Default quota')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$ldapdefaultquota?></td>
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

function ldapdefaultquota_print_form($type = "modify") {
    global $use_maxmails, $use_maxsize, $maxmails, $maxsize, $options, $PHP_SELF, $attrib, $error, $mxhost, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change quota')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="default" <?php if($options == "default"){ echo "checked"; }?>> Unlimited <input type="radio" name="options" value="user" <?php if($options == "user"){ echo "checked"; }?>> <?=$LANG->_('User defined')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum size of mailbox')?></td>
          <td><input type="checkbox" name="use_maxsize" value="1" <?php if($use_maxsize == "1"){ echo "checked"; }?>> <?php echo pql_format_error_span($error["maxsize"]); ?><input type="text" name="maxsize" value="<?=$maxsize?>"> KB</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum number of mails')?></td>
          <td><input type="checkbox" name="use_maxmails" value="1" <?php if($use_maxmails == "1"){ echo "checked"; }?>> <?php echo pql_format_error_span($error["maxmails"]); ?><input type="text" name="maxmails" value="<?=$maxmails?>"> Mails</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><?php echo pql_format_error_span($error["selected"]); ?><img src="images/info.png" width="16" height="16" border="0"> Note: This affects to all user which have set quota to standard.<br>It's recommended that you set both, max mails and max size for default quota.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// '
function ldapdefaultquota_save($type, $host) {
    global $_pql_control, $maxsize, $maxmails, $options, $use_maxmails, $use_maxsize, $LANG;
	
	switch($options) {
      case "user":
		if($use_maxsize != "") {
			$maxsize = $maxsize * 1024;
			$mailquota[] = $maxsize . "S";
		} else
		  $maxsize = $LANG->_('Unlimited size');
		
		if($use_maxmails != "")
		  $mailquota[] = $maxmails . "C";
		else
		  $maxmails = $LANG->_('Unlimited amount');
		
		if(is_array($mailquota))
		  $mailquota = join(",", $mailquota);
		
		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => pql_ldap_mailquota(array('maxsize'  => $maxsize / 1024,
																			  'maxmails' => $maxmails))));
		break;
		
      case "default":
		$mailquota = "";
		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => $LANG->_('Standard')));
    }
	
    switch($type) {
      case "modify":
      case "add";
		if(!pql_control_replace_attribute($_pql_control->ldap_linkid,
										  pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										  pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTQUOTA"),
										  $mailquota))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('quota'))) . ": " . pql_ldap_error($_pql_control->ldap_linkid);
															 
		attribute_forward($msg);
		break;
							    
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

function ldapdefaultquota_help() {
	global $LANG;
?>
	The default quota. Overridden by user's attribute mailquota if existant.
	If you are setting a default quota you should always set both size and count.
	<br><br>
	Don't forget to set quotawarning, otherwise no quota warning messages are issued.
<?php
}

function ldapdefaultquota_help_cr() {
	global $LANG;
    echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
