<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: todo.php,v 1.1 2001/08/29 14:09:27 mb Exp $ -->
</head>

<body bgcolor="#e7e7e7" background="../images/bkg.png">
<span class="title1">phpQLAdmin Documentation</span>
<br>
<br>
<a href="index.php">back to doc...</a>
<br>
<br>
<span class="title2">Todo</span>
<br>
<br>
<b>planned for some of the next releases:</b>
<ul>
	<li>support for alias-domains: phpQLAdmin will be able to create
		domain aliases e.g domain.ch will forward all registred accounts to the equal account of domain.com. changes in the aliased domain
		will be automatically replicated to the alias domain.
  <li>better interaction between user administration & server control
	<li>import of available configuration files to qmail-ldap/control via phpQLAdmin
	<li>configurable attribute names (currently, only the default qmail/ldap attributes are supported)
	<li>moving most of phpQLAdmin configuration stuff to the ldap database
	<li>support for new attribute qmailAccountPurge
	<li>store uid as cn, store firstname and lastname seperately
	<li>support for all user attributes
	<li>support for catchall address
</ul>
<br>
<br>
<b>not yet scheduled:</b>
<ul>
	<li>Mailinglist integration into phpQLAdmin (maybe <a href="http://cr.yp.to/ezmlm.html">EZMLM</a> or <a href="http://www.sympa.org">Sympa</a>)
	<li>restricted access to ldap server (maybe an administrator for each domain)
	<li>support for user templates (predefined attributes for a group of users)
</ul>

</body>
</html>
