#!/bin/sh

if [ -r "/etc/krb5.keytab.webserver" ]; then
    kinit -5 -l 5m -k -p webserver -t /etc/krb5.keytab.webserver
fi

if [ $# -lt 3 ]; then
    echo "Usage: $0 <rsh_host> <rsh_user> <rcmd> <dir>"
    exit 1
fi

host=$1 ; shift
user=$1 ; shift
cmd=$1  ; shift
opts=$*

if [ "$cmd" == "rcp" ]; then
    set -- `echo $opts`
    src=$1 ; dst=$2

    echo "rcp -x $src $user@$host:$dst"
else
    rsh -x -l $user $host "$cmd $opts" 2> /dev/null
    #echo "rsh -x -l $user $host \"$cmd $opts\""
fi
