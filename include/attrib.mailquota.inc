<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mailquota.inc,v 2.6 2003-04-07 15:26:47 turbo Exp $

function attribute_check($type){
	global $options, $use_maxsize, $use_maxmails, $maxsize, $maxmails, $error;
	
	// the checks only make sense if we're doing user defined
	// mailquotas, not if we're setting this user to the standard
	// in controls/ldapdefaultquota
	
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
	global $use_maxmails, $use_maxsize, $maxmails, $maxsize, $domain;
	global $rootdn, $options, $user, $PHP_SELF, $attrib, $error;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_MAILQUOTA_CHANGE; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_OPTIONS; ?></td>
          <td><input type="radio" name="options" value="default" <?php if($options == "default"){ echo "checked"; } //" ?>> <?php echo PQL_LANG_MAILQUOTA_DEFAULT; ?> <input type="radio" name="options" value="user" <?php if($options == "user"){ echo "checked"; } //" ?>> <?php echo PQL_LANG_MAILQUOTA_USERDEFINED; ?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_MAILQUOTA_MAXSIZE_TITLE; ?></td>
          <td><input type="checkbox" name="use_maxsize" value="1" <?php if($use_maxsize == "1"){ echo "checked"; } //" ?>> <?php echo format_error($error["maxsize"]); ?><input type="text" name="maxsize" value="<?php echo $maxsize; ?>"> KB</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_MAILQUOTA_MAXMAILS_TITLE; ?></td>
          <td><input type="checkbox" name="use_maxmails" value="1" <?php if($use_maxmails == "1"){ echo "checked"; } //" ?>> <?php echo format_error($error["maxmails"]); ?><input type="text" name="maxmails" value="<?php echo $maxmails; ?>"> Mails</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo format_error($error["selected"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">
            &nbsp;<?php echo PQL_LANG_MAILQUOTA_HELP; ?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
<?php if($attrib == 'mailquota') { ?>
    <input type="hidden" name="user" value="<?=$user?>">
<?php } ?>

    <br><br>

    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

<?php
}

function attribute_save($type) {
	global $_pql, $attrib, $domain, $user, $maxsize, $maxmails;
	global $options, $use_maxmails, $use_maxsize, $config;
	
	// recontrol values
	if(!attribute_check($type))
	  return false;
	
	switch($options){
	  case "user":
		if($use_maxsize != "") {
			$maxsize = $maxsize * 1024;
			$mailquota[] = $maxsize . "S";
		} else
		  $maxsize = PQL_LANG_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED;
		
		if($use_maxmails != "")
		  $mailquota[] = $maxmails . "C";
		else
		  $maxmails = PQL_LANG_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED;
		
		if(is_array($mailquota))
		  $mailquota = join(",", $mailquota);
		
		$msg = pql_complete_constant(PQL_LANG_MAILQUOTA_CHANGE_OK,
									 array("quota" => pql_ldap_mailquota(array("maxsize" => $maxsize / 1024,
																			   "maxmails" => $maxmails))));
		break;

	  case "default":
		$mailquota = "";
		$msg = pql_complete_constant(PQL_LANG_MAILQUOTA_CHANGE_OK, array("quota" => PQL_LANG_MAILQUOTA_DEFAULT));
	}

	switch($type) {
	  case "add":
		// no add operation possible with mail
		break;
		
	  case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_QUOTA"], $mailquota);

			if(in_array(false, $return))
			  $msg = PQL_LANG_MAILQUOTA_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
			
			attribute_forward($msg);
		}
		
	  case "modify";
		// remove attribute if it is set to ""

		if($attrib == 'basequota') {
			// We're setting default quota for domain, not user.
			if(! pql_set_domain_value($_pql->ldap_linkid, $domain, 'baseQuota', array($mailquota)))
			  $msg = PQL_LANG_MAILQUOTA_DEFAULT_CHANGE . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
		} else {
			// We're setting quota for a user
			if(! pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_QUOTA"], $mailquota))
			  $msg = PQL_LANG_MAILQUOTA_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
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
