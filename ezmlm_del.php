<?php
// $Id: ezmlm_del.php,v 1.10 2003-01-16 14:58:33 turbo Exp $
//
session_start();

require("./include/pql.inc");
require("./include/pql_ezmlm.inc");

include("./header.html");

// Initialize
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS, false, 0);

// forward back to list detail page
function list_forward($domainname, $msg){
	global $domain;

    $msg = urlencode($msg);
    $url = "home.php?msg=$msg&rlnb=3";
    header("Location: " . PQL_URI . "$url");
}

// Get base directory for mails
if(!($path = pql_get_domain_value($_pql, $domain, "basemaildir"))) {
	// TODO: What if we can't find the base maildir path!?
	die("Can't get baseMailDir path from domain '$domain'!");
}

// Load list of mailinglists
if($ezmlm = new ezmlm('alias', $path)) {
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
