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
# PQL_KADMIN_CL="-r TEST.ORG -p phpQLAdmin -s kerberos.test.org -k -t /etc/krb5.keytab.phpQLAdmin"

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
if(-x $ENV{"PQL_KADMIN_CMD"} && $ENV{"PQL_USERPASSWORD"} && $ENV{"PQL_KADMIN_CL"}) {
    $principal = (split('}', $ENV{"PQL_USERPASSWORD"}))[1];
    $principal = (split('\@', $principal))[0];

    @args = ($ENV{"PQL_KADMIN_CMD"}, $ENV{"PQL_KADMIN_CL"}, "-q 'ank -randkey $principal'");
    system(@args) == 0 or die "system command '@args' failed: $?"
}
