#!/usr/bin/suidperl -U

# Environment variables of interest
# PQL_ACCOUNTSTATUS="active"
# PQL_CN="Test User"
# PQL_DELIVERYMODE="localdelivery"
# PQL_DOMAIN="test"
# PQL_GIDNUMBER="1001"
# PQL_HOMEDIRECTORY="/afs/bayour.com/user/test/test"
# PQL_LOGINSHELL="/bin/false"
# PQL_MAIL="test@test"
# PQL_MAILHOST="emil.swe.net"
# PQL_MAILMESSAGESTORE="/var/mail/test/test"
# PQL_SN="Test"
# PQL_UID="test"
# PQL_UIDNUMBER="1001"
# PQL_USERPASSWORD="{KERBEROS}test@TEST.ORG"
# PQL_KADMIN_CMD="/usr/sbin/kadmin"
# PQL_KADMIN_REALM="TEST.ORG"
# PQL_KADMIN_PRINC="phpQLAdmin"
# PQL_KADMIN_SERVR="kerberos.test.org"
# PQL_KADMIN_KEYTB="/etc/krb5.keytab.phpQLAdmin"

$ENV{PATH} = "/bin:/usr/bin:/usr/sbin";

# One thing that we might want to do is
# create the mail directory:
#	$PQL_MAILMESSAGESTORE
if($ENV{"PQL_MAILMESSAGESTORE"}) {
    $DIR = $ENV{"PQL_MAILMESSAGESTORE"};
    @dirs = split('/', $DIR);

    $directory = '/' . $dirs[1] . '/';
    for($i=1; $dirs[$i]; $i++) {
	if(! -d $directory) {
	    print "Creating $directory\n";
	    if(! mkdir($directory) ) {
		print "Unsuccessfull in creating $dir, $!\n";
		error++;
	    }
	}

	$directory .= $dirs[$i+1] . '/';
    }
}

if(!$error) {
    chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR);
} else {
    exit($error);
}

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
