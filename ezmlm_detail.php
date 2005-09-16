<?php
// $Id: ezmlm_detail.php,v 1.34 2005-09-16 06:08:43 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
// }}}

// {{{ Print status message, if one is available
if(isset($_REQUEST["msg"])){
    pql_format_status_msg($_REQUEST["msg"]);
}
// }}}

if($_REQUEST["domain"]) {
	// Get base directory for mails
	if(!($basemaildir = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
		die(pql_complete_constant($LANG->_("Can't get %attribute% for domain '%domain%'",
										   array('attribute' => pql_get_define("PQL_ATTR_BASEMAILDIR"),
												 'domain'    => $_REQUEST["domainname"])))."!<br>");
	}

	// Initialize and load list of mailinglists
	$user  = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
	$ezmlm = new ezmlm($user, $basemaildir);
	
	include($_SESSION["path"]."/header.html");

	// reload navigation bar if needed
	if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnavezmlm.location.reload();
  //--></script>
<?php
	}

	if(!@is_numeric($_REQUEST["listno"])) {
		// No list, show all lists in the domain
		include("./tables/ezmlm_details-lists.inc");
	} else {
		// We got a list, show if's details
		$listno = $_REQUEST["listno"];
		include("./tables/ezmlm_details-detail.inc");
	}
}

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
