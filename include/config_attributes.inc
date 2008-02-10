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
// WARNING: If you _change_ a PQL_ATTR_* define (that is, not ADDING or
//          REMOVING one) AND you are using memcached, then you MUST
//          resetart memcached (or wait for it to expunge the stored
//          value). This is because phpQLAdmin will store these values
//          in the memcached cache...
//
// $Id: config_attributes.inc,v 2.69 2007-11-20 11:50:04 turbo Exp $

// {{{ These are the QmailLDAP user object attributes
pql_set_define("PQL_ATTR_DOTMODE",							strtolower("qmailDotMode"),					false);
pql_set_define("PQL_ATTR_FORWARDS",							strtolower("mailForwardingAddress"),		false);
pql_set_define("PQL_ATTR_GECOS",							strtolower("gecos"),						false);
pql_set_define("PQL_ATTR_HOMEDIR",							strtolower("homeDirectory"),				false);
pql_set_define("PQL_ATTR_ISACTIVE",							strtolower("accountStatus"),				false);
pql_set_define("PQL_ATTR_ACCOUNT_PURGE",					strtolower("qmailAccountPurge"),			false);
pql_set_define("PQL_ATTR_PASSWORD_LASTCHANGED",         	strtolower("shadowLastChange"),             false);
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
pql_set_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS",			strtolower("nonPrimaryRcptHosts"),			false);
pql_set_define("PQL_ATTR_MAILSIZEMAX",						strtolower("mailSizeMax"),					false);
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
pql_set_define("PQL_ATTR_UNIQUE_GROUP",						strtolower("uniqueMember"),					false);
pql_set_define("PQL_ATTR_IPHOSTNUMBER",						strtolower("ipHostNumber"),					false);
pql_set_define("PQL_ATTR_AUTH_TIMESTAMP",					strtolower("authTimestamp"),				false);
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
pql_set_define("PQL_ATTR_SRVRECORD",						strtolower("srvRecord"),					false);
pql_set_define("PQL_ATTR_SOARECORD",						strtolower("soaRecord"),					false);
pql_set_define("PQL_ATTR_TXTRECORD",						strtolower("txtRecord"),					false);
pql_set_define("PQL_ATTR_PTRRECORD",						strtolower("PTRRecord"),					false);
pql_set_define("PQL_ATTR_AFSRECORD",						strtolower("afsDBRecord"),					false);
// }}}

