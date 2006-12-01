#!/usr/bin/perl -w

# Script to move web-/mailservers and automounts from
# below branch/dn to new 'concentrated host' view.
#
# $Id: upgrade_hostmerge.pl,v 1.1.2.4 2006-12-01 09:04:57 turbo Exp $
#
# -----  N O T E  -----  N O T E  -----  N O T E  -----  N O T E  -----
# This file will NOT do any modifications to your LDAP database. It
# will create two files (which will be shown at the end of the run)
# in temp. One file that starts with 'add.' and one that starts with
# 'del.'. The add file is added to the LDAP server with the command
# 'ldapadd -f add.XXX' and if/when you feel ready to delete the old
# objects, you do this with the command line 'ldapdel -f del.XXX'.
#
# Remember to update your Apache and QmailLDAP/Controls files before
# (!) doing the delete, othervise neither of the systems will find
# their configuration setup...
# -----  N O T E  -----  N O T E  -----  N O T E  -----  N O T E  -----

@ROOTDN = ('c=SE', 'dc=lantrix,dc=no');
$SERVER = "-H ldapi://%2fvar%2frun%2fslapd%2fldapi.main";
if($ENV{ADDITIONAL_SEARCH_OPTIONS}) {
    $ADDITIONAL_SEARCH_OPTIONS = $ENV{ADDITIONAL_SEARCH_OPTIONS};
} else {
    $ADDITIONAL_SEARCH_OPTIONS="-Y GSSAPI";
}
$PRIMARY_SUPERADMIN="uid=turbo,ou=People,o=Fredriksson,c=SE";

$LDAPSEARCH = "ldapsearch -LLL $ADDITIONAL_SEARCH_OPTIONS $SERVER";

# {{{ Create a temp file for ldapadd
$FILE_ADD = `tempfile -p add. -s .ldif`; chomp($FILE_ADD);
open(LDAPADD, "> $FILE_ADD")
    || die("Can't pipe to ldapadd, $!");
# }}}

# {{{ Create a temp file for ldapdelete
$FILE_DEL = `tempfile -p del. -s .ldif`; chomp($FILE_DEL);
open(LDAPDEL, "> $FILE_DEL")
    || die("Can't pipe to ldapdel, $!");
# }}}

# --------------------------

# {{{ Function to fix LDIF multilines
sub fix_ldif_multiline {
    my (@object) = @_;
    my($i, @entry);

    for($i=0; $object[$i]; $i++) {
	if($object[$i+1] && ($object[$i+1] =~ /^ /)) {
	    $object[$i+1] =~ s/^ //;
	    $object[$i] .= $object[$i+1];
	    undef($object[$i+1]);
	    
	    push(@entry, $object[$i]);
	    $i += 1;
	} else {
	    push(@entry, $object[$i]);
	}
    }

    return(@entry);
}
# }}}

# {{{ Function to do LDAP search
sub upgrade_hostmerge_ldapsearch {
    my($cmd) = @_;
    my($line, @OBJECT);

    open(LDAPSEARCH, "$cmd |")
	|| die("Can't execute '$cmd', $!\n");
    while(!eof(LDAPSEARCH)) {
	$line = <LDAPSEARCH>;
	if($line) {
	    chomp($line);
	    $line =~ s/^dn: //;
	    next if($line eq '');
	    push(@OBJECT, $line);
	}
    }
    close(LDAPSEARCH);

    return(fix_ldif_multiline(@OBJECT))
}
# }}}

# {{{ Function to retreive object including ACI attribute
sub upgrade_hostmerge_get_object {
    my($dn) = @_;
    my($cmd, $line, @OBJECT, @ENTRY, $i);

    # Retreive the original webserver object
    $cmd = "$LDAPSEARCH -b $dn -s base \\* OpenLDAPaci 2> /dev/null";
    return(upgrade_hostmerge_ldapsearch($cmd));
}
# }}}

# {{{ Function to check if entry is in array
sub upgrade_hostmerge_check_array {
    my($entry, @array) = @_;
    my($i);
    $entry = lc($entry);
    
    for($i=0; $array[$i]; $i++) {
	$array[$i] = lc($array[$i]);

	if($array[$i] eq $entry) {
	    return(1);
	}
    }

    return(0);
}
# }}}

