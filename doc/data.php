<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../templates/<?=pql_get_define("PQL_CONF_GUI_TEMPLATE")?>/normal.css" type="text/css">
	<!-- $Id: data.php,v 2.2.12.2 2006-03-23 09:13:10 turbo Exp $ -->
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
+- ou=Organization number 1
|  |
|  +- cn=Zaphod Beeblebrox
|  |  |
|  |  + all attributes of this user (mail, uid, mailalternateaddress...)
|  |
|  +- cn=Arthur Dent
|     |
|     +- all attributes of this user (mail, uid, mailalternateaddress...)
|
+- ou=Organization number 2
|  |
|  +- cn=Ford Prefect
|     |
|     +- all attributes of this user (mail, uid, mailalternateaddress...)
|
+- ou=Organization number 3
   |
   +- ou=People
   |  |
   |  +- user xyz
   |
   +- ou=Groups
      |
      +- group xyz
</pre>
<br>
<br>
qmail-ldap/control needs to have one single tree to store its data. This can be within
the users database below an ou, cn or whatever-record or in a seperate database (recommended).
You have to give the path of the control tree where all your QmailLDAP/Controls objects
are located (e.g. <u>ou=Control,o=adfinis,c=CH</u>). Be sure, that the server supports the
qmailControl objectclass.
<p>
<pre>
LDAP database root (base dn) of control database (with cn=fqdn)
|
+ ldapbasedn = base dn of users database
|
+ ldapserver = host address of users database
|
+ many_more_attributes = value
</pre>
</body>
</html>
