<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: faq.php,v 2.0 2002-12-13 14:34:47 turbo Exp $ -->
</head>

<body bgcolor="#e7e7e7" background="../images/bkg.png">
<span class="title1">phpQLAdmin Documentation</span>
<br>
<br>
<a href="index.php">back to doc...</a>
<br>
<br>
<span class="title2">Frequently asked questions</span>
<br>
<br>
<b>Can I add an email address of a different domain for a user ?</b>
<br>
No, unfortunately this is not possible, phpQLAdmin strictly adds only addresses
for the current domain. But if you want to have mails for a different domain in your
mailbox, you can add a forward user there, and forward all mails to your mail account.
Sure, technically it's possible to add an address of another domain, but then it
would make no sense to organize the domains.
<br>
<br>
<b>Can I add a subdomains too (not only domains) ?</b>
<br>
Yes, you can. This issue is was a bug in phpQLAdmin 0.9beta, and was fixed in the next
higher release. Please update your application to a new release.
<br>
<br>
<b>Where can I find informations about qmail-ldap and qmail-ldap/control ?</b>
<br>
On our project page are some links in the link section. "life with qmail-ldap" is a
very good documentation on how to install an manage qmail-ldap and has a section about
qmail-ldap/control too.
<br>
<br>
<b>On your server some special chars work to name a user, on my system it fails.</b>
<br>
On our servers we use OpenLDAP 1.2.11 as server daemon. It was reported that OpenLDAP
2.0.x is more restrictive with special chars. Unfortunately, we do not have any solution yet.
<br>
<br>
<b>When should I use SHA or MD5 password encryption ?</b>
<br>
Crypt cannot handle passwords longer than eight chars (12345678xxx matches the password
12345678 stored in the user database). If you want to use longer passwords for sure, you
may use SHA or MD5. Unfortunately, LDAP does not support salted SHA / MD5, so the hash is
the same for every call of the encryption function.
<br>
<br>
<b>I'm getting the error "Object class violation" when saving control stuff</b>
<br>
The qmail-ldap/control database must support the qmailcontrol objectclass and within this objectclass
all qmail control files have to be registred (eg. if the control file has the name 'ldaplogin', there must
be a registred attribute 'ldaplogin' within qmailcontrol objectclass). Otherwise, you can't read / store
the attributes, and qmail-ldap/control will not work.
<br>
<br>
<b>I'm getting 404 error messages when I try to edit or add something on the users page</b>
<br>
In phpQLAdmin 0.9beta and partially in the 1.0 revision there are some linking bugs on the users detail page. Please update your
copy to release 1.1 or higher. For apache users there's a little workaround: you can activate the MultiViews option in the directory,
and then phpQLAdmin should work now as expected.
<br>
<br>
<b>Netscape says 'Document contains no data' when requesting phpQLAdmin</b>
<br>
This error is mostly caused when PHP does not have the LDAP extension installed. Please check the phpinfo() function
to see if the LDAP extension is there, it should be printed something like this:<br>
<h2 align="center"><a NAME="module_ldap">ldap</a></h2>
<table BORDER=0 CELLPADDING=3 CELLSPACING=1 WIDTH=600 BGCOLOR="#000000" ALIGN="CENTER">
<tr VALIGN="baseline" BGCOLOR="#CCCCCC"><td BGCOLOR="#CCCCFF" ><b>LDAP Support</b></td><td ALIGN="left">enabled</td></tr>
<tr VALIGN="baseline" BGCOLOR="#CCCCCC"><td BGCOLOR="#CCCCFF" ><b>RCS Version</b></td><td ALIGN="left">$Id: faq.php,v 2.0 2002-12-13 14:34:47 turbo Exp $</td></tr>
<tr VALIGN="baseline" BGCOLOR="#CCCCCC"><td BGCOLOR="#CCCCFF" ><b>Total Links</b></td><td ALIGN="left">0/unlimited</td></tr>
</table>
<br>
If this is there, but the error occurs however, PHP seems to crash when executing an ldap function. Try to reinstall the ldap libraries and
rebuild PHP. If it still no work, try to ask in a the phpQLAdmin or a PHP mailinglist.


</body>
</html>
