<?php
// control attribute plugin for
// 	ldapDefaultQuota (default quota for all mail users)
// or, if using new style quota
// 	defaultQuotaSize and defaultQuotaCount
//
// $Id: attrib.control.ldapdefaultquota.inc,v 2.25 2007-02-15 12:07:24 turbo Exp $

// {{{ ldapdefaultquota_check(type)
function ldapdefaultquota_check($type) {
    global $error, $LANG;
    
	$failed = false;

	// Just to save some typing.
	$options  = (!empty($_REQUEST["options"]) ? $_REQUEST["options"] : "");
	$maxsize  = (!empty($_REQUEST["maxsize"]) ? $_REQUEST["maxsize"] : "");
	$maxmails = (!empty($_REQUEST["maxmails"]) ? $_REQUEST["maxmails"] : "");
	
	// the checks only make sense if we're doing user defined
	// mailquotas, not if we're setting this user to the standard
	// in controls/ldapdefaultquota
	
	// empty
	if(($maxsize == "") and ($options == "user")) {
		$error["maxsize"] = $LANG->_('Missing');
		$failed = true;
	}
	
	// valid (numeric)
	if(($maxsize != "") and preg_match("/[^0-9]/", $maxsize) and ($options == "user")) {
		$error["maxsize"] = $LANG->_('Invalid');
		$failed = true;
	}
	
	// bigger than 0
	if(($error["maxsize"] == "") and ($maxsize <= 0) and ($options == "user")) {
		$error["maxsize"] = $LANG->_('Must be above zero');
		$failed = true;
	}
	
	// empty
	if(($maxmails == "") and ($options == "user")) {
		$error["maxmails"] = $LANG->_('Missing');
		$failed = true;
	}
	
	// valid (numeric)
	if(($maxmails != "") and preg_match("/[^0-9]/", $maxmails) and ($options == "user")) {
		$error["maxmails"] = $LANG->_('Invalid');
		$failed = true;
	}
	
	// bigger than 0
	if(($error["maxmails"] == "") and ($maxmails <= 0) and ($options == "user")) {
		$error["maxmails"] = $LANG->_('Must be above zero');
		$failed = true;
	}
	
	if($failed)
	  return false;
	else
	  return true;
}
// }}}

// {{{ ldapdefaultquota_init(host)
function ldapdefaultquota_init($host) {
    global $_pql_control;

    // fetch data from ldap server
	if($host == "Global")
	  $quota = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
								 pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $quota = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"));

    if(!is_null($quota)) {
		$quota = pql_parse_quota($quota);
		
		$_REQUEST["maxmails"] = $quota["maxmails"];
		$_REQUEST["maxsize"]  = $quota["maxsize"];
		$_REQUEST["options"]  = "user";

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
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">
<?php if(!@$_SESSION["NEW_STYLE_QUOTA"])  { ?>
      ldapDefaultQuota
<?php } else { ?>
      defaultQuotaSize, defaultQuotaCount
<?php } ?>
      (<?php echo $LANG->_('Default quota')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Maximum number of mails')?></td>
        <td class="<?php echo $class?>"><?php echo $quota["maxmails"]?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change option').' '.pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Maximum size of mailbox')?></td>
        <td class="<?php echo $class?>"><?php echo $quota["maxsize"]?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change option').' '.pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA")?>"></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Change quota')?></th>
<?php if(!@empty($_SESSION["USE_CONTROLS"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="default" <?php if($_REQUEST["options"] == "default"){ echo "checked"; }?>> Unlimited <input type="radio" name="options" value="user" <?php if($_REQUEST["options"] == "user"){ echo "checked"; }?>> <?php echo $LANG->_('User defined')?></td>
        </tr>

<?php } else { ?>
        <input type="hidden" name="options" value="user">
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Maximum size of mailbox')?></td>
          <td><?php echo pql_format_error_span($error["maxsize"]); ?><input type="text" name="maxsize" value="<?php echo $_REQUEST["maxsize"]?>"> KB</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Maximum number of mails')?></td>
          <td><?php echo pql_format_error_span($error["maxmails"]); ?><input type="text" name="maxmails" value="<?php echo $_REQUEST["maxmails"]?>"> Mails</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><?php echo pql_format_error_span($error["selected"]); ?><img src="images/info.png" width="16" height="16" border="0"> Note: This affects to all user which have set quota to standard.<br>It's recommended that you set both, max mails and max size for default quota.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// '
// }}}

// {{{ ldapdefaultquota_save(type, host)
function ldapdefaultquota_save($type, $host) {
    global $_pql_control, $LANG;
	$success = 0;

	// Just incase user 'forgets' to tick in the 'User defined' field...
	if(@empty($_REQUEST["options"]) and (!@empty($_REQUEST["maxsize"]) or !@empty($_REQUEST["maxmails"]))) {
		// Neither 'Standard' nor 'User defined' entered, but we have value(s) for quota
		$_REQUEST["options"] = "user";
	}

	switch($_REQUEST["options"]) {
      case "user":
		if($_REQUEST["maxsize"] != "")
		  $mailquota[] = ($_REQUEST["maxsize"] * 1024) . "S";
		else
		  $_REQUEST["maxsize"] = $LANG->_('Unlimited size');

		if($_REQUEST["maxmails"] != "")
		  $mailquota[] = $_REQUEST["maxmails"] . "C";
		else
		  $_REQUEST["maxmails"] = $LANG->_('Unlimited amount');

		if(is_array($mailquota))
		  $mailquota = join(",", $mailquota);

		$msg = pql_complete_constant($LANG->_("Successfully changed %what% to %new%"),
									 array('what' => $LANG->_('quota'),
										   'new'  => pql_ldap_mailquota(array('maxsize'  => ($_REQUEST["maxsize"] / 1024),
																			  'maxmails' => $_REQUEST["maxmails"]), 0)));
		break;

      case "default":
		$_REQUEST["maxsize"] = 0;
		$_REQUEST["maxmails"] = 0;

		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => $LANG->_('Standard')));
    }
	
    switch($type) {
      case "modify":
      case "add":
		// {{{ save values
		if(@$_SESSION["OLD_STYLE_QUOTA"])
		  $entry[pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA")] = $mailquota;

		if(@$_SESSION["NEW_STYLE_QUOTA"])  {
		  $entry[pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA_SIZE")]  = $_REQUEST["maxsize"] * 1024;
		  $entry[pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA_COUNT")] = $_REQUEST["maxmails"];
		}

		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host, '', '', $entry))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute(
								  pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
								  '', 1, $entry))
			$success = 1;
		}

		if(!$success)
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}
							    
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
