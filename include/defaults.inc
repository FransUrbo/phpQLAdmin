<?php
// configuration file
// This file contains what I think is 'resonable defaults'.
// Don't change here, change any values in the web interface!
//
// $Id: defaults.inc,v 2.31 2005-02-24 17:04:01 turbo Exp $

// NOTE: Some of these defines isn't called with the (root/top)
//       DN. They are (site) global variables, not per branch.

// ----------------------------
// PQL_CONF_WHOAREWE
pql_set_define("PQL_CONF_WHOAREWE", "phpQLAdmin", false);

// ----------------------------
// Default language (is changed through the GUI)
pql_set_define("PQL_CONF_LANG", "en", false);

// ----------------------------
// PQL_CONF_SHOW_USERS (true | false)
// set to false, if the domain users should not be shown in the navigation
// frame. this results in better performance on servers with large amount
// of users
pql_set_define("PQL_CONF_SHOW_USERS", true, false, $dn);

// ----------------------------
// PQL_CONF_AUTO_RELOAD (true | false)
// true, if the navigation bar should be automatically reloaded, if a domain
// or user was added or deleted
pql_set_define("PQL_CONF_AUTO_RELOAD", true, false);

// ----------------------------
// PQL_CONF_CONTROL_USE (true | false)
// set to true, if QmailLDAP/Control patch is supported by your system
pql_set_define("PQL_CONF_CONTROL_USE", false, false);

// ----------------------------
// PQL_CONF_ACI_USE (true | false)
// set to true, if you're using OpenLDAP >2.2.14 and you'd like to use ACI's.
pql_set_define("PQL_CONF_ACI_USE", false, false);

// ============================
// The trick with the next two entries (PQL_CONF_CONTROL_AUTOADD{LOCALS,RCPTHOSTS}),
// is that the master (email/qmail) server should only have 'locals', and the
// slaves should only have 'rcpthosts' attributes.
// This will make phpQLAdmin add the new domain (or the additionalDomainName
// in 'locals' on the master, and in the 'rcpthosts' on the slave(s).

// ----------------------------
// PQL_CONF_CONTROL_AUTOADDLOCALS (true | false)
// set this to true, if phpQLAdmin should automatically add all domains to
// locals. Disable this (false), if you have more than one qmail-ldap server
// and they're not identical (ie, not hosting the same domains)
// Currently, phpQLAdmin does not support more than one control database.
pql_set_define("PQL_CONF_CONTROL_AUTOADDLOCALS", false, false);

// ----------------------------
// PQL_CONF_CONTROL_AUTOADDRCPTHOSTS (true | false)
// set this to true, if phpQLAdmin should automatically add all domains to
// rcpthosts. Disable this (false), if you have more than one qmail-ldap server
// and they're not identical (ie, not hosting the same domains)
// Currently, the application do not support more than one control database.
pql_set_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", false, false);

// ----------------------------
// PQL_CONF_CONTROL_AUTOADDHOSTNAME (true | false)
// set this to true, if phpQLAdmin should automatically add the hostname to
// all domains it does autoreplication for (locals AND rcpthosts).
// Disable this (false), if you have more than one qmail-ldap server
// and they're not identical (ie, not hosting the same domains)
// Currently, the application do not support more than one control database.
pql_set_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", false, false);

// ----------------------------
// Should we enable changing of LDAP server?
// NOTE: Not currently working, so don't use (yet)
pql_set_define("PQL_CONF_CHANGE_SERVER", false, false);

// ----------------------------
// PQL_CONF_ALLOW_ABSOLUTE_PATH (true | false)
// true, if absolute paths are allowed to set
// the mailbox directory, false if not
pql_set_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", false, false, $dn);

// ----------------------------
// PQL_CONF_VERIFY_DELETE (true | false)
// Should we verify deletes of user objects?
// If you KNOW exactly what you're doing (like me :),
// you might find all these 'are you really sure' etc
// VERY ANNOYING, set this to 'false'! :)
pql_set_define("PQL_CONF_VERIFY_DELETE", true, false, $dn);

// ----------------------------
// Should we enable EZMLM mailinglist management?
// This require the admin interface (phpQLAdmin)
// to be running on the ezmlm host. And if it's
// 'replicated' (ie, round-robbing etc), then you'll
// have to do this manually (at the moment).
pql_set_define("PQL_CONF_EZMLM_USE", false, false);

// ----------------------------
// Path to EZMLM commands
pql_set_define("PQL_CONF_EZMLM_PATH", "/usr/sbin", false);

// ----------------------------
// Should we enable the domain (Bind 9) editor?
// This require that Bind 9 (and ONLY Bind 9) works
// with DNSSEC and that you (the webserver/phpQLAdmin)
// have access to a 'root' key that have access to
// update the DNS.
pql_set_define("PQL_CONF_BIND9_USE", false, false);

