<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: conf.php,v 2.8 2004-03-18 07:34:33 turbo Exp $ -->
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
with PQL_CONF_CONTROL_ are only required for qmail-ldap/control support within phpQLAdmin. You don't
have to set unless you want support for qmail-ldap/control.
<br>
<br>
<b>Langugage definition</b>
<br>
Change filename of the required language file to change the language
of the application. Language files have the name lang.xx.inc, where
xx stands for the abbreviation of the language (e.g. de for german,
en for english and so on). Look in your include directory to see
which languages are supported. Currently, only English (en) is supported, but I hope
there will be other users who contribute their languages.
<p>
If your language isn't supported, and you really want it in your language,
you will have to translate it yourself. Call the 'update_translations.php'
(from the left frame 'Home->Documentation->Language translator' URL visible
only in Advanced mode). Please send me the translation when your're done, so
others can benefit...
<p>
<b>PQL_CONF_HOST</b>
<br>
FQDN, Fully Qualified Domain Name (e.g ldap.foo.bar) to the host which is
running the LDAP server for the users database.
<br>
This value is multi-purposed, it can contain multiple hosts running LDAP servers
with different data (such as many ISP's databases). Each server is separated by
the plus (+) character.
<br>
In each server define, there is three fields separated by semicolon (;).<br>
<b>First field</b> is the hostname,<br>
<b>Second field</b> is the port number,<br>
<b>Third field</b> is the DN path to the qmail-ldap/controls database.
<p>
<u>Examples:</u><br>
<li>Two LDAP servers on separate ports and using separate TLD (bind without SSL,
one on standard port 389 and one on another port - 3389).
<pre>
	localhost;;ou=QmailLDAP,dc=example,dc=com+localhost;3389;ou=QmailLDAP,dc=domain,dc=net
</pre>
<li>Since the PHP LDAP module supports LDAP URIs, naturaly you could use that here as well
(note that the path to the socket <b>must</b> be URL encoded). Also note that this socket
must be readable and writable by the user Apache is running as.
<pre>
	ldapi://%2fvar%2frun%2fldapi;;ou=QmailLDAP,dc=example,dc=com
</pre>
<li>Using SSL is also possible. Take special care to make sure that the LDAP server acctually
works with SSL (certificate is ok and that you've configured slapd to listen on ldaps).
<pre>
	ldaps://server.example.com;;ou=QmailLDAP,dc=example,dc=com
</pre>
In both the <b>ldapi</b> and <b>ldaps</b> examples, the <u>port</u> value is pointless.<br>
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
<b>PQL_CONF_SUBTREE_USERS</b> and <b>PQL_CONF_SUBTREE_GROUPS</b>
<br>
Organizational unit below each top branch DN to store users and groups.
Example:
If your top DN is <i>dc=com</i> (see <i>namingContexts</i> above), your
branch object DN is <i>dc=example</i> (see QmailLDAP/Controls example above)
and you enter <b>ou=People</b> into the PQL_CONF_SUBTREE_USERS and
(optionally) <b>ou=Groups</b> into the PQL_CONF_SUBTREE_GROUPS, then this
would result in all users being stored below the following DN:
<pre>
ou=People,dc=example,dc=com
</pre>
<p>
<b>PQL_CONF_EZMLM_USER</b>
<br>
If you're using the ezmlm manager, you might have to configure
what user ezmlm is running as. That is, who "own's" (ie, the
directories) the mailinglist. This is usually 'alias', as in
'~alias/.qmail-LISTNAME' but can also be 'vmail'...
<p>
<b>Other</b>
<br>
that this, <b><u>NO</u></b> other values need to be entered
into the config.inc file! Everything else is <a href="../config_detail.php">
configured</a> from the user interface. Select 'Advanced mode' in the left
frame and unfold the <u>Home</u> branch. There you'll see <u>phpQLAdmin 
Configuration</u> (once as a branch name and once as a URL for a page).
</body>
</html>
