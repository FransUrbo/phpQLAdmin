<?php
// configuration file
// This file contains what I think is 'resonable defaults'.
// Don't change here, change any values in the web interface!

// ----------------------------
// PQL_SHOW_USERS (true | false)
// set to false, if the domain users should not be shown in the navigation
// frame. this results in better performance on servers with large amount
// of users
pql_define("PQL_SHOW_USERS", true, false, $dn);

// ----------------------------
// PQL_AUTO_RELOAD (true | false)
// true, if the navigation bar should be automatically reloaded, if a domain
// or user was added or deleted
pql_define("PQL_AUTO_RELOAD", true, false, $dn);

// ----------------------------
// PQL_LDAP_CONTROL_USE (true | false)
// set to true, if QmailLDAP/Control patch is supported by your system
pql_define("PQL_LDAP_CONTROL_USE", false, false, $dn);

// ----------------------------
// PQL_LDAP_CONTROL_AUTOADDLOCALS (true | false)
// set this to true, if phpQLAdmin should automatically add all domains to
// locals. Disable this (false), if you have more than one qmail-ldap server
// and they're not identical (ie, not hosting the same domains)
// Currently, the application do not support more than one control database.
pql_define("PQL_LDAP_CONTROL_AUTOADDLOCALS", false, false, $dn);

// ----------------------------
// Should we enable changing of LDAP server?
// NOTE: Not currently working, so don't use (yet)
pql_define("PQL_LDAP_CHANGE_SERVER", false, false, $dn);

// ----------------------------
// PQL_ALLOW_ABSOLUTE_PATH (true | false)
// true, if absolute paths are allowed to set
// the mailbox directory, false if not
pql_define("PQL_ALLOW_ABSOLUTE_PATH", false, false, $dn);

// ----------------------------
// PQL_VERIFY_DELETE (true | false)
// Should we verify deletes of user objects?
// If you KNOW exactly what you're doing (like me :),
// you might find all these 'are you really sure' etc
// VERY ANNOYING, set this to 'false'! :)
pql_define("PQL_VERIFY_DELETE", true, false, $dn);

// ----------------------------
// Should we enable EZMLM mailinglist management?
// This require the admin interface (phpQLAdmin)
// to be running on the ezmlm host. And if it's
// 'replicated' (ie, round-robbing etc), then you'll
// have to do this manually (at the moment).
pql_define("PQL_LDAP_EZMLM_USE", false, false, $dn);

// ----------------------------
// Path to EZMLM commands
pql_define("PQL_LDAP_EZMLM_PATH", "/usr/bin", false, $dn);

// ----------------------------
// PQL_TESTMAIL_SUBJECT (text)
// The subject of testmails
pql_define("PQL_TESTMAIL_SUBJECT", "test", false, $dn);

// ----------------------------
// PQL_TESTMAIL_MAILTEXT (text)
// This is the text of testmails.
// The following variables are supported:
// - %MAIL%				--> users mail address
// - %CN% 				--> users common name 
// - %SN%				--> users surname
// - %UID% 				--> users uid
// - %QUOTA% 			--> users quota  
// - %VERSION%			--> phpQLAdmin version number
pql_define("PQL_TESTMAIL_MAILTEXT","hi %cn%,\nyour email address '%mail%' seems to work.\n\nhave a nice day,\nyour administrator\n\n\n-------\nsent trough phpqladmin %version%", false, $dn);

// ----------------------------
// PQL_TESTMAIL_AUTOSEND (true | false)
// Automatically send a testmail when a new user or alias has been added?
pql_define("PQL_TESTMAIL_AUTOSEND", false, false, $dn);

// ----------------------------
// PQL_CATCHALLUSER (username)
// the username which receives all mails for undefined users. the default is 'catchall' (w/o quotes), so the user
// which has the email address or alias defined to catchall@domain.ch will receive all emails of undefined domain.ch
// addresses. this can be changed in qmail-ldap at compile time.
pql_define("PQL_CATCHALLUSER", "catchall", false, $dn);

// ----------------------------
// Host configuration - who are we?
// This is what's going to be the title
// of the page - in the dragbar of the
// web browser...
pql_define("PQL_WHOAREWE", "QmailLDAP administration", false, $dn);

