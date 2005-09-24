<?php
// Translation file from the old PQL_* defines to
// attributeType (objectclass: phpQLAdminConfig)
// values
//
// $Id: pql_config.inc,v 1.82 2005-09-24 09:45:58 turbo Exp $
//

// {{{ Include the API files
require($_SESSION["path"]."/include/pql.inc");					// Main API file
require($_SESSION["path"]."/include/pql_formating.inc");		// Output formating API
require($_SESSION["path"]."/include/pql_cache.inc");			// Cache API
require($_SESSION["path"]."/include/pql_search.inc");			// Search API
require($_SESSION["path"]."/include/pql_write.inc");			// Write API
require($_SESSION["path"]."/include/pql_units.inc");			// Sub branch API
// }}}

// {{{ Start profiling - what time is it now?
if(file_exists($_SESSION["path"]."/.DEBUG_ME") and file_exists($_SESSION["path"]."/.DEBUG_PROFILING")) {
  $now = pql_format_return_unixtime(); echo "Now: <b>$now</b><br>";
}
// }}}

// {{{ pql_set_define(define, value, case_insensitive, dn)
// We need this instead of define(), so that we can get rid
// of 'Constant ... already defined' if running with
//	error_reporting=E_ALL
// in PHP...
function pql_set_define($define, $value, $case_insensitive = false, $dn = NULL) {
	global $config;

	pql_format_bool($value);

	if(! defined($define)) {
	  if(is_array($value)) {
		$tmp = '';

		for($i=0; $i < count($value); $i++) {
		  $tmp .= $value[$i];
		  if(!empty($value[$i+1]))
			$tmp .= ',';
		}

		$value = $tmp;
	  }

	  define($define, $value, $case_insensitive);
	}

	if($dn) {
		// Create a global two dimensional array with the config values
		//	Dimension 1: define name
		//	Dimension 2: root DN

		$dn = urldecode($dn);

		if(empty($config[$define][$dn])) {
			$config[$define][$dn] = $value;

			if(file_exists("./.DEBUG_DEFINES"))
			  echo "config[$define][$dn] = $value<br>"; // DEBUG
		}
	} else {
		// Create a site global array with the config values
		// This variable type is only one dimensional (not DN specific)
		if(empty($config[$define])) {
		  $config[$define] = $value;
		  
		  if(file_exists("./.DEBUG_DEFINES"))
			echo "config[$define] = '$value'<br>"; // DEBUG
		}
	}
}
// }}}

// {{{ pql_get_define(define, dn)
function pql_get_define($define, $dn = NULL) {
	global $config;

	if($dn) {
		$dn = urldecode($dn);
		if(!empty($config[$define][$dn]))
		  $value = $config[$define][$dn];
		else
		  $value = 0;
	} elseif(!empty($config[$define])) {
	  if(!is_array($config[$define]))
		$value = $config[$define];
	} else
	  $value = NULL;
	  
	return($value);
}

// }}}

// ----------------------------
// 1. Load site specific configuration
require($_SESSION["path"]."/include/config.inc");

// 2. Include the attribute configuration
require($_SESSION["path"]."/include/config_attributes.inc");

// Get the LDAP host and port
if(empty($_SESSION["USER_HOST"])) {
  if(! eregi('\+', pql_get_define("PQL_CONF_HOST")))
	$host = split(';', pql_get_define("PQL_CONF_HOST"));
  else {
	$tmp  = split('\+', pql_get_define("PQL_CONF_HOST"));
	$host = split(';', $tmp[0]);

	unset($tmp);
  }

  $_SESSION["USER_HOST"] = $host[0] . ";" . $host[1] . ";" . $host[2];
  unset($host);
}

// Get the search base - controls database
if(empty($_SESSION["USER_SEARCH_DN_CTR"])) {
  $dn = split(';', $_SESSION["USER_HOST"]);
  $_SESSION["USER_SEARCH_DN_CTR"] = $dn[2];

  unset($dn);
}


if(empty($_pql)) {
  // Open a connection to the LDAP server
  $binddn = (!empty($_SESSION["USER_DN"]))   ? $_SESSION["USER_DN"]   : '';
  $bindpw = (!empty($_SESSION["USER_PASS"])) ? $_SESSION["USER_PASS"] : '';
  
  $_pql = new pql($_SESSION["USER_HOST"], $binddn, $bindpw);
}

