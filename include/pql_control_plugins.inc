<?php
// ----------------------------
// pql_control_plugins.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control plugins)
//
// $Id: pql_control_plugins.inc,v 2.2 2003-08-15 08:06:17 turbo Exp $
//
// version history:
// $Log: pql_control_plugins.inc,v $
// Revision 2.2  2003-08-15 08:06:17  turbo
// This humungus change (diff is 221k!) is because of hardcoding attributes
// into the code. This isn't THAT big a'deal, but IF someone wants to use
// phpQLAdmin for something else than QmailLDAP/Controls (SendmailLDAP comes
// to mind), then THEY would have to do this fix. And the original idea with
// taking over phpQLAdmin was to remove _ALL_ hardcoded design and database
// structure. It is NOT up to the tool to describe the database. It is the
// database who should describe the tool!!
// - This change was accomplished by replacing EVERY SINGLE occurence of a
//   attribute reference in an array ($variable["locals"] for example) to
//   a 'define' made in include/config_attributes.inc (required from
//   include/pql_config.inc). Getting this value (the attribute name) is done
//   with the function pql_get_define().
// - To make this work, I had to move all the plugin stuff from include/pql_control.inc
//   to include/pql_plugins.inc AND make it general. Previosly it was 'controls'
//   oriented. This because {domain,user}_edit_attributes.php did a 'switch()'
//   against the attribute, and this is no longer possible (can't use
//   'case pql_get_define():').
// Unfortunatly, all this calling to pql_get_define() will make it (phpQLAdmin)
// a little slower (how much I don't know), but it must be worth it (see above
// on hardcoding).
// Closes bug 26.
//
// Revision 1.0  2001/05/21 09:52:17  mb
// update version info to 1.0
//
// Revision 0.12  2001/05/18 14:22:18  mb
// reorganize categories
//
// Revision 0.11  2001/05/03 11:01:02  mb
// added some new attributes
//
// Revision 0.10  2001/05/01 15:13:50  mb
// added all newly created plugins
//
// Revision 0.9  2001/04/25 11:54:20  mb
// initial add
//
//
// ----------------------------
// Note: There are a few " followed after PHP comment
// instructions like // in this code . They are
// here to prevent Quanta+ highlighting all following
// code as a string. A dirty workaround, I know :)
//

// ----------------------------
// plugin definition
// ----------------------------