// ----------------------------
// PQL_LDAP_ATTR_* (LDAP attribute names)
// You should provide the same names here as in your qmail-ldap.h
pql_define("PQL_LDAP_ATTR_MAIL", strtolower("mail"), false, $dn);
pql_define("PQL_LDAP_ATTR_FORWARDS", strtolower("mailforwardingaddress"), false, $dn);
pql_define("PQL_LDAP_ATTR_MAILALTERNATE", strtolower("mailalternateaddress"), false, $dn);
pql_define("PQL_LDAP_ATTR_QMAILUID", strtolower("uidnumber"), false, $dn);
pql_define("PQL_LDAP_ATTR_QMAILGID", strtolower("gidnumber"), false, $dn);
pql_define("PQL_LDAP_ATTR_MAILSTORE", strtolower("mailmessagestore"), false, $dn);
pql_define("PQL_LDAP_ATTR_HOMEDIR", strtolower("homedirectory"), false, $dn);
pql_define("PQL_LDAP_ATTR_LOGINSHELL", strtolower("loginshell"), false, $dn);
pql_define("PQL_LDAP_ATTR_QUOTA", strtolower("mailquota"), false, $dn);
pql_define("PQL_LDAP_ATTR_PROGRAM", strtolower("deliveryprogrampath"), false, $dn);
pql_define("PQL_LDAP_ATTR_MAILHOST", strtolower("mailhost"), false, $dn);
pql_define("PQL_LDAP_ATTR_MODE", strtolower("deliverymode"), false, $dn);
pql_define("PQL_LDAP_ATTR_REPLYTEXT", strtolower("mailreplytext"), false, $dn);
pql_define("PQL_LDAP_ATTR_DOTMODE", strtolower("qmaildotmode"), false, $dn);
pql_define("PQL_LDAP_ATTR_UID", strtolower("uid"), false, $dn);
pql_define("PQL_LDAP_ATTR_PASSWD", strtolower("userpassword"), false, $dn);
pql_define("PQL_LDAP_ATTR_OBJECTCLASS", strtolower("objectclass"), false, $dn);
pql_define("PQL_LDAP_ATTR_ISACTIVE", strtolower("accountstatus"), false, $dn);
pql_define("PQL_LDAP_ATTR_PURGE", strtolower("qmailaccountpurge"), false, $dn);

// ----------------------------
// These attributes aren't used by qmail-ldap
// However, phpQLAdmin currently relies on them
pql_define("PQL_LDAP_ATTR_CN", strtolower("cn"), false, $dn);
pql_define("PQL_LDAP_ATTR_SN", strtolower("sn"), false, $dn);
pql_define("PQL_LDAP_ATTR_DESCRIPTION", strtolower("description"), false, $dn);
pql_define("PQL_LDAP_ATTR_GIVENNAME", strtolower("givenname"), false, $dn);

// ----------------------------
// password encryption schemes to allow
// List of schemes for password schemes that we can choose from when
// creating a user (or changing password for a user).
// Separate entries with comma (,).
//
// Possible values:
// 	- CRYPT		no additional libraries needed (use native PHP crypt) (DEFAULT)
// 	- MD5 		mhash library support needed
// 	- SHA		mhash library support needed
// 	- CLEAR		no encryption, works only if qmail-ldap is compiled w/ cleartext password support, BAD IDEA !!
//	- KERBEROS	use OpenLDAP Kerberos V mapping ({KERBEROS}PRINCIPAL@REALM)
pql_define("PQL_PASSWORD_SCHEMES", "CRYPT,MD5,SHA,CLEAR,KERBEROS", false, $dn);

// ----------------------------
// PQL_LDAP_REFERENCE_USERS_WITH (LDAP attribute name)
// Defines which attribute phpQLAdmin passes to various functions when it needs to reference users.
// This attribute is used to retrieve the dn for the referenced user account. So it's
// obvious that this attribute must be unique!  
pql_define("PQL_LDAP_REFERENCE_USERS_WITH", PQL_LDAP_ATTR_UID, false, $dn);

// ----------------------------
// PQL_LDAP_REFERENCE_DOMAINS_WITH (dc | ou | o)
// How is your tree organized? What objectClass is used
// It's either 'ou=domain,<LDAP_BASEDN>' or 'dc=domain,<LDAP_BASEDN>'
pql_define("PQL_LDAP_REFERENCE_DOMAINS_WITH", "ou", false, $dn);

// ----------------------------
// PQL_LDAP_USER_RDN_ATTRIBUTE (LDAP attribute name)
// This defines the attribute name, which will be taken to generate
// the rdn of a user object. e.g. if you choose 'uid' the rdn will be
// 'uid=idofthisuser'
// If you want phpQLAdmin to behave like earlier versions, use PQL_LDAP_ATTR_CN here.
pql_define("PQL_LDAP_USER_RDN_ATTRIBUTE", PQL_LDAP_ATTR_UID, false, $dn);

// ----------------------------
// PQL_LDAP_FORWARDINGACCOUNT_UIDNUMBER (integer)
// Even forward accounts need a UIDNumber!
pql_define("PQL_LDAP_FORWARDINGACCOUNT_UIDNUMBER", 500, false, $dn);

// ----------------------------
// PQL_URI (URL)
// the full url where phpQLAdmin resides
// ie: http://www.yourdomain.com/phpQLAdmin/
pql_define("PQL_URI", "", false, $dn);

// ----------------------------
if($config["PQL_LDAP_REFERENCE_DOMAINS_WITH"][$dn] == "dc") {
	$objectclass_domain = "domain";
} elseif($config["PQL_LDAP_REFERENCE_DOMAINS_WITH"][$dn] == "ou") {
	$objectclass_domain = "organizationalUnit";
} elseif($config["PQL_LDAP_REFERENCE_DOMAINS_WITH"][$dn] == "o") {
	$objectclass_domain = "organization";
}
pql_define("PQL_LDAP_OBJECTCLASS_DOMAIN", $objectclass_domain . " dcOrganizationNameForm phpQLAdminBranch", false, $dn);

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