// Initalize some session variable - either with the old OR with an empty value
$_SESSION["ALLOW_BRANCH_CREATE"]	= (!empty($_SESSION["ALLOW_BRANCH_CREATE"]))	? $_SESSION["ALLOW_BRANCH_CREATE"]	: '';
$_SESSION["ALLOW_CONTROL_CREATE"]	= (!empty($_SESSION["ALLOW_CONTROL_CREATE"]))	? $_SESSION["ALLOW_CONTROL_CREATE"]	: '';
$_SESSION["ALLOW_EZMLM_CREATE"]		= (!empty($_SESSION["ALLOW_EZMLM_CREATE"]))		? $_SESSION["ALLOW_EZMLM_CREATE"]	: '';
$_SESSION["ADVANCED_MODE"]			= (!empty($_SESSION["ADVANCED_MODE"]))			? $_SESSION["ADVANCED_MODE"]		: '';
$_SESSION["SINGLE_USER"]			= (!empty($_SESSION["SINGLE_USER"]))			? $_SESSION["SINGLE_USER"]			: '';

// Load configuration from the database...
foreach($_pql->ldap_basedn as $dn)  {
  $dn = urldecode($dn);
  unset($info);

  if(@(is_array($_SESSION["BASE_DN"]) and !in_array($dn, $_SESSION["BASE_DN"])) or !@$_SESSION["BASE_DN"])
	// Remember the base DN's
	$_SESSION["BASE_DN"][] = $dn;
  
  // 3. Load configurations from LDAP base DN
  $sr = @ldap_read($_pql->ldap_linkid, $dn, ('objectClass=*'))
	or pql_format_error(1);
  $info = ldap_get_entries($_pql->ldap_linkid, $sr)
	or pql_format_error(1);
  
  // Below I use two variables with VERY 'strange' names. I can't call them what I WANT to call
  // them or what's obvious because I'm afraid of variable overloading...
  // That is, if I use the variable name _ANY_ (!!) where else, and I need to remember that,
  // it will be 'destroyed' (or at least contain a value which have nothing to do with the
  // original value...
  //
  // Hence the name's "pql_config_private_{variable,array}_which_irritates_me".
  // - The "variable" variable will contain the LDAP attribute name.
  // - The "array" variable will contain the value(s) for this array from the db.
  
  // If we got something, retreive the attributes
  for($i=0; $i < $info["count"]; $i++) {
	foreach($PQL_ATTRIBUTE as $level => $array) {
	  if($level != 'USER') {
		foreach($array as $key => $pql_config_private_variable_which_irritates_me) {
		  $pql_config_private_variable_which_irritates_me = strtolower($pql_config_private_variable_which_irritates_me);
		  $pql_config_private_array_which_irritates_me = NULL;

		  if(!empty($info[$i][$pql_config_private_variable_which_irritates_me])) {
			for($k=0; $k < $info[$i][$pql_config_private_variable_which_irritates_me]["count"]; $k++) {
			  if($pql_config_private_array_which_irritates_me)
				$pql_config_private_array_which_irritates_me .= ",".$info[$i][$pql_config_private_variable_which_irritates_me][$k];
			  else
				$pql_config_private_array_which_irritates_me = $info[$i][$pql_config_private_variable_which_irritates_me][$k];
			}

			if($level == 'GLOBAL')
			  pql_set_define($key, $pql_config_private_array_which_irritates_me, true);
			else
			  pql_set_define($key, $pql_config_private_array_which_irritates_me, true, $dn);

			unset($pql_config_private_array_which_irritates_me);
		  }
		}
	  }
	}
  }
  
  // 4a. Verify if the user is administrator on the topmost DN (i.e. 'super admin') -> Allow creation of domain/branches.
  if(!empty($_SESSION["USER_DN"]) and empty($_SESSION["ALLOW_BRANCH_CREATE"])) {
	$_SESSION["ALLOW_BRANCH_CREATE"] = pql_validate_administrator($_pql->ldap_linkid, $dn,
																  pql_get_define("PQL_ATTR_ADMINISTRATOR"),
																  $_SESSION["USER_DN"]);
	
	// User is super administrator - allow for administrating QmailLDAP/Control objects and ezmlm lists as well.
	if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	  $_SESSION["ALLOW_CONTROL_CREATE"] = 1;
	  $_SESSION["ALLOW_EZMLM_CREATE"]   = 1;
	}
  }
  
  // 4b. Verify if the user is controlsAdministrator in the topmost DN -> Allow administration of QmailLDAP/Control objects.
  if(!empty($_SESSION["USER_DN"]) and empty($_SESSION["ALLOW_CONTROL_CREATE"]))
	$_SESSION["ALLOW_CONTROL_CREATE"] = pql_validate_administrator($_pql->ldap_linkid, $dn,
																   pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
																   $_SESSION["USER_DN"]);
  
  // 4c. Verify if the user is ezmlmAdministrator in the topmost DN -> Allow administration of Ezmlm lists.
  if(!empty($_SESSION["USER_DN"]) and empty($_SESSION["ALLOW_EZMLM_CREATE"]))
	$_SESSION["ALLOW_EZMLM_CREATE"] = pql_get_dn($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM").'='.$_SESSION["USER_DN"]);
  
  if(!pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $dn)) {
	$ref = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_REFERENCE_USERS_WITH"));
	pql_set_define("PQL_CONF_REFERENCE_USERS_WITH", $ref, false, $dn);
  }
  
  // 4d. Load 'resonable defaults' for each top DN
  require($_SESSION["path"]."/include/defaults.inc");
}

