<?php
// These are the attributes that is in use in phpQLAdmin
// that have to do with users, branches and QmailLDAP/Controls
// objects etc.
//
// You're not really supposed to change this file, unless your know
// exactly what you're doing! The intention with this file is to
// avoid hardcoding the attribute names in the code, but instead
// set variables that is used in the code instead.
//
// It should (not tested!) be possible to modify this file (especially
// if you're not using QmailLDAP, but maybe SendmailLDAP etc) so that
// phpQLAdmin would work for the LDAP setup of your choice.
//
// * You should provide the same names here as in your qmail-ldap.h
// * Only (!!) change the value in the SECOND column (in the strtolower()
//   function).
//
// $Id: config_attributes.inc,v 2.22 2004-10-13 17:47:29 turbo Exp $

// {{{ These are the QmailLDAP user object attributes

pql_set_define("PQL_ATTR_DOTMODE",							strtolower("qmailDotMode"),					false);
pql_set_define("PQL_ATTR_FORWARDS",							strtolower("mailForwardingAddress"),		false);
pql_set_define("PQL_ATTR_GECOS",							strtolower("gecos"),						false);
pql_set_define("PQL_ATTR_HOMEDIR",							strtolower("homeDirectory"),				false);
pql_set_define("PQL_ATTR_ISACTIVE",							strtolower("accountStatus"),				false);
pql_set_define("PQL_ATTR_EXPIRATION",						strtolower("shadowExpire"),					false);
pql_set_define("PQL_ATTR_LOGINSHELL",						strtolower("loginShell"),					false);
pql_set_define("PQL_ATTR_MAIL",								strtolower("mail"),							false);
pql_set_define("PQL_ATTR_MAILALTERNATE",					strtolower("mailAlternateAddress"),			false);
pql_set_define("PQL_ATTR_MAILHOST",							strtolower("mailHost"),						false);
pql_set_define("PQL_ATTR_MAILSTORE",						strtolower("mailMessageStore"),				false);
pql_set_define("PQL_ATTR_MODE",								strtolower("deliveryMode"),					false);
pql_set_define("PQL_ATTR_PASSWD",							strtolower("userPassword"),					false);
pql_set_define("PQL_ATTR_PROGRAM",							strtolower("deliveryProgramPath"),			false);
pql_set_define("PQL_ATTR_PURGE",							strtolower("qmailAccountPurge"),			false);
pql_set_define("PQL_ATTR_QMAILGID",							strtolower("gidNumber"),					false);
pql_set_define("PQL_ATTR_QMAILUID",							strtolower("uidNumber"),					false);
pql_set_define("PQL_ATTR_QUOTA_VALUE",						strtolower("mailQuota"),					false);
pql_set_define("PQL_ATTR_QUOTA_SIZE",						strtolower("mailQuotaSize"),				false);
pql_set_define("PQL_ATTR_QUOTA_COUNT",						strtolower("mailQuotaCount"),				false);
pql_set_define("PQL_ATTR_QUOTA_WARNING",					strtolower("quotaWarning"),					false);
pql_set_define("PQL_ATTR_REPLYTEXT",						strtolower("mailReplyText"),				false);
pql_set_define("PQL_ATTR_UID",								strtolower("uid"),							false);

// }}}

// {{{ These are the QmailLDAP/Controls attributes.

