<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: faq.php,v 2.1 2004-03-16 10:54:26 turbo Exp $ -->
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
<b>Can I add an email address of a different domain for a user ?</b><br>
It is possible to have <b>one</b> default domain, and unlimited amounts of additional 
domains for each branch. When creating a user, you will be given a choice of what 
email address domain that the user should have as the <i>default</i> email address. 
Additional email addresses can be added in the user details page.
<br>
If neither of the shown email domains is suitable for the user, you can override the 
domain value by entering the full email address in the input field, so you <b>can</b> 
enter any email address you choose for the user. phpQLAdmin will however double check 
to see that this email address isn't already taken - you can't have two users with the 
same email address.
<br>
If you want multiple recipients for one email address, create a special <u>forwarding 
account</u>, and then forward the mails to all the recipients. 
<p>
<b>Can I add a subdomains too (not only domains) ?</b><br>
Yes, you can. This issue is was a bug in phpQLAdmin 0.9beta, and was fixed in the next
higher release. Please update your application to a new release.
<p>
<b>Where can I find informations about qmail-ldap and qmail-ldap/control ?</b><br>
On our project page are some links in the link section. <a href="http://www.lifewithqmail.org/ldap/" target="_new">
Life with qmail-ldap</a> is a very good documentation on how to install an manage 
qmail-ldap.
<p>
<b>On your server some special chars work to name a user, on my system it fails.</b><br>
On my servers I use OpenLDAP 2.0.27 and 2.1.xx as server daemon. It was reported that OpenLDAP
2.0.x is more restrictive with special chars. phpQLAdmin 2.x works perfectly with both
OpenLDAP 2.0 and 2.1.
<p>
<b>When should I use SHA or MD5 password encryption ?</b><br>
Crypt cannot handle passwords longer than eight chars (<u>12345678xxx</u> matches the password
<u>12345678</u> stored in the user database). If you want to use longer passwords for sure, you
may use SHA or MD5. Unfortunately, LDAP does not support salted SHA / MD5, so the hash is
the same for every call of the encryption function.
<p>
<b>I'm getting the error "Object class violation" when saving control stuff</b><br>
The qmail-ldap/control database must support the qmailcontrol objectclass and within this
objectclass all qmail control files have to be registred (eg. if the control file has the 
name 'ldaplogin', there must be a registred attribute 'ldaplogin' within qmailcontrol 
objectclass). Otherwise, you can't read / store the attributes, and qmail-ldap/control 
will not work.
<p>
<b>I'm getting the error "called with an empty DN"</b><br>
TODO
<p>
<b>I'm getting the error "Could not bind to ldap server: Can't contact LDAP server"</b><br>
TODO
<p>
<b>Netscape says 'Document contains no data' when requesting phpQLAdmin</b><br>
This error is mostly caused when PHP does not have the LDAP extension installed. Please check the phpinfo() function
to see if the LDAP extension is there, it should be printed something like this:<br>
<h2 align="center"><a NAME="module_ldap">ldap</a></h2>
<table BORDER=0 CELLPADDING=3 CELLSPACING=1 WIDTH=600 BGCOLOR="#000000" ALIGN="CENTER">
  <tr VALIGN="baseline" BGCOLOR="#CCCCCC">
    <td BGCOLOR="#CCCCFF" ><b>LDAP Support</b></td>
    <td ALIGN="left">enabled</td>
  </tr>
</table>
<br>
If this is there, but the error occurs however, PHP seems to crash when executing an ldap function. Try to reinstall the ldap libraries and
rebuild PHP. If it still no work, try to ask in a the phpQLAdmin or a PHP mailinglist.
</body>
</html>