// 5. Check the Control DN if the user is there -> Allow management of Controls information
if(!empty($_SESSION["USER_DN"]) and empty($_SESSION["ALLOW_CONTROL_CREATE"])) {
  $_SESSION["ALLOW_CONTROL_CREATE"] = pql_validate_administrator($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
																 pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
																 $_SESSION["USER_DN"]);
  
  // 6. Get user configuration for phpQLAdmin
  $user_object = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_DN"], "(objectClass=*)", 'BASE');
  foreach($PQL_ATTRIBUTE["USER"] as $key => $attrib)
	pql_set_define($key, $user_object[$attrib][0], true, $_SESSION["USER_DN"]);
}

// 7. Get language translation
require($_SESSION["path"]."/include/pql_lang.inc");
$LANG = new pql_lang($_pql);

// 8. Include the plugin API
require($_SESSION["path"]."/include/pql_plugins.inc");

if(!empty($_SESSION["USER_DN"])) {
  // 9. Verify that the user have access to write the global phpQLAdmin config
  //    in the first base DN
  $_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"] = pql_validate_administrator($_pql->ldap_linkid, $_pql->ldap_basedn[0],
																	 pql_get_define("PQL_ATTR_ADMINISTRATOR"),
																	 $_SESSION["USER_DN"]);
}

if((empty($_SESSION["ACI_SUPPORT_ENABLED"]) and function_exists("user_generate_aci")) or empty($_SESSION["NEW_STYLE_QUOTA"])) {
  $ocs = pql_get_subschema($_pql->ldap_linkid, 'attributeTypes');
  
  // 9a. Check ifthe attribute OpenLDAPaci exists - ACI support is enabled in the LDAP server.
  //     OR: We've specified (manually) to (en)force the use of ACI's via the baseDN attribute
  //         'useACI'.
  //     -> Just make sure that useACI isn't equal to 'FALSE' which means the user/admin have
  //        elected to NOT use ACI's!
  if(empty($_SESSION["ACI_SUPPORT_ENABLED"]) and function_exists("user_generate_aci") and
	 (!empty($ocs[pql_get_define("PQL_ATTR_LDAPACI")]["OID"]) or pql_get_define("PQL_CONF_ACI_USE")) and
	 (pql_get_define("PQL_CONF_ACI_USE") != 'FALSE'))
	$_SESSION["ACI_SUPPORT_ENABLED"] = 1;
  
  // 9b. Check to see if the attributes mailQuotaSize and mailQuotaCount exists - new style quota.
  if(!empty($ocs[pql_get_define("PQL_ATTR_QUOTA_SIZE")]["OID"]) and
	 !empty($ocs[pql_get_define("PQL_ATTR_QUOTA_COUNT")]["OID"]))
	$_SESSION["NEW_STYLE_QUOTA"] = 1;
  
  // 9c. Do we support old style quota, even if new style is enabled?
  if(empty($ocs[pql_get_define("PQL_ATTR_QUOTA_VALUE")]["OID"])) {
	// The old style quota attribute isn't defined - don't support it
	$_SESSION["OLD_STYLE_QUOTA"] = 0;
  } else {
	// It's defined so support old style quota
	$_SESSION["OLD_STYLE_QUOTA"] = 1;
  }
}

// 10. Check to see if the cn=Monitor backend is availble/readable
if(!@$MONITOR_BACKEND_ENABLED) {
  $sr = @ldap_read($_pql->ldap_linkid, "cn=Monitor", 'objectClass=*');
  if($sr)
	$_SESSION["MONITOR_BACKEND_ENABLED"] = 1;
}

if(@$_SESSION["ACI_SUPPORT_ENABLED"]) {
  require($_SESSION["path"]."/include/pql_aci.inc");
}

// Remove some variables so we don't clutter the global variable name space!
unset($host); unset($servers); unset($i);
unset($pql_config_private_variable_which_irritates_me); unset($sr); unset($info);
unset($v); unset($nkey); unset($k); unset($ref);

// See bug #203...
@(($_REQUEST["domain"] = $_GET["domain"]) || ($_REQUEST["domain"] = $_POST["domain"]));

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