pql_set_define("PQL_ATTR_BADMAILFROM",						strtolower("badMailFrom"),					false);
pql_set_define("PQL_ATTR_BADRCPTTO",						strtolower("badRcptTo"),					false);
pql_set_define("PQL_ATTR_BOUNCEFROM",						strtolower("bounceFrom"),					false);
pql_set_define("PQL_ATTR_BOUNCEHOST",						strtolower("bounceHost"),					false);
pql_set_define("PQL_ATTR_BOUNCESIZE",						strtolower("bounceMaxBytes"),				false);
pql_set_define("PQL_ATTR_CONCURRENCYLOCAL",					strtolower("concurrencyLocal"),				false);
pql_set_define("PQL_ATTR_CONCURRENCYREMOTE",				strtolower("concurrencyRemote"),			false);
pql_set_define("PQL_ATTR_CUSTOMBOUNCETEXT",					strtolower("customBounceText"),				false);
pql_set_define("PQL_ATTR_DATABYTES",						strtolower("dataBytes"),					false);
pql_set_define("PQL_ATTR_DEFAULTDOMAIN",					strtolower("defaultDomain"),				false);
pql_set_define("PQL_ATTR_DEFAULTHOST",						strtolower("defaultHost"),					false);
pql_set_define("PQL_ATTR_DIRMAKER",							strtolower("dirMaker"),						false);
pql_set_define("PQL_ATTR_DOUBLEBOUNCEHOST",					strtolower("doubleBounceHost"),				false);
pql_set_define("PQL_ATTR_DOUBLEBOUNCETO",					strtolower("doubleBounceTo"),				false);
pql_set_define("PQL_ATTR_ENVNOATHOST",						strtolower("envNoAtHost"),					false);
pql_set_define("PQL_ATTR_HELOHOST",							strtolower("heloHost"),						false);
pql_set_define("PQL_ATTR_IDHOST",							strtolower("idHost"),						false);
pql_set_define("PQL_ATTR_LDAPBASEDN",						strtolower("ldapBaseDN"),					false);
pql_set_define("PQL_ATTR_LDAPCLUSTER",						strtolower("ldapCluster"),					false);
pql_set_define("PQL_ATTR_LDAPCLUSTER_COOKIE",				strtolower("smtpClusterCookie"),			false);
pql_set_define("PQL_ATTR_LDAPCLUSTER_HOSTS",				strtolower("ldapClusterHosts"),				false);
pql_set_define("PQL_ATTR_LDAPDEFAULTDOTMODE",				strtolower("ldapDefaultDotMode"),			false);
pql_set_define("PQL_ATTR_LDAPDEFAULTQUOTA",					strtolower("ldapDefaultQuota"),				false);
pql_set_define("PQL_ATTR_LDAPDEFAULTQUOTA_SIZE",			strtolower("DefaultQuotaSize"),				false);
pql_set_define("PQL_ATTR_LDAPDEFAULTQUOTA_COUNT",			strtolower("DefaultQuotaCount"),			false);
pql_set_define("PQL_ATTR_LDAPGID",							strtolower("ldapGid"),						false);
pql_set_define("PQL_ATTR_LDAPLOCALDELIVERY",				strtolower("ldapLocalDelivery"),			false);
pql_set_define("PQL_ATTR_LDAPLOGIN",						strtolower("ldapLogin"),					false);
pql_set_define("PQL_ATTR_LDAPMESSAGESTORE",					strtolower("ldapMessageStore"),				false);
pql_set_define("PQL_ATTR_LDAPPASSWORD",						strtolower("ldapPassword"),					false);
pql_set_define("PQL_ATTR_LDAPREBIND",						strtolower("ldapRebind"),					false);
pql_set_define("PQL_ATTR_LDAPTIMEOUT",						strtolower("ldapTimeOut"),					false);
pql_set_define("PQL_ATTR_LDAPSERVER",						strtolower("ldapServer"),					false);
pql_set_define("PQL_ATTR_LDAPUID",							strtolower("ldapUid"),						false);
pql_set_define("PQL_ATTR_LOCALIPHOST",						strtolower("localIpHost"),					false);
pql_set_define("PQL_ATTR_OUTGOINGIP",						strtolower("outGoingIp"),					false);
pql_set_define("PQL_ATTR_LOCALS",							strtolower("locals"),						false);
pql_set_define("PQL_ATTR_MAXRCPTCOUNT",						strtolower("maxRcptCount"),					false);
pql_set_define("PQL_ATTR_PERCENTHACK",						strtolower("percentHack"),					false);
pql_set_define("PQL_ATTR_PLUSDOMAIN",						strtolower("plusDomain"),					false);
pql_set_define("PQL_ATTR_QMQPSERVERS",						strtolower("qmqpServers"),					false);
pql_set_define("PQL_ATTR_QMQPCIP",							strtolower("qmqpcIp"),						false);
pql_set_define("PQL_ATTR_QUEUELIFETIME",					strtolower("queueLifeTime"),				false);
pql_set_define("PQL_ATTR_RCPTHOSTS",						strtolower("rcptHosts"),					false);
pql_set_define("PQL_ATTR_RELAYMAILFROM",					strtolower("relayMailFrom"),				false);
pql_set_define("PQL_ATTR_SMTPGREETING",						strtolower("smtpGreeting"),					false);
pql_set_define("PQL_ATTR_SMTPROUTES",						strtolower("smtpRoutes"),					false);
pql_set_define("PQL_ATTR_TARPITCOUNT",						strtolower("tarPitCount"),					false);
pql_set_define("PQL_ATTR_TARPITDELAY",						strtolower("tarPitDelay"),					false);
pql_set_define("PQL_ATTR_TIMEOUTCONNECT",					strtolower("timeOutConnect"),				false);
pql_set_define("PQL_ATTR_TIMEOUTREMOTE",					strtolower("timeOutRemote"),				false);
pql_set_define("PQL_ATTR_TIMEOUTSMTPD",						strtolower("timeOutSmtpd"),					false);
pql_set_define("PQL_ATTR_BIGBROTHER",						strtolower("bigBrother"),					false);
pql_set_define("PQL_ATTR_RBLLIST",							strtolower("rblList"),						false);
pql_set_define("PQL_ATTR_GOODADDRESS",						strtolower("goodMailAddr"),					false);

