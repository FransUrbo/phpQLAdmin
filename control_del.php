<?php
// Delete a mailserver controls object
// $Id: control_del.php,v 2.6 2003-04-04 16:37:20 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

if($config["PQL_GLOB_CONTROL_USE"]) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

    include("./header.html");

    // Check to see if this object exists
    $sr = ldap_search($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR,
		      "(cn=" . $host . ")", array("cn"));
    if(ldap_count_entries($_pql_control->ldap_linkid, $sr) > 0) {
	// Exists - get DN
	$dn = ldap_get_dn($_pql_control->ldap_linkid,
			  ldap_first_entry($_pql_control->ldap_linkid, $sr));
	if(! ldap_delete($_pql_control->ldap_linkid, $dn)) {
	    // could not delete object
	    $msg = urlencode("Failed to delete mailserver $host.");
	    header("Location: " . $config["PQL_GLOB_URI"] . "control_detail.php?host=$host&msg=$msg");
	} else {
	    // successfully deleted object
	    $msg = urlencode("Successfully deleted mailserver $host.");
	    header("Location: " . $config["PQL_GLOB_URI"] . "home.php?msg=$msg&rlnb=2");
	}
    }
}
// else - PQL_GLOB_CONTROL_USE isn't set!

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
