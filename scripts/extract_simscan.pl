#!/usr/bin/perl -w

# $Id: extract_simscan.pl,v 1.2 2005-09-16 06:08:52 turbo Exp $

$DEBUG		 = 1;
$SIMSCAN_CONF	 = "/etc/qmail/simcontrol";

# Setup where the commands we need are
$LDAPSEARCH	 = "/usr/bin/ldapsearch -LLL";

# Setup the location of the LDAP server and how to reach it
$LDAPBIND	 = "-Y GSSAPI";
$LDAPSERVER	 = "-H ldapi://%2fvar%2frun%2fslapd%2fldapi.main";
$LDAPBASEDN	 = "-b c=SE";

# -----------------------------------
# ---- NO MODIFYABLE PARTS BELOW ----
# -----------------------------------

# What attributes we're interested in
$attrs		 = "simScanSpamAssassin simScanClamAntiVirus simScanClamAntiVirus ";
$attrs		.= "simScanTrophie simScanSpamAssassinHits simScanAttachmentSuffix ";
$attrs		.= "defaultDomain additionalDomainName";

# How to search
$SEARCH		 = '(|(simScanSpamAssassin=*)(simScanClamAntiVirus=*)(simScanTrophie=*)(simScanSpamAssassinHits=*)(simScanAttachmentSuffix=*))';

# Shortcut
$LDAPCMD	 = "$LDAPSEARCH $LDAPBIND $LDAPSERVER $LDAPBASEDN";

# Retreive all zones
print STDERR "CMD: '$LDAPCMD -s base useSimScan'\n" if($DEBUG);
open(CMD, "$LDAPCMD -s base useSimScan 2> /dev/null |") ||
    die("Can't execute ldapsearch, $!\n");
while(! eof(CMD)) {
    $line = <CMD>; chomp($line);
    $use_simscan = 1 if($line =~ /^useSimScan: TRUE/i);
}
close(CMD);

if($use_simscan) {
    print STDERR "CMD: '$LDAPCMD -s sub '$SEARCH' $attrs'\n\n" if($DEBUG);

    $i = 0; $j = 0;
    open(CMD, "$LDAPCMD -s sub '$SEARCH' $attrs 2> /dev/null |") ||
	die("Can't retreive information, $!\n");
    while(! eof(CMD)) {
	$line = <CMD>; chomp($line);
	next if($line =~ /^$/);
#	print STDERR "line: '$line'\n" if($DEBUG);

	if($line =~ /^dn: /i) {
	    $i++; $j = 0;

	    $line =~ s/dn: //;
	    $DNs[$i] = $line;
	} elsif($line =~ /^ /) {
	    $line =~ s/^ //;
	    $DNs[$i-1] .= $line;
	} else {
	    ($attr, $value) = split(': ', $line);
	    $attr = lc($attr);
	    print "$attr = $value\n";

	    if($attr eq 'simscanattachmentsuffix') {
		$SUFFIX[$i][$j] = $value;
		$j++;
	    } else {
		if($value eq 'TRUE') {
		    $VALUE[$i]{$attr} = 1;
		}
	    }
	}
    }
    close(CMD);
}

print "\n";

@ATTRS = split(' ', $attrs);
foreach $attr (@ATTRS) {
    $attr = lc($attr);

    for($i=1; $i < count($VALUE); $i++) {
	if($VALUE[$i]{$attr}) {
	    print "VALUE ($i/$attr): '",$VALUE[$i]{$attr},"'\n";
	}
    }
}
