#!/usr/bin/perl -w

# This script is to be run on the QmailLDAP server.
# It restarts Qmail if it detects a change in domains
# ie, addition/removal of a 'locals' or 'rcpthost' value.

# Config options
open(CONFIG, "< /etc/qmail/.restart_qmail.conf")
    || die("Can't open config, $!\n");
while(!eof(CONFIG)) {
    $line = <CONFIG>; chomp($line);
    $line =~ s/"$//;
    @conf = split("=\"", $line);
    $CONFIG{$conf[0]} = $conf[1];
}
close(CONFIG);

# Don't touch these
$local = $rcpthost = '';
@FILES = ("locals", "rcpthosts");

# ----------------------------
# Get the QmailLDAP/Controls object for specified host ($HOSTNAME)
$CMD = "$CONFIG{'LDAP_SEARCH'} -x -LLL -h $CONFIG{'LDAP_SERVER'} -b '$CONFIG{'LDAP_CTRLDN'}' 'cn=$CONFIG{'HOSTNAME'}' locals rcpthosts";
open(SEARCH, "$CMD |")
    || die("Can't search, $!\n");
while(!eof(SEARCH)) {
    $line = <SEARCH>; chomp($line);
    if($line =~ /^locals: /) {
	$line =~ s/^locals: //;
	$LOCALS{$line} = $line;
    } elsif($line =~ /^rcpthosts/) {
	$line =~ s/^rcpthosts: //i;
	$RCPTHOSTS{$line} = $line;
    }
}
close(SEARCH);

# ----------------------------
# Count how many Locals and RCPTHosts attributes we have in this object
$AMOUNT{'calculated'}{'locals'} = 0; $AMOUNT{'calculated'}{'rcpthosts'} = 0;
foreach $local (sort(keys(%LOCALS))) { $AMOUNT{'calculated'}{'locals'}++; }
foreach $rcpthost (sort(keys(%RCPTHOSTS))) { $AMOUNT{'calculated'}{'rcpthosts'}++; }

# ----------------------------
# Open the files with the old values
foreach $file (@FILES) {
    if(open(FILE, "< /etc/qmail/.$file")) {
        $line = <FILE>; chomp($line);
        $AMOUNT{'oldvalue'}{$file} = $line;
        close(FILE);
    } else {
        $AMOUNT{'oldvalue'}{$file} = 0;
    }

    if($AMOUNT{'oldvalue'}{$file} != $AMOUNT{'calculated'}{$file}) {
	# Value have changed!
	open(FILE, "> /etc/qmail/.$file") || die("Can't open file, $!\n");
	print FILE $AMOUNT{'calculated'}{$file} . "\n";
	close(FILE);
	    
	$changed = 1;
    }
}

if($changed) {
    print "Value have changed\n";
    system($CONFIG{'QMAIL_INIT'}, "restart");
}
