#!/usr/bin/perl -w

# This script is to be run on the QmailLDAP server.
# It restarts Qmail if it detects a change in domains
# ie, addition/removal of a 'locals' or 'rcpthost' value.

# Config options
$LDAP_SERVER="localhost";
$LDAP_SEARCH="/usr/bin/ldapsearch";
$LDAP_CTRLDN="ou=QmailLDAP,dc=bayour,dc=com";
$QMAIL_INIT="/etc/init.d/qmail";
$HOSTNAME="papadoc.bayour.com";

# Don't touch these
$local = $rcpthost = '';
@FILES = ("locals", "rcpthosts");

# ----------------------------
# Get the QmailLDAP/Controls object for specified host ($HOSTNAME)
open(SEARCH, "$LDAP_SEARCH -x -LLL -h $LDAP_SERVER -b '$LDAP_CTRLDN' 'cn=$HOSTNAME' locals rcpthosts |")
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
foreach $local (sort(keys(%LOCALS))) { $AMOUNT{'calculated'}{'locals'}++; }
foreach $rcpthost (sort(keys(%RCPTHOSTS))) { $AMOUNT{'calculated'}{'rcpthosts'}++; }

# ----------------------------
# Open the files with the old values
foreach $file (@FILES) {
    open(FILE, "< /etc/qmail/.$file");
    $line = <FILE>; chomp($line);
    $AMOUNT{'oldvalue'}{$file} = $line;
    close(FILE);

    if($AMOUNT{'oldvalue'}{$file} and $AMOUNT{'calculated'}{$file}) {
	if($AMOUNT{'oldvalue'}{$file} != $AMOUNT{'calculated'}{$file}) {
	    # Value have changed!
	    open(FILE, "> /etc/qmail/.$file") || die("Can't open file, $!\n");
	    print FILE $AMOUNT{'calculated'}{$file} . "\n";
	    close(FILE);
	    
	    $changed = 1;
	}
    }
}

if($changed) {
    print "Value have changed\n";
    system($QMAIL_INIT, "restart");
}