// }}}

// {{{ Qmail-Group attributes in QmailLDAP
pql_set_define("PQL_ATTR_GROUP_LOGIN",						strtolower("ldapGroupLogin"),				false);
pql_set_define("PQL_ATTR_GROUP_PASSWD",						strtolower("ldapGroupPassWord"),			false);
pql_set_define("PQL_ATTR_GROUP_OC",							strtolower("qmailGroup"),					false);
pql_set_define("PQL_ATTR_GROUP_MEMBERS_ONLY",				strtolower("membersOnly"),					false);
pql_set_define("PQL_ATTR_GROUP_CONFIRM",					strtolower("senderConfirm"),				false);
pql_set_define("PQL_ATTR_GROUP_CONFIRM_TEXT",				strtolower("confirmText"),					false);
pql_set_define("PQL_ATTR_GROUP_MODERATOR_TEXT",				strtolower("moderatorText"),				false);
pql_set_define("PQL_ATTR_GROUP_DN_MODERATOR",				strtolower("dnModerator"),					false);
pql_set_define("PQL_ATTR_GROUP_DN_MEMBER",					strtolower("dnMember"),						false);
pql_set_define("PQL_ATTR_GROUP_DN_SENDER",					strtolower("dnSender"),						false);
pql_set_define("PQL_ATTR_GROUP_822_MODERATOR",				strtolower("rfc822Moderator"),				false);
pql_set_define("PQL_ATTR_GROUP_822_MEMBER",					strtolower("rfc822Member"),					false);
pql_set_define("PQL_ATTR_GROUP_822_SENDER",					strtolower("rfc822Sender"),					false);
pql_set_define("PQL_ATTR_GROUP_FILTER_MEMBER",				strtolower("filterMember"),					false);
pql_set_define("PQL_ATTR_GROUP_FILTER_SENDER",				strtolower("filterSender"),					false);
// }}}

// {{{ PopBeforeSMTP attributes in QmailLDAP
pql_set_define("PQL_ATTR_PBS_SERVERS",						strtolower("pbsServers"),					false);
pql_set_define("PQL_ATTR_PBS_IP",							strtolower("pbsIp"),						false);
pql_set_define("PQL_ATTR_PBS_PORT",							strtolower("pbsPort"),						false);
pql_set_define("PQL_ATTR_PBS_SECRET",						strtolower("pbsSecret"),					false);
pql_set_define("PQL_ATTR_PBS_CACHESIZE",					strtolower("pbsCacheSize"),					false);
pql_set_define("PQL_ATTR_PBS_TIMEOUT",						strtolower("pbsTimeOut"),					false);
pql_set_define("PQL_ATTR_PBS_ENV",							strtolower("pbsEnv"),						false);
// }}}

