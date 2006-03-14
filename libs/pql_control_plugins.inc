<?php
// ----------------------------
// pql_control_plugins.inc
// phpQLAdmin Application Programming Interface (API)
// (special functions for server control plugins)
//
// $Id: pql_control_plugins.inc,v 1.1.2.1 2006-03-14 14:46:40 turbo Exp $
//

// ----------------------------
// plugin definition
// ----------------------------

// qmail-ldap specific options
pql_plugin_register(pql_get_define("PQL_ATTR_LDAPSERVER"),
					"LDAP Core",
					"attrib.control.ldapserver.inc",
					array(pql_get_define("PQL_ATTR_LDAPSERVER"),
						  pql_get_define("PQL_ATTR_LDAPBASEDN"),
						  pql_get_define("PQL_ATTR_LDAPLOGIN"),
						  pql_get_define("PQL_ATTR_LDAPPASSWORD"),
						  pql_get_define("PQL_ATTR_GROUP_LOGIN"),
						  pql_get_define("PQL_ATTR_GROUP_PASSWD")));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPREBIND"),
					"LDAP Core",
					"attrib.control.ldaprebind.inc",
					pql_get_define("PQL_ATTR_LDAPREBIND"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPCLUSTER"),
					"LDAP Clustering",
					"attrib.control.ldapcluster.inc",
					pql_get_define("PQL_ATTR_LDAPCLUSTER"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"),
					"Mail Deliveries",
					"attrib.control.ldapdefaultquota.inc",
					pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"),
					"Mail Deliveries",
					"attrib.control.ldapmessagestore.inc",
					pql_get_define("PQL_ATTR_LDAPMESSAGESTORE"));

pql_plugin_register(pql_get_define("PQL_ATTR_DIRMAKER"),
					"Mail Deliveries",
					"attrib.control.dirmaker.inc",
					pql_get_define("PQL_ATTR_DIRMAKER"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY"),
					"Mail Deliveries",
					"attrib.control.ldaplocaldelivery.inc",
					pql_get_define("PQL_ATTR_LDAPLOCALDELIVERY"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUOTA_WARNING"),
					"Mail Deliveries",
					"attrib.control.quotawarning.inc",
					pql_get_define("PQL_ATTR_QUOTA_WARNING"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"),
					"Mail Deliveries",
					"attrib.control.ldapdefaultdotmode.inc",
					pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"));

pql_plugin_register(pql_get_define("PQL_ATTR_LDAPUID"),
					"Virtual User",
					"attrib.control.ldapuid.inc",
					array(pql_get_define("PQL_ATTR_LDAPUID"),
						  pql_get_define("PQL_ATTR_LDAPGID")));

pql_plugin_register(pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"),
					"Mail Bouncing",
					"attrib.control.custombouncetext.inc",
					pql_get_define("PQL_ATTR_CUSTOMBOUNCETEXT"));

pql_plugin_register(pql_get_define("PQL_ATTR_TARPITCOUNT"),
					"Spam Control",
					"attrib.control.tarpitcount.inc",
					array(pql_get_define("PQL_ATTR_TARPITCOUNT"),
						  pql_get_define("PQL_ATTR_TARPITDELAY")));

pql_plugin_register(pql_get_define("PQL_ATTR_MAXRCPTCOUNT"),
					"Spam Control",
					"attrib.control.maxrcptcount.inc",
					pql_get_define("PQL_ATTR_MAXRCPTCOUNT"));

// core qmail specific options
pql_plugin_register(pql_get_define("PQL_ATTR_BOUNCEFROM"),
					"Mail Bouncing",
					"attrib.control.bouncefrom.inc",
					array(pql_get_define("PQL_ATTR_BOUNCEFROM"),
						  pql_get_define("PQL_ATTR_BOUNCEHOST")));

pql_plugin_register(pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
					"qmail-send",
					"attrib.control.concurrencylocal.inc",
					array(pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
						  pql_get_define("PQL_ATTR_CONCURRENCYREMOTE")));

pql_plugin_register(pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
					"Mail Bouncing",
					"attrib.control.doublebouncehost.inc",
					array(pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
						  pql_get_define("PQL_ATTR_DOUBLEBOUNCETO")));

pql_plugin_register(pql_get_define("PQL_ATTR_DATABYTES"),
					"qmail-smtpd",
					"attrib.control.databytes.inc",
					pql_get_define("PQL_ATTR_DATABYTES"));

pql_plugin_register(pql_get_define("PQL_ATTR_QUEUELIFETIME"),
					"qmail-send",
					"attrib.control.queuelifetime.inc",
					pql_get_define("PQL_ATTR_QUEUELIFETIME"));

pql_plugin_register(pql_get_define("PQL_ATTR_SMTPROUTES"),
					"qmail-remote",
					"attrib.control.smtproutes.inc",
					pql_get_define("PQL_ATTR_SMTPROUTES"));

pql_plugin_register(pql_get_define("PQL_ATTR_LOCALS"),
					"Accepted domains",
					"attrib.control.locals.inc",
					pql_get_define("PQL_ATTR_LOCALS"));

pql_plugin_register(pql_get_define("PQL_ATTR_RCPTHOSTS"),
					"Accepted domains",
					"attrib.control.rcpthosts.inc",
					pql_get_define("PQL_ATTR_RCPTHOSTS"));

pql_plugin_register(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
					"Mail Preprocessing",
					"attrib.control.defaultdomain.inc",
					array(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
						  pql_get_define("PQL_ATTR_DEFAULTHOST"),
						  pql_get_define("PQL_ATTR_PLUSDOMAIN")));

pql_plugin_register(pql_get_define("PQL_ATTR_BADMAILFROM"),
					"qmail-smtpd",
					"attrib.control.badmailfrom.inc",
					pql_get_define("PQL_ATTR_BADMAILFROM"));

pql_plugin_register(pql_get_define("PQL_ATTR_ENVNOATHOST"),
					"qmail-send",
					"attrib.control.envnoathost.inc",
					pql_get_define("PQL_ATTR_ENVNOATHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_HELOHOST"),
					"qmail-remote",
					"attrib.control.helohost.inc",
					pql_get_define("PQL_ATTR_HELOHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_IDHOST"),
					"Mail Preprocessing",
					"attrib.control.idhost.inc",
					pql_get_define("PQL_ATTR_IDHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_LOCALIPHOST"),
					"qmail-smtpd",
					"attrib.control.localiphost.inc",
					pql_get_define("PQL_ATTR_LOCALIPHOST"));

pql_plugin_register(pql_get_define("PQL_ATTR_PERCENTHACK"),
					"qmail-send",
					"attrib.control.percenthack.inc",
					pql_get_define("PQL_ATTR_PERCENTHACK"));

pql_plugin_register(pql_get_define("PQL_ATTR_TIMEOUTSMTPD"),
					"qmail-smtpd",
					"attrib.control.timeoutsmtpd.inc",
					pql_get_define("PQL_ATTR_TIMEOUTSMTPD"));

pql_plugin_register(pql_get_define("PQL_ATTR_TIMEOUTCONNECT"),
					"qmail-remote",
					"attrib.control.timeoutconnect.inc",
					array(pql_get_define("PQL_ATTR_TIMEOUTCONNECT"),
						  pql_get_define("PQL_ATTR_TIMEOUTREMOTE")));

pql_plugin_register(pql_get_define("PQL_ATTR_SMTPGREETING"),
					"qmail-smtpd",
					"attrib.control.smtpgreeting.inc",
					pql_get_define("PQL_ATTR_SMTPGREETING"));

pql_plugin_register(pql_get_define("PQL_ATTR_QMQPSERVERS"),
					"Mail Queueing",
					"attrib.control.qmqpservers.inc",
					pql_get_define("PQL_ATTR_QMQPSERVERS"));

pql_plugin_register(pql_get_define("PQL_ATTR_RELAYMAILFROM"),
					"Mail Relaying",
					"attrib.control.relaymailfrom.inc",
					pql_get_define("PQL_ATTR_RELAYMAILFROM"));

pql_plugin_register(pql_get_define("PQL_ATTR_BADRCPTTO"),
					"Spam Control",
					"attrib.control.badrcptto.inc",
					pql_get_define("PQL_ATTR_BADRCPTTO"));

pql_plugin_register("simscan",
					"Simscan Setup",
					"attrib.control.simscan.inc",
					array(pql_get_define("PQL_ATTR_SIMSCAN_SPAM"),
						  pql_get_define("PQL_ATTR_SIMSCAN_CLAM"),
						  pql_get_define("PQL_ATTR_SIMSCAN_TROPHIE"),
						  pql_get_define("PQL_ATTR_SIMSCAN_SPAM_HITS"),
						  pql_get_define("PQL_ATTR_SIMSCAN_ATTACHMENTS")));

if(!empty($_SESSION["NEW_STYLE_QUOTA"])) {
	// Oki, so these have nothing to do with quota, but the varible
	// (the existance of the new style quota attributes) existance
	// (in the LDAP server) should indicate that we're running a newer
	// QmailLDAP, where these are defined as well...

	// ----- LDAP stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_LDAPTIMEOUT"),
						"LDAP Core",
						"attrib.control.ldaptimeout.inc",
						pql_get_define("PQL_ATTR_LDAPTIMEOUT"));
	
	// ----- Cluster stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"),
						"LDAP Clustering",
						"attrib.control.ldapclusterhosts.inc",
						pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"));
	
	pql_plugin_register(pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"),
						"LDAP Clustering",
						"attrib.control.ldapclustercookie.inc",
						pql_get_define("PQL_ATTR_LDAPCLUSTER_COOKIE"));

	// ----- SMTP stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_OUTGOINGIP"),
						"qmail-smtpd",
						"attrib.control.outgoingip.inc",
						pql_get_define("PQL_ATTR_OUTGOINGIP"));

	pql_plugin_register(pql_get_define("PQL_ATTR_GOODADDRESS"),
						"qmail-smtpd",
						"attrib.control.goodaddress.inc",
						pql_get_define("PQL_ATTR_GOODADDRESS"));

	// ----- QMQP stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_QMQPCIP"),
						"Mail Queueing",
						"attrib.control.qmqpcip.inc",
						pql_get_define("PQL_ATTR_QMQPCIP"));

	// ----- Other/Misc stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_BIGBROTHER"),
						"qmail-send",
						"attrib.control.bigbrother.inc",
						pql_get_define("PQL_ATTR_BIGBROTHER"));

	pql_plugin_register(pql_get_define("PQL_ATTR_RBLLIST"),
						"Spam Control",
						"attrib.control.rbllist.inc",
						pql_get_define("PQL_ATTR_RBLLIST"));

	pql_plugin_register(pql_get_define("PQL_ATTR_BOUNCESIZE"),
						"Mail Bouncing",
						"attrib.control.bouncesize.inc",
						pql_get_define("PQL_ATTR_BOUNCESIZE"));

	// ----- PopBeforeSmtp stuff

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_SERVERS"),
						"POP before SMTP",
						"attrib.control.pbsservers.inc",
						pql_get_define("PQL_ATTR_PBS_SERVERS"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_IP"),
						"POP before SMTP",
						"attrib.control.pbsip.inc",
						pql_get_define("PQL_ATTR_PBS_IP"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_PORT"),
						"POP before SMTP",
						"attrib.control.pbsport.inc",
						pql_get_define("PQL_ATTR_PBS_PORT"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_SECRET"),
						"POP before SMTP",
						"attrib.control.pbssecret.inc",
						pql_get_define("PQL_ATTR_PBS_SECRET"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_CACHESIZE"),
						"POP before SMTP",
						"attrib.control.pbscachesize.inc",
						pql_get_define("PQL_ATTR_PBS_CACHESIZE"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_TIMEOUT"),
						"POP before SMTP",
						"attrib.control.pbstimeout.inc",
						pql_get_define("PQL_ATTR_PBS_TIMEOUT"));

	pql_plugin_register(pql_get_define("PQL_ATTR_PBS_ENV"),
						"POP before SMTP",
						"attrib.control.pbsenv.inc",
						pql_get_define("PQL_ATTR_PBS_ENV"));
}

// ----- Fix for bug 257
// : Manually added rcpthost values not keept with a manual replication
pql_plugin_register(pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"),
					"Accepted domains",
					"attrib.control.nonprimmx.inc",
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