# {{{ Function to check if physical host exists
sub upgrade_hostmerge_check_physical {
    my($dn, $host) = @_;
    my($cmd, $line, $added_object, $i, $ip, $fqdn, @dn_parts);

    # {{{ Check if the physical host exists at destination
    $cmd = "$LDAPSEARCH -b $ROOTDN[0] -s base -b $dn 2> /dev/null";
    open(LDAPSEARCH, "$cmd |")
	|| die("Can't find $physical_dn, $!\n");
    $line = <LDAPSEARCH>;
    close(LDAPSEARCH);
    
    $added_object = 0;
    if(!$line) {
	# Not in LDAP. Have I (this script) created it?
	if(upgrade_hostmerge_check_array($dn, @ADDED_PHYSICAL)) {
	    $added_object = 1;
	}
    }
    # }}}

    # {{{ Add physical host if didn't exists already
    if(!$added_object) {
	# Find the IP address of this physical host
	$ip = `host $physical | sed 's/.*	//'`;
	chomp($ip);

	# Just make sure we get a FQDN
	$fqdn = `host $physical | sed -e 's/	.*//' -e 's/ .*//'`;
	if($fqdn and ($fqdn ne $host)) {
	    # Not the same that we're called with. Better use the one from DNS.
	    $host = $fqdn; chomp($host);

	    @dn_parts = split(',', $dn);
	    $new_dn = "cn=$host,";
	    for($i=1; $dn_parts[$i]; $i++) {
		$new_dn .= $dn_parts[$i];
		if($dn_parts[$i+1]) {
		    $new_dn .= ',';
		}
	    }

	    $dn = $new_dn;

	    if(upgrade_hostmerge_check_array($dn, @ADDED_PHYSICAL)) {
		# THIS do, however, exists in the array!
		return(($dn, $host));
	    }
	}

	print LDAPADD <<EOF
dn: $dn
cn: $host
objectClass: ipHost
objectClass: groupOfUniqueNames
uniqueMember: $PRIMARY_SUPERADMIN
ipHostNumber: $ip
openldapaci: 0#entry#grant;r,s,c;objectClass,[entry]#public#
openldapaci: 1#entry#grant;w,r,s,c,x;[all]#access-id#$PRIMARY_SUPERADMIN

EOF
    ;
	print "  Physical host: '$dn'\n";
	push(@ADDED_PHYSICAL, $dn);
    }
    # }}}

    return(($dn, $host));
}
# }}}

# --------------------------

# {{{ Find the 'ou=Computers,<rootdn>' object
print "Looking for 'ou=Computers in: ";
$got_computers_object = 0;
for($i=0; $ROOTDN[$i]; $i++) {
    print $ROOTDN[$i];
    if($ROOTDN[$i+1]) {
	print " ";
    }

    $cmd = "$LDAPSEARCH -b $ROOTDN[$i] -s one ou=Computers 2> /dev/null";
    open(LDAPSEARCH, "$cmd |")
	|| die("Can't find ou=Computers, $!\n");
    $line = <LDAPSEARCH>;
    close(LDAPSEARCH);
    
    if($line) {
	$got_computers_object = 1;
	print "(found)\n";
    }
}

if(!$got_computers_object) {
    print "\nLooking for any other ou object in: ";

    # See if there's another ou object below the root DN
    undef($line);
    for($i=0; $ROOTDN[$i]; $i++) {
	print $ROOTDN[$i];
	if($ROOTDN[$i+1]) {
	    print " ";
	}

	$cmd = "$LDAPSEARCH -b ".$ROOTDN[$i]." -s one ou=* dn 2> /dev/null";
	@tmp = upgrade_hostmerge_ldapsearch($cmd);
	if($tmp[0]) {
	    $line = $tmp[0];
	    print "\n";
	    last;
	}
    }

    if(!$line) {
	print "Can't find any objects to take ACI's from!\n";
	exit 1;
    }

    # Retreive the OpenLDAPaci attributes from the ou
    $cmd = "$LDAPSEARCH -b $line -s base openldapaci 2> /dev/null";
    @ACIS = upgrade_hostmerge_ldapsearch($cmd);

    # Create ou=Computers object
    $computers_dn = "ou=Computers,".$ROOTDN[$i];
    print "Adding '$computers_dn' to LDIF.\n";
    print LDAPADD <<EOF
dn: ou=Computers,$ROOTDN[$i]
ou: Computers
objectClass: organizationalUnit
EOF
    ;

    # $i=1 because 0 is the DN of the object we took the ACI's from!
    for($i=1; $ACIS[$i]; $i++) {
	print LDAPADD $ACIS[$i],"\n";
    }
    print LDAPADD "\n";
}
# }}}