// {{{ Attributes used by the Apache config
pql_set_define("PQL_ATTR_WEBSRV_USE", 						strtolower('useWebSrv'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_IP", 					strtolower('ApacheSectionArg'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_URL", 					strtolower('ApacheServerName'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SCRIPT_URL", 				strtolower('ApacheScriptAlias'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SCRIPT_PATH", 				strtolower('ApacheAlias'),					false);
pql_set_define("PQL_ATTR_WEBSRV_LOG_ERROR", 				strtolower('ApacheErrorLog'),				false);
pql_set_define("PQL_ATTR_WEBSRV_LOG_TRANSFER", 				strtolower('ApacheTransferLog'),			false);

pql_set_define("PQL_ATTR_WEBSRV_DOCROOT", 					strtolower('ApacheDocumentRoot'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ALIAS", 				strtolower('ApacheServerAlias'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ADMIN", 				strtolower('ApacheServerAdmin'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_NAME", 					strtolower('ApacheServerName'),				false);

pql_set_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE",		strtolower('ApacheRewriteEngine'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE",			strtolower('ApacheRewriteRule'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS", 		strtolower('ApacheRewriteOptions'),			false);

pql_set_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME",			strtolower('ApacheSectionName'),			false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER",			strtolower('ApacheSetHandler'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER",			strtolower('ApacheOrder'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY",			strtolower('ApacheDeny'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW",			strtolower('ApacheAllow'),					false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_OPTIONS",				strtolower('ApacheOptions'),				false);
pql_set_define("PQL_ATTR_WEBSRV_SRV_RAW_ARG", 				strtolower('ApacheRawArg'),					false);

pql_set_define("PQL_ATTR_WEBSRV_LDAP_SERVER", 				strtolower('webLDAPServer'),				false);
pql_set_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN", 		strtolower('webLDAPServerLoginDN'),			false);
pql_set_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW", 		strtolower('webLDAPServerLoginPW'),			false);
// }}}

// {{{ Attributes used by the SimScan config
pql_set_define("PQL_ATTR_SIMSCAN_USE", 						strtolower('useSimScan'),					false);
pql_set_define("PQL_ATTR_SIMSCAN_SPAM",						strtolower('simScanSpamAssassin'),			false);
pql_set_define("PQL_ATTR_SIMSCAN_CLAM",						strtolower('simScanClamAntiVirus'),			false);
pql_set_define("PQL_ATTR_SIMSCAN_TROPHIE",					strtolower('simScanTrophie'),				false);

pql_set_define("PQL_ATTR_SIMSCAN_SPAM_HITS",				strtolower('simScanSpamAssassinHits'),		false);
pql_set_define("PQL_ATTR_SIMSCAN_ATTACHMENTS",				strtolower('simScanAttachmentSuffix'),		false);
// }}}

// {{{ Attributes for external ezmlm mailing lists
pql_set_define("PQL_ATTR_EZMLM_REMOTE", 					strtolower('ezmlmRemote'),					false);
pql_set_define("PQL_ATTR_EZMLM_REMOTE_HOST", 				strtolower('ezmlmRemoteHost'),				false);
pql_set_define("PQL_ATTR_EZMLM_REMOTE_USER", 				strtolower('ezmlmRemoteUser'),				false);
pql_set_define("PQL_ATTR_EZMLM_REMOTE_RETREIVE_SCRIPT", 	strtolower('ezmlmRemoteRetreiveScript'),	false);
// }}}

// {{{ Attributes used by the Samba config
pql_set_define("PQL_ATTR_SAMBA_USE",						strtolower('useSamba'),						false);
pql_set_define("PQL_ATTR_SAMBASID",							strtolower('sambaSID'),						false);
pql_set_define("PQL_ATTR_SAMBAPROFILEPATH",					strtolower('sambaProfilePath'),				false);
pql_set_define("PQL_ATTR_SAMBAHOMEDRIVE",					strtolower('sambaHomeDrive'),				false);
pql_set_define("PQL_ATTR_SAMBAHOMEPATH",					strtolower('sambaHomePath'),				false);
pql_set_define("PQL_ATTR_SAMBADOMAINNAME",					strtolower('sambaDomainName'),				false);
pql_set_define("PQL_ATTR_SAMBALOGONSCRIPT",					strtolower('sambaLogonScript'),				false);
pql_set_define("PQL_ATTR_SAMBAUSERWORKSTATIONS",			strtolower('sambaUserWorkstations'),		false);
pql_set_define("PQL_ATTR_SAMBALMPASSWORD",					strtolower('sambaLMPassword'),				false);
pql_set_define("PQL_ATTR_SAMBANTPASSWORD",					strtolower('sambaNTPassword'),				false);
pql_set_define("PQL_ATTR_SAMBAACCTFLAGS",					strtolower('sambaAcctFlags'),				false);
pql_set_define("PQL_ATTR_SAMBAACCTFLAGS",					strtolower('sambaAcctFlags'),				false);
pql_set_define("PQL_ATTR_SAMBAPWDMUSTCHANGE",				strtolower('sambaPwdMustChange'),			false);
pql_set_define("PQL_ATTR_SAMBAPWDLASTSET",					strtolower('sambaPwdLastSet'),				false);
pql_set_define("PQL_ATTR_SAMBAOBJECTCLASS",					strtolower('sambaSamAccount'),				false);
// }}}

// {{{ Attributes used by the Sudo config
pql_set_define("PQL_ATTR_SUDO_USE",							strtolower('useSudo'),						false);
pql_set_define("PQL_ATTR_SUDOUSER",							strtolower('sudoUser'),						false);
pql_set_define("PQL_ATTR_SUDORUNAS",						strtolower('sudoRunAs'),					false);
pql_set_define("PQL_ATTR_SUDOHOST",							strtolower('sudoHost'),						false);
pql_set_define("PQL_ATTR_SUDOPTION",						strtolower('sudoOption'),					false);
pql_set_define("PQL_ATTR_SUDOCOMMAND",						strtolower('sudoCommand'),					false);
pql_set_define("PQL_ATTR_SUDOOBJECTCLASS",					strtolower('sudoRole'),						false);
// }}}

// {{{ Attributes used by the HostACL config
pql_set_define("PQL_ATTR_HOSTACL_USE",						strtolower('useHostACL'),					false);
pql_set_define("PQL_ATTR_ACI_USE",							strtolower('useACI'),						false);
// }}}

// {{{ Attributes used by the Strong Password Enforcement
pql_set_define("PQL_ATTR_STRONGPASSWD_USE",					strtolower('useStrongPasswd'),				false);
// }}}

// {{{ Attributes used by the Automount config
pql_set_define("PQL_ATTR_AUTOMOUNT_USE",					strtolower('useAutoMount'),					false);
pql_set_define("PQL_ATTR_AUTOMOUNT_INFO",					strtolower('autoMountInformation'),			false);
// }}}

// {{{ Attributes used by the RADIUS config
pql_set_define("PQL_ATTR_RADIUS_USE",						strtolower('useRadius'),					false);

// Used by the RADIUS profile manager
// NOTE: Don't forget to update $PQL_RADIUS[] below!

// Check items
pql_set_define("PQL_ATTR_RADIUS_AUTH_TYPE",					strtolower('radiusAuthType'),				false); // checkItem Auth-Type
pql_set_define("PQL_ATTR_RADIUS_CALLED_STATION_ID",			strtolower('radiusCalledStationId'),		false); // checkItem Called-Station-Id
pql_set_define("PQL_ATTR_RADIUS_CALLING_STATION_ID",		strtolower('radiusCallingStationId'),		false); // checkItem Calling-Station-Id
pql_set_define("PQL_ATTR_RADIUS_SIMULT_USE",				strtolower('radiusSimultaneousUse'),		false); // checkItem Simultaneous-Use
pql_set_define("PQL_ATTR_RADIUS_EXPIRATION",				strtolower('radiusExpiration'),				false); // checkItem Expiration
pql_set_define("PQL_ATTR_RADIUS_CHECK_ITEM",				strtolower('radiusCheckItem'),				false); // checkItem $GENERIC$

// Reply items
pql_set_define("PQL_ATTR_RADIUS_CALLBACK_ID",				strtolower('radiusCallbackId'),				false); // replyItem Callback-Id
pql_set_define("PQL_ATTR_RADIUS_CALLBACK_NR",				strtolower('radiusCallbackNumber'),			false); // replyItem Callback-Number
pql_set_define("PQL_ATTR_RADIUS_CLASS",						strtolower('radiusClass'),					false); // replyItem Class
pql_set_define("PQL_ATTR_RADIUS_FILTER_ID",					strtolower('radiusFilterId'),				false); // replyItem Filter-Id
pql_set_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_LINK",		strtolower('radiusFramedAppleTalkLink'),	false); // replyItem Framed-AppleTalk-Link
pql_set_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_NETW",		strtolower('radiusFramedAppleTalkNetwork'),	false); // replyItem Framed-AppleTalk-Network
pql_set_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_ZONE",		strtolower('radiusFramedAppleTalkZone'),	false); // replyItem Framed-AppleTalk-Zone
pql_set_define("PQL_ATTR_RADIUS_FRAMED_COMPRESSION",		strtolower('radiusFramedCompression'),		false); // replyItem Framed-Compression
pql_set_define("PQL_ATTR_RADIUS_FRAMED_IP_ADDR",			strtolower('radiusFramedIPAddress'),		false); // replyItem Framed-IP-Address
pql_set_define("PQL_ATTR_RADIUS_FRAMED_IP_NET",				strtolower('radiusFramedIPNetmask'),		false); // replyItem Framed-IP-Netmask
pql_set_define("PQL_ATTR_RADIUS_FRAMED_IPX_NET",			strtolower('radiusFramedIPXNetwork'),		false); // replyItem Framed-IPX-Network
pql_set_define("PQL_ATTR_RADIUS_FRAMED_MTU",				strtolower('radiusFramedMTU'),				false); // replyItem Framed-MTU
pql_set_define("PQL_ATTR_RADIUS_FRAMED_PROTO",				strtolower('radiusFramedProtocol'),			false); // replyItem Framed-Protocol
pql_set_define("PQL_ATTR_RADIUS_FRAMED_ROUTE",				strtolower('radiusFramedRoute'),			false); // replyItem Framed-Route
pql_set_define("PQL_ATTR_RADIUS_FRAMED_ROUTING",			strtolower('radiusFramedRouting'),			false); // replyItem Framed-Routing
pql_set_define("PQL_ATTR_RADIUS_IDLE_TIMEOUT",				strtolower('radiusIdleTimeout'),			false); // replyItem Idle-Timeout
pql_set_define("PQL_ATTR_RADIUS_LOGIN_IP_HOST",				strtolower('radiusLoginIPHost'),			false); // replyItem Login-IP-Host
pql_set_define("PQL_ATTR_RADIUS_LOGIN_LAT_GROUP",			strtolower('radiusLoginLATGroup'),			false); // replyItem Login-LAT-Group
pql_set_define("PQL_ATTR_RADIUS_LOGIN_LAT_NODE",			strtolower('radiusLoginLATNode'),			false); // replyItem Login-LAT-Node
pql_set_define("PQL_ATTR_RADIUS_LOGIN_LAT_PORT",			strtolower('radiusLoginLATPort'),			false); // replyItem Login-LAT-Port
pql_set_define("PQL_ATTR_RADIUS_LOGIN_LAT_SERVICE",			strtolower('radiusLoginLATService'),		false); // replyItem Login-LAT-Service
pql_set_define("PQL_ATTR_RADIUS_LOGIN_SERVICE",				strtolower('radiusLoginService'),			false); // replyItem Login-Service
pql_set_define("PQL_ATTR_RADIUS_LOGIN_TCP_PORT",			strtolower('radiusLoginTCPPort'),			false); // replyItem Login-TCP-Port
pql_set_define("PQL_ATTR_RADIUS_PORT_LIMIT",				strtolower('radiusPortLimit'),				false); // replyItem Port-Limit
pql_set_define("PQL_ATTR_RADIUS_REPLY_MESSAGE",				strtolower('radiusReplyMessage'),			false); // replyItem Reply-Message
pql_set_define("PQL_ATTR_RADIUS_SERVICE_TYPE",				strtolower('radiusServiceType'),			false); // replyItem Service-Type
pql_set_define("PQL_ATTR_RADIUS_SESSION_TIMEOUT",			strtolower('radiusSessionTimeout'),			false); // replyItem Session-Timeout
pql_set_define("PQL_ATTR_RADIUS_TERMINATE_ACTION",			strtolower('radiusTerminationAction'),		false); // replyItem Termination-Action
pql_set_define("PQL_ATTR_RADIUS_REPLY_ITEM",				strtolower('radiusReplyItem'),				false); // replyItem $GENERIC$

// These don't have a RADIUS dictionary attributes
pql_set_define("PQL_ATTR_RADIUS_ARAP_FEATURES",				strtolower('radiusArapFeatures'),			false); // ARAP-Features
pql_set_define("PQL_ATTR_RADIUS_ARAP_SECURITY",				strtolower('radiusArapSecurity'),			false); // ARAP-Security
pql_set_define("PQL_ATTR_RADIUS_ARAP_ZONE_ACCESS",			strtolower('radiusArapZoneAccess'),			false); // ARAP-Zone-Access
pql_set_define("PQL_ATTR_RADIUS_CLIENT_IP",					strtolower('radiusClientIPAddress'),		false); // Client-IP-Address
pql_set_define("PQL_ATTR_RADIUS_GROUP_NAME",				strtolower('radiusGroupName'),				false); // Group-Name
pql_set_define("PQL_ATTR_RADIUS_HINT",						strtolower('radiusHint'),					false); // Hint
pql_set_define("PQL_ATTR_RADIUS_HUNTGROUP_NAME",			strtolower('radiusHuntgroupName'),			false); // Huntgroup-Name
pql_set_define("PQL_ATTR_RADIUS_LOGIN_TIME",				strtolower('radiusLoginTime'),				false); // Login-Time
pql_set_define("PQL_ATTR_RADIUS_PASSWORD_RETRY",			strtolower('radiusPasswordRetry'),			false); // Password-Retry
pql_set_define("PQL_ATTR_RADIUS_PROFILE_DN",				strtolower('radiusProfileDn'),				false); // Regular-Profile
pql_set_define("PQL_ATTR_RADIUS_PROMPT",					strtolower('radiusPrompt'),					false); // Prompt
pql_set_define("PQL_ATTR_RADIUS_PROXY2REALM",				strtolower('radiusProxyToRealm'),			false); // Proxy-To-Realm
pql_set_define("PQL_ATTR_RADIUS_REPLICATE2REALM",			strtolower('radiusReplicateToRealm'),		false); // Deprecated in FreeRADIUS v1.0.0
pql_set_define("PQL_ATTR_RADIUS_REALM",						strtolower('radiusRealm'),					false); // Realm
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_ASSIGN_ID",			strtolower('radiusTunnelAssignmentId'),		false); // Tunnel-Assignment-ID
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_MEDIUM_TYPE",		strtolower('radiusTunnelMediumType'),		false); // Tunnel-Medium-Type
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_PASSWORD",			strtolower('radiusTunnelPassword'),			false); // Tunnel-Password
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_PREFS",				strtolower('radiusTunnelPreference'),		false); // Tunnel-Preference
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_PRIVATE_GROUP_ID",	strtolower('radiusTunnelPrivateGroupId'),	false); // Tunnel-Private-Group-ID
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_SERVER_ENDPOINT",	strtolower('radiusTunnelServerEndpoint'),	false); // Tunnel-Server-Endpoint
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_CLIENT_ENDPOINT",	strtolower('radiusTunnelClientEndpoint'),	false); // radiusTunnelClientEndpoint
pql_set_define("PQL_ATTR_RADIUS_TUNNEL_TYPE",				strtolower('radiusTunnelType'),				false); // Tunnel-Type
pql_set_define("PQL_ATTR_RADIUS_VSA",						strtolower('radiusVSA'),					false);
pql_set_define("PQL_ATTR_RADIUS_USER_CATEGORY",				strtolower('radiusUserCategory'),			false);
pql_set_define("PQL_ATTR_RADIUS_STRIP_USER_NAME",			strtolower('radiusStripUserName'),			false); // Strip-User-Name
pql_set_define("PQL_ATTR_RADIUS_DIALUP_ACCESS",				strtolower('dialupAccess'),					false); // Dialup-Access
// }}}

// {{{ Attributes used by the Password Policy config
// Global password policy attributes
pql_set_define("PQL_ATTR_PPOLICY_NAME",						strtolower('passwordPolicyName'),			false);
pql_set_define("PQL_ATTR_PPOLICY_DESC",						strtolower('passwordPolicyDesc'),			false);
pql_set_define("PQL_ATTR_PPOLICY_ATTR",						strtolower('pwdAttribute'),					false);
pql_set_define("PQL_ATTR_PPOLICY_MINAGE",					strtolower('pwdMinAge'),					false);
pql_set_define("PQL_ATTR_PPOLICY_MAXAGE",					strtolower('pwdMaxAge'),					false);
pql_set_define("PQL_ATTR_PPOLICY_INHIST",					strtolower('pwdInHistory'),					false);
pql_set_define("PQL_ATTR_PPOLICY_CHKQUAL",					strtolower('pwdCheckQuality'),				false);
pql_set_define("PQL_ATTR_PPOLICY_MINLEN",					strtolower('pwdMinLength'),					false);
pql_set_define("PQL_ATTR_PPOLICY_EXPIRE_WARNING",			strtolower('pwdExpireWarning'),				false);
pql_set_define("PQL_ATTR_PPOLICY_GRACEAUTHLIMIT",			strtolower('pwdGraceAuthnLimit'),			false);
pql_set_define("PQL_ATTR_PPOLICY_LOCKOUT",					strtolower('pwdLockout'),					false);
pql_set_define("PQL_ATTR_PPOLICY_LOCKOUT_DURATION",			strtolower('pwdLockoutDuration'),			false);
pql_set_define("PQL_ATTR_PPOLICY_MAXFAILURE",				strtolower('pwdMaxFailure'),				false);
pql_set_define("PQL_ATTR_PPOLICY_FAILURE_COUNT_INTERVAL",	strtolower('pwdFailureCountInterval'),		false);
pql_set_define("PQL_ATTR_PPOLICY_MUST_CHANGE",				strtolower('pwdMustChange'),				false);
pql_set_define("PQL_ATTR_PPOLICY_ALLOW_USER_CHANGE",		strtolower('pwdAllowUserChange'),			false);
pql_set_define("PQL_ATTR_PPOLICY_SAFE_MODIFY",				strtolower('pwdSafeModify'),				false);
pql_set_define("PQL_ATTR_PPOLICY_CHECK_MODULE",				strtolower('pwdCheckModule'),				false);

// User password policy attributes
pql_set_define("PQL_ATTR_PPOLICY_ENTRY",					strtolower('pwdPolicySubentry'),			false);
pql_set_define("PQL_ATTR_PPOLICY_CHANGED",					strtolower('pwdChangedTime'),				false);
pql_set_define("PQL_ATTR_PPOLICY_LOCKED",					strtolower('pwdAccountLockedTime'),			false);
pql_set_define("PQL_ATTR_PPOLICY_FAILURE",					strtolower('pwdFailureTime'),				false);
pql_set_define("PQL_ATTR_PPOLICY_HISTORY",					strtolower('pwdHistory'),					false);
pql_set_define("PQL_ATTR_PPOLICY_GRACEUSE",					strtolower('pwdGraceUseTime'),				false);
pql_set_define("PQL_ATTR_PPOLICY_RESET",					strtolower('pwdReset'),						false);
// }}}

// {{{ Attributes used by the DHCP3 config
// phpQLAdmin attributes
pql_set_define("PQL_ATTR_DHCP3_USE",						strtolower("useDHCP3"),						false);

// DHCP3 schema attributes
pql_set_define("PQL_ATTR_DHCP3_PRIMARYDN",					strtolower("dhcpPrimaryDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_SECONDARYDN",				strtolower("dhcpSecondaryDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_OPTIONSDN",					strtolower("dhcpOptionsDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_HOSTDN",						strtolower("dhcpHostDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_POOLDN",						strtolower("dhcpPoolDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_GROUPDN",					strtolower("dhcpGroupDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_SUBNETDN",					strtolower("dhcpSubnetDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_LEASEDN",					strtolower("dhcpLeaseDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_LEASESDN",					strtolower("dhcpLeasesDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_CLASSESDN",					strtolower("dhcpClassesDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_SUBCLASSESDN",				strtolower("dhcpSubclassesDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_SHAREDNETWORKDN",			strtolower("dhcpSharedNetworkDN"),			false);
pql_set_define("PQL_ATTR_DHCP3_SERVICEDN",					strtolower("dhcpServiceDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_LOCATORDN",					strtolower("dhcpLocatorDN"),				false);
pql_set_define("PQL_ATTR_DHCP3_KEYDN",						strtolower("dhcpKeyDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_ZONEDN",						strtolower("dhcpZoneDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_SERVERDN",					strtolower("dhcpServerDN"),					false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_PEERDN",			strtolower("dhcpFailOverPeerDN"),			false);

pql_set_define("PQL_ATTR_DHCP3_STATEMENTS",					strtolower("dhcpStatements"),				false);
pql_set_define("PQL_ATTR_DHCP3_RANGE",						strtolower("dhcpRange"),					false);
pql_set_define("PQL_ATTR_DHCP3_PERMIT_LIST",				strtolower("dhcpPermitList"),				false);
pql_set_define("PQL_ATTR_DHCP3_NETMASK",					strtolower("dhcpNetMask"),					false);
pql_set_define("PQL_ATTR_DHCP3_OPTION",						strtolower("dhcpOption"),					false);
pql_set_define("PQL_ATTR_DHCP3_CLASS_DATA",					strtolower("dhcpClassData"),				false);
pql_set_define("PQL_ATTR_DHCP3_VERSION",					strtolower("dhcpVersion"),					false);
pql_set_define("PQL_ATTR_DHCP3_IMPLEMENTATION",				strtolower("dhcpImplementation"),			false);
pql_set_define("PQL_ATTR_DHCP3_ADDRESS_STATE",				strtolower("dhcpAddressState"),				false);
pql_set_define("PQL_ATTR_DHCP3_EXPIRATION_TIME",			strtolower("dhcpExpirationTime"),			false);
pql_set_define("PQL_ATTR_DHCP3_START_TIME_OF_STATE",		strtolower("dhcpStartTimeOfState"),			false);
pql_set_define("PQL_ATTR_DHCP3_LAST_TRANSACTION_TIME",		strtolower("dhcpLastTransactionTime"),		false);
pql_set_define("PQL_ATTR_DHCP3_BOOTP_FLAG",					strtolower("dhcpBootpFlag"),				false);
pql_set_define("PQL_ATTR_DHCP3_DOMAIN_NAME",				strtolower("dhcpDomainName"),				false);
pql_set_define("PQL_ATTR_DHCP3_DNS_STATUS",					strtolower("dhcpDnsStatus"),				false);
pql_set_define("PQL_ATTR_DHCP3_REQUESTED_HOSTNAME",			strtolower("dhcpRequestedHostName"),		false);
pql_set_define("PQL_ATTR_DHCP3_ASSIGNED_HOSTNAME",			strtolower("dhcpAssignedHostName"),			false);
pql_set_define("PQL_ATTR_DHCP3_RESERVED_FOR_CLIENT",		strtolower("dhcpReservedForClient"),		false);
pql_set_define("PQL_ATTR_DHCP3_ASSIGNED_TO_CLIENT",			strtolower("dhcpAssignedToClient"),			false);
pql_set_define("PQL_ATTR_DHCP3_RELAY_AGENT_INFO",			strtolower("dhcpRelayAgentInfo"),			false);
pql_set_define("PQL_ATTR_DHCP3_HW_ADDRESS",					strtolower("dhcpHWAddress"),				false);
pql_set_define("PQL_ATTR_DHCP3_HASH_BUCKET_ASSIGNMENT",		strtolower("dhcpHashBucketAssignment"),		false);
pql_set_define("PQL_ATTR_DHCP3_DELAYED_SERVICE_PARAMETER",	strtolower("dhcpDelayedServiceParameter"),	false);
pql_set_define("PQL_ATTR_DHCP3_MAX_CLIENT_LEAD_TIME",		strtolower("dhcpMaxClientLeadTime"),		false);
pql_set_define("PQL_ATTR_DHCP3_ERROR_LOG",					strtolower("dhcpErrorLog"),					false);
pql_set_define("PQL_ATTR_DHCP3_KEY_ALGORITHM",				strtolower("dhcpKeyAlgorithm"),				false);
pql_set_define("PQL_ATTR_DHCP3_KEY_SECRET",					strtolower("dhcpKeySecret"),				false);
pql_set_define("PQL_ATTR_DHCP3_DNS_ZONE_SERVER",			strtolower("dhcpDnsZoneServer"),			false);
pql_set_define("PQL_ATTR_DHCP3_COMMENTS",					strtolower("dhcpComments"),					false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_END_POINT_STATE",	strtolower("dhcpFailOverEndpointState"),	false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_PRIMARY_SERVER",	strtolower("dhcpFailOverPrimaryServer"),	false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_SECONDARY_SERVER",	strtolower("dhcpFailOverSecondaryServer"),	false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_PRIMARY_PORT",		strtolower("dhcpFailOverPrimaryPort"),		false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_SECONDARY_PORT",	strtolower("dhcpFailOverSecondaryPort"),	false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_RESPONSE_DELAY",	strtolower("dhcpFailOverResponseDelay"),	false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_UNACKED_UPDATES",	strtolower("dhcpFailOverUnackedUpdates"),	false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_SPLIT",			strtolower("dhcpFailOverSplit"),			false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_LOAD_BALANCETIME",	strtolower("dhcpFailOverLoadBalanceTime"),	false);

// DHCP3 schema object classes
pql_set_define("PQL_ATTR_DHCP3_SERVICE",					strtolower("dhcpService"),					false);
pql_set_define("PQL_ATTR_DHCP3_SHARED_NETWORK",				strtolower("dhcpSharedNetwork"),			false);
pql_set_define("PQL_ATTR_DHCP3_SUBNET",						strtolower("dhcpSubnet"),					false);
pql_set_define("PQL_ATTR_DHCP3_POOL",						strtolower("dhcpPool"),						false);
pql_set_define("PQL_ATTR_DHCP3_GROUP",						strtolower("dhcpGroup"),					false);
pql_set_define("PQL_ATTR_DHCP3_HOST",						strtolower("dhcpHost"),						false);
pql_set_define("PQL_ATTR_DHCP3_CLASS",						strtolower("dhcpClass"),					false);
pql_set_define("PQL_ATTR_DHCP3_SUBCLASS",					strtolower("dhcpSubClass"),					false);
pql_set_define("PQL_ATTR_DHCP3_OPTIONS",					strtolower("dhcpOptions"),					false);
pql_set_define("PQL_ATTR_DHCP3_LEASES",						strtolower("dhcpLeases"),					false);
pql_set_define("PQL_ATTR_DHCP3_LOG",						strtolower("dhcpLog"),						false);
pql_set_define("PQL_ATTR_DHCP3_SERVER",						strtolower("dhcpServer"),					false);
pql_set_define("PQL_ATTR_DHCP3_TSIGKEY",					strtolower("dhcpTSigKey"),					false);
pql_set_define("PQL_ATTR_DHCP3_DNSZONE",					strtolower("dhcpDnsZone"),					false);
pql_set_define("PQL_ATTR_DHCP3_FAIL_OVER_PEER",				strtolower("dhcpFailOverPeer"),				false);
pql_set_define("PQL_ATTR_DHCP3_LOCATOR",					strtolower("dhcpLocator"),					false);
// }}}

// {{{ Miscellaneous external attributes
pql_set_define("PQL_ATTR_SPAMASSASSIN", 					strtolower('spamAssassin'),					false);
pql_set_define("PQL_ATTR_PASSWD_CLEAR", 					strtolower('clearTextPassword'),			false);
pql_set_define("PQL_ATTR_DAYSUNTILEXPIRE",     				strtolower('daysUntilExpire'),				false);
pql_set_define("PQL_ATTR_PHOTO",							strtolower('jpegPhoto'),					false);
// }}}


// {{{ Attributes used by the cn=config branch
pql_set_define("PQL_ATTR_OLC_ALLOWS", 						strtolower('olcAllows'),					false);
pql_set_define("PQL_ATTR_OLC_ARGS_FILE", 					strtolower('olcArgsFile'),					false);
pql_set_define("PQL_ATTR_OLC_ATTRS_OPTS", 					strtolower('olcAttributeOptions'),			false);
pql_set_define("PQL_ATTR_OLC_AUTHZ_POLICY", 				strtolower('olcAuthzPolicy'),				false);
pql_set_define("PQL_ATTR_OLC_AUTHZ_REGEXP", 				strtolower('olcAuthzRegexp'),				false);
pql_set_define("PQL_ATTR_OLC_CONCURRENCY", 					strtolower('olcConcurrency'),				false);
pql_set_define("PQL_ATTR_OLC_CONFIG_DIR", 					strtolower('olcConfigDir'),					false);
pql_set_define("PQL_ATTR_OLC_CONFIG_FILE", 					strtolower('olcConfigFile'),				false);
pql_set_define("PQL_ATTR_OLC_CONN_MAX_PEND", 				strtolower('olcConnMaxPending'),			false);
pql_set_define("PQL_ATTR_OLC_CONN_MAX_PEND_AUTH", 			strtolower('olcConnMaxPendingAuth'),		false);
pql_set_define("PQL_ATTR_OLC_GENTLE_HUP", 					strtolower('olcGentleHUP'),					false);
pql_set_define("PQL_ATTR_OLC_IDLE_TIMEOUT", 				strtolower('olcIdleTimeout'),				false);
pql_set_define("PQL_ATTR_OLC_INDEX_INT_LEN", 				strtolower('olcIndexIntLen'),				false);
pql_set_define("PQL_ATTR_OLC_INDEX_SUBSTR_ANY_LEN", 		strtolower('olcIndexSubstrAnyLen'),			false);
pql_set_define("PQL_ATTR_OLC_INDEX_SUBSTR_ANY_STEP", 		strtolower('olcIndexSubstrAnyStep'),		false);
pql_set_define("PQL_ATTR_OLC_INDEX_SUBSTR_IF_MAX_LEN", 		strtolower('olcIndexSubstrIfMaxLen'),		false);
pql_set_define("PQL_ATTR_OLC_INDEX_SUBSTR_IF_MIN_LEN", 		strtolower('olcIndexSubstrIfMinLen'),		false);
pql_set_define("PQL_ATTR_OLC_LOCAL_SSF", 					strtolower('olcLocalSSF'),					false);
pql_set_define("PQL_ATTR_OLC_LOG_LEVEL", 					strtolower('olcLogLevel'),					false);
pql_set_define("PQL_ATTR_OLC_PID_FILE", 					strtolower('olcPidFile'),					false);
pql_set_define("PQL_ATTR_OLC_READ_ONLY", 					strtolower('olcReadOnly'),					false);
pql_set_define("PQL_ATTR_OLC_REVERSE_LOOKUP", 				strtolower('olcReverseLookup'),				false);
pql_set_define("PQL_ATTR_OLC_SASL_HOST", 					strtolower('olcSaslHost'),					false);
pql_set_define("PQL_ATTR_OLC_SASL_REALM", 					strtolower('olcSaslRealm'),					false);
pql_set_define("PQL_ATTR_OLC_SASL_SEC_PROPS", 				strtolower('olcSaslSecProps'),				false);
pql_set_define("PQL_ATTR_OLC_SIZE_LIMIT", 					strtolower('olcSizeLimit'),					false);
pql_set_define("PQL_ATTR_OLC_SOCK_BUFF_MAX_INCOMING", 		strtolower('olcSockbufMaxIncoming'),		false);
pql_set_define("PQL_ATTR_OLC_SOCK_BUFF_MAX_INCOMING_AUTH",	strtolower('olcSockbufMaxIncomingAuth'),	false);
pql_set_define("PQL_ATTR_OLC_TLS_CA_CERT_FILE", 			strtolower('olcTLSCACertificateFile'),		false);
pql_set_define("PQL_ATTR_OLC_TLS_CRL_CHECK", 				strtolower('olcTLSCRLCheck'),				false);
pql_set_define("PQL_ATTR_OLC_TLS_CERT_FILE", 				strtolower('olcTLSCertificateFile'),		false);
pql_set_define("PQL_ATTR_OLC_TLS_CERT_KEY_FILE", 			strtolower('olcTLSCertificateKeyFile'),		false);
pql_set_define("PQL_ATTR_OLC_TLS_VERIFY_CLIENT", 			strtolower('olcTLSVerifyClient'),			false);
pql_set_define("PQL_ATTR_OLC_THREADS", 						strtolower('olcThreads'),					false);
pql_set_define("PQL_ATTR_OLC_TIME_LIMIT", 					strtolower('olcTimeLimit'),					false);
pql_set_define("PQL_ATTR_OLC_TOOL_THREADS", 				strtolower('olcToolThreads'),				false);
// }}}

// {{{ Attributes used in branches _below_ the cn=config base branch
pql_set_define("PQL_ATTR_OLC_MODULE_PATH", 					strtolower('olcModulePath'),				false);
pql_set_define("PQL_ATTR_OLC_MODULE_LOAD", 					strtolower('olcModuleLoad'),				false);

pql_set_define("PQL_ATTR_OLC_DB", 							strtolower('olcDatabase'),					false);
pql_set_define("PQL_ATTR_OLC_DB_URI", 						strtolower('olcDbUri'),						false);
pql_set_define("PQL_ATTR_OLC_DB_DIRECTORY", 				strtolower('olcDbDirectory'),				false);
pql_set_define("PQL_ATTR_OLC_DB_CACHE_SIZE", 				strtolower('olcDbCacheSize'),				false);
pql_set_define("PQL_ATTR_OLC_DB_CACHE_FREE", 				strtolower('olcDbCacheFree'),				false);
pql_set_define("PQL_ATTR_OLC_DB_CHECK_POINT", 				strtolower('olcDbCheckPoint'),				false);
pql_set_define("PQL_ATTR_OLC_DB_CONFIG", 					strtolower('olcDbConfig'),					false);
pql_set_define("PQL_ATTR_OLC_DB_NO_SYNC", 					strtolower('olcDbNoSync'),					false);
pql_set_define("PQL_ATTR_OLC_DB_DIRTY_READ", 				strtolower('olcDbDirtyRead'),				false);
pql_set_define("PQL_ATTR_OLC_DB_IDL_CACHE_SIZE", 			strtolower('olcDbIDLcacheSize'),			false);
pql_set_define("PQL_ATTR_OLC_DB_SUFFIX", 					strtolower('olcSuffix'),					false);
pql_set_define("PQL_ATTR_OLC_DB_INDEX", 					strtolower('olcDbIndex'),					false);
pql_set_define("PQL_ATTR_OLC_DB_LINEAR_INDEX", 				strtolower('olcDbLinearIndex'),				false);
pql_set_define("PQL_ATTR_OLC_DB_MODE", 						strtolower('olcDbMode'),					false);
pql_set_define("PQL_ATTR_OLC_DB_SEARCH_STACK", 				strtolower('olcDbSearchStack'),				false);
pql_set_define("PQL_ATTR_OLC_DB_SHM_KEY", 					strtolower('olcDbShmKey'),					false);
pql_set_define("PQL_ATTR_OLC_DB_DN_CACHE_SIZE", 			strtolower('olcDbDNcacheSize'),				false);

pql_set_define("PQL_ATTR_OLC_ACCESS", 						strtolower('olcAccess'),					false);
pql_set_define("PQL_ATTR_OLC_ROOT_DN", 						strtolower('olcRootDN'),					false);
pql_set_define("PQL_ATTR_OLC_SCHEMA_DN", 					strtolower('olcSchemaDN'),					false);
pql_set_define("PQL_ATTR_OLC_DEFAULT_SEARCH_BASE", 			strtolower('olcDefaultSearchBase'),			false);
pql_set_define("PQL_ATTR_OLC_PASSWORD_HASH", 				strtolower('olcPasswordHash'),				false);

pql_set_define("PQL_ATTR_OLC_ADD_CONNTENT_ACL", 			strtolower('olcAddContentAcl'),				false);
pql_set_define("PQL_ATTR_OLC_LAST_MOD", 					strtolower('olcLastMod'),					false);
pql_set_define("PQL_ATTR_OLC_MAX_DEREF_DEPTH", 				strtolower('olcMaxDerefDepth'),				false);
pql_set_define("PQL_ATTR_OLC_MONITORING", 					strtolower('olcMonitoring'),				false);
pql_set_define("PQL_ATTR_OLC_OVERLAY", 						strtolower('olcOverlay'),					false);
pql_set_define("PQL_ATTR_OLC_REFINT_ATTRS", 				strtolower('olcRefintAttribute'),			false);
pql_set_define("PQL_ATTR_OLC_UNIQ_BASE", 					strtolower('olcUniqueBase'),				false);
pql_set_define("PQL_ATTR_OLC_UNIQ_ATTR", 					strtolower('olcUniqueAttribute'),			false);

pql_set_define("PQL_ATTR_OLC_PPOLICY_DEFAULT", 				strtolower('olcPPolicyDefault'),			false);
pql_set_define("PQL_ATTR_OLC_PPOLICY_HASH_CLEARTEXT", 		strtolower('olcPPolicyHashCleartext'),		false);
pql_set_define("PQL_ATTR_OLC_PPOLICY_USE_LOCKOUT", 			strtolower('olcPPolicyUseLockout'),			false);
pql_set_define("PQL_ATTR_OLC_VAL_SORT_ATTR", 				strtolower('olcValSortAttr'),				false);
pql_set_define("PQL_ATTR_OLC_AUDIT_LOG_FILE", 				strtolower('olcAuditLogFile'),				false);
pql_set_define("PQL_ATTR_OLC_ACCESSLOG_DB", 				strtolower('olcAccessLogDB'),				false);
pql_set_define("PQL_ATTR_OLC_ACCESSLOG_OPS", 				strtolower('olcAccessLogOps'),				false);
pql_set_define("PQL_ATTR_OLC_ACCESSLOG_PURGE", 				strtolower('olcAccessLogPurge'),			false);
pql_set_define("PQL_ATTR_OLC_ACCESSLOG_OLD", 				strtolower('olcAccessLogOld'),				false);

pql_set_define("PQL_ATTR_OLC_ATTRIBUTE_TYPES", 				strtolower('olcAttributeTypes'),			false);
pql_set_define("PQL_ATTR_OLC_OBJECT_CLASSES", 				strtolower('olcObjectClasses'),				false);

// }}}


// {{{ These are the internal phpQLAdmin attributes - GLOBAL configuration
// Toggles
pql_set_define("PQL_ATTR_CONTROL_USE", 						strtolower('useControls'),					false);
pql_set_define("PQL_ATTR_EZMLM_USE", 						strtolower('useEzmlm'),						false);
pql_set_define("PQL_ATTR_BIND9_USE", 						strtolower('useBind9'),						false);
pql_set_define("PQL_ATTR_ACI_USE", 							strtolower('useACI'),						false);
pql_set_define("PQL_ATTR_AUTO_RELOAD", 						strtolower('autoReload'),					false);
pql_set_define("PQL_ATTR_CHANGE_SERVER", 					strtolower('allowServerChange'),			false);
pql_set_define("PQL_ATTR_CREATE_MBOX", 						strtolower('useMBox'),						false);
pql_set_define("PQL_ATTR_DEBUG_ME", 						strtolower('runInDebugMode'),				false);

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
// }}}

// {{{ These are the internal phpQLAdmin attributes - BRANCH configuration
// Toggles
pql_set_define("PQL_ATTR_SHOW_USERS", 						strtolower('showUsers'),					false);
pql_set_define("PQL_ATTR_TESTMAIL_AUTOSEND", 				strtolower('autoSend'),						false);
pql_set_define("PQL_ATTR_ALLOW_ABSOLUTE_PATH", 				strtolower('allowAbsoluteMailPath'),		false);
pql_set_define("PQL_ATTR_ALLOW_ALL_CHARS",	 				strtolower('allowAllPasswordChars'),		false);
pql_set_define("PQL_ATTR_VERIFY_DELETE", 					strtolower('deleteVerification'),			false);
pql_set_define("PQL_ATTR_AUTOCREATE_MAILADDRESS",			strtolower('autoCreateMailAddress'),		false);
pql_set_define("PQL_ATTR_AUTOCREATE_USERNAME",				strtolower('autoCreateUserName'),			false);
pql_set_define("PQL_ATTR_AUTOCREATE_PASSWORD",				strtolower('autoCreatePassWord'),			false);
pql_set_define("PQL_ATTR_START_IN_MY_ACCOUNT",				strtolower('startinmyaccount'),				false);
pql_set_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME",	 		strtolower('autoAddHostName'),				false);
pql_set_define("PQL_ATTR_CONTROL_AUTOADDLOCALS", 			strtolower('autoAddLocals'),				false);
pql_set_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS", 		strtolower('autoAddRCPTHosts'),				false);

// Setup
pql_set_define("PQL_ATTR_SEEALSO",							strtolower('seeAlso'),						false);
pql_set_define("PQL_ATTR_BASEHOMEDIR",						strtolower('baseHomeDir'),					false);
pql_set_define("PQL_ATTR_BASEMAILDIR",						strtolower('baseMailDir'),					false);
pql_set_define("PQL_ATTR_BASEQUOTA",						strtolower('baseQuota'),					false);
pql_set_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME",			strtolower('defaultPasswordScheme'),		false);
pql_set_define("PQL_ATTR_DEFAULT_ACCOUNTTYPE",				strtolower('defaultAccountType'),			false);
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
pql_set_define("PQL_ATTR_SCRIPT_DELETE_USER", 				strtolower('externalScriptUserDelete'),		false);
pql_set_define("PQL_ATTR_SCRIPT_DELETE_DOMAIN", 			strtolower('externalScriptDomainDelete'),	false);
pql_set_define("PQL_ATTR_REFERENCE_USERS_WITH", 			strtolower('userReference'),				false);
pql_set_define("PQL_ATTR_REFERENCE_DOMAINS_WITH", 			strtolower('branchReference'),				false);
pql_set_define("PQL_ATTR_SUBTREE_USERS", 					strtolower('subTreeUsers'),					false);
pql_set_define("PQL_ATTR_SUBTREE_GROUPS", 					strtolower('subTreeGroups'),				false);
pql_set_define("PQL_ATTR_VAT_NUMBER",						strtolower('vatNumber'),					false);
pql_set_define("PQL_ATTR_LOCK_USERNAME",					strtolower('lockUserName'),					false);
pql_set_define("PQL_ATTR_LOCK_EMAILADDRESS",				strtolower('lockEmailAddress'),				false);
pql_set_define("PQL_ATTR_LOCK_DOMAINADDRESS",				strtolower('lockDomainAddress'),			false);
pql_set_define("PQL_ATTR_LOCK_PASSWORD",					strtolower('lockPassword'),					false);
pql_set_define("PQL_ATTR_LOCK_ACCOUNTTYPE",					strtolower('lockAccountType'),				false);
pql_set_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY",			strtolower("radiusConfigIdentity"),			false);
pql_set_define("PQL_ATTR_ADMINISTRATOR_WEBSRV",				strtolower("webSrvAdministrator"),			false);
pql_set_define("PQL_ATTR_ADMINISTRATOR_BIND9",				strtolower("dNSAdministrator"),				false);
pql_set_define("PQL_ATTR_USER_FILTER",						strtolower("userSearchFilter"),				false);
// }}}

// {{{ These are the internal phpQLAdmin attributes - USER configuration
// Toggles
pql_set_define("PQL_ATTR_START_ADVANCED",					strtolower("startWithAdvancedMode"),		false);
pql_set_define("PQL_ATTR_DISABLE_ADVANCED_MODE",			strtolower("disableAdvancedMode"),			false);
// }}}

// {{{ These are the internal phpQLAdmin attributes - TEMPLATE configuration
pql_set_define("PQL_ATTR_USER_TEMPLATE_NAME",				strtolower("userTemplateName"),				false);
pql_set_define("PQL_ATTR_USER_TEMPLATE_DESC_SHORT",			strtolower("userTemplateDescriptionShort"),	false);
pql_set_define("PQL_ATTR_USER_TEMPLATE_DESC_LONG",			strtolower("userTemplateDescriptionLong"),	false);
pql_set_define("PQL_ATTR_AUTOCREATE_UGIDNUMBER",			strtolower('autoCreateUGidNumber'),			false);
// }}}


// {{{ Map attribute to configuration define - GLOBAL
// Format:   Define to set => Attribute where value is located
// Location: In the base DN(s)
$PQL_ATTRIBUTE["GLOBAL"] = array("PQL_CONF_CONTROL_USE"					=> pql_get_define("PQL_ATTR_CONTROL_USE"),
								 "PQL_CONF_EZMLM_USE"					=> pql_get_define("PQL_ATTR_EZMLM_USE"),
								 "PQL_CONF_BIND9_USE"					=> pql_get_define("PQL_ATTR_BIND9_USE"),
								 "PQL_CONF_SIMSCAN_USE"					=> pql_get_define("PQL_ATTR_SIMSCAN_USE"),
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
								 "PQL_CONF_ADMINISTRATOR_WEBSRV"		=> pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"),
								 "PQL_CONF_VIRTUAL_BASEHOME"			=> pql_get_define("PQL_ATTR_VIRTUAL_BASEHOME"),
								 "PQL_CONF_ACI_USE"						=> pql_get_define("PQL_ATTR_ACI_USE"),
								 "PQL_CONF_HOSTACL_USE"					=> pql_get_define("PQL_ATTR_HOSTACL_USE"),
								 "PQL_CONF_SUDO_USE"					=> pql_get_define("PQL_ATTR_SUDO_USE"),
								 "PQL_CONF_STRONGPASSWD_USE"			=> pql_get_define("PQL_ATTR_STRONGPASSWD_USE"),
								 "PQL_CONF_AUTOMOUNT_USE"				=> pql_get_define("PQL_ATTR_AUTOMOUNT_USE"),
								 "PQL_CONF_RADIUS_USE"					=> pql_get_define("PQL_ATTR_RADIUS_USE"),
								 "PQL_CONF_SAMBA_USE"					=> pql_get_define("PQL_ATTR_SAMBA_USE"),
								 "PQL_CONF_CREATE_MBOX"					=> pql_get_define("PQL_ATTR_CREATE_MBOX"),
								 "PQL_CONF_DEBUG_ME"					=> pql_get_define("PQL_ATTR_DEBUG_ME"),
								 "PQL_CONF_EZMLM_REMOTE"				=> pql_get_define("PQL_ATTR_EZMLM_REMOTE"),
								 "PQL_CONF_EZMLM_REMOTE_HOST"			=> pql_get_define("PQL_ATTR_EZMLM_REMOTE_HOST"),
								 "PQL_CONF_EZMLM_REMOTE_USER"			=> pql_get_define("PQL_ATTR_EZMLM_REMOTE_USER"),
								 "PQL_CONF_EZMLM_REMOTE_RETREIVE_SCRIPT"=> pql_get_define("PQL_ATTR_EZMLM_REMOTE_RETREIVE_SCRIPT"),
								 "PQL_CONF_DHCP3_USE"					=> pql_get_define("PQL_ATTR_DHCP3_USE"));

// }}}

// {{{ Map attribute to configuration define - SUFFIX
// Format:   Define to set => Attribute where value is located
// Location: In the branch DN(s)
$PQL_ATTRIBUTE["SUFFIX"] = array("PQL_CONF_SHOW_USERS"					=> pql_get_define("PQL_ATTR_SHOW_USERS"),
								 "PQL_CONF_TESTMAIL_AUTOSEND"			=> pql_get_define("PQL_ATTR_TESTMAIL_AUTOSEND"),
								 "PQL_CONF_ALLOW_ABSOLUTE_PATH"			=> pql_get_define("PQL_ATTR_ALLOW_ABSOLUTE_PATH"),
								 "PQL_CONF_ALLOW_ALL_CHARS"				=> pql_get_define("PQL_ATTR_ALLOW_ALL_CHARS"),
								 "PQL_CONF_VERIFY_DELETE"				=> pql_get_define("PQL_ATTR_VERIFY_DELETE"),
								 "PQL_CONF_AUTOCREATE_MAILADDRESS"		=> pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
								 "PQL_CONF_AUTOCREATE_USERNAME"			=> pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
								 "PQL_CONF_AUTOCREATE_PASSWORD"			=> pql_get_define("PQL_ATTR_AUTOCREATE_PASSWORD"),
								 "PQL_CONF_START_IN_MY_ACCOUNT"			=> pql_get_define("PQL_ATTR_START_IN_MY_ACCOUNT"),
								 "PQL_CONF_CONTROL_AUTOADDHOSTNAME"		=> pql_get_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME"),
								 "PQL_CONF_CONTROL_AUTOADDLOCALS"		=> pql_get_define("PQL_ATTR_CONTROL_AUTOADDLOCALS"),
								 "PQL_CONF_CONTROL_AUTOADDRCPTHOSTS"	=> pql_get_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS"),
								 "PQL_CONF_ADMINISTRATOR"				=> pql_get_define("PQL_ATTR_ADMINISTRATOR"),
								 "PQL_CONF_ADMINISTRATOR_WEBSRV"		=> pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"),
								 "PQL_CONF_ADMINISTRATOR_BIND9"			=> pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"),
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
								 "PQL_CONF_SCRIPT_DELETE_USER"			=> pql_get_define("PQL_ATTR_SCRIPT_DELETE_USER"),
								 "PQL_CONF_SCRIPT_DELETE_DOMAIN"		=> pql_get_define("PQL_ATTR_SCRIPT_DELETE_DOMAIN"),
								 "PQL_CONF_REFERENCE_USERS_WITH"		=> pql_get_define("PQL_ATTR_REFERENCE_USERS_WITH"),
								 "PQL_CONF_REFERENCE_DOMAINS_WITH"		=> pql_get_define("PQL_ATTR_REFERENCE_DOMAINS_WITH"),
								 "PQL_CONF_SUBTREE_USERS"				=> pql_get_define("PQL_ATTR_SUBTREE_USERS"),
								 "PQL_CONF_SUBTREE_GROUPS"				=> pql_get_define("PQL_ATTR_SUBTREE_GROUPS"),
								 "PQL_CONF_EZMLM_USER"					=> pql_get_define("PQL_ATTR_EZMLM_USER"),
								 "PQL_CONF_VAT_NUMBER"					=> pql_get_define("PQL_ATTR_VAT_NUMBER"),
								 "PQL_CONF_SIMSCAN_SPAM"				=> pql_get_define("PQL_ATTR_SIMSCAN_SPAM"),
								 "PQL_CONF_SIMSCAN_CLAM"				=> pql_get_define("PQL_ATTR_SIMSCAN_CLAM"),
								 "PQL_CONF_SIMSCAN_TROPHIE"				=> pql_get_define("PQL_ATTR_SIMSCAN_TROPHIE"),
								 "PQL_CONF_SIMSCAN_SPAM_HITS"			=> pql_get_define("PQL_ATTR_SIMSCAN_SPAM_HITS"),
								 "PQL_CONF_SIMSCAN_ATTACHMENTS"			=> pql_get_define("PQL_ATTR_SIMSCAN_ATTACHMENTS"),
								 "PQL_CONF_RADIUS_CONFIG_IDENTITY"		=> pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY"),
								 "PQL_CONF_SAMBASID"					=> pql_get_define("PQL_ATTR_SAMBASID"),
								 "PQL_CONF_DAYSUNTILEXPIRE"				=> pql_get_define("PQL_ATTR_DAYSUNTILEXPIRE"),
								 "PQL_CONF_USER_FILTER"					=> pql_get_define("PQL_ATTR_USER_FILTER"));
// }}}

// {{{ Map attribute to configuration define - USER
// Format:   Define to set => Attribute where value is located
// Location: In the user DN(s)
$PQL_ATTRIBUTE["USER"] = array("PQL_CONF_START_ADVANCED"				=> pql_get_define("PQL_ATTR_START_ADVANCED"),
							   "PQL_CONF_DISABLE_ADVANCED_MODE"			=> pql_get_define("PQL_ATTR_DISABLE_ADVANCED_MODE"));
// }}}

// {{{ Map attribute to configuration define - RADIUS/checkItem
$PQL_RADIUS["check"] = array("PQL_ATTR_RADIUS_AUTH_TYPE",
							 "PQL_ATTR_RADIUS_CALLED_STATION_ID",
							 "PQL_ATTR_RADIUS_CALLING_STATION_ID",
							 "PQL_ATTR_RADIUS_SIMULT_USE",
							 "PQL_ATTR_RADIUS_EXPIRATION");
// }}}

// {{{ Map attribute to configuration define - RADIUS/replyItem
$PQL_RADIUS["reply"] = array("PQL_ATTR_RADIUS_CALLBACK_ID",
							 "PQL_ATTR_RADIUS_CALLBACK_NR",
							 "PQL_ATTR_RADIUS_CLASS",
							 "PQL_ATTR_RADIUS_FILTER_ID",
							 "PQL_ATTR_RADIUS_FRAMED_APPLETALK_LINK",
							 "PQL_ATTR_RADIUS_FRAMED_APPLETALK_NETW",
							 "PQL_ATTR_RADIUS_FRAMED_APPLETALK_ZONE",
							 "PQL_ATTR_RADIUS_FRAMED_COMPRESSION",
							 "PQL_ATTR_RADIUS_FRAMED_IP_ADDR",
							 "PQL_ATTR_RADIUS_FRAMED_IP_NET",
							 "PQL_ATTR_RADIUS_FRAMED_IPX_NET",
							 "PQL_ATTR_RADIUS_FRAMED_MTU",
							 "PQL_ATTR_RADIUS_FRAMED_PROTO",
							 "PQL_ATTR_RADIUS_FRAMED_ROUTE",
							 "PQL_ATTR_RADIUS_FRAMED_ROUTING",
							 "PQL_ATTR_RADIUS_IDLE_TIMEOUT",
							 "PQL_ATTR_RADIUS_LOGIN_IP_HOST",
							 "PQL_ATTR_RADIUS_LOGIN_LAT_GROUP",
							 "PQL_ATTR_RADIUS_LOGIN_LAT_NODE",
							 "PQL_ATTR_RADIUS_LOGIN_LAT_PORT",
							 "PQL_ATTR_RADIUS_LOGIN_LAT_SERVICE",
							 "PQL_ATTR_RADIUS_LOGIN_SERVICE",
							 "PQL_ATTR_RADIUS_LOGIN_TCP_PORT",
							 "PQL_ATTR_RADIUS_PORT_LIMIT",
							 "PQL_ATTR_RADIUS_REPLY_MESSAGE",
							 "PQL_ATTR_RADIUS_SERVICE_TYPE",
							 "PQL_ATTR_RADIUS_SESSION_TIMEOUT",
							 "PQL_ATTR_RADIUS_TERMINATE_ACTION",
							 "PQL_ATTR_RADIUS_REPLY_ITEM");
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
