#!/usr/bin/suidperl -w

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

print "// beg ($0)\n";
system('/usr/bin/env | grep ^PQL | sort');
print "// end\n";
