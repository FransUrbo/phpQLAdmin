<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="../normal.css" type="text/css">
	<!-- $Id: troubleshoot.php,v 2.8 2006-12-16 12:02:12 turbo Exp $ -->
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
<a href="http://www.OpenLDAP.org/doc/admin/" target="_new">OpenLDAP's Administrator Guide</a> may be helpful too. Regarding to installation
issues of qmail-ldap and qmail-ldap/control, <a href="http://www.lifewithqmail.org/ldap/" target="_new">life with qmail-ldap</a> is a very good place.
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
    <td>It's not possible to initialize a connection to ldap server.</td>
    <td>Make sure your ldap server is running, and your webserver which is
        running phpQLAdmin has the permission to connect to.
    </td>
  </tr>

  <tr class="c2" valign="top">
    <td>could not bind to ldap server: Invalid credentials</td>
    <td>phpQLAdmin could not find the credentials for binding to the ldap server.</td>
    <td>Does your DN have access to connect to the LDAP server? When you login, phpQLAdmin
        will find your RDN in the database, and use this to connect to the database. If
        this RDN doesn't have access to the database, then it will be denied.<br>
        All access control is done at the LDAP server, NONE in phpQLAdmin
    <td>
  </tr>

  <tr class="c2" valign="top">
    <td>LDAP Error: no such object</td>
    <td>This error is typically caused by an invalid base dn.</td>
    <td>Check that the <u>third</u> field in PQL_CONF_HOST (in config.inc) is
        correctly set. The base dn must be a valid dn of your ldap server and
        have to point to the base dn of your qmail-ldap database. If this error
        raised in the control section of phpQLAdmin, then check the <u>fourth</u>
        field in the PQL_CONF_HOST define
    </td>
  </tr>

  <tr class="c1" valign="top">
    <td>Failed setting xxx: Insufficient access</td>
    <td>You don't have appropriate permissions on ldap database.</td>
    <td>Your DN does not have access to read, write and/or modify the LDAP database.
        phpQLAdmin needs rights to create, modify and delete entries within the ldap
        database, and this is controled at the LDAP server en by the DN of the user
        logged in.
    </td>
  </tr>

  <tr class="c2" valign="top">
    <td>Failed saving xxx: Object violation</td>
    <td>This error is typically caused within control database. The objectclasses within
        control tree are not well defined.
    </td>
    <td>To store control attributes, the ldap server must have defined the objectclass
        'qmailcontrol' and the tree which is holding the attributes must have the
        attribute 'objectlass=qmailcontrol'. Else, the ldap server is not able to store
        the control attributes.
    </td>
  </tr>

  <tr class="c1" valign="top">
    <td>Could not bind to ldap server: Protocol error</td>
    <td>The LDAP server does not allow a LDAPv2 bind</td>
    <td>The PHP/LDAP module does an LDAPv2 bind, which means that you will
        have to allow a version 2 bind. In OpenLDAP version 2.0, this is done
        by making sure that you DON'T (!) have <b>disallow bind_v2</b> in your
        <i>slapd.conf</i> file. In OpenLDAP version 2.1, disallowing v2 bind is the
        default and you will have to enable v2 bind by putting <b>allow bind_v2</b>
        in the slapd.conf file.
    </td>
  </tr>

  <tr class="c2" valign="top">
    <td>Could not bind to ldap server: Can't contact LDAP server</td>
    <td>Miss configuration or ACI/ACL problems</td>
    <td>Either you've specified the wrong host/URI in the <b>PQL_CONF_HOST</b>
        define or the LDAP server isn't listening on that URI. For example, if
        you try to use SSL (with the keyword <b>https://</b>), but your LDAP
        server isn't correctly configured for this (the SSL certificate isn't
        allowing the FQDN you call with etc) then you'll get this problem.<br>
        The fix is to double check with the command line tool (or another LDAP
        client) that came with your LDAP server to see where the problem lies.
    </td>
  </tr>

  <tr class="c1" valign="top">
    <td>Object class violation</td>
    <td>Miss configuration</td>
    <td>Have you checked the <u>HOME->phpQLAdmin Configuration-><i>branch</i>->{User,Domain} objectclasses</u>
        setup for missing object classes? There's rudimentary support for adding
        missing object classes and attributes automatically, but there's no way
        I'll be able to catch everything...
    </td>
  </tr>

  <tr class="c2" valign="top">
    <td>This is weird. We're called (from <i>faulty function</i>) with an empty DN</td>
    <td>Bug in phpQLAdmin</td>
    <td>See below</td>
  </tr>

  <tr class="c1" valign="top">
    <td>This is weird. We couldn't find the root dn for some reason</td>
    <td>Bug in phpQLAdmin</td>
    <td>See below</td>
  </tr>

  <tr class="c2" valign="top">
    <td>Supplied argument is not a valid ldap result resource</td>
    <td>Bug in phpQLAdmin</td>
    <td>This usually happens when a previous search in the code failed, but phpQLAdmin
        could not deal with this. See below.
    </td>
  </tr>

  <tr class="c1" valign="top">
    <td>Document contains no data</td>
    <td>Incorrect PHP/PHP-LDAP installation</td>
    <td>Please reinstall the ldap libraries and rebuild PHP. If it still no work, try to ask
        on the phpQLAdmin or the PHP mailinglist(s).
    </td>
  </tr>
</table>
<br>
<br>
Please <a href="mailto:phpqladmin@bayour.com">contact us</a> if you found a bug or have seen another error we can describe here.
<p>
<b>For bugs in phpQLAdmin:</b>
There's not much you can do (unless you're a PHP coder) other than reporting the
bug (at the <a href="http://apache.bayour.com/anthill/" target="_new">bug tracker</a>
<i>please</i>, including the exact ways to reproduce the problem. Everything from
the URL you clicked on in the left frame all the way up to where you got the problem.
</body>
</html>
