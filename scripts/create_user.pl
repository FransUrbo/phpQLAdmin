#!/usr/bin/perl -U

exit(0);

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

# PQL_HOMEDIRECTORY="/afs/bayour.com/user/test/test"
# PQL_LOGINSHELL="/bin/false"
# PQL_MAIL="test@test"
# PQL_MAILHOST="emil.swe.net"
# PQL_MAILMESSAGESTORE="/var/mail/test/test"

# PQL_KADMIN_CMD="/usr/sbin/kadmin"
# PQL_KADMIN_REALM="TEST.ORG"
# PQL_KADMIN_PRINC="phpQLAdmin"
# PQL_KADMIN_SERVR="kerberos.test.org"
# PQL_KADMIN_KEYTB="/etc/krb5.keytab.phpQLAdmin"

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
}
chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR);

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

if($ENV{"PQL_USERPASSWORD"} =~ /kerberos/i) {
    # Add the Kerberos principal
    if(-x $ENV{"PQL_KADMIN_CMD"} && $ENV{"PQL_USERPASSWORD"}) {
	$principal = (split('}', $ENV{"PQL_USERPASSWORD"}))[1];
	$principal = (split('\@', $principal))[0];
	
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
	push(@args, "ank -randkey $principal");
	
	system(@args) == 0 or die "system command '@args' failed: $?"
    }
}
