#!/usr/bin/perl -w

# Hostname (FQDN or IP) to your LDAP server
$LDAPSERVER = "localhost";

# Your base dn(s)
@BASEDN = ('c=SE', 'dc=com');

# If your OpenLDAP server is _OLDER_ than 2.1.22,
# then enable (set it to one - 1).
$OLD_SLAPD = 0;

# Keep it simple - allow write access to all
# attributes by specified dn.
$RIGHTS		= "grant;w,r,s,c,x;[all]";
$SUBJECT	= "cn=Turbo Fredriksson,ou=People,o=Fredriksson,c=SE";

# ========= D O N T  C H A N G E  A N Y T H I N G  B E L O W =========

# --> Get all objects in the database!
$i = 0; $j = 0; # Counters, don't touch!
foreach $base (@BASEDN) {
    open(SEARCH, "ldapsearch -LLL -h $LDAPSERVER -b '$base' objectClass |")
	|| die("Can't search, $!\n");
    while(! eof(SEARCH)) {
	$line = <SEARCH>; chomp($line);
	if($line =~ /^$/) {
	    $j = 0;
	    next;
	}
	
	if($line =~ /^dn: /) {
	    $line =~ s/dn: //;

	    $DNs[$i] = $line; $i++;
	} elsif($line =~ /^ /) {
	    $line =~ s/^ //;

	    $DNs[$i-1] .= $line;
	} elsif($line =~ /^objectClass/i) {
	    $line =~ s/objectClass: //i;

	    if($line ne 'top') {
		# Don't ad the 'top' objectclass!
		$OCs{$DNs[$i-1]}[$j] = $line; $j++;
	    }
	}
    }
    close(SEARCH);
}

# --> Go through all objects
foreach $dn (@DNs) {
    $phpQLAdminAttribs = '';

    print "dn: " . $dn . "\n";

    if($OLD_SLAPD) {
	# Add the original object classes from the object
	print "replace: objectClass\n";
	for($i=0; $OCs{$dn}[$i]; $i++) {
	    print "objectClass: $OCs{$dn}[$i]\n";
	}
	
	# Add the new object class - openLDAPacl
	print "objectClass: openLDAPacl\n";

	print "-\n";
    }

    # Add the ACI attribute - openLDAPaci
    # These lines will create the following into EVERY object:
    #	OpenLDAPaci: 1.2.3#entry#grant;r;[entry]#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;r,s,c;objectClass,[entry]#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;r,s,c;c,userReference,branchReference,administrator#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;w,r,s,c;[children]#access-id#cn=Turbo Fredriksson,ou=People,o=Fredriksson,c=SE
    #	OpenLDAPaci: 1.2.3#entry#grant;w,r,s,c,x;[all]#access-id#cn=Turbo Fredriksson,ou=People,o=Fredriksson,c=SE
    #
    # Second line depends on what type of object it is, and if it's a
    # top DN or not (if it is, some special phpQLAdmin attribs must be
    # publicly readable)...
    #
    # A 'user' object is very special. It will look like something like this:
    #	OpenLDAPaci: 1.2.3#entry#grant;r;[entry]#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;r,s,c;objectClass,[entry]#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;x;userPassword,krb5PrincipalName#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;r,s,c;uid,cn,accountStatus,uidNumber,gidNumber,gecos,homeDirectory,loginShell#public#
    #	OpenLDAPaci: 1.2.3#entry#grant;r,s,c;sn,givenName,homePostalAddress,mobile,homePhone,labeledURI,mailForwardingAddress,
    #	 street,physicalDeliveryOfficeName,mailMessageStore,o,l,st,telephoneNumber,postalCode,title#users#
    #	OpenLDAPaci: 1.2.3#entry#grant;r,s,c;sn,givenName,homePostalAddress,mobile,homePhone,labeledURI,mailForwardingAddress,
    #	 street,physicalDeliveryOfficeName,mailMessageStore,o,l,st,telephoneNumber,postalCode,title#self#
    #	OpenLDAPaci: 1.2.3#entry#grant;w,r,s,c;[children]#access-id#cn=Turbo Fredriksson,ou=People,o=Fredriksson,c=SE
    #	OpenLDAPaci: 1.2.3#entry#grant;w,r,s,c,x;[all]#access-id#cn=Turbo Fredriksson,ou=People,o=Fredriksson,c=SE

    print "replace: OpenLDAPaci\n";
    print "OpenLDAPaci: 1.2.3#entry#grant;r;[entry]#public#\n";
    print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;objectClass,[entry]#public#\n";
    
    if($dn !~ /\,/) {
	# Top DN - make sure some phpQLAdmin attribs are readable
	$phpQLAdminAttribs = ",userReference,branchReference,administrator";
    }
    
    if($dn =~ /^dc/) {
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;dc$phpQLAdminAttribs#public#\n";
    } elsif(($dn =~ /^c/) && ($dn !~ /^cn/)) {
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;c$phpQLAdminAttribs#public#\n";
    } elsif(($dn =~ /^uid/) || ($dn =~ /^cn/)) {
	# This is (most likley!) a user object. Special circumstances!

	# userPassword and krb5PrincipalName needs 'auth' privileges
	print "OpenLDAPaci: 1.2.3#entry#grant;x;userPassword,krb5PrincipalName$phpQLAdminAttribs#public#\n";

	# Some things must be anonymously/publicly readable to be able to allow the user to login
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;uid,cn,accountStatus,uidNumber,gidNumber,gecos,";
	print "homeDirectory,loginShell$phpQLAdminAttribs#public#\n";

	# Some values should be readable by authenticated users
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;sn,givenName,homePostalAddress,mobile,homePhone,";
	print "labeledURI,mailForwardingAddress,street,physicalDeliveryOfficeName,mailMessageStore,o,l,";
	print "st,telephoneNumber,postalCode,title#users#\n";

	# Same values, but writable to 'self' ('owner' of the object)
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;sn,givenName,homePostalAddress,mobile,homePhone,";
	print "labeledURI,mailForwardingAddress,street,physicalDeliveryOfficeName,mailMessageStore,o,l,";
	print "st,telephoneNumber,postalCode,title#self#\n";
    } elsif($dn =~ /^o/) {
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;o$phpQLAdminAttribs#public#\n";
    } elsif($dn =~ /^ou/) {
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;ou$phpQLAdminAttribs#public#\n";
    } elsif($dn =~ /^relativeDomainName/) {
	print "OpenLDAPaci: 1.2.3#entry#grant;r,s,c;relativeDomainName,zoneName,DNSTTL,DNSClass,ARecord,MDRecord,MXRecord,NSRecord,SOARecord,CNAMERecord,PTRRecord,HINFORecord,MINFORecord,TXTRecord,SIGRecord,KEYRecord,AAAARecord,LOCRecord,NXTRecord,SRVRecord,NAPTRRecord,KXRecord,CERTRecord,A6Record,DNAMERecord$phpQLAdminAttribs#public#\n";
    }

    print "OpenLDAPaci: 1.2.3#entry#grant;w,r,s,c;[children]#access-id#$SUBJECT\n";

    print "OpenLDAPaci: 1.2.3#entry#$RIGHTS#access-id#$SUBJECT\n";

    print "\n";
}
