#!/usr/bin/perl -w

$HOST = "localhost";
$USER = 'cn=Turbo Fredriksson,ou=People,o=Turbo Fredriksson';
$PASS = `cat /tmp/...`; chomp($PASS);

# These must be lowercased!!
$OC		 = 'phpqladminwebsrv';
$SRV_NAME	 = 'cn';
$SRV_IP		 = 'webserverip';
$SRV_ADM	 = 'webserveradmin';
$SRV_ALIAS	 = 'webserveralias';
$SRV_SCIPT_ALIAS = 'webscriptaliasurl';
$SRV_SCIPT_PATH	 = 'webscriptaliaspath';
$SRV_LOG_ERRORS	 = 'weblogerror';
$SRV_LOG_TRANSF	 = 'weblogtransfer';
$SRV_OPTIONS	 = 'weboptions';
$DOC_ROOT	 = 'webdocumentroot';

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

$search = "'(objectclass=$OC)'";
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

	if($value) {
	    if($ENTRY{$name}{$attrib}) {
		$ENTRY{$name}{$attrib} .= ';'.$value;
	    } else {
		$ENTRY{$name}{$attrib} = $value;
	    }
	}
    }
    close(SEARCH);
}

foreach $srv (sort(keys(%ENTRY))) {
    print "<VirtualHost ".$ENTRY{$srv}{$SRV_IP}.">\n";
    foreach $attr (sort(keys(%{$ENTRY{$srv}}))) {
	if($attr eq $SRV_NAME) {
	    print "\tServerName ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq $DOC_ROOT) {
	    print "\tDocumentRoot ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq $SRV_ADM) {
	    print "\tServerAdmin ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq $SRV_ALIAS) {
	    @aliases = split(';', $ENTRY{$srv}{$attr});
	    foreach $alias (sort(@aliases)) {
		print "\tServerAlias $alias\n";
	    }
	} elsif($attr eq $SRV_SCIPT_ALIAS) {
	    print "\tScriptAlias ".$ENTRY{$srv}{$SRV_SCIPT_ALIAS};
	    print " ".$ENTRY{$srv}{$SRV_SCIPT_PATH}."\n";
	} elsif($attr eq $SRV_LOG_ERRORS) {
	    print "\tErrorLog ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq $SRV_LOG_TRANSF) {
	    print "\tTransferLog ".$ENTRY{$srv}{$attr}."\n";
	} elsif($attr eq $SRV_OPTIONS) {
	    print "\tOptions ".$ENTRY{$srv}{$attr}."\n";
	}
    }
    print "</VirtualHost>\n\n";
}