# {{{ Find out if there's any web- and/or mailservers
print "\nLooking for objects to move:\n";
for($i=0; $ROOTDN[$i]; $i++) {
    $cmd = "$LDAPSEARCH -b $computers_dn -s one '(&(cn=*)(|(objectClass=ipHost)(objectClass=device)))' 2> /dev/null";
    open(LDAPSEARCH, "$cmd |")
	|| die("Can't find any physical hosts below ou=Computers,$ROOTDN, $!\n");
    $line = <LDAPSEARCH>;
    close(LDAPSEARCH);
    if(!$line) {
	# Nope, no hosts! Look for them in other places

	# {{{ Find webservers
	undef(@WEBSERVERS);
	$cmd = "$LDAPSEARCH -b ".$ROOTDN[$i]." '(&(apacheservername=*)(|(objectclass=ApacheVirtualHostObj)(objectclass=ApacheSectionObj)))' dn 2> /dev/null";
	@WEBSERVERS = upgrade_hostmerge_ldapsearch($cmd);

	@servers = fix_ldif_multiline(@WEBSERVERS);
	for($j=0; $servers[$j]; $j++) {
	    push(@DNS2MOVE, $servers[$j]);
	}
	# }}}

	# {{{ Find mailservers
	undef(@MAILSERVERS);
	$cmd = "$LDAPSEARCH -b ".$ROOTDN[$i]." '(&(cn=*)(objectclass=qmailControl))' dn 2> /dev/null";
	@MAILSERVERS = upgrade_hostmerge_ldapsearch($cmd);

	@servers = fix_ldif_multiline(@MAILSERVERS);
	for($j=0; $servers[$j]; $j++) {
	    push(@DNS2MOVE, $servers[$j]);
	}
	# }}}

	# {{{ Find automounts
	$cmd = "$LDAPSEARCH -b $ROOTDN[$i] 'ou=auto.*' dn 2> /dev/null";
	@AUTOMOUNTS = upgrade_hostmerge_ldapsearch($cmd);

	@servers = fix_ldif_multiline(@AUTOMOUNTS);
	for($j=0; $servers[$j]; $j++) {
	    push(@DNS2MOVE, $servers[$j]);
	}
	# }}}
    }
}

for($i=0; $DNS2MOVE[$i]; $i++) {
    printf("  %3d: %s\n", $i, $DNS2MOVE[$i]);
}
# }}}

