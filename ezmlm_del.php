<?php
// $Id: ezmlm_del.php,v 1.23 2004-11-17 07:30:06 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

include("./header.html");

// forward back to list detail page
function list_forward($domainname, $msg) {
    $msg = urlencode($msg);
    $url = "home.php?msg=$msg&rlnb=3";
    header("Location: " . pql_get_define("PQL_CONF_URI") . "$url");
}

// Get base directory for mails
if(!($path = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die("Can't get ".pql_get_define("PQL_ATTR_BASEMAILDIR")." path from domain '".$_REQUEST["domain"]."'!");
}

// Load list of mailinglists
$user = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
if($ezmlm = new ezmlm($user, $path)) {
	if(is_array($ezmlm->mailing_lists[$listno])) {
		$listname = $ezmlm->mailing_lists[$listno]["name"] . "@" . $ezmlm->mailing_lists[$listno]["host"];
		$listpath = $ezmlm->mailing_lists[$listno]["directory"];

		$ezmlm->deletelistentry($listno);
	} else {
		echo "List does not exists<br>";
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
