#!/usr/bin/perl -w

$HOST = "localhost";
$USER = 'cn=Turbo Fredriksson,ou=People,o=Fredriksson,c=SE';
$PASS = `cat /tmp/...`; chomp($PASS);

# ------------------------
# No editable parts below!

$cmd = "ldapsearch -LLL -x -h $HOST -b '' -s base -D '$USER' -w $PASS namingContexts";
open(ROOT, "$cmd |") or die($@);
$line = <ROOT>; # Trow away the first line
while(!eof(ROOT)) {
    $line = <ROOT>; chomp($line);
    next if($line =~ /^$/);

    $dn = $line; $dn =~ s/.*: //;
    push(@DNS, $dn);
}
close(ROOT);

$search = "'(objectclass=phpQLAdminWebsrv)'";
foreach $basedn (@DNS) {
    $cmd = "ldapsearch -LLL -x -h $HOST -b '$basedn' -s sub -D '$USER' -w $PASS $search";
    open(SEARCH, "$cmd |") or die($@);
    while(!eof(SEARCH)) {
	$line = <SEARCH>; chomp($line);
	next if($line =~ /^$/);
	if($line =~ /^dn: /) {
	    @tmp  = split(',', $line);
	    @tmp  = split('=', $tmp[0]);
	    $name = $tmp[1];
	}

	($attrib, $value) = split(': ', $line);
	$attrib = lc($attrib);

	if($ENTRY{$name}{$attrib}) {
	    $ENTRY{$name}{$attrib} .= ';'.$value;
	} else {
	    $ENTRY{$name}{$attrib} = $value;
	}
    }
    close(SEARCH);
}

foreach $srv (sort(keys(%ENTRY))) {
    print "<VirtualHost ".$ENTRY{$srv}{'webserverip'}.">\n";
    foreach $attr (sort(keys(%{$ENTRY{$srv}}))) {
	if($attr eq 'cn') {
	    print "\tServerName ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq 'webdocumentroot') {
	    print "\tDocumentRoot ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq 'webserveradmin') {
	    print "\tServerAdmin ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq 'webserveralias') {
	    @aliases = split(';', $ENTRY{$srv}{$attr});
	    foreach $alias (sort(@aliases)) {
		print "\tServerAlias $alias\n";
	    }
	} elsif($attr eq 'webscriptaliasurl') {
	    print "\tScriptAlias ".$ENTRY{$srv}{webscriptaliasurl};
	    print " ".$ENTRY{$srv}{webscriptaliaspath}."\n";
	} elsif($attr eq 'weblogerror') {
	    print "\tErrorLog ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq 'weblogtransfer') {
	    print "\tTransferLog ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq 'weboptions') {
	    print "\tOptions ".$ENTRY{$srv}{$attr}."\n";
	}
    }
    print "</VirtualHost>\n\n";
}