// ----------------------------
// Should we enable the webserver administration editor?
// This (likley) require an external script that will
// search the LDAP database for the configuration and
// create file(s) that the webserver will understand.
pql_set_define("PQL_CONF_WEBSRV_USE", false, false);

// ----------------------------
// Should we automaticly create usernames when adding
// a user to the system?
pql_set_define("PQL_CONF_AUTOCREATE_USERNAME", false, false, $dn);

// ----------------------------
// Should we automaticly create email address when
// adding a user to the system?
pql_set_define("PQL_CONF_CREATE_ADDRESS", false, false, $dn);

// ----------------------------
// PQL_CONF_TESTMAIL_SUBJECT (text)
// The subject of testmails
pql_set_define("PQL_CONF_TESTMAIL_SUBJECT", "test", false, $dn);

// ----------------------------
// PQL_CONF_TESTMAIL_MAILTEXT (text)
// This is the text of testmails.
// The following variables are supported:
// - %MAIL%				--> users mail address
// - %CN% 				--> users common name 
// - %SN%				--> users surname
// - %UID% 				--> users uid
// - %QUOTA% 			--> users quota  
// - %VERSION%			--> phpQLAdmin version number
pql_set_define("PQL_CONF_TESTMAIL_MAILTEXT","hi %cn%,\nyour email address '%mail%' seems to work.\n\nhave a nice day,\nyour administrator\n\n\n-------\nsent trough phpqladmin %version%", false, $dn);

// ----------------------------
// PQL_CONF_TESTMAIL_AUTOSEND (true | false)
// Automatically send a testmail when a new user or alias has been added?
pql_set_define("PQL_CONF_TESTMAIL_AUTOSEND", false, false, $dn);

// ----------------------------
// PQL_CONF_CATCHALLUSER (username)
// the username which receives all mails for undefined users. the default is 'catchall' (w/o quotes), so the user
// which has the email address or alias defined to catchall@domain.ch will receive all emails of undefined domain.ch
// addresses. this can be changed in qmail-ldap at compile time.
pql_set_define("PQL_CONF_CATCHALLUSER", "catchall", false);

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
pql_set_define("PQL_CONF_PASSWORD_SCHEMES", "CRYPT,MD5,SHA,CLEAR,KERBEROS", false, $dn);

// ----------------------------
// PQL_CONF_REFERENCE_DOMAINS_WITH (dc | ou | o)
// How is your tree organized? What objectClass is used
// It's either 'ou=domain,<LDAP_BASEDN>' or 'dc=domain,<LDAP_BASEDN>'
pql_set_define("PQL_CONF_REFERENCE_DOMAINS_WITH", "ou", false, $dn);

// ----------------------------
// PQL_CONF_USER_RDN_ATTRIBUTE (LDAP attribute name)
// This defines the attribute name, which will be taken to generate
// the rdn of a user object. e.g. if you choose 'uid' the rdn will be
// 'uid=idofthisuser'
// If you want phpQLAdmin to behave like earlier versions, use PQL_CONF_CN here.
pql_set_define("PQL_CONF_USER_RDN_ATTRIBUTE", pql_get_define("PQL_ATTR_UID"), false, $dn);

// ----------------------------
// PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER (integer)
// Even forward accounts need a UIDNumber!
pql_set_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", 500, false, $dn);

// ----------------------------
// PQL_CONF_MAXIMUM_DOMAIN_USERS (integer)
// Maximum amount of users allowed in a domain branch
// If zero (0) or 'FALSE', then unlimited
pql_set_define("PQL_CONF_MAXIMUM_DOMAIN_USERS", 'FALSE', false);

// ----------------------------
// PQL_CONF_URI (URL)
// the full url where phpQLAdmin resides
// ie: http://www.yourdomain.com/phpQLAdmin/
pql_set_define("PQL_CONF_URI", "", false);

// ----------------------------
// PQL_ATTR_VIRTUAL_BASEHOME (string)
// The (base directory of) virtual users home directory
// Doesn not need to exists, but it is not allowed to be world writable
// (Qmail thingie, not phpQLAdmin!).
pql_set_define("PQL_CONF_VIRTUAL_BASEHOME", "/tmp/virtual", false);

// ----------------------------
if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $dn) == "dc") {
	$objectclass_domain = "domain dcOrganizationNameForm";
} elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $dn) == "ou") {
	$objectclass_domain = "organizationalUnit";
} elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $dn) == "o") {
	$objectclass_domain = "organization";
}
pql_set_define("PQL_CONF_OBJECTCLASS_DOMAIN", $objectclass_domain . " phpQLAdminBranch", false, $dn);

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
