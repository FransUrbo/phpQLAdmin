#!/bin/sh

# user ID counter
uidNumber=10000

# BRANCHES
for branch in `seq 1 300`; do
    branch=`printf "%0.3d" $branch`
    cat <<EOF
dn: o=TST_$branch,c=SE
o: TST_$branch
objectClass: organization
OpenLDAPaci: 0#entry#grant;r,s,c;objectclass#public#
OpenLDAPaci: 1#entry#grant;r,s,c;entry#public#
OpenLDAPaci: 2#entry#grant;r,s,c;o#public#
OpenLDAPaci: 3#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=people,o=fredriksson,c=se

dn: ou=People,o=TST_$branch,c=SE
objectclass: organizationalUnit
ou: People
OpenLDAPaci: 0#entry#grant;r,s,c;objectclass#public#
OpenLDAPaci: 1#entry#grant;r,s,c;entry#public#
OpenLDAPaci: 2#entry#grant;r,s,c;ou#public#
OpenLDAPaci: 3#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=people,o=fredriksson,c=se

dn: ou=Groups,o=TST_$branch,c=SE
objectclass: organizationalUnit
ou: Groups
OpenLDAPaci: 0#entry#grant;r,s,c;objectclass#public#
OpenLDAPaci: 1#entry#grant;r,s,c;entry#public#
OpenLDAPaci: 2#entry#grant;r,s,c;ou#public#
OpenLDAPaci: 3#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=people,o=fredriksson,c=se

EOF

    # USERS
    for user in `seq 1 10`; do
	user=`printf "%0.3d" $user`
	cat <<EOF
