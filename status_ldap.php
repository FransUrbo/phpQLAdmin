<?php
// Show Connection/Suffixes status of LDAP server
// $Id: status_ldap.php,v 2.4 2004-02-14 14:01:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_status.inc");

require("./left-head.html");
include("./header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// Get the LDAP server bootup time
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Start,cn=Time,cn=Monitor", "createTimeStamp");
$timestamp_start = preg_replace('/Z$/', '', $tmp);
$time_start = pql_format_timestamp($tmp);

// Get the current LDAP server time
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Current,cn=Time,cn=Monitor", "modifyTimeStamp");
if($tmp) {
	$timestamp_current = preg_replace('/Z$/', '', $tmp);
	$time_current = pql_format_timestamp($tmp);
} else {
	$timestamp_current = $time_current = "n/a";
}

// Calculate the uptime
if($timestamp_current and $timestamp_start) {
	$time_uptime  = round(($timestamp_current - $timestamp_start) / 60, 2);
} else {
	$time_uptime  = "n/a";
}

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
