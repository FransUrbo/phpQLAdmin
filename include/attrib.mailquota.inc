<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mailquota.inc,v 2.21 2004-03-01 10:09:08 turbo Exp $

function attribute_check($type) {
    global $error, $LANG;

	$failed = false;

	// Just to save some typing.
	$options = (!empty($_REQUEST["options"]) ? $_REQUEST["options"] : "");
	$maxsize = (!empty($_REQUEST["maxsize"]) ? $_REQUEST["maxsize"] : "");
	$maxmails = (!empty($_REQUEST["maxmails"]) ? $_REQUEST["maxmails"] : "");
	
	// the checks only make sense if we're doing user defined
	// mailquotas, not if we're setting this user to the standard
	// in controls/ldapdefaultquota
	
	// empty
	if($maxsize == "" and $options == "user") {
		$error["maxsize"] = $LANG->_('Missing');
		$failed = true;
	}
	
	// valid (numeric)
	if($maxsize != "" and preg_match("/[^0-9]/", $maxsize) and $options == "user") {
		$error["maxsize"] = $LANG->_('Invalid');
		$failed = true;
	}
	
	// bigger than 0
	if($error["maxsize"] == "" and $maxsize <= 0 and $options == "user") {
		$error["maxsize"] = $LANG->_('Must be above zero');
		$failed = true;
	}
	
	// empty
	if($maxmails == ""  and $options == "user") {
		$error["maxmails"] = $LANG->_('Missing');
		$failed = true;
	}
	
	// valid (numeric)
	if($maxmails != "" and preg_match("/[^0-9]/", $maxmails)  and $options == "user") {
		$error["maxmails"] = $LANG->_('Invalid');
		$failed = true;
	}
	
	// bigger than 0
	if($error["maxmails"] == "" and $maxmails <= 0 and $options == "user"){
		$error["maxmails"] = $LANG->_('Must be above zero');
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
		$_REQUEST["maxsize"]  = $quota["maxsize"];
		$_REQUEST["options"]  = "user";
	} else
	  $_REQUEST["options"]    = "default";
}