dn: uid=user$user,ou=People,o=TST_$branch,c=SE
uid: user$user
userPassword: seCreT
loginShell: /bin/false
uidNumber: $uidNumber
gidNumber: $uidNumber
mailMessageStore: /var/mail/TST_$branch/user$user/
homeDirectory: /afs/bayour.com/tests/TST_$branch/user$user/
mailHost: aurora.bayour.com
cn: Test User $user
gecos: Test User #$user
sn: $user
qmailDotMode: none
objectclass: person
objectclass: inetOrgPerson
objectclass: posixAccount
objectclass: mailRecipient
objectclass: qmailUser
deliveryProgramPath: /usr/bin/maildrop
deliveryMode: nolocal
mail: user$user@test.bayour.com
OpenLDAPaci: 0#entry#grant;r,s,c;objectclass#public#
OpenLDAPaci: 1#entry#grant;r,s,c;entry#public#
OpenLDAPaci: 2#entry#grant;c,x;userpassword#public#
OpenLDAPaci: 3#entry#grant;c,x;krb5principalname#public#
OpenLDAPaci: 4#entry#grant;c,x;cn#public#
OpenLDAPaci: 5#entry#grant;c,x;mail#public#
OpenLDAPaci: 6#entry#grant;c,x;mailalternateaddress#public#
OpenLDAPaci: 7#entry#grant;s,c;mail#access-id#uid=spam,ou=system,o=bayour.com,c=se
OpenLDAPaci: 8#entry#grant;s,c;mailalternateaddress#access-id#uid=spam,ou=system,o=bayour.com,c=se
OpenLDAPaci: 9#entry#grant;r;spamassassin#access-id#uid=spam,ou=system,o=bayour.com,c=se
OpenLDAPaci: 10#entry#grant;r,s,c,x;userpassword#access-id#uid=ldap/proxy-1,ou=ldap,o=bayour.com,c=se
OpenLDAPaci: 11#entry#grant;r,s,c,x;krb5principalname#access-id#uid=ldap/proxy-1,ou=ldap,o=bayour.com,c=se
OpenLDAPaci: 12#entry#grant;r,s,c,x;cn#access-id#uid=ldap/proxy-1,ou=ldap,o=bayour.com,c=se
OpenLDAPaci: 13#entry#grant;r,s,c,x;mail#access-id#uid=ldap/proxy-1,ou=ldap,o=bayour.com,c=se
OpenLDAPaci: 14#entry#grant;r,s,c,x;mailalternateaddress#access-id#uid=ldap/proxy-1,ou=ldap,o=bayour.com,c=se
OpenLDAPaci: 15#entry#grant;r,s,c;uid#public#
OpenLDAPaci: 16#entry#grant;r,s,c;cn#public#
OpenLDAPaci: 17#entry#grant;r,s,c;accountstatus#public#
OpenLDAPaci: 18#entry#grant;r,s,c;uidnumber#public#
OpenLDAPaci: 19#entry#grant;r,s,c;gidnumber#public#
OpenLDAPaci: 20#entry#grant;r,s,c;gecos#public#
OpenLDAPaci: 21#entry#grant;r,s,c;homedirectory#public#
OpenLDAPaci: 22#entry#grant;r,s,c;loginshell#public#
OpenLDAPaci: 23#entry#grant;r,s,c;trustmodel#public#
OpenLDAPaci: 24#entry#grant;r,s,c;accessto#public#
OpenLDAPaci: 25#entry#grant;r,s,c;mailmessagestore#public#
OpenLDAPaci: 26#entry#grant;r,s,c;mail#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 27#entry#grant;r,s,c;mailalternateaddress#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 28#entry#grant;r,s,c;mailhost#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 29#entry#grant;r,s,c;mailquotasize#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 30#entry#grant;r,s,c;mailquotacount#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 31#entry#grant;r,s,c;accountstatus#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 32#entry#grant;r,s,c;deliverymode#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 33#entry#grant;r,s,c;userpassword#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 34#entry#grant;r,s,c;mailmessagestore#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 35#entry#grant;r,s,c;deliveryprogrampath#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 36#entry#grant;r,s,c;mailforwardingaddress#access-id#uid=qmail,ou=system,o=bayour.com,c=se
OpenLDAPaci: 37#entry#grant;r,s,c;sn#users#
OpenLDAPaci: 38#entry#grant;r,s,c;givenname#users#
OpenLDAPaci: 39#entry#grant;r,s,c;homepostaladdress#users#
OpenLDAPaci: 40#entry#grant;r,s,c;mobile#users#
OpenLDAPaci: 41#entry#grant;r,s,c;homephone#users#
OpenLDAPaci: 42#entry#grant;r,s,c;labeleduri#users#
OpenLDAPaci: 43#entry#grant;r,s,c;mailforwardingaddress#users#
OpenLDAPaci: 44#entry#grant;r,s,c;street#users#
OpenLDAPaci: 45#entry#grant;r,s,c;physicaldeliveryofficename#users#
OpenLDAPaci: 46#entry#grant;r,s,c;mailmessagestore#users#
OpenLDAPaci: 47#entry#grant;r,s,c;o#users#
OpenLDAPaci: 48#entry#grant;r,s,c;l#users#
OpenLDAPaci: 49#entry#grant;r,s,c;st#users#
OpenLDAPaci: 50#entry#grant;r,s,c;telephonenumber#users#
OpenLDAPaci: 51#entry#grant;r,s,c;postalcode#users#
OpenLDAPaci: 52#entry#grant;r,s,c;title#users#
OpenLDAPaci: 53#entry#grant;w,r,s,c;sn#self#
OpenLDAPaci: 54#entry#grant;w,r,s,c;givenname#self#
OpenLDAPaci: 55#entry#grant;w,r,s,c;homepostaladdress#self#
OpenLDAPaci: 56#entry#grant;w,r,s,c;mobile#self#
OpenLDAPaci: 57#entry#grant;w,r,s,c;homephone#self#
OpenLDAPaci: 58#entry#grant;w,r,s,c;labeleduri#self#
OpenLDAPaci: 59#entry#grant;w,r,s,c;mailforwardingaddress#self#
OpenLDAPaci: 60#entry#grant;w,r,s,c;street#self#
OpenLDAPaci: 61#entry#grant;w,r,s,c;physicaldeliveryofficename#self#
OpenLDAPaci: 62#entry#grant;w,r,s,c;o#self#
OpenLDAPaci: 63#entry#grant;w,r,s,c;l#self#
OpenLDAPaci: 64#entry#grant;w,r,s,c;st#self#
OpenLDAPaci: 65#entry#grant;w,r,s,c;telephonenumber#self#
OpenLDAPaci: 66#entry#grant;w,r,s,c;postalcode#self#
OpenLDAPaci: 67#entry#grant;w,r,s,c;title#self#
OpenLDAPaci: 68#entry#grant;w,r,s,c;deliverymode#self#
OpenLDAPaci: 69#entry#grant;w,r,s,c;userpassword#self#
OpenLDAPaci: 70#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=people,o=fredriksson,c=se

dn: cn=user$user,ou=Groups,o=TST_$branch,c=SE
objectclass: posixGroup
cn: user$user
gidNumber: $uidNumber
OpenLDAPaci: 0#entry#grant;r,s,c;objectclass#public#
OpenLDAPaci: 1#entry#grant;r,s,c;entry#public#
OpenLDAPaci: 2#entry#grant;r,s,c;cn#public#
OpenLDAPaci: 3#entry#grant;r,s,c;gidnumber#public#
OpenLDAPaci: 4#entry#grant;r,s,c;memberuid#public#
OpenLDAPaci: 5#entry#grant;w,r,s,c;[all]#access-id#uid=turbo,ou=people,o=fredriksson,c=se

EOF
    done
done
