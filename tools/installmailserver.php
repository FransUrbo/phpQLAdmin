<?php
// This creates a script to be executed on the new mailserver,
// and configures all the nessesary files for QmailLDAP/Controls.
// $Id: installmailserver.php,v 1.4.2.1 2003-11-24 18:07:02 dlw Exp $
//
// Creates the following files
//	ldapserver
//	ldaplogin
//	ldappassword
//	ldapcontroldn
//	me
session_start();
require("./include/pql_config.inc");
include("./include/pql_control.inc");

$_pql = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
$ldap = $_pql->ldap_linkid;

$me = $host;
$cn = pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $me . "," . $_SESSION["USER_SEARCH_DN_CTR"];

$attribs = array(pql_get_define("PQL_GLOB_ATTR_LDAPSERVER"),
		 pql_get_define("PQL_GLOB_ATTR_LDAPLOGIN"),
		 pql_get_define("PQL_GLOB_ATTR_LDAPPASSWORD"));
foreach($attribs as $attrib) {
    $value = pql_control_get_attribute($ldap, $cn, $attrib);
    if(! $value[0])
      $unset = 1;
    $$attrib = $value[0];
}
?>
<html>
  <body>
    <pre>
#!/bin/sh

<?php	if($unset) { ?>
# There's unset values, correct this, then remove the exit
# line below...
exit 1

<?php	} ?>
cd ~qmaild/control || exit 1
echo > me		"<?php if($me) { echo $me; } else { echo "<b>DON'T FORGET THIS</b>"; }?>"
echo > ldapcontroldn	"<?php if($_SESSION["USER_SEARCH_DN_CTR"]) { echo $_SESSION["USER_SEARCH_DN_CTR"]; } else { echo "<b>DON'T FORGET THIS</b>"; }?>"
echo > ldapserver	"<?php if($ldapserver) { echo $ldapserver; } else { echo "<b>DON'T FORGET THIS</b>"; }?>"
echo > ldaplogin	"<?php if($ldaplogin) { echo $ldaplogin; } else { echo "<b>DON'T FORGET THIS</b>"; }?>"
echo > ldappassword	"<?php if($ldappassword) { echo $ldappassword; } else { echo "<b>DON'T FORGET THIS</b>"; }?>"

# Don't change below!
chown qmails.qmail ldapcontroldn ldapserver ldaplogin ldappassword
chmod 644 me ldapcontroldn ldapserver
chmod 640 ldaplogin ldappassword
    </pre>
  </body>
</html>
