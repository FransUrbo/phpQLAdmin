<?php
// Show Connection/Suffixes status of LDAP server
// $Id: status_ldap.php,v 2.1 2004-01-22 10:25:54 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

require("./left-head.html");
include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
    </table>
<?php require("./left-trailer.html"); ?>
