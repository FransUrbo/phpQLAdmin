<?php
// $Id: ezmlm_detail.php,v 1.17 2003-06-21 20:19:25 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

if($domain) {
	// Get base directory for mails
	if(!($basemaildir = pql_get_domain_value($_pql, $domain, "basemaildir"))) {
		die("Can't get base mail directory for domain '$domain'!<br>");
	}

	// Initialize and load list of mailinglists
	$ezmlm = new ezmlm($config["PQL_GLOB_EZMLM_USER"], $basemaildir);
	
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
