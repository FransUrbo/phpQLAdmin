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
# PQL_USERPASSWORD="{KERBEROS}test@SWE.NET"

$ENV{PATH} = "/bin:/usr/bin";

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
	    }
	}

	$directory .= $dirs[$i+1] . '/';
    }
}
chown($ENV{"PQL_UIDNUMBER"}, "mail", $DIR);

#print "// beg ($0)\n";
#system('/usr/bin/env | grep ^PQL | sort');
#print "// end\n";
