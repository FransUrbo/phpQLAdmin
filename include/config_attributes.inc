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
// $Id: config_attributes.inc,v 2.3 2003-10-15 05:25:23 turbo Exp $

// User object attributes
pql_set_define("PQL_GLOB_ATTR_DOTMODE",				strtolower("qmailDotMode"),		false);
pql_set_define("PQL_GLOB_ATTR_FORWARDS",			strtolower("mailForwardingAddress"),	false);
pql_set_define("PQL_GLOB_ATTR_GECOS",				strtolower("gecos"),			false);
pql_set_define("PQL_GLOB_ATTR_HOMEDIR",				strtolower("homeDirectory"),		false);
pql_set_define("PQL_GLOB_ATTR_ISACTIVE",			strtolower("accountStatus"),		false);
pql_set_define("PQL_GLOB_ATTR_EXPIRATION",			strtolower("shadowExpire"),		false);
pql_set_define("PQL_GLOB_ATTR_LOGINSHELL",			strtolower("loginShell"),		false);
pql_set_define("PQL_GLOB_ATTR_MAIL",				strtolower("mail"),			false);
pql_set_define("PQL_GLOB_ATTR_MAILALTERNATE",			strtolower("mailAlternateAddress"),	false);
pql_set_define("PQL_GLOB_ATTR_MAILHOST",			strtolower("mailHost"),			false);
pql_set_define("PQL_GLOB_ATTR_MAILSTORE",			strtolower("mailMessageStore"),		false);
pql_set_define("PQL_GLOB_ATTR_MODE",				strtolower("deliveryMode"),		false);
pql_set_define("PQL_GLOB_ATTR_PASSWD",				strtolower("userPassword"),		false);
pql_set_define("PQL_GLOB_ATTR_PROGRAM",				strtolower("deliveryProgramPath"),	false);
pql_set_define("PQL_GLOB_ATTR_PURGE",				strtolower("qmailAccountPurge"),	false);
pql_set_define("PQL_GLOB_ATTR_QMAILGID",			strtolower("gidNumber"),		false);
pql_set_define("PQL_GLOB_ATTR_QMAILUID",			strtolower("uidNumber"),		false);
pql_set_define("PQL_GLOB_ATTR_QUOTA",				strtolower("mailQuota"),		false);
pql_set_define("PQL_GLOB_ATTR_REPLYTEXT",			strtolower("mailReplyText"),		false);
pql_set_define("PQL_GLOB_ATTR_UID",				strtolower("uid"),			false);

