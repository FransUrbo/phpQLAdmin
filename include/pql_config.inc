<?php
// Translation file from the old PQL_* defines to
// attributeType (objectclass: phpQLAdminConfig)
// values
//
// $Id: pql_config.inc,v 1.51 2004-01-26 05:19:02 turbo Exp $
//

// {{{ pql_set_define(define, value, case_insensitive, dn)
// We need this instead of define(), so that we can get rid
// of 'Constant ... already defined' if running with
//	error_reporting=E_ALL
// in PHP...
function pql_set_define($define, $value, $case_insensitive = false, $dn = NULL) {
	global $config;

	if($value == 'TRUE')
	  $value = '1';
	elseif($value == 'FALSE')
	  $value = '0';

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
		return($config[$define][$dn]);
	} else
	  return($config[$define]);
}
// }}}

// ----------------------------
// SINGLE VALUED ATTRIBUTES

// {{{ BOOLean values (true/false - 0/1)
$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"] = 'showUsers';
$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_USE"] = 'useControls';
$PQL_ATTRIBUTE["PQL_GLOB_EZMLM_USE"] = 'useEzmlm';
$PQL_ATTRIBUTE["PQL_GLOB_AUTO_RELOAD"] = 'autoReload';
$PQL_ATTRIBUTE["PQL_CONF_TESTMAIL_AUTOSEND"] = 'autoSend';
$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDLOCALS"] = 'autoAddLocals';
$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS"] = 'autoAddRCPTHosts';
$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDHOSTNAME"] = 'autoAddHostName';
$PQL_ATTRIBUTE["PQL_GLOB_CHANGE_SERVER"] = 'allowServerChange';
$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"] = 'allowAbsoluteMailPath';
$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"] = 'deleteVerification';
$PQL_ATTRIBUTE["PQL_CONF_MINIMUM_UIDNUMBER"] = 'minimumUIDNumber';
$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"] = 'forwardUIDNumber';
$PQL_ATTRIBUTE["PQL_GLOB_BIND9_USE"] = 'useBind9';
$PQL_ATTRIBUTE["PQL_GLOB_WEBSRV_USE"] = 'useWebSrv';
// }}}

// {{{ phpQLAdmin setup
$PQL_ATTRIBUTE["PQL_GLOB_WHOAREWE"] = 'whoAreWe';
$PQL_ATTRIBUTE["PQL_GLOB_LANG"] = 'language';
$PQL_ATTRIBUTE["PQL_GLOB_HOSTMASTER"] = 'hostMaster';
$PQL_ATTRIBUTE["PQL_CONF_TESTMAIL_SUBJECT"] = 'testMailSubject';
$PQL_ATTRIBUTE["PQL_CONF_TESTMAIL_MAILTEXT"] = 'testMailText';
$PQL_ATTRIBUTE["PQL_CONF_CATCHALLUSER"] = 'catchAllUser';
$PQL_ATTRIBUTE["PQL_GLOB_EZMLM_PATH"] = 'ezmlmBinaryPath';
$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"] = 'passWordScheme';
$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"] = 'externalScriptUser';
$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"] = 'externalScriptDomain';

$PQL_ATTRIBUTE["PQL_GLOB_KRB5_REALM"] = 'krb5RealmName';
$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_SERVER"] = 'krb5AdminServer';
$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_PRINCIPAL"] = 'krb5PrincipalName';
$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_KEYTAB"] = 'krb5AdminKeytab';
$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"] = 'krb5AdminCommandPath';
// }}}

// {{{ Save path for users and groups
$PQL_ATTRIBUTE["PQL_CONF_SUBTREE"]["USERS"] = 'subTreeUsers';
$PQL_ATTRIBUTE["PQL_CONF_SUBTREE"]["GROUPS"] = 'subTreeGroups';
// }}}

// {{{ The webserver administrator attributes
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_IP"] = 'webServerIP';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_URL"] = 'webServerURL';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ADMIN"] = 'webServerAdmin';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_DOCROOT"] = 'webDocumentRoot';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ALIAS"] = 'webServerAlias';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SCRIPT_URL"] = 'webScriptAliasURL';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SCRIPT_PATH"] = 'WebScriptAliasPath';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_LOG_ERROR"] = 'webLogError';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_LOG_TRANSFER"] = 'webLogTransfer';
$PQL_ATTRIBUTE["PQL_CONF_WEBSRV_OPTIONS"] = 'webOptions';
// }}} 

// {{{ References
$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"] = 'userReference';
$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"] = 'branchReference';
$PQL_ATTRIBUTE["PQL_CONF_USER_RDN_ATTRIBUTE"] = $PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"];
$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_DN"] = 'controlBaseDN'; 
$PQL_ATTRIBUTE["PQL_CONF_MAX_DOMAIN_USERS"] = 'maximumDomainUsers';
$PQL_ATTRIBUTE["PQL_CONF_MAX_LISTS"] = 'maximumMailingLists';
$PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"] = 'additionalDomainName';
// }}}

// ---------------------------
// - MULTI VALUED ATTRIBUTES -

// {{{ Objectclasses for domain/branches and users
$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"] = 'branchObjectClass';
$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"] = 'userObjectClass';
// }}}