function attribute_print_form($type = "modify") {
    global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change quota')?></th>
<?php if(!empty($_SESSION["USE_CONTROLS"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td>
            <input type="radio" name="options" value="default" <?php if($_REQUEST["options"] == "default") { echo "checked"; } ?>>
            <?=$LANG->_('Standard')?>
            <input type="radio" name="options" value="user" <?php if($_REQUEST["options"] == "user") { echo "checked"; } ?>>
            <?=$LANG->_('User defined')?>
          </td>
        </tr>
<?php } else { ?>
        <input type="hidden" name="options" value="user">
<?php } ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum size of mailbox')?></td>
          <td><?php echo pql_format_error_span($error["maxsize"]); ?><input type="text" name="maxsize" value="<?php echo $_REQUEST["maxsize"]; ?>"> KB</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum number of mails')?></td>
          <td><?php echo pql_format_error_span($error["maxmails"]); ?><input type="text" name="maxmails" value="<?php echo $_REQUEST["maxmails"]; ?>"> <?=$LANG->_('Mails')?></td>
        </tr>

<?php if(!empty($_SESSION["USE_CONTROLS"])) { ?>
        <tr class="subtitle">
          <table>
            <tr>
              <td>
                <?php echo pql_format_error_span($error["selected"]); ?>
                <img src="images/info.png" width="16" height="16" alt="" border="0">
              </td>
              <td>
                &nbsp;<?=$LANG->_('With \'Standard\', QmailLDAP will get quotas from its configuration file (~controls/ldapdefaultquota) or corresponding value in QmailLDAP/Controls. See "q-l mailbox" below your QmailLDAP/Controls object(s).')?>
              </td>
            </tr>
          </table>
        </tr>
<?php } ?>
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

function attribute_save($type) {
    global $_pql, $LANG;

	// Just incase user 'forgets' to tick in the 'User defined' field...
	if(empty($_REQUEST["options"]) and (!empty($_REQUEST["maxsize"]) or !empty($_REQUEST["maxmails"]))) {
		// Neither 'Standard' nor 'User defined' entered, but we have value(s) for quota
		$_REQUEST["options"] = "user";
	}

	switch($_REQUEST["options"]) {
	  case "user":
		if($_REQUEST["maxsize"] != "") {
			$_REQUEST["maxsize"] = $_REQUEST["maxsize"] * 1024;
			$mailquota[] = $_REQUEST["maxsize"] . "S";
		} else
		  $_REQUEST["maxsize"] = $LANG->_('Unlimited size');
		
		if($_REQUEST["maxmails"] != "")
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
		if(!empty($_SESSION["USE_CONTROLS"])) {
			unset($mailquota); // need to be unset initialy in check later

			// Instead of doing this again, we include the file where this is done.
			global $quota, $_pql_control;
			include("./include/pql_control.inc");
			include("./include/attrib.control.ldapdefaultquota.inc");

			// Initialize a QmailLDAP/Controls session
			$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

			// Retreive all QmailLDAP/Controls object in the database...
			$hosts = pql_control_get_hosts($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"]);
			if(is_array($hosts)) {
				foreach($hosts as $host) {
					if(empty($mailquota)) {
						// Retreive default quota for this QmailLDAP/Controls object.
						$quota = ldapdefaultquota_init($host);
						if(is_array($quota)) {
							$mailquota = $quota["maxsize"]."S,".$quota["maxmails"]."C";
						}
					}
				}
			}
		} else
		  $mailquota = "";

		$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									 array('what' => $LANG->_('quota'),
										   'new'  => $LANG->_('Standard')." ($mailquota)"));
	}

	switch($type) {
	  case "add":
		// no add operation possible with mail
		break;
		
	  case "fulldomain":
		$users = pql_user_get($_pql->ldap_linkid, $_REQUEST["domain"]);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_QUOTA_VALUE"),
												$_REQUEST["mailquota"]);

			if(in_array(false, $return)) 
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);
			
			attribute_forward($msg);
		}
		
	  case "modify";
		// remove attribute if it is set to ""

		if($_REQUEST["attrib"] == 'basequota') {
			$domaindn = urldecode($_REQUEST["domain"]);

			// Setup the array to send to LDAP server
			$entry['baseQuota']     = $mailquota;
			$entry['objectClass']   = pql_setup_branch_objectclasses(0, $domaindn, $_pql->ldap_linkid);
			$entry['objectClass'][] = 'phpQLAdminConfig';

			// We're setting default quota for domain, not user.
			if(! pql_set_domain_value($_pql->ldap_linkid, $_REQUEST["domain"], 'baseQuota', $entry))
			  // Failed
			  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
										   array('what'   => $LANG->_('default quota'),
												 'domain' => $domaindn)) . ": " . pql_ldap_error(0);
			// The success msg is set above (in the switch($type) code).
		} else {
			// We're setting quota for a user
			if($_SESSION["NEW_STYLE_QUOTA"]) {
				if($_REQUEST["maxsize"] != "")
				  if(! pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"],
											 pql_get_define("PQL_GLOB_ATTR_QUOTA_SIZE"),
											 $_REQUEST["maxsize"]))
					$msg  = pql_complete_constant($LANG->_('Failed to change %what%'),
												 array('what' => $LANG->_('quota size'))) . ": " . pql_format_error(0);
				
				if($_REQUEST["maxmails"] != "")
				  if(! pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"],
											 pql_get_define("PQL_GLOB_ATTR_QUOTA_COUNT"),
											 $_REQUEST["maxmails"]))
					$msg .= "<br>".pql_complete_constant($LANG->_('Failed to change %what%'),
														 array('what' => $LANG->_('quota count'))) . ": " . pql_format_error(0);
			} else {
				if(! pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"],
										   pql_get_define("PQL_GLOB_ATTR_QUOTA_VALUE"),
										   $mailquota))
				  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
											   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);
			}
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
