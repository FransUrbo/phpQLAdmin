<?php
// Translation file from the old PQL_* defines to
// attributeType (objectclass: phpQLAdminConfig)
// values
//
// $Id: pql_config.inc,v 1.61.10.2.4.1.2.1 2004-09-17 14:43:32 turbo Exp $
//

// include the API files
require("./include/pql.inc");				// Main API file
require("./include/pql_formating.inc");			// Output formating API
require("./include/pql_cache.inc");			// Cache API
require("./include/pql_search.inc");			// Search API
require("./include/pql_units.inc");			// Sub branch API

// {{{ pql_set_define(define, value, case_insensitive, dn)
// We need this instead of define(), so that we can get rid
// of 'Constant ... already defined' if running with
//	error_reporting=E_ALL
// in PHP...
function pql_set_define($define, $value, $case_insensitive = false, $dn = NULL) {
	global $config;

	pql_format_bool($value);

	if(! defined($define))
	  define($define, $value, $case_insensitive);

	if($dn) {
		// Create a global two dimensional array with the config values
		//	Dimension 1: define name
		//	Dimension 2: root DN

		$dn = urldecode($dn);

		if(!isset($config[$define][$dn])) {
			$config[$define][$dn] = $value;
//			echo "config[$define][$dn] = $value<br>"; // DEBUG
		}
	} else {
		// Create a site global array with the config values
		// This variable type is only one dimensional (not DN specific)
		if(!isset($config[$define])) {
			$config[$define] = $value;
//			echo "config[$define] = '$value'<br>"; // DEBUG
		}
	}
}
// }}}

// {{{ pql_get_define(define, dn)
function pql_get_define($define, $dn = NULL) {
	global $config;

	if($dn) {
		$dn = urldecode($dn);
		$value = $config[$define][$dn];
	} elseif(!is_array($config[$define]))
	  $value = $config[$define];
	else
	  $value = NULL;

	// Just incase it's a BOOLEAN...
	$value = pql_format_bool($value);

	return($value);
}
// }}}

// ----------------------------
// 1. Load site specific configuration
require("./include/config.inc");			// Generic site configuration

// 2. Include the attribute configuration
require("./include/config_attributes.inc");		// Attribute definitions

// 3. Load configurations from LDAP base DN
if(!$_SESSION["USER_HOST"]) {
	if(! eregi('\+', pql_get_define("PQL_CONF_HOST"))) {
		$host = split(';', pql_get_define("PQL_CONF_HOST"));
		$_SESSION["USER_HOST"] = $host[0] . ";" . $host[1] . ";" . $host[2];
	} else {
		// TODO: We have multiple hosts defined
		$servers = split('\+', pql_get_define("PQL_CONF_HOST"));

		// For the time beeing, use the first value
		$host = split(';', $servers[0]);
		$_SESSION["USER_HOST"] = $host[0] . ";" . $host[1] . ";" . $host[2];
	}
}

// Open a connection to the LDAP server
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// Remember the base DN's
$_SESSION["BASE_DN"] = $_pql->ldap_basedn;

// Get base configurations
foreach($_pql->ldap_basedn as $dn)  {
	$dn = urldecode($dn);

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
	for ($i=0; $i<$info["count"]; $i++) {
		foreach($PQL_ATTRIBUTE as $level => $array) {
			foreach($array as $key => $pql_config_private_variable_which_irritates_me) {
				$pql_config_private_variable_which_irritates_me = strtolower($pql_config_private_variable_which_irritates_me);
				$pql_config_private_array_which_irritates_me = NULL;
				
				for($k=0; isset($info[$i][$pql_config_private_variable_which_irritates_me][$k]); $k++) {
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

	// 4. Load 'resonable defaults' for each top DN
	require("./include/defaults.inc");

	// Check some builtin ACL's
	$_SESSION["ALLOW_BRANCH_CREATE"] = 0; $_SESSION["ALLOW_CONTROL_CREATE"] = 0;
	if($_SESSION["USER_DN"] && (!$_SESSION["ALLOW_BRANCH_CREATE"] or !$_SESSION["ALLOW_CONTROL_CREATE"])) {
		// Verify that the user is administrator on the topmost DN -> Allow creation of domain/branches.
		$_SESSION["ALLOW_BRANCH_CREATE"] = pql_validate_administrator($_pql->ldap_linkid, $dn, $_SESSION["USER_DN"]);
		
		// Check the Control DN if the user is there -> Allow management of Controls information
		$_SESSION["ALLOW_CONTROL_CREATE"] = pql_validate_administrator($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $_SESSION["USER_DN"]);
	}
}

// 5. Get language translation
require("./include/pql_lang.inc");
$LANG = new pql_lang($_pql);

// 6. Include the plugin API
require("./include/pql_plugins.inc");

// 7. Verify that the user have access to write the global phpQLAdmin config
//    in the first base DN
$_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"] = pql_validate_administrator($_pql->ldap_linkid, $_pql->ldap_basedn[0], $_SESSION["USER_DN"]);

if((empty($_SESSION["ACI_SUPPORT_ENABLED"]) and function_exists("user_generate_aci")) or empty($_SESSION["NEW_STYLE_QUOTA"])) {
	$ocs = pql_get_subschema($_pql->ldap_linkid, 'attributeTypes');

	// 9a. Check if the attribute openLdapAci exists - ACI support is enabled in the LDAP server
	//     OR: We've specified (manually) to (en)force the use of ACI's via the baseDN attribute
	//         'useACI'.
	//     -> Just make sure that useACI isn't equal to 'FALSE' which means the user/admin have
	//        elected to NOT use ACI's!
	if(empty($_SESSION["ACI_SUPPORT_ENABLED"]) and function_exists("user_generate_aci") and
	   ($ocs[pql_get_define("PQL_ATTR_LDAPACI")]["OID"] or pql_get_define("PQL_CONF_ACI_USE")) and
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

if($_SESSION["ACI_SUPPORT_ENABLED"])
	 require("./include/pql_aci.inc");

// 10. Check to see if the cn=Monitor backend is availble/readable
if(!$MONITOR_BACKEND_ENABLED) {
    $sr = @ldap_read($_pql->ldap_linkid, "cn=Monitor", 'objectClass=*');
    if($sr)
	  $_SESSION["MONITOR_BACKEND_ENABLED"] = 1;
}

// Remove some variables so we don't clutter the global
// variable name space!
unset($host); unset($servers); unset($i);
unset($pql_config_private_variable_which_irritates_me); unset($sr); unset($info);
unset($v); unset($nkey); unset($k);

// See bug #203...
($_REQUEST["domain"] = $_GET["domain"] ) || ($_REQUEST["domain"] = $_POST["domain"]);

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
