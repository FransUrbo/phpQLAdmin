<?php
// ----------------------------
// pql_control_plugins.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control plugins)
//
// $Id: pql_control_plugins.inc,v 2.0 2002-12-13 14:34:58 turbo Exp $
//
// version history:
// $Log: pql_control_plugins.inc,v $
// Revision 2.0  2002-12-13 14:34:58  turbo
// Since I've forked, update the revision number...
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
pql_control_plugin_register("ldapserver", "attrib.control.ldapserver.inc", "q-l ldap core",
														array("ldapserver", "ldapbasedn", "ldaplogin", "ldappassword"));

pql_control_plugin_register("ldaprebind", "attrib.control.ldaprebind.inc", "q-l ldap core",
														"ldaprebind");

pql_control_plugin_register("ldapcluster", "attrib.control.ldapcluster.inc", "q-l ldap core",
														"ldapcluster");

pql_control_plugin_register("ldapdefaultquota", "attrib.control.ldapdefaultquota.inc", "q-l mailbox",
														"ldapdefaultquota");

pql_control_plugin_register("ldapmessagestore", "attrib.control.ldapmessagestore.inc", "q-l mailbox",
														"ldapmessagestore");

pql_control_plugin_register("dirmaker", "attrib.control.dirmaker.inc", "q-l mailbox",
														"dirmaker");

pql_control_plugin_register("ldaplocaldelivery", "attrib.control.ldaplocaldelivery.inc", "q-l mailbox",
														"ldaplocaldelivery");

pql_control_plugin_register("quotawarning", "attrib.control.quotawarning.inc", "q-l mailbox",
														"quotawarning");

pql_control_plugin_register("ldapdefaultdotmode", "attrib.control.ldapdefaultdotmode.inc", "q-l mailbox",
														"ldapdefaultdotmode");

pql_control_plugin_register("ldapuid", "attrib.control.ldapuid.inc", "q-l virtual user",
														array("ldapuid", "ldapgid"));

pql_control_plugin_register("custombouncetext", "attrib.control.custombouncetext.inc", "q-l bounce",
														"custombouncetext");

pql_control_plugin_register("tarpitcount", "attrib.control.tarpitcount.inc", "q-l spam control",
														array("tarpitcount", "tarpitdelay"));

pql_control_plugin_register("maxrcptcount", "attrib.control.maxrcptcount.inc", "q-l spam control",
														"maxrcptcount");

// core qmail specific options
pql_control_plugin_register("bouncefrom", "attrib.control.bouncefrom.inc", "qmail-send",
														array("bouncefrom", "bouncehost"));

pql_control_plugin_register("concurrencylocal", "attrib.control.concurrencylocal.inc", "qmail-send",
														array("concurrencylocal", "concurrencyremote"));

pql_control_plugin_register("doublebouncehost", "attrib.control.doublebouncehost.inc", "qmail-send",
														array("doublebouncehost", "doublebounceto"));

pql_control_plugin_register("databytes", "attrib.control.databytes.inc", "qmail-smtpd",
														"databytes");

pql_control_plugin_register("queuelifetime", "attrib.control.queuelifetime.inc", "qmail-send",
														"queuelifetime");

pql_control_plugin_register("smtproutes", "attrib.control.smtproutes.inc", "qmail-remote",
														"smtproutes");

pql_control_plugin_register("locals", "attrib.control.locals.inc", "locals",
														"locals");

pql_control_plugin_register("defaultdomain", "attrib.control.defaultdomain.inc", "qmail-inject",
														array("defaultdomain", "defaulthost"));

pql_control_plugin_register("badmailfrom", "attrib.control.badmailfrom.inc", "qmail-smtpd",
														"badmailfrom");

pql_control_plugin_register("envnoathost", "attrib.control.envnoathost.inc", "qmail-send",
														"envnoathost");

pql_control_plugin_register("helohost", "attrib.control.helohost.inc", "qmail-remote",
														"helohost");

pql_control_plugin_register("idhost", "attrib.control.idhost.inc", "qmail-inject",
														"idhost");

pql_control_plugin_register("localiphost", "attrib.control.localiphost.inc", "qmail-smtpd",
														"localiphost");

pql_control_plugin_register("percenthack", "attrib.control.percenthack.inc", "qmail-send",
														"percenthack");

pql_control_plugin_register("timeoutsmtpd", "attrib.control.timeoutsmtpd.inc", "qmail-smtpd",
														"timeoutsmtpd");

pql_control_plugin_register("timeoutconnect", "attrib.control.timeoutconnect.inc", "qmail-remote",
														array("timeoutconnect", "timeoutremote"));

pql_control_plugin_register("smtpgreeting", "attrib.control.smtpgreeting.inc", "qmail-smtpd",
														"smtpgreeting");

pql_control_plugin_register("qmqpservers", "attrib.control.qmqpservers.inc", "qmail-qmqpc",
														"qmqpservers");

pql_control_plugin_register("relaymailfrom", "attrib.control.relaymailfrom.inc", "q-l relay",
														"relaymailfrom");

pql_control_plugin_register("badrcptto", "attrib.control.badrcptto.inc", "q-l spam control",
														"badrcptto");

pql_control_plugin_register("rcpthosts", "attrib.control.rcpthosts.inc", "rcpthosts",
														"rcpthosts");


// ---------------------------------------------
// list all registred attributes and their files
// ---------------------------------------------
//
// uncomment this piece of code to view which
// plugins are registred and to which file
// they belong.
//

/*
$i = 0;

foreach($_pql_control_plugins->attribs as $key => $value){
	echo "$key --> attrib.control.$value.inc <br>";
	$i++;
}

echo "Attribute count: $i<br><br>";
*/

?>