# {{{ Go through each server dn, add the new object to the LDIF
@REMOVE = ();
if(@DNS2MOVE) {
    print "\nAdding objects to LDIF:\n";
    for($i=0; $DNS2MOVE[$i]; $i++) {
	@dn_parts = split(',', $DNS2MOVE[$i]);

	if($DNS2MOVE[$i] =~ /^ApacheServerName=/i) {
	    # {{{ A webserver     - 'apacheservername=apache.bayour.com,cn=aurora.bayour.com:80,ou=web,o=bayour.com,c=se'

	    # {{{ Extract the base DN of the object
	    $basedn = '';
	    for($j=2; $dn_parts[$j]; $j++) {
		$basedn .= $dn_parts[$j];
		$basedn .= "," if($dn_parts[$j+1]);
	    }
	    # }}}
	    
	    # {{{ Extract the physical host from the second part ('cn=aurora.bayour.com:80')
	    $tmp = $dn_parts[1]; $tmp =~ s/cn=//; $tmp =~ s/:.*//;
	    $physical = $tmp;
	    $physical_dn = "cn=$physical,ou=Computers,".$ROOTDN[0];
	    # }}}

	    # Check (and possibly create) the physical host
	    ($physical_dn, $physical) = upgrade_hostmerge_check_physical($physical_dn, $physical);

	    # {{{ Clone the webserver container
	    # Setup the DN to the new webserver container
	    $webcontainer_dn = $dn_parts[1].",$physical_dn";

	    if(!upgrade_hostmerge_check_array($webcontainer_dn, @ADDED_CONTAINER)) {
		# Retreive the original webserver container
		$dn = $dn_parts[1].','.$basedn;
		@entry = upgrade_hostmerge_get_object($dn);
		push(@REMOVE, $dn);
		
		# Add the webserver container
		print LDAPADD "dn: $webcontainer_dn\n";
		for($j=1; $entry[$j]; $j++) {
		    print LDAPADD $entry[$j]."\n";
		}
		print LDAPADD "\n";
		
		print "    webserver container: '$webcontainer_dn'\n";
		push(@ADDED_CONTAINER, $webcontainer_dn);
	    }
	    # }}}

	    # {{{ Clone the webserver object
	    # Setup the DN to the new webserver object
	    $webserver_dn = $dn_parts[0].",$webcontainer_dn";

	    if(!upgrade_hostmerge_check_array($webserver_dn, @ADDED_SERVER)) {
		# Retreive the original webserver container
		@entry = upgrade_hostmerge_get_object($DNS2MOVE[$i]);
		push(@REMOVE, $DNS2MOVE[$i]);
		
		# Add the webserver container
		print LDAPADD "dn: $webserver_dn\n";
		for($j=1; $entry[$j]; $j++) {
		    print LDAPADD $entry[$j]."\n";
		}
		print LDAPADD "\n";
		
		print "      webserver object: '$webserver_dn'\n";
		push(@ADDED_SERVER, $webserver_dn);
	    }
	    # }}}

	    # {{{ Clone any webserver location objects
	    # Find all objects below the original webserver object
	    # (just in case there's virtual host locations)
	    $cmd = "$LDAPSEARCH -b $DNS2MOVE[$i] -s one 'objectClass=*' dn 2> /dev/null";
	    @LOCATIONS = upgrade_hostmerge_ldapsearch($cmd);
	    for($j=0; $LOCATIONS[$j]; $j++) {
		# Extract the location from the DN
		@dn_parts = split(',', $LOCATIONS[$j]);

		# Setup the DN to the new webserver location object
		$location_dn = $dn_parts[0].','.$webserver_dn;

		if(!upgrade_hostmerge_check_array($location_dn, @ADDED_LOCATION)) {
		    # Retreive the original location object
		    @entry = upgrade_hostmerge_get_object($LOCATIONS[$j]);
		    push(@REMOVE, $LOCATIONS[$j]);
		    
		    # Add the location object
		    print LDAPADD "dn: $location_dn\n";
		    for($k=1; $entry[$k]; $k++) {
			print LDAPADD $entry[$k]."\n";
		    }
		    print LDAPADD "\n";
		    
		    print "        webserver location object: '$location_dn'\n";
		    push(@ADDED_LOCATION, $location_dn);
		}
	    }
	    # }}}
	    # }}}

	} elsif($DNS2MOVE[$i] =~ /^cn=/i) {
	    # {{{ A mailserver    - 'cn=aurora.bayour.com,ou=qmailldap,o=bayour.com,c=se'

	    # {{{ Extract the physical host from the second part ('cn=aurora.bayour.com:80')
	    $tmp = $dn_parts[0]; $tmp =~ s/cn=//; $tmp =~ s/:.*//;
	    $physical = $tmp;
	    $physical_dn = "cn=$physical,ou=Computers,".$ROOTDN[0];
	    # }}}

	    # Check (and possibly create) the physical host
	    ($physical_dn, $physical) = upgrade_hostmerge_check_physical($physical_dn, $physical);

	    # Retreive the original mailserver object
	    @entry = upgrade_hostmerge_get_object($DNS2MOVE[$i]);
	    push(@REMOVE, $DNS2MOVE[$i]);

	    # Add the mailserver in new location
	    $mailserver_dn = "cn=$physical,$physical_dn";
	    print LDAPADD "dn: $mailserver_dn\n";
	    for($j=1; $entry[$j]; $j++) {
		print LDAPADD $entry[$j]."\n";
	    }
	    print LDAPADD "\n";

	    print "    mailserver: '$mailserver_dn'\n";
	    # }}}

	} elsif($DNS2MOVE[$i] =~ /^ou=auto\./i) {
	    # {{{ A automount map - 'ou=auto.mnt,ou=Automounts,dc=pumba,o=Bayour.COM,c=SE'

	    # {{{ Extract the physical host from the third part
	    $tmp = $dn_parts[2]; $tmp =~ s/cn=//; $tmp =~ s/dc=//; $tmp =~ s/:.*//;
	    $physical = $tmp;
	    $physical_dn = "cn=$physical,ou=Computers,".$ROOTDN[0];
	    # }}}

	    # Check (and possibly create) the physical host
	    ($physical_dn, $physical) = upgrade_hostmerge_check_physical($physical_dn, $physical);

	    # Setup the DN to the new automount map
	    $automount_dn = $dn_parts[0].",$physical_dn";

	    if(!upgrade_hostmerge_check_array($automount_dn, @ADDED_AUTOMOUNTS)) {
		# {{{ Retreive the original automount
		@entry = upgrade_hostmerge_get_object($DNS2MOVE[$i]);
		push(@REMOVE, $DNS2MOVE[$i]);
		# }}}

		# {{{ Add the automount
		print LDAPADD "dn: $automount_dn\n";
		for($j=1; $entry[$j]; $j++) {
		    print LDAPADD $entry[$j]."\n";
		}
		print LDAPADD "\n";
		
		print "    automount map: '$automount_dn'\n";
		push(@ADDED_AUTOMOUNTS, $automount_dn);
		# }}}

		# {{{ Find any automount maps below this automount
		$cmd = "$LDAPSEARCH -b $DNS2MOVE[$i] -s one '(&(cn=*)(objectClass=automount))' dn 2> /dev/null";
		@AUTOMOUNT_ENTRIES_1 = upgrade_hostmerge_ldapsearch($cmd);
		for($j=0; $AUTOMOUNT_ENTRIES_1[$j]; $j++) {
		    # Extract the location from the DN
		    @dn_parts = split(',', $AUTOMOUNT_ENTRIES_1[$j]);

		    # Setup the DN to the new automount entry
		    $automount_entry_dn_1 = $dn_parts[0].','.$automount_dn;

		    if(!upgrade_hostmerge_check_array($location_dn, @ADDED_AUTOMOUNT_ENTRY_1)) {
			# {{{ Retreive the original automount entry
			@entry = upgrade_hostmerge_get_object($AUTOMOUNT_ENTRIES_1[$j]);
			push(@REMOVE, $AUTOMOUNT_ENTRIES_1[$j]);
			# }}}

			# {{{ Add the automount map
			print LDAPADD "dn: $automount_entry_dn_1\n";
			for($k=1; $entry[$k]; $k++) {
			    print LDAPADD $entry[$k]."\n";
			}
			print LDAPADD "\n";
			
			print "      automount entry: $automount_entry_dn_1\n";
			push(@ADDED_AUTOMOUNT_ENTRY_1, $automount_entry_dn_1);
			# }}}

			# {{{ Find any automount maps below this automount entry
			$cmd = "$LDAPSEARCH -b $AUTOMOUNT_ENTRIES_1[$j] -s one '(&(cn=*)(objectClass=automount))' dn 2> /dev/null";
			@AUTOMOUNT_ENTRIES_2 = upgrade_hostmerge_ldapsearch($cmd);
			for($l=0; $AUTOMOUNT_ENTRIES_2[$l]; $l++) {
			    # Extract the location from the DN
			    @dn_parts = split(',', $AUTOMOUNT_ENTRIES_2[$l]);
			    
			    # Setup the DN to the new automount entry
			    $automount_entry_dn_2 = $dn_parts[0].','.$automount_entry_dn_1;
			    
			    if(!upgrade_hostmerge_check_array($location_dn, @ADDED_AUTOMOUNT_ENTRY_2)) {
				# {{{ Retreive the original automount entry
				@entry = upgrade_hostmerge_get_object($AUTOMOUNT_ENTRIES_2[$l]);
				push(@REMOVE, $AUTOMOUNT_ENTRIES_2[$l]);
				# }}}
				
				# {{{ Add the automount map
				print LDAPADD "dn: $automount_entry_dn_2\n";
				for($m=1; $entry[$m]; $m++) {
				    print LDAPADD $entry[$m]."\n";
				}
				print LDAPADD "\n";
				
				print "        automount entry: $automount_entry_dn_2\n";
				push(@ADDED_AUTOMOUNT_ENTRY_2, $automount_entry_dn_2);
				# }}}
			    }
			}
			# }}}
		    }
		}
		# }}}
	    }
	    # }}}

	}
    }
}
# }}}

# --------------------------

# {{{ In reverse order, remove the original objects
$count = $#REMOVE;
print "\nObjects to remove: '$count'\n";
for($i=$count; $i >= 0; $i--) {
    print "  ".$REMOVE[$i]."'\n";
    print LDAPDEL $REMOVE[$i]."\n";
}
# }}}

print "\n";
print "Temp file (add): '$FILE_ADD'\n";
print "Temp file (del): '$FILE_DEL'\n";
