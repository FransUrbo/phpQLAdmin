<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mailquota.inc,v 2.17 2004-02-14 14:25:36 turbo Exp $

function attribute_check($type) {
    global $error, $LANG;

	$failed = false;

	// Just to save some typing.
	$options = (!empty($_REQUEST["options"]) ? $_REQUEST["options"] : "");
	$use_maxsize = (!empty($_REQUEST["use_maxsize"]) ? $_REQUEST["use_maxsize"] : "");
	$use_maxmails = (!empty($_REQUEST["use_maxmails"]) ? $_REQUEST["use_maxmails"] : "");
	$maxsize = (!empty($_REQUEST["maxsize"]) ? $_REQUEST["maxsize"] : "");
	$maxmails = (!empty($_REQUEST["maxmails"]) ? $_REQUEST["maxmails"] : "");
	
	// the checks only make sense if we're doing user defined
	// mailquotas, not if we're setting this user to the standard
	// in controls/ldapdefaultquota
	
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
	if($error["maxmails"] == "" and $maxmails <= 0 and $options == "user" and $use_maxmails != ""){
		$error["maxmails"] = $LANG->_('Must be above zero');
		$failed = true;
	}
	
	// userdefined, but no quota selected;
	if($options == "user" and $use_maxsize == "" and $use_maxmails == "") {
		$error["selected"] = $LANG->_('You must select at least one type of quota');
		$failed = true;
	}
	
	if($failed) {
		return false;
	} else {
		return true;
  }
}

function attribute_init() {
    global $_pql;

    $quota = pql_get_userquota($_pql->ldap_linkid, $_REQUEST["user"]);
	if(is_array($quota)) {
		$_REQUEST["maxmails"] = $quota["maxmails"];
		if(!empty($_REQUEST["maxmails"]))
		  $_REQUEST["use_maxmails"] = "1";
		
		$_REQUEST["maxsize"] = $quota["maxsize"];
		if(!empty($_REQUEST["maxsize"]))
		  $_REQUEST["use_maxsize"] = "1";

		$_REQUEST["options"] = "user";
	} else
	  $_REQUEST["options"] = "default";
}

function attribute_print_form($type = "modify") {
    global $error, $LANG;

?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change quota')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="default" <?php if($_REQUEST["options"] == "default"){ echo "checked"; } //" ?>><?=$LANG->_('Standard')?><input type="radio" name="options" value="user" <?php if($_REQUEST["options"] == "user"){ echo "checked"; } //" ?>><?=$LANG->_('User defined')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum size of mailbox')?></td>
          <td><input type="checkbox" name="use_maxsize" value="1" <?php if($_REQUEST["use_maxsize"] == "1"){ echo "checked"; } //" ?>> <?php echo pql_format_error_span($error["maxsize"]); ?><input type="text" name="maxsize" value="<?php echo $_REQUEST["maxsize"]; ?>"> KB</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum number of mails')?></td>
          <td><input type="checkbox" name="use_maxmails" value="1" <?php if($_REQUEST["use_maxmails"] == "1"){ echo "checked";
		  } //" ?>> <?php echo pql_format_error_span($error["maxmails"]);
		  ?><input type="text" name="maxmails" value="<?php echo $_REQUEST["maxmails"]; ?>"> <?=$LANG->_('Mails')?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo pql_format_error_span($error["selected"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">
            &nbsp;<?=$LANG->_('With \'Standard\', QmailLDAP will get quotas from its configuration file')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="domain" value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
<?php if($_REQUEST["attrib"] == 'mailquota') { ?>
    <input type="hidden" name="user" value="<?=$_REQUEST["user"]?>">
<?php } ?>
    <input type="hidden" name="view" value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// '
function attribute_save($type) {
    global $_pql, $LANG;
	
	// recontrol values
	if(!attribute_check($type))
	  return false;

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
		  $mailquota = join(",", $mailquota);
		
		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => pql_ldap_mailquota(array('maxsize'  => $_REQUEST["maxsize"] / 1024,
																			  'maxmails' => $_REQUEST["maxmails"]))));
		break;

	  case "default":
		$mailquota = "";
		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => $LANG->_('Standard')));
	}

	switch($type) {
	  case "add":
		// no add operation possible with mail
		break;
		
	  case "fulldomain":
		$users = pql_user_get($_pql->ldap_linkid, $_REQUEST["domain"]);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_QUOTA"),
												$_REQUEST["mailquota"]);

			if(in_array(false, $return)) 
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);
			
			attribute_forward($msg);
		}
		
	  case "modify";
		// remove attribute if it is set to ""

		if($_REQUEST["attrib"] == 'basequota') {
			$domaindn = urldecode($domain);

			// Setup the array to send to LDAP server
			$entry['baseQuota']     = $mailquota;
			$entry['objectClass']   = pql_setup_branch_objectclasses(0, $domaindn, $_pql->ldap_linkid);
			$entry['objectClass'][] = 'phpQLAdminConfig';

			// We're setting default quota for domain, not user.
			if(! pql_set_domain_value($_pql->ldap_linkid, $_REQUEST["domain"], 'baseQuota', $entry))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
										   array('what'   => $LANG->_('default quota'),
												 'domain' => urldecode($_REQUEST["domain"]))) . ": " . pql_ldap_error(0);
		} else {
			// We're setting quota for a user
			if(! pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_GLOB_ATTR_QUOTA"),
									   $mailquota))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);
		}
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
