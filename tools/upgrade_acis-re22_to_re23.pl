#!/usr/bin/perl -w

while(! eof(STDIN)) {
    $line = <STDIN>;
    chomp($line);
    push(@DATA, $line)
}

for($i=0,$j=0; defined($DATA[$i]);) {
    if(defined($DATA[$i+1]) && $DATA[$i+1] =~ /^ /) {
	$DATA[$i] =~ s/ $//;
	$line = $DATA[$i];

	$i++; # Next line is line which starts with space
	while($DATA[$i] =~ /^ /) {
	    # Remove starting and ending space(s)
	    $DATA[$i] =~ s/^ //;
	    $DATA[$i] =~ s/ $//;

	    # Add this part to the line
	    $line .= $DATA[$i];

	    $i++; # Get next data
	}

	$FILE[$j] = $line;

	$j++;
    } else {
	$FILE[$j] = $DATA[$i];
	$j++;
	$i++;
    }
}

for($i=0; defined($FILE[$i]); $i++) {
    if($FILE[$i] =~ /^OpenLDAPaci: /) {
	$FILE[$i] =~ s/OpenLDAPaci: //;

	($oid, $scope, $access, $match, $target) = split('#', $FILE[$i]);
	($rule, $perm, $what) = split(';', $access);

	if($what =~ /,/) {
	    #OpenLDAPaci: 1#entry#grant;c,x;userPassword,krb5PrincipalName,cn,mail,mailAlternateAddress#public#
	    #=>
	    #OpenLDAPaci: 1#entry#grant;c,x;userPassword#public#
	    #OpenLDAPaci: 2#entry#grant;c,x;krb5PrincipalName#public#
	    #OpenLDAPaci: 3#entry#grant;c,x;cn#public#
	    #OpenLDAPaci: 4#entry#grant;c,x;mail#public#
	    #OpenLDAPaci: 5#entry#grant;c,x;mailAlternateAddress#public#

	    @attrs = split(',', $what);
	    foreach $attr (@attrs) {
		print "OpenLDAPaci: $OID#$scope#$rule;$perm;".lc($attr)."#$match#";
		print lc($target) if($target);
		print "\n";

		$OID++;
	    }
	} elsif($access =~ /\[children\]/) {
	    #OpenLDAPaci: 1#entry#grant;w,r,s,c;[children]#access-id#uid=turbo,ou=People,o=Fredriksson,c=SE
	    #=>
	    #OpenLDAPaci: 1#children#grant;w,r,s,c;[all]#access-id#uid=turbo,ou=People,o=Fredriksson,c=SE

	    $access =~ s/children/all/;
	    print "OpenLDAPaci: $OID#children#$access#$match#";
	    print lc($target) if($target);
	    print "\n";
	    
	    $OID++;
	} elsif($access =~ /\[entry\]/) {
	    #OpenLDAPaci: 81#entry#grant;w,r,s,c;[entry]#access-id#uid=fernanda,ou=People,o=Bortheiry,c=SE
	    #=>
	    #OpenLDAPaci: 81#entry#grant;w,r,s,c;entry#access-id#uid=fernanda,ou=People,o=Bortheiry,c=SE

	    $access =~ s/\[entry\]/entry/;
	    print "OpenLDAPaci: $OID#entry#$access#$match#";
	    print lc($target) if($target);
	    print "\n";
	    
	    $OID++;
	} else {
	    print "OpenLDAPaci: $OID#$scope#$access#$match#";
	    print lc($target) if($target);
	    print "\n";

	    $OID++;
	}
    } elsif($FILE[$i] =~ /^dn: /) {
	print $FILE[$i]."\n";
	$OID = 0;
    } else {
	print $FILE[$i]."\n";
    }
}
