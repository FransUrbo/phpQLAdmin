<?php
// Show Connection/Suffixes status of LDAP server
// $Id: status_ldap.php,v 2.2 2004-01-26 05:27:52 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_status.inc");

require("./left-head.html");
include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

if($type == 'basics') {
	include("./tables/status_ldap-basic.inc");
} elseif($type == 'connections') {
	include("./tables/status_ldap-connections.inc");
} elseif($type == 'databases') {
	include("./tables/status_ldap-databases.inc");
}
?>

<?php
require("./left-trailer.html");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
