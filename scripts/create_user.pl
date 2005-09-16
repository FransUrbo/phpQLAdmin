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
    for($i=1; $i < count($dirs); $i++) {
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
    for($i=1; $i < count($dirs); $i++) {
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
