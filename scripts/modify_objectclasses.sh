#!/bin/sh

# This script is quite uggly! What it (tries) to do is
# _add_ the objectclass 'phpQLAdminGlobal' to all objects
# that need it (usually ALL the base dn's).
#
# Pipe the output of this file to either 'ldapmodify', or
# if you want to be sure it's doing what it's supposed to,
# a file which you can then run using 'ldapmodify -f file'.

old_IFS=$IFS

DNs=`ldapsearch -x -LLL -h localhost -s base -b '' objectclass=* namingContexts | grep -i ^namingContexts | sed -e 's@.*: @@' -e 's@$@;@'`
DNs=`echo $DNs`

IFS=\;
for basedn in $DNs; do
    basedn=`echo $basedn | sed 's@^ @@'`

    IFS=$old_IFS
    OCs=`ldapsearch -LLL -h localhost -b "$basedn" -s base \
	'(|(krb5RealmName=*)(whoAreWe=*)(controlBaseDn=*)(ezmlmAdministrator=*)(controlsAdministrator=*)(allowServerChange=*)(autoReload=*)(language=*)(useEzmlm=*)(useBind9=*)(useControls=*))' objectClass 2> /dev/null | \
	grep -v '^$' | grep -i ^objectClass | sed -e 's@objectClass: @@i' -e 's@$@;@'`
    OCs=`echo $OCs`

    echo "dn: $basedn"
    echo "replace: objectClass"

    IFS=\;
    for oc in $OCs; do
	oc=`echo $oc | sed 's@^ @@'`
	echo "objectClass: $oc"
    done
    echo "objectClass: phpQLAdminGlobal"
    echo
done
