#!/bin/sh

# $Id: extract_dnsserver_zones.sh,v 1.2 2005-02-24 17:04:02 turbo Exp $

# Uncomment for real live action!!
# -> BE CAREFULL!!
DEBUG=1

# Setup paths to where the bind zones are
BIND9PATH=/etc/bind
BIND9CONF="$BIND9PATH/named.conf.zones-master"

# Setup where the commands we need are
LDAP2ZONE="/usr/sbin/ldap2zone"
LDAPSEARCH="/usr/bin/ldapsearch -LLL"

# Setup the location of the LDAP server and how to reach it
LDAPBIND="-Y GSSAPI"
LDAPSERVER="-H ldapi://%2fvar%2frun%2fslapd%2fldapi.provider"
LDAPBASEDN="-b c=SE"

# -----------------------------------
# ---- NO MODIFYABLE PARTS BELOW ----
# -----------------------------------

# Retreive all zones
LDAPCMD="$LDAPSEARCH $LDAPBIND $LDAPSERVER $LDAPBASEDN "
ZONES=`$LDAPCMD zoneName=* zonename 2> /dev/null | grep -i ^zonename | sort | uniq | sed 's@^zonename: @@i'`
ZONES=`echo $ZONES` # Get rid of newlines...

# Get rid of the '-H ' in the server variable
set -- `echo $LDAPSERVER`
server=$2 

# Get rid of the '-b ' in the base DN
set -- `echo $LDAPBASEDN`
basedn=$2

# Create a temporary directory where we can put our
# retreived zone files in.
TMPDIR=`tempfile -p bind.`
rm -f $TMPDIR && mkdir $TMPDIR
cd $TMPDIR || exit 10

[ ! -z "$DEBUG" ] && echo=echo
for zone in $ZONES; do
    # Retreive the zone data for each zone
    [ ! -z "$DEBUG" ] && echo -n "Retreiving $zone: "
    ldap2zone $zone $server/$basedn 3600 > db.$zone 2> /dev/null
    [ ! -z "$DEBUG" ] && echo "done."

    # Check differences between retreived zone and the one bind9 knows about
    [ ! -z "$DEBUG" ] && echo -n "Diffing $zone: "
    if [ -e "$BIND9PATH/db.$zone" ]; then
	set -- `diff -q $BIND9PATH/db.$zone db.$zone`
	if [ "$5" == "differ" ]; then
	    # No matter what, LDAP _always_ overrides the filesystem!!
	    $echo cp -v db.$zone $BIND9PATH/db.$zone

	    error=1 # Make sure the DNS is reloaded
	elif [ ! -z "$DEBUG" ]; then
	    echo "no difference."
	fi
    else
	# Does not exist previosly
	$echo cp -v db.$zone $BIND9PATH/db.$zone

	error=1 # Make sure the DNS is reloaded
    fi

    # Check if this zone is configured into the config file
    # BUG: Doesn't take into account if the zone parts is commented out!!
    [ ! -z "$DEBUG" ] && echo -n "Checking existance: "
    if ! grep -q $zone $BIND9CONF; then
	echo "ERROR: $zone doesn't exists in '$BIND9CONF'!"
	error=2 # Make sure the DNS is fixed
    elif [ ! -z "$DEBUG" ]; then
	echo "exists."
    fi

    [ ! -z "$DEBUG" ] && echo
done


if [ ! -z "$error" ]; then
    case "$error" in
	1) echo "Zones have changed - reload the DNS!"; break;;
	2) echo "Zones are missing - fix the '$BIND9CONF' file!"; break;;
	*) echo "Unknown error: '$error'"; break;;
    esac
    
    rm -Rf $TMPDIR

    exit $error
fi
