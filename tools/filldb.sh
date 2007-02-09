#!/bin/sh

# user ID counter
uidNumber=10000

# BRANCHES
for branch in `seq 0 300`; do
    branch=`printf "%0.3d" $branch`
    cat <<EOF
dn: o=TST_$branch,c=SE
o: TST_$branch
objectClass: organization
OpenLDAPaci: 0#entry#grant;r,s,c;objectClass,[entry]#public#
OpenLDAPaci: 1#entry#grant;r,s,c;o#public#
OpenLDAPaci: 2#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=People,o=Fredriksson,c=SE

dn: ou=People,o=TST_$branch,c=SE
objectClass: organizationalUnit
ou: People
OpenLDAPaci: 0#entry#grant;r,s,c;objectClass,[entry]#public#
OpenLDAPaci: 1#entry#grant;r,s,c;ou#public#
OpenLDAPaci: 2#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=People,o=Fredriksson,c=SE

EOF

    # USERS
    for user in `seq 0 10`; do
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
cn: Test User 
gecos: Test User #$user
sn: #$user
qmailDotMode: none
objectClass: person
objectClass: inetOrgPerson
objectClass: posixAccount
objectClass: mailRecipient
objectClass: qmailUser
deliveryProgramPath: /usr/bin/maildrop
deliveryMode: nolocal
mail: user$user@test.bayour.com
OpenLDAPaci: 0#entry#grant;r,s,c;objectClass,[entry]#public#
OpenLDAPaci: 1#entry#grant;c,x;userPassword,krb5PrincipalName,cn,mail,mailAlte
 rnateAddress#public#
OpenLDAPaci: 2#entry#grant;s,c;mail,mailAlternateAddress#access-id#uid=spam,ou
 =System,o=Bayour.COM,c=SE
OpenLDAPaci: 3#entry#grant;r;spamAssassin#access-id#uid=spam,ou=System,o=Bayou
 r.COM,c=SE
OpenLDAPaci: 4#entry#grant;r,s,c,x;userPassword,krb5PrincipalName,cn,mail,mail
 AlternateAddress#access-id#uid=ldap/proxy-1,ou=LDAP,o=Bayour.COM,c=SE
OpenLDAPaci: 5#entry#grant;r,s,c;uid,cn,accountStatus,uidNumber,gidNumber,geco
 s,homeDirectory,loginShell,trustModel,accessTo,mailMessageStore#public#
OpenLDAPaci: 6#entry#grant;r,s,c;mail,mailAlternateAddress,mailHost,mailQuotaS
 ize,mailQuotaCount,accountStatus,deliveryMode,userPassword,mailMessageStore,d
 eliveryProgramPath,mailForwardingAddress#access-id#uid=qmail,ou=System,o=Bayo
 ur.COM,c=SE
OpenLDAPaci: 7#entry#grant;r,s,c;sn,givenName,homePostalAddress,mobile,homePho
 ne,labeledURI,mailForwardingAddress,street,physicalDeliveryOfficeName,mailMes
 sageStore,o,l,st,telephoneNumber,postalCode,title#users#
OpenLDAPaci: 8#entry#grant;w,r,s,c;sn,givenName,homePostalAddress,mobile,homeP
 hone,labeledURI,mailForwardingAddress,street,physicalDeliveryOfficeName,o,l,s
 t,telephoneNumber,postalCode,title,deliveryMode,userPassword#self#
OpenLDAPaci: 9#entry#grant;w,r,s,c,x;[all]#access-id#uid=turbo,ou=People,o=Fre
 driksson,c=SE

EOF
    done
done
