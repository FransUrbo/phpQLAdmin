#!/usr/bin/perl -U

#exit(0);

# Environment variables of interest
# PQL_ACCOUNTSTATUS="active"
# PQL_DELIVERYMODE="localdelivery"
# PQL_DOMAIN="test"

# PQL_UID="test"
# PQL_CN="Test User"
# PQL_SN="Test"
# PQL_UIDNUMBER="1001"
# PQL_USERPASSWORD="{KERBEROS}test@TEST.ORG"
# PQL_GIDNUMBER="1001"

# PQL_HOMEDIRECTORY="/afs/test.org/user/test/test"
# PQL_LOGINSHELL="/bin/false"
# PQL_MAIL="test@test.org"
# PQL_MAILHOST="mail.test.org"
# PQL_MAILMESSAGESTORE="/var/mail/test/test"

# PQL_KADMIN_CMD="/usr/sbin/kadmin"
# PQL_KADMIN_REALM="TEST.ORG"
# PQL_KADMIN_PRINC="phpQLAdmin"
# PQL_KADMIN_SERVR="kerberos.test.org"
# PQL_KADMIN_KEYTB="/etc/krb5.keytab.phpQLAdmin"

sub setup_command {
    my @cmd = @_;
    my $cmd;

    for($i=0; $i < @CMD; $i++) {
	$cmd .= $CMD[$i];
	$cmd .= " " if($CMD[$i+1]);
    }

    return $cmd;
}

# Since we're suid (or should be), we must untaint the path.
$ENV{PATH} = "/bin:/usr/bin:/usr/sbin";

# Create the mail directory: $PQL_MAILMESSAGESTORE
if($ENV{"PQL_MAILMESSAGESTORE"}) {
    $DIR = $ENV{"PQL_MAILMESSAGESTORE"};
    @dirs = split('/', $DIR);

    $directory = '/' . $dirs[1] . '/';
    for($i=1; $dirs[$i]; $i++) {
	if(! -d $directory) {
	    print "Creating $directory\n";
	    if(! mkdir($directory) ) {
		die("Unsuccessfull in creating $dir, $!\n");
	    }
	}

	$directory .= $dirs[$i+1] . '/';
    }

    # Chown' the mail directory to the user, group mail.
    chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR);

    # Create the Maildir structure inside the mail directory
    mkdir($DIR."/new"); chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR."/new");
    mkdir($DIR."/cur"); chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR."/cur");
    mkdir($DIR."/tmp"); chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR."/tmp");
}

# Create the home directory: $PQL_HOMEDIRECTORY
if($ENV{"PQL_HOMEDIRECTORY"}) {
    $DIR = $ENV{"PQL_HOMEDIRECTORY"};
    @dirs = split('/', $DIR);

    $directory = '/' . $dirs[1] . '/';
    for($i=1; $dirs[$i]; $i++) {
	if(! -d $directory) {
	    print "Creating $directory\n";
	    if(! mkdir($directory) ) {
		die("Unsuccessfull in creating $dir, $!\n");
	    }
	}

	$directory .= $dirs[$i+1] . '/';
    }
}
chown($ENV{"PQL_UIDNUMBER"}, $ENV{"PQL_GIDNUMBER"}, $DIR);

if(($ENV{"PQL_USERPASSWORD"} =~ /kerberos/i) || ($ENV{"PQL_USERPASSWORD"} =~ /sasl/i)) {
    # Add the Kerberos principal
    if(-x $ENV{"PQL_KADMIN_CMD"} && $ENV{"PQL_USERPASSWORD"}) {
	$principal = (split('}', $ENV{"PQL_USERPASSWORD"}))[1];
	$principal = (split('\@', $principal))[0];
	print "Creating KerberosV principal '$principal': ";
	
	push(@args, $ENV{"PQL_KADMIN_CMD"});
	if($ENV{"PQL_KADMIN_REALM"}) {
	    push(@args, "-r");
	    push(@args, $ENV{"PQL_KADMIN_REALM"});
	}
	
	if($ENV{"PQL_KADMIN_PRINC"}) {
	    push(@args, "-p");
	    push(@args, $ENV{"PQL_KADMIN_PRINC"});
	}
	
	if($ENV{"PQL_KADMIN_SERVR"}) {
	    push(@args, "-s");
	    push(@args, $ENV{"PQL_KADMIN_SERVR"});
	}
	
	if($ENV{"PQL_KADMIN_KEYTB"}) {
	    push(@args, "-k");
	    push(@args, "-t");
	    push(@args, $ENV{"PQL_KADMIN_KEYTB"});
	}
	push(@args, "-q");
	
	# See if the principal already exists
	@CMD = @args;
	push(@CMD, "'getprinc $principal'");
	push(@CMD, "2> /dev/null");
	$cmd = &setup_command(@CMD);

	$exists = 0;
	open(CMD, "$cmd |") || die "Can't execute '$cmd', $!\n";
	while(! eof(CMD)) {
	    $line = <CMD>; chomp($line);
	    if($line =~ /^Expiration date:/) {
		$exists = 1;
		print "already exists\n";
		exit 0;
	    }
	}

	if(!$exists) {
	    @CMD = @args;
	    if($ENV{"PQL_CLEARTEXT_PASSWORD"}) {
		push(@CMD, "'ank -pw ".$ENV{"PQL_CLEARTEXT_PASSWORD"}." $principal'");
	    } else {
		push(@CMD, "'ank -randkey $principal'");
	    }
	    push(@CMD, "2> /dev/null");
	    $cmd = &setup_command(@CMD);

	    $created = 0;
	    open(CMD, "$cmd |") || die "Can't execute '$cmd', $!\n";
	    while(! eof(CMD)) {
		$line = <CMD>; chomp($line);
		if($line =~ /^Principal .* created\.$/) {
		    $created = 1;
		}
	    }

	    if($created) {
		print "done.\n";
	    } else {
		print "FAILED.\n";
	    }
	}
    }
}