// {{{ Attribute configuration
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["MAIL"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["FORWARDS"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["MAILALTERNATE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["QMAILUID"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["QMAILGID"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["MAILSTORE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["HOMEDIR"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["QUOTA"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["PROGRAM"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["MAILHOST"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["MODE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["REPLYTEXT"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["DOTMODE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["UID"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["PASSWD"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"][pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["ISACTIVE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["PURGE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"][pql_get_define("PQL_GLOB_ATTR_CN")] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["SN"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["DESCRIPTION"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["GIVENNAME"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_CONF_ATTR"]["DOMAIN"] = 'userAttribute';
// }}}

// ----------------------------
// 1. Load site specific configuration
require("./include/config.inc");				// Generic site configuration

// 2. Include the attribute configuration
require("./include/config_attributes.inc");		// Attribute definitions

// include the API files
require("./include/pql.inc");					// Main API file
require("./include/pql_formating.inc");			// Output formating API
require("./include/pql_cache.inc");				// Cache API
require("./include/pql_search.inc");			// Search API
require("./include/pql_units.inc");				// Sub branch API

// 3. Load configurations from LDAP base DN
if(!$USER_HOST) {
	if(! eregi('\+', PQL_GLOB_HOST)) {
		$host = split(';', PQL_GLOB_HOST);
		$USER_HOST = $host[0] . ";" . $host[1] . ";" . $host[2];
		
		session_register("USER_HOST");
	} else {
		// TODO: We have multiple hosts defined
		$servers = split('\+', PQL_GLOB_HOST);

		// For the time beeing, use the first value
		$host = split(';', $servers[0]);
		$USER_HOST = $host[0] . ";" . $host[1] . ";" . $host[2];
		
		session_register("USER_HOST");
	}
}

if(isset($USER_DN))
	 session_register("USER_DN");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
foreach($_pql->ldap_basedn as $dn)  {
	$dn = urldecode($dn);

	$sr = @ldap_read($_pql->ldap_linkid, $dn, ('objectClass=*'))
	  or pql_format_error(1);
	$info = ldap_get_entries($_pql->ldap_linkid, $sr)
	  or pql_format_error(1);

	// If we got something, retreive the attributes
	for ($i=0; $i<$info["count"]; $i++) {
		foreach($PQL_ATTRIBUTE as $key => $pql_config_private_variable_which_irritates_me) {
			if(is_array($pql_config_private_variable_which_irritates_me)) {
				foreach($pql_config_private_variable_which_irritates_me as $k => $v) {
					$v = strtolower($v);
					$nkey = $key."_".$k;
					
					if(isset($info[$i][$v][0])) {
						if($pql_config_private_array_which_irritates_me)
						  $pql_config_private_array_which_irritates_me .= ",".$info[$i][$v][0];
						else
						  $pql_config_private_array_which_irritates_me = $info[$i][$v][0];
					}
				}
			} else {
				$pql_config_private_variable_which_irritates_me = strtolower($pql_config_private_variable_which_irritates_me);
				$pql_config_private_array_which_irritates_me = NULL;

				for($k=0; isset($info[$i][$pql_config_private_variable_which_irritates_me][$k]); $k++) {
					if($pql_config_private_array_which_irritates_me)
					  $pql_config_private_array_which_irritates_me .= ",".$info[$i][$pql_config_private_variable_which_irritates_me][$k];
					else
					  $pql_config_private_array_which_irritates_me = $info[$i][$pql_config_private_variable_which_irritates_me][$k];
				}

				if(ereg("_GLOB_", $key)) 
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
	$ALLOW_BRANCH_CREATE = 0; $ALLOW_CONTROL_CREATE = 0;
	if($USER_DN && (!$ALLOW_BRANCH_CREATE or !$ALLOW_CONTROL_CREATE)) {
		// Verify that the user is administrator on the topmost DN -> Allow creation of domain/branches.
		$ALLOW_BRANCH_CREATE = pql_validate_administrator($_pql->ldap_linkid, $dn, $USER_DN);
		
		// Check the Control DN if the user is there -> Allow management of Controls information
		$ALLOW_CONTROL_CREATE = pql_validate_administrator($_pql->ldap_linkid, $USER_SEARCH_DN_CTR, $USER_DN);
	}
	
	session_register("ALLOW_BRANCH_CREATE", "ALLOW_CONTROL_CREATE");
}

// 5. Get language translation
require("./include/pql_lang.inc");
$LANG = new pql_lang($_pql);

// 6. Include the ACI API
require("./include/pql_aci.inc");

// 7. Include the plugin API
require("./include/pql_plugins.inc");

// 8. Verify that the user have access to write the global phpQLAdmin config
//    in the first base DN
$ALLOW_GLOBAL_CONFIG_SAVE = pql_validate_administrator($_pql->ldap_linkid, $_pql->ldap_basedn[0], $USER_DN);
session_register("ALLOW_GLOBAL_CONFIG_SAVE");

// 9. Check if ACI support is enabled in LDAP server
if(!$ACI_SUPPORT_ENABLED and function_exists("user_generate_aci")) {
	$ocs = pql_get_subschema($_pql->ldap_linkid, 'attributeTypes');
	if($ocs["openldapaci"]["OID"]) {
		$ACI_SUPPORT_ENABLED = 1;
		session_register("ACI_SUPPORT_ENABLED");
	}
}

// 10. Check to see if the cn=Monitor backend is availble/readable
if(!$MONITOR_BACKEND_ENABLED) {
    $sr = @ldap_read($_pql->ldap_linkid, "cn=Monitor", 'objectClass=*');
    if($sr) {
		$MONITOR_BACKEND_ENABLED = 1;
		session_register("MONITOR_BACKEND_ENABLED");
	}
}

// Remove some variables so we don't clutter the global
// variable name space!
unset($host); unset($servers); unset($i);
unset($pql_config_private_variable_which_irritates_me); unset($sr); unset($info);
unset($key); unset($v); unset($nkey);
unset($k);

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
