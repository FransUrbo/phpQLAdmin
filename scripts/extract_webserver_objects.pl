#!/usr/bin/perl -w

$LDAP_SEARCH="/usr/bin/ldapsearch -LLL";
$LDAP_SERVER="-H ldapi://%2fvar%2frun%2fslapd%2fldapi.main";
$LDAP_BASEDN="ou=Computers,c=SE";
$LDAP_PARAMS="-Y GSSAPI";

$cmd = "$LDAP_SEARCH $LDAP_PARAMS $LDAP_SERVER -b '$LDAP_BASEDN' '(&(objectClass=ApacheVirtualHostObj)(cn=*))'";
open(LDAPSEARCH, "$cmd |") || die("Can't open $cmd, $!\n");

# ----------------------------------
# Load the whole input into an array
$count = 0;
while(! eof(LDAPSEARCH)) {
    $tmp = <LDAPSEARCH>; chomp($tmp);

    if($tmp =~ /^ /) {
        $tmp =~ s/^ //;
        $line[$count-1] .= $tmp;
        $count--;
    } else {
        $line[$count] = $tmp;
    }

    $count++;
}
close(LDAPSEARCH);

# ----------------------------------
# Create an array which is sorted/ordered by server object
for($i = 0; $i < $count; $i++) {
    # dn: cn=testing.bayour.com,ou=WEB,o=Bayour.COM,c=SE
    # objectClass: device
    # objectClass: ApacheSectionObj
    # objectClass: ApacheVirtualHostObj
    # objectClass: ApacheModLogConfigObj
    # cn: testing.bayour.com
    # ApacheSectionName: VirtualHost
    # ApacheSectionArg: 192.168.1.4
    # ApacheServerName: testing.bayour.com
    # ApacheDocumentRoot: /var/www/testing
    # ApacheServerAdmin: turbo@phpqladmin.com
    # ApacheErrorLog: /var/log/apache/testing_error.log
    # ApacheTransferLog: /var/log/apache/testing_access.log

    if($line[$i]) {
	if($line[$i] =~ /^dn: cn=/) {
	    $dn = (split(' ', $line[$i]))[1];
	} else {
	    $attrib =  (split(':', $line[$i]))[0];
	    $attrib =~ s/^Apache//;

	    $value =  $line[$i];
	    $value =~ s/Apache$attrib: //;

	    if($attrib =~ /ServerName/) {
		$HOSTS{$value} = $value;
	    }
	    
	    $SERVER{$dn}{$attrib} = $value;
	}
    }
}

# ----------------------------------
# Define the virtual hosts
foreach $host (keys %HOSTS) {
    print "NameVirtualHost $host\n";
}
print "\n";

# Create the virtual host directives for Apache.
foreach $dn (keys %SERVER) {
    print "<VirtualHost $SERVER{$dn}{SectionArg}>\n";

    print "    ServerName $SERVER{$dn}{ServerName}\n" if($SERVER{$dn}{ServerName});
    print "    DocumentRoot $SERVER{$dn}{DocumentRoot}\n" if($SERVER{$dn}{DocumentRoot});
    print "    ServerAdmin $SERVER{$dn}{ServerAdmin}\n" if($SERVER{$dn}{ServerAdmin});
    print "    ErrorLog $SERVER{$dn}{ErrorLog}\n" if($SERVER{$dn}{ErrorLog});
    print "    TransferLog $SERVER{$dn}{TransferLog}\n" if($SERVER{$dn}{TransferLog});
    print "    $SERVER{$dn}{Options}\n" if($SERVER{$dn}{Options});

    print "</VirtualHost>\n\n";
}

print "#<VirtualHost _default_:*>\n";
print "#</VirtualHost>\n";
