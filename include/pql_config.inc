<?php
// Translation file from the old PQL_* defines to
// attributeType (objectclass: phpQLAdminConfig)
// values
//
// $Id: pql_config.inc,v 1.4 2003-01-22 10:25:43 turbo Exp $
//

// ----------------------------
// - SINGLE VALUED ATTRIBUTES -

// BOOLean values (true/false - 0/1)
$PQL_ATTRIBUTE["PQL_SHOW_USERS"] = 'showUsers';
$PQL_ATTRIBUTE["PQL_LDAP_CONTROL_USE"] = 'useControls';
$PQL_ATTRIBUTE["PQL_LDAP_EZMLM_USE"] = 'useEzmlm';
$PQL_ATTRIBUTE["PQL_AUTO_RELOAD"] = 'autoReload';
$PQL_ATTRIBUTE["PQL_TESTMAIL_AUTOSEND"] = 'autoSend';
$PQL_ATTRIBUTE["PQL_LDAP_CONTROL_AUTOADDLOCALS"] = 'autoAddLocals';
$PQL_ATTRIBUTE["PQL_LDAP_CHANGE_SERVER"] = 'allowServerChange';
$PQL_ATTRIBUTE["PQL_ALLOW_ABSOLUTE_PATH"] = 'allowAbsoluteMailPath';
$PQL_ATTRIBUTE["PQL_VERIFY_DELETE"] = 'deleteVerification';

// phpQLAdmin setup
$PQL_ATTRIBUTE["PQL_LANG"] = 'language';
$PQL_ATTRIBUTE["PQL_WHOAREWE"] = 'whoAreWe';
$PQL_ATTRIBUTE["PQL_HOSTMASTER"] = 'hostMaster';
$PQL_ATTRIBUTE["PQL_TESTMAIL_SUBJECT"] = 'testMailSubject';
$PQL_ATTRIBUTE["PQL_TESTMAIL_MAILTEXT"] = 'testMailText';
$PQL_ATTRIBUTE["PQL_CATCHALLUSER"] = 'catchAllUser';
$PQL_ATTRIBUTE["PQL_LDAP_EZMLM_PATH"] = 'ezmlmBinaryPath';
$PQL_ATTRIBUTE["PQL_PASSWORD_SCHEMES"] = 'passWordScheme';

// Save path for users and groups
$PQL_ATTRIBUTE["PQL_LDAP_SUBTREE"]["USERS"] = 'subTreeUsers';
$PQL_ATTRIBUTE["PQL_LDAP_SUBTREE"]["GROUPS"] = 'subTreeGroups';

// References
$PQL_ATTRIBUTE["PQL_LDAP_REFERENCE_USERS_WITH"] = 'userReference';
$PQL_ATTRIBUTE["PQL_LDAP_REFERENCE_DOMAINS_WITH"] = 'branchReference';
$PQL_ATTRIBUTE["PQL_LDAP_USER_RDN_ATTRIBUTE"] = $PQL_ATTRIBUTE["PQL_LDAP_REFERENCE_USERS_WITH"];

// ---------------------------
// - MULTI VALUED ATTRIBUTES -

// Objectclasses for domain/branches and users
$PQL_ATTRIBUTE["PQL_LDAP_OBJECTCLASS_DOMAIN"] = 'branchObjectClass';
$PQL_ATTRIBUTE["PQL_LDAP_OBJECTCLASS_USERID"] = 'userObjectClass';
$PQL_ATTRIBUTE["PQL_LDAP_OBJECTCLASS_USER_EXTRA"] = 'userObjectClass';

// Attribute configuration
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["MAIL"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["FORWARDS"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["MAILALTERNATE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["QMAILUID"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["QMAILGID"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["MAILSTORE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["HOMEDIR"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["QUOTA"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["PROGRAM"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["MAILHOST"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["MODE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["REPLYTEXT"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["DOTMODE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["UID"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["PASSWD"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["OBJECTCLASS"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["ISACTIVE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["PURGE"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["CN"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["SN"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["DESCRIPTION"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["GIVENNAME"] = 'userAttribute';
$PQL_ATTRIBUTE["PQL_LDAP_ATTR"]["DOMAIN"] = 'userAttribute';

// ----------------------------
// 1. Load site specific configuration
require("./include/config.inc");

// 2. Load configurations from LDAP base DN
require("./include/pql.inc");
if(!$USER_HOST) {
	if(! eregi(' ', PQL_LDAP_HOST)) {
		$host = split(';', PQL_LDAP_HOST);
		$USER_HOST = $host[0] . ";" . $host[1];
		
		session_register("USER_HOST");
	} else {
		// TODO: We have multiple hosts defined
	}
}

if($USER_DN) session_register("USER_DN");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
$sr   = @ldap_read($_pql->ldap_linkid, $_pql->ldap_basedn[0], $_pql->ldap_basedn[0])
	 or pql_errormsg($_pql->ldap_linkid);
$info = ldap_get_entries($_pql->ldap_linkid, $sr)
	 or pql_errormsg($_pql->ldap_linkid);

// If we got something, retreive the attributes
for ($i=0; $i<$info["count"]; $i++) {
	foreach($PQL_ATTRIBUTE as $key => $value) {
		if(is_array($value)) {
			foreach($value as $k => $v) {
				$v = strtolower($v);

				$nkey = $key."_".$k;

				if($info[$i][$v][0] != '') {
					if($info[$i][$v][0]=='0')
					  $V = 'false';
					elseif($info[$i][$v][0]=='1')
					  $V = 'true';
					else
					  $V = $info[$i][$v][0];

					//echo $nkey."($v): <b>$V</b><br>";
					define($nkey, $info[$i][$v][0], true);
				}
			}
		} else {
			$value = strtolower($value);
			if($info[$i][$value][0] != '') {
				if($info[$i][$value][0]=='0')
				  $V = 'false';
				elseif($info[$i][$value][0]=='1')
				  $V = 'true';
				else
				  $V = $info[$i][$value][0];

				//echo "'".$key."' ($value): <b>$V</b><br>";
				define($key, $info[$i][$value][0], true);
			}
		}
	}
	//echo "<br>";
}

// 3. Load 'resonable defaults'
require("./include/defaults.inc");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
