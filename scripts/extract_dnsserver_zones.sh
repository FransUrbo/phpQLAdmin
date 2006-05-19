#!/bin/sh

# $Id: extract_dnsserver_zones.sh,v 1.4 2006-05-19 07:14:45 turbo Exp $

# Uncomment for real live action!!
# -> BE CAREFULL!!
#DEBUG=1

SOA_REPLACEMENT=ns1.bayour.com

# Setup paths to where the bind zones are
BIND9PATH=/etc/bind
BIND9CONF="$BIND9PATH/named.conf.zones-master"

# Setup where the commands we need are
LDAP2ZONE="/usr/sbin/ldap2zone"
LDAPSEARCH="/usr/bin/ldapsearch -LLL"

basedn="<CHANGE>"  # DN
binddn="<CHANGE>"  # DN
bindpw="<CHANGE>"  # Cleartext password
server="<CHANGE>"  # LDAP URI (used with 'ldapXXX -H ...')

# Setup the location of the LDAP server and how to reach it
LDAPBIND="-x -D $binddn -w $bindpw"
LDAPSERVER="-H $server"
LDAPBASEDN="-b $basedn"

# -----------------------------------
# ---- NO MODIFYABLE PARTS BELOW ----
# -----------------------------------

# Retreive all zones
LDAPCMD="$LDAPSEARCH $LDAPBIND $LDAPSERVER $LDAPBASEDN "
ZONES=`$LDAPCMD zoneName=* zonename 2> /dev/null | grep -i ^zonename | sort | uniq | sed 's@^zonename: @@i'`
ZONES=`echo $ZONES` # Get rid of newlines...

# Create a temporary directory where we can put our
# retreived zone files in.
TMPDIR=`tempfile -p bind.`
rm -f $TMPDIR && mkdir $TMPDIR
pushd $TMPDIR > /dev/null 2>&1 || exit 10

[ -n "$DEBUG" ] && echo=echo
for zone in $ZONES; do
    # Skipping this. It keeps changing outside my control...
    if [ "$zone" != "dagdrivarn.se" ]; then
	# Retreive the zone data for each zone
	[ -n "$DEBUG" ] && echo -n "Retreiving $zone: "
	ldap2zone -D $binddn -w $bindpw $zone $server/$basedn 3600 > db.$zone 2> /dev/null
	if [ -s "db.$zone" ]; then
	    [ -n "$DEBUG" ] && echo "done."

	    # Replace the SOA
	    cat db.$zone | sed "s@\(.*\)ns.*\. \([a-z]\)\(.*\)@\1$SOA_REPLACEMENT. \2\3@" > db.$zone.new
	    mv db.$zone.new db.$zone
	    
	    # Check differences between retreived zone and the one bind9 knows about
	    [ -n "$DEBUG" ] && echo "Diffing $zone: "
	    if [ -e "$BIND9PATH/db.$zone" ]; then
		set -- `diff -q $BIND9PATH/db.$zone db.$zone`
		if [ "$5" == "differ" ]; then
		    # No matter what, LDAP _always_ overrides the filesystem!!
		    $echo cp -v db.$zone $BIND9PATH/db.$zone
		    $echo chown bind9.bind9 $BIND9PATH/db.$zone
		    $echo chmod 640 $BIND9PATH/db.$zone
		    
		    error=1 # Make sure the DNS is reloaded
		elif [ -n "$DEBUG" ]; then
		    echo "no difference."
		fi
	    else
		# Does not exist previosly
		$echo cp -v db.$zone $BIND9PATH/db.$zone
		$echo chown bind9.bind9 $BIND9PATH/db.$zone
		$echo chmod 640 $BIND9PATH/db.$zone
		
		error=1 # Make sure the DNS is reloaded
	    fi
	    
	    # Check if this zone is configured into the config file
	    # BUG: Doesn't take into account if the zone parts is commented out!!
	    [ -n "$DEBUG" ] && echo -n "Checking existance: "
	    if ! grep -q $zone $BIND9CONF; then
		echo "ERROR: $zone doesn't exists in '$BIND9CONF'!"
		error=2 # Make sure the DNS is fixed
	    elif [ -n "$DEBUG" ]; then
		echo "exists."
	    fi
	    
	    [ -n "$DEBUG" ] && echo
	fi
    fi
done


if [ -n "$error" ]; then
    case "$error" in
	1)
	    echo -n "Zones have changed - reloading named: "
	    /etc/init.d/bind9 restart > /dev/null 2>&1
	    if [ "$?" = "0" ]; then
		echo "success."
	    else
		echo "FAILURE!"
	    fi
	    break
	    ;;
	2) echo "Zones are missing - fix the '$BIND9CONF' file!"; break;;
	*) echo "Unknown error: '$error'"; break;;
    esac
fi

[ -n "$DEBUG" ] && rm -Rf $TMPDIR
exit $error
