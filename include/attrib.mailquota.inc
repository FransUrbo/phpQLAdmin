<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mailquota.inc,v 2.15 2004-01-01 12:35:34 turbo Exp $

function attribute_check($type) {
	global $options, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $error, $LANG;
	
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
	global $_pql, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $domain;
	global $rootdn, $user, $options;

	$quota = pql_get_userquota($_pql->ldap_linkid, $user);
	if(is_array($quota)) {
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

function attribute_print_form($type = "modify") {
	global $use_maxmails, $use_maxsize, $maxmails, $maxsize, $domain, $view, $LANG;
	global $rootdn, $options, $user, $PHP_SELF, $attrib, $error;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change quota')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="default" <?php if($options == "default"){ echo "checked"; } //" ?>><?=$LANG->_('Standard')?><input type="radio" name="options" value="user" <?php if($options == "user"){ echo "checked"; } //" ?>><?=$LANG->_('User defined')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum size of mailbox')?></td>
          <td><input type="checkbox" name="use_maxsize" value="1" <?php if($use_maxsize == "1"){ echo "checked"; } //" ?>> <?php echo pql_format_error_span($error["maxsize"]); ?><input type="text" name="maxsize" value="<?php echo $maxsize; ?>"> KB</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum number of mails')?></td>
          <td><input type="checkbox" name="use_maxmails" value="1" <?php if($use_maxmails == "1"){ echo "checked"; } //" ?>> <?php echo pql_format_error_span($error["maxmails"]); ?><input type="text" name="maxmails" value="<?php echo $maxmails; ?>"> <?=$LANG->_('Mails')?></td>
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
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
<?php if($attrib == 'mailquota') { ?>
    <input type="hidden" name="user" value="<?=$user?>">
<?php } ?>
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// '
function attribute_save($type) {
	global $_pql, $attrib, $domain, $user, $maxsize, $maxmails, $options, $use_maxmails, $use_maxsize, $view, $LANG;
	
	// recontrol values
	if(!attribute_check($type))
	  return false;
	
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
	  case "add":
		// no add operation possible with mail
		break;
		
	  case "fulldomain":
		$users = pql_user_get($_pql->ldap_linkid, $domain);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_attribute($_pql->ldap_linkid, $user,
													pql_get_define("PQL_GLOB_ATTR_QUOTA"),
													$mailquota);

			if(in_array(false, $return)) 
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('quota'))) . ": " . pql_format_error(0);
			
			attribute_forward($msg);
		}
		
	  case "modify";
		// remove attribute if it is set to ""

		if($attrib == 'basequota') {
			// We're setting default quota for domain, not user.
			if(! pql_set_domain_value($_pql->ldap_linkid, $domain, 'baseQuota', array($mailquota)))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
										   array('what'   => $LANG->_('default quota'),
												 'domain' => urldecode($domain))) . ": " . pql_format_error(0);
		} else {
			// We're setting quota for a user
			if(! pql_replace_attribute($_pql->ldap_linkid, $user,
										   pql_get_define("PQL_GLOB_ATTR_QUOTA"),
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