// qmail-ldap specific options
pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPSERVER"),
					"attrib.control.ldapserver.inc",
					"q-l ldap core",
					array(pql_get_define("PQL_GLOB_ATTR_LDAPSERVER"),
						  pql_get_define("PQL_GLOB_ATTR_LDAPBASEDN"),
						  pql_get_define("PQL_GLOB_ATTR_LDAPLOGIN"),
						  pql_get_define("PQL_GLOB_ATTR_LDAPPASSWORD")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPREBIND"),
					"attrib.control.ldaprebind.inc",
					"q-l ldap core",
					pql_get_define("PQL_GLOB_ATTR_LDAPREBIND"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPCLUSTER"),
					"attrib.control.ldapcluster.inc",
					"q-l ldap core",
					pql_get_define("PQL_GLOB_ATTR_LDAPCLUSTER"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTQUOTA"),
					"attrib.control.ldapdefaultquota.inc",
					"q-l mailbox",
					pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTQUOTA"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPMESSAGESTORE"),
					"attrib.control.ldapmessagestore.inc",
					"q-l mailbox",
					pql_get_define("PQL_GLOB_ATTR_LDAPMESSAGESTORE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DIRMAKER"),
					"attrib.control.dirmaker.inc",
					"q-l mailbox",
					pql_get_define("PQL_GLOB_ATTR_DIRMAKER"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPLOCALDELIVERY"),
					"attrib.control.ldaplocaldelivery.inc",
					"q-l mailbox",
					pql_get_define("PQL_GLOB_ATTR_LDAPLOCALDELIVERY"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_QUOTAWARNING"),
					"attrib.control.quotawarning.inc",
					"q-l mailbox",
					pql_get_define("PQL_GLOB_ATTR_QUOTAWARNING"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTDOTMODE"),
					"attrib.control.ldapdefaultdotmode.inc",
					"q-l mailbox",
					pql_get_define("PQL_GLOB_ATTR_LDAPDEFAULTDOTMODE"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LDAPUID"),
					"attrib.control.ldapuid.inc",
					"q-l virtual user",
					array(pql_get_define("PQL_GLOB_ATTR_LDAPUID"),
						  pql_get_define("PQL_GLOB_ATTR_LDAPGID")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_CUSTOMBOUNCETEXT"),
					"attrib.control.custombouncetext.inc",
					"q-l bounce",
					pql_get_define("PQL_GLOB_ATTR_CUSTOMBOUNCETEXT"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_TARPITCOUNT"),
					"attrib.control.tarpitcount.inc",
					"q-l spam control",
					array(pql_get_define("PQL_GLOB_ATTR_TARPITCOUNT"),
						  pql_get_define("PQL_GLOB_ATTR_TARPITDELAY")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_MAXRCPTCOUNT"),
					"attrib.control.maxrcptcount.inc",
					"q-l spam control",
					pql_get_define("PQL_GLOB_ATTR_MAXRCPTCOUNT"));

// core qmail specific options
pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_BOUNCEFROM"),
					"attrib.control.bouncefrom.inc",
					"qmail-send",
					array(pql_get_define("PQL_GLOB_ATTR_BOUNCEFROM"),
						  pql_get_define("PQL_GLOB_ATTR_BOUNCEHOST")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_CONCURRENCYLOCAL"),
					"attrib.control.concurrencylocal.inc",
					"qmail-send",
					array(pql_get_define("PQL_GLOB_ATTR_CONCURRENCYLOCAL"),
						  pql_get_define("PQL_GLOB_ATTR_CONCURRENCYREMOTE")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCEHOST"),
					"attrib.control.doublebouncehost.inc",
					"qmail-send",
					array(pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCEHOST"),
						  pql_get_define("PQL_GLOB_ATTR_DOUBLEBOUNCETO")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DATABYTES"),
					"attrib.control.databytes.inc",
					"qmail-smtpd",
					pql_get_define("PQL_GLOB_ATTR_DATABYTES"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_QUEUELIFETIME"),
					"attrib.control.queuelifetime.inc",
					"qmail-send",
					pql_get_define("PQL_GLOB_ATTR_QUEUELIFETIME"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"),
					"attrib.control.smtproutes.inc",
					"qmail-remote",
					pql_get_define("PQL_GLOB_ATTR_SMTPROUTES"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LOCALS"),
					"attrib.control.locals.inc",
					pql_get_define("PQL_GLOB_ATTR_LOCALS"),
					pql_get_define("PQL_GLOB_ATTR_LOCALS"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"),
					"attrib.control.defaultdomain.inc",
					"qmail-inject",
					array(pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"),
						  pql_get_define("PQL_GLOB_ATTR_DEFAULTHOST")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_BADMAILFROM"),
					"attrib.control.badmailfrom.inc",
					"qmail-smtpd",
					pql_get_define("PQL_GLOB_ATTR_BADMAILFROM"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_ENVNOATHOST"),
					"attrib.control.envnoathost.inc",
					"qmail-send",
					pql_get_define("PQL_GLOB_ATTR_ENVNOATHOST"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_HELOHOST"),
					"attrib.control.helohost.inc",
					"qmail-remote",
					pql_get_define("PQL_GLOB_ATTR_HELOHOST"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_IDHOST"),
					"attrib.control.idhost.inc",
					"qmail-inject",
					pql_get_define("PQL_GLOB_ATTR_IDHOST"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_LOCALIPHOST"),
					"attrib.control.localiphost.inc",
					"qmail-smtpd",
					pql_get_define("PQL_GLOB_ATTR_LOCALIPHOST"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_PERCENTHACK"),
					"attrib.control.percenthack.inc",
					"qmail-send",
					pql_get_define("PQL_GLOB_ATTR_PERCENTHACK"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_TIMEOUTSMTPD"),
					"attrib.control.timeoutsmtpd.inc",
					"qmail-smtpd",
					pql_get_define("PQL_GLOB_ATTR_TIMEOUTSMTPD"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_TIMEOUTCONNECT"),
					"attrib.control.timeoutconnect.inc",
					"qmail-remote",
					array(pql_get_define("PQL_GLOB_ATTR_TIMEOUTCONNECT"),
						  pql_get_define("PQL_GLOB_ATTR_TIMEOUTREMOTE")));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_SMTPGREETING"),
					"attrib.control.smtpgreeting.inc",
					"qmail-smtpd",
					pql_get_define("PQL_GLOB_ATTR_SMTPGREETING"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_QMQPSERVERS"),
					"attrib.control.qmqpservers.inc",
					"qmail-qmqpc",
					pql_get_define("PQL_GLOB_ATTR_QMQPSERVERS"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_RELAYMAILFROM"),
					"attrib.control.relaymailfrom.inc",
					"q-l relay",
					pql_get_define("PQL_GLOB_ATTR_RELAYMAILFROM"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_BADRCPTTO"),
					"attrib.control.badrcptto.inc",
					"q-l spam control",
					pql_get_define("PQL_GLOB_ATTR_BADRCPTTO"));

pql_plugin_register(pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"),
					"attrib.control.rcpthosts.inc",
					pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"),
					pql_get_define("PQL_GLOB_ATTR_RCPTHOSTS"));

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
