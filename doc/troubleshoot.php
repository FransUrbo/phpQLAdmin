<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: troubleshoot.php,v 2.0 2002-12-13 14:34:47 turbo Exp $ -->
</head>

<body bgcolor="#e7e7e7" background="../images/bkg.png">
<span class="title1">phpQLAdmin Documentation</span>
<br>
<br>
<a href="index.php">back to doc...</a>
<br>
<br>
<span class="title2">Troubleshooting</span>
<br>
<br>
The most common errors with phpQLAdmin are listed here. See also in the <a href="faq.php">FAQ</a> if there's a
solution for your problem. If you are not sure if a problem is caused by phpQLAdmin or your by ldap server try
to connect using another ldap client like <a href="http://www.mountpoint.ch/~oliver/kldap/" target="_new">kldap</a>
or <a href="http://biot.com/gq/" target="_new">GQ</a>.
<a href="http://www.OpenLDAP.org/doc/admin/">OpenLDAP's Administrator Guide</a> may be helpful too. Regarding to installation
issues of qmail-ldap and qmail-ldap/control, <a href="http://www.lifewithqmail.org/ldap/">life with qmail-ldap</a> is a very good place.
<br>
<br>
<table cellspacing="0" cellpadding="3" border="0">
	<tr>
  	<td class="title">Error</td>
		<td class="title">Reason</td>
  	<td class="title">Possible solution</td>
	</tr>
  <tr class="c1" valign="top">
    <td>could not connect to ldap server</td>
    <td>it's not possible to initialize a connection to ldap server.</td>
		<td>make sure your ldap server is running, and your webserver which is
		running phpQLAdmin has the permission to connect to.</td>
  </tr>
  <tr class="c2" valign="top">
    <td>could not bind to ldap server: Invalid credentials</td>
    <td>phpQLAdmin could not find the credentials for binding to the ldap server.</td>
		<td>check PQL_LDAP_ROOTDN in your config.inc if it contains a valid root dn.
		If you are not sure to which value PQL_LDAP_ROOTDN is set, look at the 'Show configuration' page.</td>
  </tr>
  <tr class="c1" valign="top">
    <td>could not bind to ldap server: Inappropriate authentication</td>
    <td>the password stored in config.inc does not match the password in ldap database</td>
		<td>make sure PQL_LDAP_ROOTPW is correctly set in config.inc and the password in slapd.conf (for OpenLDAP) is
		correctly typed. If the password in config.inc is set correctly, try to connect using another ldap client.</td>
  </tr>
	<tr class="c2" valign="top">
    <td>LDAP Error: no such object</td>
    <td>This error is typically caused by an invalid base dn.</td>
		<td>check PQL_LDAP_BASEDN in config.inc if it is correctly set. PQL_LDAP_BASEDN must be a valid dn of your ldap server
		and have to point to the base dn of your qmail-ldap database. If this error raised in the control section of phpQLAdmin
		check PQL_LDAP_CONTROL_BASEDN in config.inc</td>
  </tr>
  <tr class="c1" valign="top">
    <td>Failed setting xxx: Insufficient access</td>
    <td>You don't have root permissions on ldap database.</td>
		<td>PQL_LDAP_ROOTDN is not pointing to the dn of root or a user with equivalent permissions in the ldap database. phpQLAdmin needs
		rights to create, modify and delete entries within the ldap database.</td>
  </tr>
	<tr class="c2" valign="top">
    <td>Failed saving xxx: Object violation</td>
    <td>This error is typically caused within control database. The objectclasses within control tree are not well defined.</td>
		<td>To store control attributes, the ldap server must have defined the objectclass 'qmailcontrol' and the
		tree which is holding the attributes must have the attribute 'objectlass=qmailcontrol'. Else, the ldap server is not
		able to store the control attributes.</td>
  </tr>
</table>
<br>
<br>
Please <a href="mailto:opensource@adfinis.com">contact us</a> if you found a bug or have seen another error we can describe here.

</body>
</html>
