#!/usr/bin/suidperl -w

# Environment variables of interest
# PQL_DOMAIN="test"
# PQL_DOMAINNAME=""
# PQL_HOMEDIRS="/afs/bayour.com/user/test/"
# PQL_MAILDIRS="/var/mail/test/"
# PQL_QUOTA=""

$ENV{PATH} = "/bin:/usr/bin";

print "// beg ($0)\n";
system('/usr/bin/env | grep ^PQL | sort');
print "// end\n";
