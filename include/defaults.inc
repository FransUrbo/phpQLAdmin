<?php
// configuration file
// This file contains what I think is 'resonable defaults'.
// Don't change here, change any values in the web interface!

// ----------------------------
// PQL_SHOW_USERS (true | false)
// set to false, if the domain users should not be shown in the navigation
// frame. this results in better performance on servers with large amount
// of users
define("PQL_SHOW_USERS", true);

// ----------------------------
// PQL_AUTO_RELOAD (true | false)
// true, if the navigation bar should be automatically reloaded, if a domain
// or user was added or deleted
define("PQL_AUTO_RELOAD", true);

// ----------------------------
// PQL_LDAP_CONTROL_USE (true | false)
// set to true, if QmailLDAP/Control patch is supported by your system
define("PQL_LDAP_CONTROL_USE", false);

// ----------------------------
// PQL_LDAP_CONTROL_AUTOADDLOCALS (true | false)
// set this to true, if phpQLAdmin should automatically add all domains to
// locals. Disable this (false), if you have more than one qmail-ldap server
// and don't want that phpQLAdmin register all domains on this server. Currently, the
// application do not support more than one control database.
define("PQL_LDAP_CONTROL_AUTOADDLOCALS", false);

// ----------------------------
// Should we enable changing of LDAP server?
// NOTE: Not currently working, so don't use (yet)
define("PQL_LDAP_CHANGE_SERVER", false);

// ----------------------------
// PQL_ALLOW_ABSOLUTE_PATH (true | false)
// true, if absolute paths are allowed to set
// the mailbox directory, false if not
define("PQL_ALLOW_ABSOLUTE_PATH", false);

// ----------------------------
// PQL_VERIFY_DELETE (true | false)
// Should we verify deletes of user objects?
// If you KNOW exactly what you're doing (like me :),
// you might find all these 'are you really sure' etc
// VERY ANNOYING! :)
define("PQL_VERIFY_DELETE", true);

// ----------------------------
// Should we enable EZMLM mailinglist management?
// This require the admin interface (phpQLAdmin)
// to be running on the ezmlm host. And if it's
// 'replicated' (ie, round-robbing etc), then you'll
// have to do this manually (at the moment).
define("PQL_LDAP_EZMLM_USE", false);

// ----------------------------
// Path to EZMLM commands
define("PQL_LDAP_EZMLM_PATH", "/usr/bin");

// ----------------------------
// PQL_TESTMAIL_SUBJECT (text)
// The subject of testmails
define("PQL_TESTMAIL_SUBJECT", "test");

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
define("PQL_TESTMAIL_MAILTEXT","hi %cn%,\nyour email address '%mail%' seems to work.\n\nhave a nice day,\nyour administrator\n\n\n-------\nsent trough phpqladmin %version%");

// ----------------------------
// PQL_TESTMAIL_AUTOSEND (true | false)
// Automatically send a testmail when a new user or alias has been added?
define("PQL_TESTMAIL_AUTOSEND", false);

// ----------------------------
// PQL_CATCHALLUSER (username)
// the username which receives all mails for undefined users. the default is 'catchall' (w/o quotes), so the user
// which has the email address or alias defined to catchall@domain.ch will receive all emails of undefined domain.ch
// addresses. this can be changed in qmail-ldap at compile time.
define("PQL_CATCHALLUSER", "catchall");

// ----------------------------
// Host configuration - who are we?
// This is what's going to be the title
// of the page - in the dragbar of the
// web browser...
define("PQL_WHOAREWE", "QmailLDAP administration");

