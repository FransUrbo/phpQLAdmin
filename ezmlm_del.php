<?php
// $Id: ezmlm_del.php,v 1.20 2004-03-11 18:13:32 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);

include("./header.html");

// forward back to list detail page
function list_forward($domainname, $msg){
	global $domain;

    $msg = urlencode($msg);
    $url = "home.php?msg=$msg&rlnb=3";
    header("Location: " . pql_get_define("PQL_CONF_URI") . "$url");
}

// Get base directory for mails
if(!($path = pql_domain_get_value($_pql, $domain, pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die("Can't get ".pql_get_define("PQL_ATTR_BASEMAILDIR")." path from domain '$domain'!");
}

// Load list of mailinglists
if($ezmlm = new ezmlm(pql_get_define("PQL_CONF_EZMLM_USER"), $path)) {
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