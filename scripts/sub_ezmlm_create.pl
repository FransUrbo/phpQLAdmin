#!/usr/bin/suidperl -U

# Environment variables of interest
# PQL_USERGROUP=alias.www-admin
# PQL_DIRECTORY=/var/mail/test/test

$ENV{PATH} = "/bin:/usr/bin";

if($ENV{"PQL_DIRECTORY"} && $ENV{"PQL_USERGROUP"}) {
    $ug  = $ENV{PQL_USERGROUP};
    $dir = $ENV{PQL_DIRECTORY};

    system("chown -R $ug $dir");
}