// ----------------------------
// PQL_LDAP_ATTR_* (LDAP attribute names)
// You should provide the same names here as in your qmail-ldap.h
define("PQL_LDAP_ATTR_MAIL", strtolower("mail"));
define("PQL_LDAP_ATTR_FORWARDS", strtolower("mailforwardingaddress"));
define("PQL_LDAP_ATTR_MAILALTERNATE", strtolower("mailalternateaddress"));
define("PQL_LDAP_ATTR_QMAILUID", strtolower("uidnumber"));
define("PQL_LDAP_ATTR_QMAILGID", strtolower("gidnumber"));
define("PQL_LDAP_ATTR_MAILSTORE", strtolower("mailmessagestore"));
define("PQL_LDAP_ATTR_HOMEDIR", strtolower("homedirectory"));
define("PQL_LDAP_ATTR_LOGINSHELL", strtolower("loginshell"));
define("PQL_LDAP_ATTR_QUOTA", strtolower("mailquota"));
define("PQL_LDAP_ATTR_PROGRAM", strtolower("deliveryprogrampath"));
define("PQL_LDAP_ATTR_MAILHOST", strtolower("mailhost"));
define("PQL_LDAP_ATTR_MODE", strtolower("deliverymode"));
define("PQL_LDAP_ATTR_REPLYTEXT", strtolower("mailreplytext"));
define("PQL_LDAP_ATTR_DOTMODE", strtolower("qmaildotmode"));
define("PQL_LDAP_ATTR_UID", strtolower("uid"));
define("PQL_LDAP_ATTR_PASSWD", strtolower("userpassword"));
define("PQL_LDAP_ATTR_OBJECTCLASS", strtolower("objectclass"));
define("PQL_LDAP_ATTR_ISACTIVE", strtolower("accountstatus"));
define("PQL_LDAP_ATTR_PURGE", strtolower("qmailaccountpurge"));

// ----------------------------
// These attributes aren't used by qmail-ldap
// However, phpQLAdmin currently relies on them
define("PQL_LDAP_ATTR_CN", strtolower("cn"));
define("PQL_LDAP_ATTR_SN", strtolower("sn"));
define("PQL_LDAP_ATTR_DESCRIPTION", strtolower("description"));
define("PQL_LDAP_ATTR_GIVENNAME", strtolower("givenname"));

// ----------------------------
// PQL_LDAP_OBJECTCLASS_USERID (qmailUser | mailRecipent)
// What is the objectclass of a user object?
// qmailUser is default Qmail-LDAP objectClass.
define("PQL_LDAP_OBJECTCLASS_USERID", "qmailuser");

// ----------------------------
// PQL_LDAP_OBJECTCLASS_USER_EXTRA
// What extra objectclasses should be used when adding a
// user object? The existing (non changable) is 'top', 'person'
// and PQL_LDAP_OBJECTCLASS_USERID. Add any extra objectclasses
// here, separated with space
// -> Do NOT list inetOrgPerson and/or posixAccount here!
// -> Don't quite work yet. It won't take notice of MAY's in
//    these object classes!
//define("PQL_LDAP_OBJECTCLASS_USER_EXTRA", "person pilotperson organizationalperson krb5principal extraposixaccount trustaccount");

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
define("PQL_PASSWORD_SCHEMES", "crypt,md5,sha,clear,kerberos");

// ----------------------------
// PQL_LDAP_REFERENCE_USERS_WITH (LDAP attribute name)
// Defines which attribute phpQLAdmin passes to various functions when it needs to reference users.
// This attribute is used to retrieve the dn for the referenced user account. So it's
// obvious that this attribute must be unique!  
define("PQL_LDAP_REFERENCE_USERS_WITH", PQL_LDAP_ATTR_UID);

// ----------------------------
// PQL_LDAP_REFERENCE_DOMAINS_WITH (dc | ou)
// How is your tree organized? What objectClass is used
// It's either 'ou=domain,<LDAP_BASEDN>' or 'dc=domain,<LDAP_BASEDN>'
define("PQL_LDAP_REFERENCE_DOMAINS_WITH", "ou");

// ----------------------------
// PQL_LDAP_USER_RDN_ATTRIBUTE (LDAP attribute name)
// This defines the attribute name, which will be taken to generate
// the rdn of a user object. e.g. if you choose 'uid' the rdn will be
// 'uid=idofthisuser'
// If you want phpQLAdmin to behave like earlier versions, use PQL_LDAP_ATTR_CN here.
define("PQL_LDAP_USER_RDN_ATTRIBUTE", PQL_LDAP_ATTR_UID);

// ----------------------------
// PQL_URI (URL)
// the full url where phpQLAdmin resides
// ie: http://www.yourdomain.com/phpQLAdmin/
define("PQL_URI", "");

// ----------------------------
if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
	$objectclass_domain = "domain";
} else {
	$objectclass_domain = "organizationalUnit";
}
define("PQL_LDAP_OBJECTCLASS_DOMAIN", strtolower($objectclass_domain) . " dcOrganizationNameForm phpQLAdminBranch");

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
