<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: conf.php,v 2.2 2003-02-14 10:10:31 turbo Exp $ -->
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
have to set unless you want support for qmail-ldap/control.
<br>
<br>
<b>include langugage definition</b>
<br>
Change filename of the required language file to change the language
of the application. Language files have the name lang.xx.inc, where
xx stands for the abbreviation of the language (e.g. de for german,
en for english and so on). Look in your include directory to see
which languages are supported. Currently these are german (de) and english (en),
italian (it) and japanes (jp), but I hope, there will be other users who contribute
their languages too.
<br>
These files have been hoplessly left behind while working on version 2.0, the only
language that's been keept up to date is the english one. Also, there's quite a lot
of stuff hardcoded in the PHP files.
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
ldap-server for the users database.<p>
In version 2.x, this define is multi-purposed. It is multiple hosts running
LDAP servers (database layout must match though) with different data (such as
many ISP's databases). Each server is separated by space.<p>
In each server define, there is three fields separated by semicolon (;).<br>
<b>First field</b> is the hostname,<br>
<b>Second field</b> is the port number,<br>
<b>Third field</b> is the DN path to the qmail-ldap/controls database.<br>
<u>Example:</u> Two LDAP servers on separate ports and using separate TLD.
<pre>
localhost;389;ou=QmailLDAP,dc=example,dc=com localhost;3389;ou=QmailLDAP,dc=domain,dc=net
</pre>
As you might notice (especially if you're used to older versions of this
software) that the Base DN is no more. It is found by doing a search for
the naming context of the LDAP server. An LDAP search command would look like
this:
<pre>
[papadoc.pts/3]$ ldapsearch -x -h localhost -p 389 -b '' -s base objectclass=* namingContexts
dn:
namingContexts: dc=com
</pre>
This say that my base DN (what I've entered as <b>defaultsearchbase</b> and <b>suffix</b>
configuration in my slapd.conf file) is <u>dc=com</u>.
<p>
<b>PQL_LDAP_CONTROL_USE</b>
<br>
(true | false) set to true, if qmail-ldap/control patch is supported by your system. Make sure
you have complied all requirements listed in INSTALL. Configuration values which are beginning with
PQL_LDAP_CONTROL_ are only required if you need qmail-ldap/control support and set
PQL_LDAP_CONTROL_USE to true, otherwise they will be ignored.
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
<b>PQL_PASSWORD_SCHEMES</b>
<br>
(CRYPT | CLEAR | SHA | MD5 | KERBEROS). defines the password encryption type which is allowd to
encrypt passwords in the ldap database. qmail/ldap is supporting SHA, MD5, CRYPT and cleartext
passwords (if compiled with cleartext password support). phpQLAdmin is supporting
all hashes, but for SHA and MD5 support you need to install PHP's mhash extension. PHP's native
MD5 Hash is not compatible with the required hashes of ldap. It's recommended that you use
SHA or MD5 if you have mhash support in your PHP engine, because crypt allows only 8 characters in
a password, others will be ignored. Avoid using cleartext passwords, they are not secured, and have
not support in qmail/ldap by default (can be changed only at compile time).
<br>
You can define all of them, separated with comma (,) and you'll get a selectable list of these
when creating a user.
<br>
<br>
<b>PQL_ALLOW_ABSOLUTE_PATH</b>
<br>
(true | false). set this to true, if absolute paths are allowed to set
the mailbox directory.
</body>
</html>
