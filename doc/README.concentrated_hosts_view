The special thing about this branch is that I
took version 2.3.5(CVS) as of 2006-11-12 to try
to merge the different host entries described
in the following bug reports:

312    http://bugs.bayour.com/query.php?bug=312
308    http://bugs.bayour.com/query.php?bug=308

The idea I'm trying to do here is to merge the
QmailLDAP/Controls objects/branch/frame and the
webserver manager into ONE frame.

This is the basic idea for the 'Hosts' frame
(previosly named 'QmailLDAP/Controls') frame:

----- s n i p -----
Hosts
Add host
+ host1.domain.tld
  Add QmailLDAP/Controls object
  Add Webserver object
  + QmailLDAP/Controls
    > Accepted domains
    > LDAP Clustering 
    > LDAP Core 
    > Mail Bouncing 
    > Mail Deliveries 
    > Mail Preprocessing 
    > Mail Queueing 
    > Mail Relaying 
    > POP before SMTP 
    > Simscan Setup 
    > Spam Control 
    > Virtual User 
    > qmail-remote 
    > qmail-send 
    > qmail-smtpd
  + Webservers
    > Virtualhost1
    > Virtualhost2
      ....
  + Access Control
    > user1
    > user2
      ....
----- s n i p -----


And the LDAP database will have to be modified in the
following maner:

From this:
----- s n i p -----
+ dc=tld
  + dc=domain
    + ou=QmailLDAP/Controls
      > cn=host1.domain.tld
      > cn=host2.domain.tld
        ....
    + ou=Web
      + cn=host1.domain.tld
        + ApacheServerName=virthost1.domain.tld
	  > ApacheSectionArg=/doc
	    ....
        + ApacheServerName=virthost2.domain.tld
	  ....
      + cn=host2.domain.tld
        + ApacheServerName=virthost3.domain.tld
        + ApacheServerName=virthost4.domain.tld
        ....
    + ou=Computers
      > cn=host1.domain.tld
      > cn=host2.domain.tld
        ....
----- s n i p -----

To something like this:
----- s n i p -----
+ dc=tld
  + dc=domain
    + ou=Computers
      + cn=host1.domain.tld	 [*1]
        > user1			 [*2]
	> user2
	  ....
        + cn=host1.domain.tld	 [*3]
        + cn=host1.domain.tld:80 [*4]
          + <Virtualhost1>	 [*5]
	    > ApacheSectionArg
	  + <Virtualhost2>
	    ....
----- s n i p -----

*1: Simple 'device' container:

    ----- s n i p -----
    dn: cn=host1.domain.tld,ou=Computers,dc=domain,dc=tld
    objectClass: device
    cn: host1.domain.tld
    ----- s n i p -----

*2: If user is added to host, the *1 is changed into this:

    ----- s n i p -----
    dn: cn=host1.domain.tld,ou=Computers,dc=domain,dc=tld
    cn: host1.domain.tld
    objectClass: ipHost
    objectClass: groupOfUniqueNames
    ipHostNumber: 192.168.1.9
    uniqueMember: uid=turbo,ou=people,dc=domain,dc=tld
    ----- s n i p -----

*3: The QLC object needs to be a separate object because
    it's not possible to merge the 'groupOfUniqueNames'
    (for Host ACLs) object class with the 'qmailControl'
    (for QmailLDAP/Controls) object class. They are both
    STRUCTURAL.

*4: Since the port needs to be (or should be) specified
    in a webserver container, we make a separate subbranch
    for the webserver containers. This is just a simple
    'device' object:

    ----- s n i p -----
    dn: cn=host1.domain.tld:80,cn=host1.domain.tld,ou=Computers,dc=domain,dc=tld
    objectClass: device
    cn: host1.domain.tld:80
    ----- s n i p -----

    NOTE: This isn't as pretty as I'd like, because this object
	  looks almost identical to the one in *1 (exept the
	  port).

*5: Any virtual host is added _below_ the webserver
    container object. Example:

    ----- s n i p -----
    dn: ApacheServerName=virthost1.domain.tld,cn=host1.domain.tld:80,cn=host1.domain.tld,ou=Computers,dc=domain,dc=tld
    objectClass: device
    objectClass: ApacheSectionObj
    objectClass: ApacheVirtualHostObj
    objectClass: ApacheModLogConfigObj
    cn: virthost1.domain.tld
    ApacheServerName: host1.domain.tld
    ApacheSectionName: VirtualHost
    ApacheSectionArg: 192.168.1.9:80
    ApacheServerAdmin: turbo@bayour.com
    ApacheErrorLog: /var/log/apache/apache-error.log
    ApacheTransferLog: /var/log/apache/apache-access.log
    ApacheDocumentRoot: /var/www-ldap
    ----- s n i p -----

    This would result in the following LDAP tree:

    + dc=tld
      + dc=domain
        + ou=Computers
          + cn=host1.domain.tld
            + cn=host1.domain.tld:80
              + ApacheServerName=virthost1.domain.tld
