<?php
// $Id: ezmlm_detail.php,v 1.21.2.1 2003-11-24 18:07:02 dlw Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// print status message, if one is available
if(isset($msg)){
    pql_format_status_msg($msg);
}

if($domain) {
	// Get base directory for mails
	if(!($basemaildir = pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR")))) {
		die("Can't get ".pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR")." for domain '$domain'!<br>");
	}

	// Initialize and load list of mailinglists
	$ezmlm = new ezmlm(pql_get_define("PQL_GLOB_EZMLM_USER"), $basemaildir);
	
	include("./header.html");
	
	if(!is_numeric($listno)) {
		// No list, show all lists in the domain
		include("./tables/ezmlm_details-lists.inc");
	} else {
		// We got a list, show if's details
		include("./tables/ezmlm_details-detail.inc");
	}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
