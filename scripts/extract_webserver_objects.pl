#!/usr/bin/perl -w

$LDAP_SEARCH="/usr/bin/ldapsearch -LLL";
$LDAP_SERVER="-H ldapi://%2fvar%2frun%2fslapd%2fldapi.main";
$LDAP_BASEDN="ou=WEB,o=Bayour.COM,c=SE";
$LDAP_PARAMS="-Y GSSAPI";

$cmd = "$LDAP_SEARCH $LDAP_PARAMS $LDAP_SERVER -b '$LDAP_BASEDN' '(&(objectClass=phpQLAdminWebSrv)(cn=*))'";
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
    # dn: cn=cacti.bayour.com,ou=WEB,o=Bayour.COM,c=SE
    # webServerIP: 212.214.70.54
    # webServerURL: http://cacti.bayour.com/
    # webServerAdmin: turbo@bayour.com
    # webDocumentRoot: /usr/share/cacti/
    # webServerName: cacti.bayour.com
    # objectClass: phpQLAdminWebSrv
    # cn: cacti.bayour.com
    # webLogError: /var/log/apache/cacti_error.log
    # webLogTransfer: /var/log/apache/cacti-access.log
    # webOptions: Include /etc/cacti/apache.conf

    if($line[$i]) {
	if($line[$i] =~ /^dn: cn=/) {
	    $dn = (split(' ', $line[$i]))[1];
	} else {
	    $attrib = (split(':', $line[$i]))[0];

	    $value =  $line[$i];
	    $value =~ s/$attrib: //;
	    
	    if($attrib =~ /webServerIP/) {
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
    print "<VirtualHost $SERVER{$dn}{webServerIP}>\n";

    print "    ServerName $SERVER{$dn}{cn}\n" if($SERVER{$dn}{cn});
    print "    DocumentRoot $SERVER{$dn}{webDocumentRoot}\n" if($SERVER{$dn}{webDocumentRoot});
    print "    ServerAdmin $SERVER{$dn}{webServerAdmin}\n" if($SERVER{$dn}{webServerAdmin});
    print "    ErrorLog $SERVER{$dn}{webLogError}\n" if($SERVER{$dn}{webLogError});
    print "    TransferLog $SERVER{$dn}{webLogTransfer}\n" if($SERVER{$dn}{webLogTransfer});
    print "    $SERVER{$dn}{webOptions}\n" if($SERVER{$dn}{webOptions});

    print "</VirtualHost>\n\n";
}

print "#<VirtualHost _default_:*>\n";
print "#</VirtualHost>\n";
