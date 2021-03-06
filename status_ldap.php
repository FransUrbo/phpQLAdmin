<?php
// Show Connection/Suffixes status of LDAP server
// $Id: status_ldap.php,v 2.14 2007-02-12 15:32:05 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_status.inc");

require("./left-head.html");
include($_SESSION["path"]."/header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// Get the LDAP server bootup and current time
//
// dn: cn=Current,cn=Time,cn=Monitor
// monitorTimestamp: 20041004133358Z  <- current time
// createTimestamp: 20041004064512Z   <- bootup time
$tmp = pql_get_status($_pql->ldap_linkid, "cn=Current,cn=Time,".$_pql->ldap_monitor, array("createTimeStamp", "monitorTimestamp"));
if($tmp['createtimestamp'] and $tmp['monitortimestamp']) {
    $timestamp_start = pql_format_timestamp_unixtime($tmp['createtimestamp']);
    $time_start = pql_format_timestamp($tmp['createtimestamp']);

    $timestamp_current = pql_format_timestamp_unixtime($tmp['monitortimestamp']);
    $time_current = pql_format_timestamp($tmp['monitortimestamp']);
} else {
    $timestampstart = $time_start = "0";
    $timestamp_current = $time_current = "0";
}

if($_REQUEST["type"] == 'basics') {
	include("./tables/status_ldap-basic.inc");
} elseif($_REQUEST["type"] == 'connections') {
	include("./tables/status_ldap-connections.inc");
} elseif($_REQUEST["type"] == 'databases') {
	include("./tables/status_ldap-databases.inc");
}
?>

<?php
require("./left-trailer.html");

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
