<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: troubleshoot.php,v 2.2 2003-04-04 06:54:09 turbo Exp $ -->
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
		<td>Does your DN have access to connect to the LDAP server? When you login, phpQLAdmin will find your RDN in the database, and use this to connect to the database. If this RDN doesn't have access to the database, then it will be denied.<br>
		All access control is done at the LDAP server, NONE in phpQLAdmin<td>
  </tr>
	<tr class="c2" valign="top">
    <td>LDAP Error: no such object</td>
    <td>This error is typically caused by an invalid base dn.</td>
		<td>check that the <u>third</u> field in PQL_CONF_HOST (in config.inc) is correctly set. The base dn must be a valid dn of your ldap server and have to point to the base dn of your qmail-ldap database. If this error raised in the control section of phpQLAdmin, then check the <u>fourth</u> field in the PQL_CONF_HOST define</td>
  </tr>
  <tr class="c1" valign="top">
    <td>Failed setting xxx: Insufficient access</td>
    <td>You don't have appropriate permissions on ldap database.</td>
		<td>Your DN does not have access to read, write and/or modify the LDAP database. phpQLAdmin needs
		rights to create, modify and delete entries within the ldap database, and this is controled at the LDAP server en by the DN of the user logged in.</td>
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
Please <a href="mailto:phpqladmin@bayour.com">contact us</a> if you found a bug or have seen another error we can describe here.

</body>
</html>
