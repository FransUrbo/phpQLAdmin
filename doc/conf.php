<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: conf.php,v 2.0 2002-12-13 14:34:47 turbo Exp $ -->
</head>

<body bgcolor="#e7e7e7" background="../images/bkg.png">
<span class="title1">phpQLAdmin Documentation</span>
<br>
<br>
<a href="index.php">back to doc...</a>
<br>
<br>
<span class="title2">Configuration</span>
<br>
<br>
Edit the config.inc file to configure phpQLAdmin.
<br>
<br>
Note: the configuration values beginning
with PQL_LDAP_CONTROL_ are only required for qmail-ldap/control support within phpQLAdmin. You don't
have to set these if you don't need support for qmail-ldap/control.
<br>
<br>
<b>include langugage definition</b>
<br>
Change filename of the required language file to change the language
of the application. Language files have the name lang.xx.inc, wehre
xx stands for the abbreviation of the language (e.g. de for german,
en for english and so on). Look in your include directory to see
which languages are supported. Currently these are german (de) and english (en),
italian (it) and japanes (jp), but I hope, there will be other users who contribute
their languages too.
<br>
<br>
<b>PQL_SHOW_USERS</b>
<br>
(true or false). set to false, if the domain users should not be shown in the navigation
frame. this results in better performance on servers with large amount of users.
<br>
<br>
<b>PQL_AUTO_RELOAD</b>
<br>
(true or false). true, if the navigation bar should automatically be reloaded, if a domain
or user was added or deleted. with large amount of domains / users reloading the navigation
bar could get slow, at least if PQL_SHOW_USERS is turned on.
<br>
<br>
<b>PQL_HOSTMASTER</b>
<br>
(email address). this is the sender of testmails.
<br>
<br>
<b>PQL_LDAP_HOST</b>
<br>
(full qualified domain name, e.g ldap.foo.bar). the host which is running the
ldap-server for the users database.
<br>
<br>
<b>PQL_LDAP_BASEDN</b>
<br>
(valid dn). the dn path to the qmail-ldap database (same as the ldapbasedn file in
qmail/control directory).
<br>
<br>
<b>PQL_LDAP_ROOTDN</b> and <b>PQL_LDAP_ROOTPW</b>
<br>
(valid DN), (valid password in cleartext). the dn path to the root user and its
password of the user database. phpQLAdmin need these values to have write access
to the ldap database.
<br>
<br>
<b>PQL_LDAP_CONTROL_USE</b>
<br>
(true | false) set to true, if qmail-ldap/control patch is supported by your system. Make sure
you have complied all requirements listed in INSTALL. Configuration values which are beginning with
PQL_LDAP_CONTROL_ are only required if you need qmail-ldap/control support and set
PQL_LDAP_CONTROL_USE to true, otherwise they will be ignored.
<br>
<br>
<b>PQL_LDAP_CONTROL_HOST</b>
<br>
(valid FQDN). the host which is running the ldap-server for the control database
<br>
<br>
<b>PQL_LDAP_CONTROL_BASEDN</b>
<br>
(valid DN) the dn path to the qmail-ldap database (same as the ldapcontroldn file in
qmail/control directory).
<br>
<br>
<b>PQL_LDAP_CONTROL_ROOTDN</b> and <b>PQL_LDAP_CONTROL_ROOTPW</b>
<br>
(valid DN), (valid password in cleartext).the dn path to the root user and its password
of the control database. phpQLAdmin need these values to have write acces to the control database
<br>
<br>
<b>PQL_LDAP_CONTROL_AUTOADDLOCALS</b>
<br>
(true | false). set this to true, if phpQLAdmin should automatically add all domains to
locals. Disable this (false), if you have more than one qmail-ldap server
and don't want that phpQLAdmin register all domains on this server. Currently, the
application do not support more than one control database.
<br>
<br>
<b>PQL_LDAP_CONTROL_ME</b>
<br>
(valid FQDN, value in 'me') This MUST be equal to the value in the configuration file 'me'.
phpQLAdmin need this to display default values of configuration
vars, because many configuration files use this for their default
values if they are not present. phpQLAdmin can't lookup the file
dynamically, because it's not possible to store 'me' in LDAP.
<br>
<br>
<b>PQL_PW_HASH</b>
<br>
(CRYPT | CLEAR | SHA | MD5). defines the password encryption type which is used to
encrypt passwords in the ldap database. qmail/ldap is supporting SHA, MD5, CRYPT and cleartext
passwords (if compiled with cleartext password support). phpQLAdmin is supporting
all hashes, but for SHA and MD5 support you need to install PHP's mhash extension. PHP's native
MD5 Hash is not compatible with the required hashes of ldap. It's recommended that you use
SHA or MD5 if you have mhash support in your PHP engine, because crypt allows only 8 characters in
a password, others will be ignored. Avoid using cleartext passwords, they are not secured, and have
not support in qmail/ldap by default (can be changed only at compile time).
<br>
<br>
<b>PQL_ALLOW_ABSOLUTE_PATH</b>
<br>
(true | false). set this to true, if absolute paths are allowed to set
the mailbox directory.
<br>
<br>
<b>PQL_DEFAULT_PATH</b>
<br>
(valid path). the default path for mailbox when creating
a new user. if this path is not absolute (w/o trailing slash)
it will be prefixed by qmail-ldap with the value in qmail/control/ldapmessagestore
<br>
<br>
following variables are supported<br>
<ul>
 	<li>%d% --> domain
	<li>%u% --> username
	<li>%h% --> mailhost
</ul>
example:<br>
%d%/%u% means, the directory will be prefixed (e.g. /maildata/) and the resulting path
will be /maildata/domain/username

</body>
</html>
