<?php
// $Id: ezmlm_detail.php,v 1.26 2004-05-06 14:21:29 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// print status message, if one is available
if(isset($_REQUEST["msg"])){
    pql_format_status_msg($_REQUEST["msg"]);
}

if($_REQUEST["rootdn"]) {
	// Get base directory for mails
	if(!($basemaildir = pql_domain_get_value($_pql, $_REQUEST["rootdn"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
		die(pql_complete_constant($LANG->_("Can't get %attribute% for domain '%domain%'",
										   array('attribute' => pql_get_define("PQL_ATTR_BASEMAILDIR"),
												 'domain'    => $_REQUEST["rootdn"])))."!<br>");
	}

	// Initialize and load list of mailinglists
	$ezmlm = new ezmlm(pql_get_define("PQL_CONF_EZMLM_USER"), $basemaildir);
	
	include("./header.html");

	// reload navigation bar if needed
	if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnavezmlm.location.reload();
  //--></script>
<?php
	}

	if(!is_numeric($_REQUEST["listno"])) {
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
