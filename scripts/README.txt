This directory contain some EXAMPLE scripts.

NOTE: They are EXAMPLES - you MUST change them before use!


In some cases you might want to have external scripts
that does 'some magic' when creating a branch, user
or web server object. This is possible by have 
phpQLAdmin call any of these scripts:
     create_domain.pl
     create_user.pl
     create_websrv_cfg.pl
     sub_ezmlm_create.pl

To be able to dynamically add/remove/modify branch
administrators, you MUST have ACI's. Othervise, for
each admin change, you must update the ACL file and
restart slapd. I wrote a script that adds ACI's to
an existing database:
     add_aci_support_to_all_objects_in_database.pl

A while back, I modified the phpQLAdmin schema (moved
some attributes from one object class to another). This
script takes care of this change:
     modify_objectclasses.sh

When using QmailLDAP/Controls, qmail needs to be restarted
every time a change to the locals and/or rcpthosts attributes
have been done. It's not nice to restart it regularly,
but only when a change have been done/discovered. This cron
script takes care of checking the database against local
files, and restarts qmail if/when a change have been done.
Run it from cron every now and then (every thirty minute is
a nice value):
     restart_qmail.pl

I have failed to get virtual hosting for Apache into LDAP
working, so instead (to get SOMETHING going), I created a
script that extracts the needed information and creates
the virtual hosting part of httpd.conf. This script
can/should be used from a cron script to create a file that
is then included from Apache's httpd.conf.