// {{{ Builtin LDAP attributes used by phpQLAdmin
pql_set_define("PQL_ATTR_C",								strtolower("c"),							false);
pql_set_define("PQL_ATTR_CN",								strtolower("cn"),							false);
pql_set_define("PQL_ATTR_DC",								strtolower("dc"),							false);
pql_set_define("PQL_ATTR_DESCRIPTION",						strtolower("description"),					false);
pql_set_define("PQL_ATTR_FACSIMILETELEPHONENUMBER",			strtolower("facsimileTelePhoneNumber"),		false);
pql_set_define("PQL_ATTR_GIVENNAME",						strtolower("givenName"),					false);
pql_set_define("PQL_ATTR_HOMEPHONE",						strtolower("homePhone"),					false);
pql_set_define("PQL_ATTR_HOMEPOSTALADDRESS",				strtolower("homePostalAddress"),			false);
pql_set_define("PQL_ATTR_INFO",								strtolower("info"),							false);
pql_set_define("PQL_ATTR_KRB5PRINCIPALNAME",				strtolower("krb5PrincipalName"),			false);
pql_set_define("PQL_ATTR_L",								strtolower("l"),							false);
pql_set_define("PQL_ATTR_MOBILE",							strtolower("mobile"),						false);
pql_set_define("PQL_ATTR_O",								strtolower("o"),							false);
pql_set_define("PQL_ATTR_OBJECTCLASS",						strtolower("objectClass"),					false);
pql_set_define("PQL_ATTR_LDAPACI",							strtolower("OpenLDAPaci"),					false);
pql_set_define("PQL_ATTR_OU",								strtolower("ou"),							false);
pql_set_define("PQL_ATTR_PAGER",							strtolower("pager"),						false);
pql_set_define("PQL_ATTR_PERSONALTITLE",					strtolower("personalTitle"),				false);
pql_set_define("PQL_ATTR_PHYSICALDELIVERYOFFICENAME",		strtolower("physicalDeliveryOfficeName"),	false);
pql_set_define("PQL_ATTR_POSTALCODE",						strtolower("postalCode"),					false);
pql_set_define("PQL_ATTR_POSTALADDRESS",					strtolower("postalAddress"),				false);
pql_set_define("PQL_ATTR_STREETADDRESS",					strtolower("streetAddress"),				false);
pql_set_define("PQL_ATTR_REGISTEREDADDRESS",				strtolower("registeredAddress"),			false);
pql_set_define("PQL_ATTR_POSTOFFICEBOX",					strtolower("postOfficeBox"),				false);
pql_set_define("PQL_ATTR_ROOMNUMBER",						strtolower("roomNumber"),					false);
pql_set_define("PQL_ATTR_SECRETARY",						strtolower("secretary"),					false);
pql_set_define("PQL_ATTR_SN",								strtolower("sn"),							false);
pql_set_define("PQL_ATTR_ST",								strtolower("st"),							false);
pql_set_define("PQL_ATTR_STREET",							strtolower("street"),						false);
pql_set_define("PQL_ATTR_SUBSCHEMASUBENTRY",				strtolower("subSchemaSubEntry"),			false);
pql_set_define("PQL_ATTR_TELEPHONENUMBER",					strtolower("telePhoneNumber"),				false);
pql_set_define("PQL_ATTR_TITLE",							strtolower("title"),						false);
pql_set_define("PQL_ATTR_ADDITIONAL_GROUP",					strtolower("memberUid"),					false);
// }}}

// {{{ Attributes used by the Bind9/LDAP sdb driver

pql_set_define("PQL_ATTR_ARECORD",							strtolower("aRecord"),						false);
pql_set_define("PQL_ATTR_CNAMERECORD",						strtolower("cNameRecord"),					false);
pql_set_define("PQL_ATTR_DNSTTL",							strtolower("dnsTtl"),						false);
pql_set_define("PQL_ATTR_HINFORECORD",						strtolower("hInfoRecord"),					false);
pql_set_define("PQL_ATTR_MXRECORD",							strtolower("mxRecord"),						false);
pql_set_define("PQL_ATTR_NSRECORD",							strtolower("nsRecord"),						false);
pql_set_define("PQL_ATTR_RELATIVEDOMAINNAME",				strtolower("relativeDomainName"),			false);
pql_set_define("PQL_ATTR_ZONENAME",							strtolower("zoneName"),						false);

// }}}

// {{{ Attributes used by the Apache config