// These are the QmailLDAP/Controls attributes.
pql_set_define("PQL_GLOB_ATTR_BADMAILFROM",			strtolower("badMailFrom"),			false);
pql_set_define("PQL_GLOB_ATTR_BADRCPTTO",			strtolower("badRcptTo"),			false);
pql_set_define("PQL_GLOB_ATTR_BOUNCEFROM",			strtolower("bounceFrom"),			false);
pql_set_define("PQL_GLOB_ATTR_BOUNCEHOST",			strtolower("bounceHost"),			false);
pql_set_define("PQL_GLOB_ATTR_CONCURRENCYLOCAL",		strtolower("concurrencyLocal"),			false);
pql_set_define("PQL_GLOB_ATTR_CONCURRENCYREMOTE",		strtolower("concurrencyRemote"),		false);
pql_set_define("PQL_GLOB_ATTR_CUSTOMBOUNCETEXT",		strtolower("customBounceText"),			false);
pql_set_define("PQL_GLOB_ATTR_DATABYTES",			strtolower("dataBytes"),			false);
pql_set_define("PQL_GLOB_ATTR_DEFAULTDOMAIN",			strtolower("defaultDomain"),			false);
pql_set_define("PQL_GLOB_ATTR_DEFAULTHOST",			strtolower("defaultHost"),			false);
pql_set_define("PQL_GLOB_ATTR_DIRMAKER",			strtolower("dirMaker"),				false);
pql_set_define("PQL_GLOB_ATTR_DOUBLEBOUNCEHOST",		strtolower("doubleBounceHost"),			false);
pql_set_define("PQL_GLOB_ATTR_DOUBLEBOUNCETO",			strtolower("doubleBounceTo"),			false);
pql_set_define("PQL_GLOB_ATTR_ENVNOATHOST",			strtolower("envNoAtHost"),			false);
pql_set_define("PQL_GLOB_ATTR_HELOHOST",			strtolower("heloHost"),				false);
pql_set_define("PQL_GLOB_ATTR_IDHOST",				strtolower("idHost"),				false);
pql_set_define("PQL_GLOB_ATTR_LDAPBASEDN",			strtolower("ldapBaseDN"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPCLUSTER",			strtolower("ldapCluster"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPDEFAULTDOTMODE",		strtolower("ldapDefaultDotMode"),		false);
pql_set_define("PQL_GLOB_ATTR_LDAPDEFAULTQUOTA",		strtolower("ldapDefaultQuota"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPGID",				strtolower("ldapGid"),				false);
pql_set_define("PQL_GLOB_ATTR_LDAPLOCALDELIVERY",		strtolower("ldapLocalDelivery"),		false);
pql_set_define("PQL_GLOB_ATTR_LDAPLOGIN",			strtolower("ldapLogin"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPMESSAGESTORE",		strtolower("ldapMessageStore"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPPASSWORD",			strtolower("ldapPassword"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPREBIND",			strtolower("ldapRebind"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPSERVER",			strtolower("ldapServer"),			false);
pql_set_define("PQL_GLOB_ATTR_LDAPUID",				strtolower("ldapUid"),				false);
pql_set_define("PQL_GLOB_ATTR_LOCALIPHOST",			strtolower("localIpHost"),			false);
pql_set_define("PQL_GLOB_ATTR_LOCALS",				strtolower("locals"),				false);
pql_set_define("PQL_GLOB_ATTR_MAXRCPTCOUNT",			strtolower("maxRcptCount"),			false);
pql_set_define("PQL_GLOB_ATTR_PERCENTHACK",			strtolower("percentHack"),			false);
pql_set_define("PQL_GLOB_ATTR_PLUSDOMAIN",			strtolower("plusDomain"),			false);
pql_set_define("PQL_GLOB_ATTR_QMQPSERVERS",			strtolower("qmqpServers"),			false);
pql_set_define("PQL_GLOB_ATTR_QUEUELIFETIME",			strtolower("queueLifeTime"),			false);
pql_set_define("PQL_GLOB_ATTR_QUOTAWARNING",			strtolower("quotaWarning"),			false);
pql_set_define("PQL_GLOB_ATTR_RCPTHOSTS",			strtolower("rcptHosts"),			false);
pql_set_define("PQL_GLOB_ATTR_RELAYMAILFROM",			strtolower("relayMailFrom"),			false);
pql_set_define("PQL_GLOB_ATTR_SMTPGREETING",			strtolower("smtpGreeting"),			false);
pql_set_define("PQL_GLOB_ATTR_SMTPROUTES",			strtolower("smtpRoutes"),			false);
pql_set_define("PQL_GLOB_ATTR_TARPITCOUNT",			strtolower("tarPitCount"),			false);
pql_set_define("PQL_GLOB_ATTR_TARPITDELAY",			strtolower("tarPitDelay"),			false);
pql_set_define("PQL_GLOB_ATTR_TIMEOUTCONNECT",			strtolower("timeOutConnect"),			false);
pql_set_define("PQL_GLOB_ATTR_TIMEOUTREMOTE",			strtolower("timeOutRemote"),			false);
pql_set_define("PQL_GLOB_ATTR_TIMEOUTSMTPD",			strtolower("timeOutSmtpd"),			false);

// Builtin LDAP attributes
pql_set_define("PQL_GLOB_ATTR_C",				strtolower("c"),				false);
pql_set_define("PQL_GLOB_ATTR_CN",				strtolower("cn"),				false);
pql_set_define("PQL_GLOB_ATTR_DC",				strtolower("dc"),				false);
pql_set_define("PQL_GLOB_ATTR_DESCRIPTION",			strtolower("description"),			false);
pql_set_define("PQL_GLOB_ATTR_FACSIMILETELEPHONENUMBER",	strtolower("facsimileTelePhoneNumber"),		false);
pql_set_define("PQL_GLOB_ATTR_GIVENNAME",			strtolower("givenName"),			false);
pql_set_define("PQL_GLOB_ATTR_HOMEPHONE",			strtolower("homePhone"),			false);
pql_set_define("PQL_GLOB_ATTR_HOMEPOSTALADDRESS",		strtolower("homePostalAddress"),		false);
pql_set_define("PQL_GLOB_ATTR_INFO",				strtolower("info"),				false);
pql_set_define("PQL_GLOB_ATTR_KRB5PRINCIPALNAME",		strtolower("krb5PrincipalName"),		false);
pql_set_define("PQL_GLOB_ATTR_L",				strtolower("l"),				false);
pql_set_define("PQL_GLOB_ATTR_MOBILE",				strtolower("mobile"),				false);
pql_set_define("PQL_GLOB_ATTR_O",				strtolower("o"),				false);
pql_set_define("PQL_GLOB_ATTR_OBJECTCLASS",			strtolower("objectClass"),			false);
pql_set_define("PQL_GLOB_ATTR_OPENLDAPACI",			strtolower("OpenLDAPaci"),			false);
pql_set_define("PQL_GLOB_ATTR_OU",				strtolower("ou"),				false);
pql_set_define("PQL_GLOB_ATTR_PAGER",				strtolower("pager"),				false);
pql_set_define("PQL_GLOB_ATTR_PERSONALTITLE",			strtolower("personalTitle"),			false);
pql_set_define("PQL_GLOB_ATTR_PHYSICALDELIVERYOFFICENAME",	strtolower("physicalDeliveryOfficeName"),	false);
pql_set_define("PQL_GLOB_ATTR_POSTALCODE",			strtolower("postalCode"),			false);
pql_set_define("PQL_GLOB_ATTR_POSTALADDRESS",			strtolower("postalAddress"),			false);
pql_set_define("PQL_GLOB_ATTR_POSTOFFICEBOX",			strtolower("postOfficeBox"),			false);
pql_set_define("PQL_GLOB_ATTR_ROOMNUMBER",			strtolower("roomNumber"),			false);
pql_set_define("PQL_GLOB_ATTR_SECRETARY",			strtolower("secretary"),			false);
pql_set_define("PQL_GLOB_ATTR_SN",				strtolower("sn"),				false);
pql_set_define("PQL_GLOB_ATTR_ST",				strtolower("st"),				false);
pql_set_define("PQL_GLOB_ATTR_STREET",				strtolower("street"),				false);
pql_set_define("PQL_GLOB_ATTR_SUBSCHEMASUBENTRY",		strtolower("subSchemaSubEntry"),		false);
pql_set_define("PQL_GLOB_ATTR_TELEPHONENUMBER",			strtolower("telePhoneNumber"),			false);
pql_set_define("PQL_GLOB_ATTR_TITLE",				strtolower("title"),				false);

// Bind9/LDAP attributes
pql_set_define("PQL_GLOB_ATTR_ARECORD",				strtolower("aRecord"),				false);
pql_set_define("PQL_GLOB_ATTR_CNAMERECORD",			strtolower("cNameRecord"),			false);
pql_set_define("PQL_GLOB_ATTR_DNSTTL",				strtolower("dnsTtl"),				false);
pql_set_define("PQL_GLOB_ATTR_HINFORECORD",			strtolower("hInfoRecord"),			false);
pql_set_define("PQL_GLOB_ATTR_MXRECORD",			strtolower("mxRecord"),				false);
pql_set_define("PQL_GLOB_ATTR_NSRECORD",			strtolower("nsRecord"),				false);
pql_set_define("PQL_GLOB_ATTR_RELATIVEDOMAINNAME",		strtolower("relativeDomainName"),		false);
pql_set_define("PQL_GLOB_ATTR_ZONENAME",			strtolower("zoneName"),				false);

// phpQLAdmin attributes
pql_set_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME",		strtolower("additionalDomainName"),		false);
pql_set_define("PQL_GLOB_ATTR_ADMINISTRATOR",			strtolower("administrator"),			false);
pql_set_define("PQL_GLOB_ATTR_AUTOCREATEMAILADDRESS",		strtolower("autoCreateMailAddress"),		false);
pql_set_define("PQL_GLOB_ATTR_AUTOCREATEUSERNAME",		strtolower("autoCreateUserName"),		false);
pql_set_define("PQL_GLOB_ATTR_BASEHOMEDIR",			strtolower("baseHomeDir"),			false);
pql_set_define("PQL_GLOB_ATTR_BASEMAILDIR",			strtolower("baseMailDir"),			false);
pql_set_define("PQL_GLOB_ATTR_BASEQUOTA",			strtolower("baseQuota"),			false);
pql_set_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR",		strtolower("controlsAdministrator"),		false);
pql_set_define("PQL_GLOB_ATTR_DEFAULTPASSWORDSCHEME",		strtolower("defaultPasswordScheme"),		false);
pql_set_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR",		strtolower("ezmlmAdministrator"),		false);
pql_set_define("PQL_GLOB_ATTR_LANGUAGE",			strtolower("language"),				false);
pql_set_define("PQL_GLOB_ATTR_MAXIMUMDOMAINUSERS",		strtolower("maximumDomainUsers"),		false);
pql_set_define("PQL_GLOB_ATTR_MAXIMUMMAILINGLISTS",		strtolower("maximumMailingLists"),		false);
pql_set_define("PQL_GLOB_ATTR_SEEALSO",				strtolower("seeAlso"),				false);
pql_set_define("PQL_GLOB_ATTR_USERNAMEPREFIX",			strtolower("userNamePrefix"),			false);
?>
