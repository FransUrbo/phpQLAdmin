<?php
// ----------------------------
// pql_control_plugins.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control plugins)
//
// $Id: pql_control_plugins.inc,v 2.8 2005-04-17 09:38:56 turbo Exp $
//

// ----------------------------
// plugin definition
// ----------------------------

// qmail-ldap specific options
pql_plugin_register(pql_get_define("PQL_ATTR_LDAPSERVER"),
					"attrib.control.ldapserver.inc",
					"q-l ldap core",
					array(pql_get_define("PQL_ATTR_LDAPSERVER"),
						  pql_get_define("PQL_ATTR_LDAPBASEDN"),
						  pql_get_define("PQL_ATTR_LDAPLOGIN"),
						  pql_get_define("PQL_ATTR_LDAPPASSWORD"),
						  pql_get_define("PQL_ATTR_GROUP_LOGIN"),
						  pql_get_define("PQL_ATTR_GROUP_PASSWD")));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPREBIND"),
					"attrib.control.ldaprebind.inc",
					"q-l ldap core",
					pql_get_define("PQL_ATTR_LDAPREBIND"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPCLUSTER"),
					"attrib.control.ldapcluster.inc",
					"q-l ldap cluster",
					pql_get_define("PQL_ATTR_LDAPCLUSTER"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"),
					"attrib.control.ldapdefaultquota.inc",
					"q-l mailbox",
					pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"),
					"attrib.control.ldapmessagestore.inc",
					"q-l mailbox",
					pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"));

pql_plugin_register(pql_get_define("PQL_ATTR_DIRMAKER"),
					"attrib.control.dirmaker.inc",
					"q-l mailbox",
					pql_get_define("PQL_ATTR_DIRMAKER"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY"),
					"attrib.control.ldaplocaldelivery.inc",
					"q-l mailbox",
					pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUOTA_WARNING"),
					"attrib.control.quotawarning.inc",
					"q-l mailbox",
					pql_get_define("PQL_ATTR_QUOTA_WARNING"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"),
					"attrib.control.ldapdefaultdotmode.inc",
					"q-l mailbox",
					pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPUID"),
					"attrib.control.ldapuid.inc",
					"q-l virtual user",
					array(pql_get_define("PQL_ATTR_LDAPUID"),
						  pql_get_define("PQL_ATTR_LDAPGID")));

pql_plugin_register(pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"),
					"attrib.control.custombouncetext.inc",
					"q-l bounce",
					pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"));

pql_plugin_register(pql_get_define("PQL_ATTR_TARPITCOUNT"),
					"attrib.control.tarpitcount.inc",
					"q-l spam control",
					array(pql_get_define("PQL_ATTR_TARPITCOUNT"),
						  pql_get_define("PQL_ATTR_TARPITDELAY")));

pql_plugin_register(pql_get_define("PQL_ATTR_MAXRCPTCOUNT"),
					"attrib.control.maxrcptcount.inc",
					"q-l spam control",
					pql_get_define("PQL_ATTR_MAXRCPTCOUNT"));

// core qmail specific options
pql_plugin_register(pql_get_define("PQL_ATTR_BOUNCEFROM"),
					"attrib.control.bouncefrom.inc",
					"q-l bounce",
					array(pql_get_define("PQL_ATTR_BOUNCEFROM"),
						  pql_get_define("PQL_ATTR_BOUNCEHOST")));

pql_plugin_register(pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
					"attrib.control.concurrencylocal.inc",
					"qmail-send",
					array(pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
						  pql_get_define("PQL_ATTR_CONCURRENCYREMOTE")));

pql_plugin_register(pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
					"attrib.control.doublebouncehost.inc",
					"q-l bounce",
					array(pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
						  pql_get_define("PQL_ATTR_DOUBLEBOUNCETO")));

pql_plugin_register(pql_get_define("PQL_ATTR_DATABYTES"),
					"attrib.control.databytes.inc",
					"qmail-smtpd",
					pql_get_define("PQL_ATTR_DATABYTES"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUEUELIFETIME"),
					"attrib.control.queuelifetime.inc",
					"qmail-send",
					pql_get_define("PQL_ATTR_QUEUELIFETIME"));

pql_plugin_register(pql_get_define("PQL_ATTR_SMTPROUTES"),
					"attrib.control.smtproutes.inc",
					"qmail-remote",
					pql_get_define("PQL_ATTR_SMTPROUTES"));

pql_plugin_register(pql_get_define("PQL_ATTR_LOCALS"),
					"attrib.control.locals.inc",
					pql_get_define("PQL_ATTR_LOCALS"),
					pql_get_define("PQL_ATTR_LOCALS"));

pql_plugin_register(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
					"attrib.control.defaultdomain.inc",
					"qmail-inject",
					array(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
						  pql_get_define("PQL_ATTR_DEFAULTHOST")));

pql_plugin_register(pql_get_define("PQL_ATTR_BADMAILFROM"),
					"attrib.control.badmailfrom.inc",
					"qmail-smtpd",
					pql_get_define("PQL_ATTR_BADMAILFROM"));

pql_plugin_register(pql_get_define("PQL_ATTR_ENVNOATHOST"),
					"attrib.control.envnoathost.inc",
					"qmail-send",
					pql_get_define("PQL_ATTR_ENVNOATHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_HELOHOST"),
					"attrib.control.helohost.inc",
					"qmail-remote",
					pql_get_define("PQL_ATTR_HELOHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_IDHOST"),
					"attrib.control.idhost.inc",
					"qmail-inject",
					pql_get_define("PQL_ATTR_IDHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_LOCALIPHOST"),
					"attrib.control.localiphost.inc",
					"qmail-smtpd",
					pql_get_define("PQL_ATTR_LOCALIPHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_PERCENTHACK"),
					"attrib.control.percenthack.inc",
					"qmail-send",
					pql_get_define("PQL_ATTR_PERCENTHACK"));

pql_plugin_register(pql_get_define("PQL_ATTR_TIMEOUTSMTPD"),
					"attrib.control.timeoutsmtpd.inc",
					"qmail-smtpd",
					pql_get_define("PQL_ATTR_TIMEOUTSMTPD"));

pql_plugin_register(pql_get_define("PQL_ATTR_TIMEOUTCONNECT"),
					"attrib.control.timeoutconnect.inc",
					"qmail-remote",
					array(pql_get_define("PQL_ATTR_TIMEOUTCONNECT"),
						  pql_get_define("PQL_ATTR_TIMEOUTREMOTE")));

pql_plugin_register(pql_get_define("PQL_ATTR_SMTPGREETING"),
					"attrib.control.smtpgreeting.inc",
					"qmail-smtpd",
					pql_get_define("PQL_ATTR_SMTPGREETING"));

pql_plugin_register(pql_get_define("PQL_ATTR_QMQPSERVERS"),
					"attrib.control.qmqpservers.inc",
					"qmail-qmqpc",
					pql_get_define("PQL_ATTR_QMQPSERVERS"));

pql_plugin_register(pql_get_define("PQL_ATTR_RELAYMAILFROM"),
					"attrib.control.relaymailfrom.inc",
					"q-l relay",
					pql_get_define("PQL_ATTR_RELAYMAILFROM"));

pql_plugin_register(pql_get_define("PQL_ATTR_BADRCPTTO"),
					"attrib.control.badrcptto.inc",
					"q-l spam control",
					pql_get_define("PQL_ATTR_BADRCPTTO"));

pql_plugin_register(pql_get_define("PQL_ATTR_RCPTHOSTS"),
					"attrib.control.rcpthosts.inc",
					pql_get_define("PQL_ATTR_RCPTHOSTS"),
					pql_get_define("PQL_ATTR_RCPTHOSTS"));

if($_SESSION["NEW_STYLE_QUOTA"]) {
	// Oki, so these have nothing to do with quota, but the varible
	// (the existance of the new style quota attributes) existance
	// (in the LDAP server) should indicate that we're running a newer
	// QmailLDAP, where these are defined as well...

	// ----- LDAP stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_LDAPTIMEOUT"),
						"attrib.control.ldaptimeout.inc",
						"q-l ldap core",
						pql_get_define("PQL_ATTR_LDAPTIMEOUT"));
	
	// ----- Cluster stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"),
						"attrib.control.ldapclusterhosts.inc",
						"q-l ldap cluster",
						pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"));
	
	pql_plugin_register(pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"),
						"attrib.control.ldapclustercookie.inc",
						"q-l ldap cluster",
						pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"));

	// ----- SMTP stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_OUTGOINGIP"),
						"attrib.control.outgoingip.inc",
						"qmail-smtpd",
						pql_get_define("PQL_ATTR_OUTGOINGIP"));

	pql_plugin_register(pql_get_define("PQL_ATTR_GOODADDRESS"),
						"attrib.control.goodaddress.inc",
						"qmail-smtpd",
						pql_get_define("PQL_ATTR_GOODADDRESS"));

	// ----- QMQP stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_QMQPCIP"),
						"attrib.control.qmqpcip.inc",
						"qmail-qmqpc",
						pql_get_define("PQL_ATTR_QMQPCIP"));

	// ----- Other/Misc stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_BIGBROTHER"),
						"attrib.control.bigbrother.inc",
						"qmail-send",
						pql_get_define("PQL_ATTR_BIGBROTHER"));

	pql_plugin_register(pql_get_define("PQL_ATTR_RBLLIST"),
						"attrib.control.rbllist.inc",
						"q-l spam control",
						pql_get_define("PQL_ATTR_RBLLIST"));

	pql_plugin_register(pql_get_define("PQL_ATTR_BOUNCESIZE"),
						"attrib.control.bouncesize.inc",
						"q-l bounce",
						pql_get_define("PQL_ATTR_BOUNCESIZE"));

	// ----- PopBeforeSmtp stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_SERVERS"),
						"attrib.control.pbsservers.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_SERVERS"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_IP"),
						"attrib.control.pbsip.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_IP"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_PORT"),
						"attrib.control.pbsport.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_PORT"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_SECRET"),
						"attrib.control.pbssecret.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_SECRET"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_CACHESIZE"),
						"attrib.control.pbscachesize.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_CACHESIZE"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_TIMEOUT"),
						"attrib.control.pbstimeout.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_TIMEOUT"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_ENV"),
						"attrib.control.pbsenv.inc",
						"q-l popbeforesmtp",
						pql_get_define("PQL_ATTR_PBS_ENV"));
}

// ----- Fix for bug 257
// : Manually added rcpthost values not keept with a manual replication
pql_plugin_register(pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"),
					"attrib.control.nonprimmx.inc",
					pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"),
					pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"));

// List all registred attributes and their files
// - Uncomment this piece of code to view which
//   plugins are registred and to which file
//   they belong.

//$i = 0;
//$attribs = $_pql_plugins->attribs; asort($attribs);
//foreach($attribs as $key => $value) {
//	echo "$key --> attrib.control.$value.inc <br>";
//	$i++;
//}
//echo "Attribute count: $i<br><br>";

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