pql_set_define("PQL_ATTR_WEBSRV_USE", 						strtolower('useWebSrv'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_IP", 					strtolower('webServerIP'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_URL", 					strtolower('webServerURL'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SCRIPT_URL", 				strtolower('webScriptAliasURL'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SCRIPT_PATH", 				strtolower('WebScriptAliasPath'),			false);
pql_set_define("PQL_ATTR_WEBSRV_LOG_ERROR", 				strtolower('webLogError'),					false);
pql_set_define("PQL_ATTR_WEBSRV_LOG_TRANSFER", 				strtolower('webLogTransfer'),				false);
pql_set_define("PQL_ATTR_WEBSRV_OPTIONS", 					strtolower('webOptions'),					false);

// If not using 'mod_cfg_ldap.schema', then enable these (default)
pql_set_define("PQL_ATTR_WEBSRV_OC",	 					strtolower('phpQLAdminWebsrv'),				false);
pql_set_define("PQL_ATTR_WEBSRV_DOCROOT", 					strtolower('webDocumentRoot'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ALIAS", 				strtolower('webServerAlias'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ADMIN", 				strtolower('webServerAdmin'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_NAME", 					strtolower('webServerName'),				false);

// If you're using the 'mod_cfg_ldap.schema' (see README.apache), then enable these instead.
//pql_set_define("PQL_ATTR_WEBSRV_OC",	 					strtolower('apacheConfig'),					false);
//pql_set_define("PQL_ATTR_WEBSRV_DOCROOT", 					strtolower('apacheDocumentRoot'),			false);
//pql_set_define("PQL_ATTR_WEBSRV_SRV_ALIAS", 				strtolower('apacheServerAlias'),			false);
//pql_set_define("PQL_ATTR_WEBSRV_SRV_ADMIN", 				strtolower('apacheServerAdmin'),			false);
//pql_set_define("PQL_ATTR_WEBSRV_SRV_NAME",					strtolower('apacheServerName'),				false);

// }}}

// {{{ Miscellaneous external attributes
pql_set_define("PQL_ATTR_SPAMASSASSIN", 					strtolower('spamAssassin'),					false);
// }}}

// {{{ These are the internal phpQLAdmin attributes - GLOBAL configuration

// Toggles
pql_set_define("PQL_ATTR_CONTROL_USE", 						strtolower('useControls'),					false);
pql_set_define("PQL_ATTR_EZMLM_USE", 						strtolower('useEzmlm'),						false);
pql_set_define("PQL_ATTR_BIND9_USE", 						strtolower('useBind9'),						false);
pql_set_define("PQL_ATTR_ACI_USE", 							strtolower('useACI'),						false);
pql_set_define("PQL_ATTR_AUTO_RELOAD", 						strtolower('autoReload'),					false);
pql_set_define("PQL_ATTR_CHANGE_SERVER", 					strtolower('allowServerChange'),			false);

// Setup
pql_set_define("PQL_ATTR_WHOAREWE", 						strtolower('whoAreWe'),						false);
pql_set_define("PQL_ATTR_LANG", 							strtolower('language'),						false);
pql_set_define("PQL_ATTR_HOSTMASTER", 						strtolower('hostMaster'),					false);
pql_set_define("PQL_ATTR_CONTROL_DN", 						strtolower('controlBaseDN'),				false);
pql_set_define("PQL_ATTR_EZMLM_PATH", 						strtolower('ezmlmBinaryPath'),				false);
pql_set_define("PQL_ATTR_EZMLM_USER", 						strtolower('ezmlmVirtualUser'),				false);
pql_set_define("PQL_ATTR_KRB5_REALM", 						strtolower('krb5RealmName'),				false);
pql_set_define("PQL_ATTR_KRB5_ADMIN_SERVER", 				strtolower('krb5AdminServer'),				false);
pql_set_define("PQL_ATTR_KRB5_ADMIN_PRINCIPAL", 			strtolower('krb5PrincipalName'),			false);
pql_set_define("PQL_ATTR_KRB5_ADMIN_KEYTAB", 				strtolower('krb5AdminKeytab'),				false);
pql_set_define("PQL_ATTR_KRB5_ADMIN_COMMAND_PATH", 			strtolower('krb5AdminCommandPath'),			false);
pql_set_define("PQL_ATTR_ADMINISTRATOR",					strtolower("administrator"),				false);
pql_set_define("PQL_ATTR_ADMINISTRATOR_EZMLM",				strtolower("ezmlmAdministrator"),			false);
pql_set_define("PQL_ATTR_ADMINISTRATOR_CONTROLS",			strtolower("controlsAdministrator"),		false);
pql_set_define("PQL_ATTR_VIRTUAL_BASEHOME",					strtolower('virtualBaseHomeDir'),			false);

// Map attribute to configuration define
$PQL_ATTRIBUTE["GLOBAL"] = array("PQL_CONF_CONTROL_USE"					=> pql_get_define("PQL_ATTR_CONTROL_USE"),
								 "PQL_CONF_EZMLM_USE"					=> pql_get_define("PQL_ATTR_EZMLM_USE"),
								 "PQL_CONF_BIND9_USE"					=> pql_get_define("PQL_ATTR_BIND9_USE"),
								 "PQL_CONF_ACI_USE"						=> pql_get_define("PQL_ATTR_ACI_USE"),
								 "PQL_CONF_WEBSRV_USE"					=> pql_get_define("PQL_ATTR_WEBSRV_USE"),
								 "PQL_CONF_AUTO_RELOAD"					=> pql_get_define("PQL_ATTR_AUTO_RELOAD"),
								 "PQL_CONF_CHANGE_SERVER"				=> pql_get_define("PQL_ATTR_CHANGE_SERVER"),
								 "PQL_CONF_WHOAREWE"					=> pql_get_define("PQL_ATTR_WHOAREWE"),
								 "PQL_CONF_LANG"						=> pql_get_define("PQL_ATTR_LANG"),
								 "PQL_CONF_HOSTMASTER"					=> pql_get_define("PQL_ATTR_HOSTMASTER"),
								 "PQL_CONF_CONTROL_DN"					=> pql_get_define("PQL_ATTR_CONTROL_DN"),
								 "PQL_CONF_EZMLM_PATH"					=> pql_get_define("PQL_ATTR_EZMLM_PATH"),
								 "PQL_CONF_KRB5_REALM"					=> pql_get_define("PQL_ATTR_KRB5_REALM"),
								 "PQL_CONF_KRB5_ADMIN_SERVER"			=> pql_get_define("PQL_ATTR_KRB5_ADMIN_SERVER"),
								 "PQL_CONF_KRB5_ADMIN_PRINCIPAL"		=> pql_get_define("PQL_ATTR_KRB5_ADMIN_PRINCIPAL"),
								 "PQL_CONF_KRB5_ADMIN_KEYTAB"			=> pql_get_define("PQL_ATTR_KRB5_ADMIN_KEYTAB"),
								 "PQL_CONF_KRB5_ADMIN_COMMAND_PATH"		=> pql_get_define("PQL_ATTR_KRB5_ADMIN_COMMAND_PATH"),
								 "PQL_CONF_ADMINISTRATOR"				=> pql_get_define("PQL_ATTR_ADMINISTRATOR"),
								 "PQL_CONF_ADMINISTRATOR_EZMLM"	    	=> pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"),
								 "PQL_CONF_ADMINISTRATOR_CONTROLS"		=> pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
								 "PQL_CONF_VIRTUAL_BASEHOME"			=> pql_get_define("PQL_ATTR_VIRTUAL_BASEHOME"));

// }}}

// {{{ These are the internal phpQLAdmin attributes - BRANCH configuration

// Toggles
pql_set_define("PQL_ATTR_SHOW_USERS", 						strtolower('showUsers'),					false);
pql_set_define("PQL_ATTR_TESTMAIL_AUTOSEND", 				strtolower('autoSend'),						false);
pql_set_define("PQL_ATTR_ALLOW_ABSOLUTE_PATH", 				strtolower('allowAbsoluteMailPath'),		false);
pql_set_define("PQL_ATTR_VERIFY_DELETE", 					strtolower('deleteVerification'),			false);
pql_set_define("PQL_ATTR_AUTOCREATE_MAILADDRESS",			strtolower('autoCreateMailAddress'),		false);
pql_set_define("PQL_ATTR_AUTOCREATE_USERNAME",				strtolower('autoCreateUserName'),			false);
pql_set_define("PQL_ATTR_AUTOCREATE_PASSWORD",				strtolower('autoCreatePassWord'),			false);
pql_set_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME",	 		strtolower('autoAddHostName'),				false);
pql_set_define("PQL_ATTR_CONTROL_AUTOADDLOCALS", 			strtolower('autoAddLocals'),				false);
pql_set_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS", 		strtolower('autoAddRCPTHosts'),				false);

// Setup
pql_set_define("PQL_ATTR_ADMINISTRATOR",					strtolower('administrator'),				false);
pql_set_define("PQL_ATTR_SEEALSO",							strtolower('seeAlso'),						false);
pql_set_define("PQL_ATTR_BASEHOMEDIR",						strtolower('baseHomeDir'),					false);
pql_set_define("PQL_ATTR_BASEMAILDIR",						strtolower('baseMailDir'),					false);
pql_set_define("PQL_ATTR_BASEQUOTA",						strtolower('baseQuota'),					false);
pql_set_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME",			strtolower('defaultPasswordScheme'),		false);
pql_set_define("PQL_ATTR_USERNAME_PREFIX",					strtolower('userNamePrefix'),				false);
pql_set_define("PQL_ATTR_USERNAME_PREFIX_LENGTH",			strtolower('userNamePrefixLength'),			false);
pql_set_define("PQL_ATTR_ADDITIONAL_DOMAINNAME", 			strtolower('additionalDomainName'),			false);
pql_set_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS", 			strtolower('maximumDomainUsers'),			false);
pql_set_define("PQL_ATTR_MAXIMUM_MAILING_LISTS",			strtolower('maximumMailingLists'),			false);
pql_set_define("PQL_ATTR_OBJECTCLASS_DOMAIN", 				strtolower('branchObjectClass'),			false);
pql_set_define("PQL_ATTR_OBJECTCLASS_USER", 				strtolower('userObjectClass'),				false);
pql_set_define("PQL_ATTR_MINIMUM_UIDNUMBER", 				strtolower('minimumUIDNumber'),				false);
pql_set_define("PQL_ATTR_MINIMUM_GIDNUMBER", 				strtolower('minimumGIDNumber'),				false);
pql_set_define("PQL_ATTR_FORWARDINGACCOUNT_UIDNUMBER",	 	strtolower('forwardUIDNumber'),				false);
pql_set_define("PQL_ATTR_TESTMAIL_SUBJECT", 				strtolower('testMailSubject'),				false);
pql_set_define("PQL_ATTR_TESTMAIL_MAILTEXT", 				strtolower('testMailText'),					false);
pql_set_define("PQL_ATTR_CATCHALLUSER", 					strtolower('catchAllUser'),					false);
pql_set_define("PQL_ATTR_PASSWORD_SCHEMES", 				strtolower('passWordScheme'),				false);
pql_set_define("PQL_ATTR_SCRIPT_CREATE_USER", 				strtolower('externalScriptUser'),			false);
pql_set_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN", 			strtolower('externalScriptDomain'),			false);
pql_set_define("PQL_ATTR_REFERENCE_USERS_WITH", 			strtolower('userReference'),				false);
pql_set_define("PQL_ATTR_REFERENCE_DOMAINS_WITH", 			strtolower('branchReference'),				false);
pql_set_define("PQL_ATTR_SUBTREE_USERS", 					strtolower('subTreeUsers'),					false);
pql_set_define("PQL_ATTR_SUBTREE_GROUPS", 					strtolower('subTreeGroups'),				false);
pql_set_define("PQL_ATTR_VAT_NUMBER",						strtolower('vatNumber'),					false);

// Map attribute to configuration define
$PQL_ATTRIBUTE["BRANCH"] = array("PQL_CONF_SHOW_USERS"					=> pql_get_define("PQL_ATTR_SHOW_USERS"),
								 "PQL_CONF_TESTMAIL_AUTOSEND"			=> pql_get_define("PQL_ATTR_TESTMAIL_AUTOSEND"),
								 "PQL_CONF_ALLOW_ABSOLUTE_PATH"			=> pql_get_define("PQL_ATTR_ALLOW_ABSOLUTE_PATH"),
								 "PQL_CONF_VERIFY_DELETE"				=> pql_get_define("PQL_ATTR_VERIFY_DELETE"),
								 "PQL_CONF_AUTOCREATE_MAILADDRESS"		=> pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
								 "PQL_CONF_AUTOCREATE_USERNAME"			=> pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
								 "PQL_CONF_AUTOCREATE_PASSWORD"			=> pql_get_define("PQL_ATTR_AUTOCREATE_PASSWORD"),
								 "PQL_CONF_CONTROL_AUTOADDHOSTNAME"		=> pql_get_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME"),
								 "PQL_CONF_CONTROL_AUTOADDLOCALS"		=> pql_get_define("PQL_ATTR_CONTROL_AUTOADDLOCALS"),
								 "PQL_CONF_CONTROL_AUTOADDRCPTHOSTS"	=> pql_get_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS"),
								 "PQL_CONF_ADMINISTRATOR"				=> pql_get_define("PQL_ATTR_ADMINISTRATOR"),
								 "PQL_CONF_SEEALSO"						=> pql_get_define("PQL_ATTR_SEEALSO"),
								 "PQL_CONF_BASEHOMEDIR"					=> pql_get_define("PQL_ATTR_BASEHOMEDIR"),
								 "PQL_CONF_BASEMAILDIR"					=> pql_get_define("PQL_ATTR_BASEMAILDIR"),
								 "PQL_CONF_BASEQUOTA"					=> pql_get_define("PQL_ATTR_BASEQUOTA"),
								 "PQL_CONF_DEFAULT_PASSWORDSCHEME"		=> pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"),
								 "PQL_CONF_USERNAME_PREFIX"				=> pql_get_define("PQL_ATTR_USERNAME_PREFIX"),
								 "PQL_CONF_USERNAME_PREFIX_LENGTH"		=> pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"),
								 "PQL_CONF_ADDITIONAL_DOMAINNAME"		=> pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"),
								 "PQL_CONF_MAXIMUM_DOMAIN_USERS"		=> pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"),
								 "PQL_CONF_MAXIMUM_MAILING_LISTS"		=> pql_get_define("PQL_ATTR_MAXIMUM_MAILING_LISTS"),
								 "PQL_CONF_OBJECTCLASS_DOMAIN"			=> pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"),
								 "PQL_CONF_OBJECTCLASS_USER"			=> pql_get_define("PQL_ATTR_OBJECTCLASS_USER"),
								 "PQL_CONF_MINIMUM_UIDNUMBER"			=> pql_get_define("PQL_ATTR_MINIMUM_UIDNUMBER"),
								 "PQL_CONF_MINIMUM_GIDNUMBER"			=> pql_get_define("PQL_ATTR_MINIMUM_GIDNUMBER"),
								 "PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"	=> pql_get_define("PQL_ATTR_FORWARDINGACCOUNT_UIDNUMBER"),
								 "PQL_CONF_TESTMAIL_SUBJECT"			=> pql_get_define("PQL_ATTR_TESTMAIL_SUBJECT"),
								 "PQL_CONF_TESTMAIL_MAILTEXT"			=> pql_get_define("PQL_ATTR_TESTMAIL_MAILTEXT"),
								 "PQL_CONF_CATCHALLUSER"				=> pql_get_define("PQL_ATTR_CATCHALLUSER"),
								 "PQL_CONF_PASSWORD_SCHEMES"			=> pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"),
								 "PQL_CONF_SCRIPT_CREATE_USER"			=> pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER"),
								 "PQL_CONF_SCRIPT_CREATE_DOMAIN"		=> pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN"),
								 "PQL_CONF_REFERENCE_USERS_WITH"		=> pql_get_define("PQL_ATTR_REFERENCE_USERS_WITH"),
								 "PQL_CONF_REFERENCE_DOMAINS_WITH"		=> pql_get_define("PQL_ATTR_REFERENCE_DOMAINS_WITH"),
								 "PQL_CONF_SUBTREE_USERS"				=> pql_get_define("PQL_ATTR_SUBTREE_USERS"),
								 "PQL_CONF_SUBTREE_GROUPS"				=> pql_get_define("PQL_ATTR_SUBTREE_GROUPS"),
								 "PQL_CONF_EZMLM_USER"					=> pql_get_define("PQL_ATTR_EZMLM_USER"),
								 "PQL_CONF_VAT_NUMBER"					=> pql_get_define("PQL_ATTR_VAT_NUMBER"));

// }}}

// {{{ These are the internal phpQLAdmin attributes - USER configuration
// Toggles
pql_set_define("PQL_ATTR_START_ADVANCED",					strtolower("startWithAdvancedMode"),		false);

// Map attribute to configuration define
$PQL_ATTRIBUTE["USER"] = array("PQL_CONF_START_ADVANCED"				=> pql_get_define("PQL_ATTR_START_ADVANCED"));
// }}}

pql_set_define("PQL_ATTR_USER_RDN_ATTRIBUTE",	strtolower(pql_get_define("PQL_ATTR_REFERENCE_USERS_WITH")),	false);

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
