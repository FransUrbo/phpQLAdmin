<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: data.php,v 2.0 2002-12-13 14:34:47 turbo Exp $ -->
</head>

<body bgcolor="#e7e7e7" background="../images/bkg.png">
<span class="title1">phpQLAdmin Documentation</span>
<br>
<br>
<a href="index.php">back to doc...</a>
<br>
<br>
<span class="title2">How phpQLAdmin handles ldap data</span>
<br>
<br>
qmail-ldap needs to have all data of the users stored in a single ldap database,
but does not define how to organize the data within, because qmail-ldap searches
the whole database when looking up for an email address or user id. phpQLAdmin
organizes the different domains with OrganizationalUnits (ou objectClass) and add
the users below this ou's. So, a user must be unique within a domain (exactly:
the combination of firstname and name, e.g. 'Arthur Dent'), because it will be
generated a canonical name (cn objectClass) for each user.
<br>
<br>
<pre>
LDAP database root (base dn)
 |
 + ou=domain.tld
 |   |
 |   +- cn=Zaphod Beeblebrox
 |   |     |
 |   |     + all attributes of this user (mail, uid, mailalternateaddress...)
 |   |
 |   +- cn=Arthur Dent
 |         |
 |         + all attributes of this user (mail, uid, mailalternateaddress...)
 |
 + ou=otherdomain.tld
     |
     +- cn=Ford Prefect
           |
           + all attributes of this user (mail, uid, mailalternateaddress...)
</pre>
<br>
<br>
qmail-ldap/control needs to have one single tree to store its data. This can be within
the users database below an ou, cn or whatever-record, or in a seperate database (recommended).
You have to give the full path to the location of the control tree (with cn=fqdn.of.mailserver, eg: cn=mail.adfinis.com,ou=Control, o=adfinis, c=CH). 
Be sure, that the server supports the qmailcontrol objectclass.
<br>
<br>
<pre>
LDAP database root (base dn) of control database (with cn=fqdn)
 |
 + ldapbasedn = base dn of users database
 |
 + ldapserver = host address of users database
 |
 + many_more_attributes = value

</pre>
<br>
<br>

</body>
</html>
