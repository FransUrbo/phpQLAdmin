#!/usr/bin/perl -U

# Environment variables of interest
# PQL_DOMAIN="test"
# PQL_DOMAINNAME=""
# PQL_HOMEDIRS="/afs/bayour.com/user/test/"
# PQL_MAILDIRS="/var/mail/test/"
# PQL_QUOTA=""
# PQL_WEBUSER="33"

$ENV{PATH} = "/bin:/usr/bin";

# One thing that we might want to do is
# create the directory:
#	$PQL_MAILDIRS/mailinglists
# and chown it to the user PHP/Webserver
# is running as. This so that we can create
# ezmlm mailinglists from phpQLAdmin.
#
if($ENV{"PQL_MAILDIRS"} && $ENV{"PQL_WEBUSER"}) {
    $DIR = $ENV{PQL_MAILDIRS}."mailinglists";
    @dirs = split('/', $DIR);

    $directory = '/' . $dirs[1] . '/';
    for($i=1; $dirs[$i]; $i++) {
	if(! -d $directory) {
	    print "Creating $directory\n";
	    if(! mkdir($directory) ) {
		print "Unsuccessfull in creating $dir, $!\n";
	    }
	}

	$directory .= $dirs[$i+1] . '/'
    }

    chown($ENV{"PQL_WEBUSER"}, 0, $DIR);
}

if($ENV{"PQL_HOMEDIRS"}) {
    $DIR = $ENV{PQL_HOMEDIRS};
    @dirs = split('/', $DIR);

    $directory = '/' . $dirs[1] . '/';
    for($i=1; $dirs[$i]; $i++) {
	if(! -d $directory) {
	    print "Creating $directory\n";
	    if(! mkdir($directory) ) {
		print "Unsuccessfull in creating $dir, $!\n";
	    }
	}

	$directory .= $dirs[$i+1] . '/'
    }
}

#print "// beg ($0)\n";
#system('/usr/bin/env');
#print "// end\n";
