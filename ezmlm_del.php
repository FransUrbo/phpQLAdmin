<?php
// $Id: ezmlm_del.php,v 1.32 2007-02-15 12:07:11 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to list detail page
function list_forward($domainname, $msg) {
    $msg = urlencode($msg);
    $url = "home.php?msg=$msg&rlnb=3";
    pql_header($url);
}
// }}}

// {{{ Get base directory for mails
if(!($basedir = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die("Can't get ".pql_get_define("PQL_ATTR_BASEMAILDIR")." path from domain '".$_REQUEST["domain"]."'!");
}
// }}}

// {{{ Load list of mailinglists
$user = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
if($ezmlm = new ezmlm($user, $basedir)) {
	if(is_array($ezmlm->mailing_lists[$listno])) {
		$listname = $ezmlm->mailing_lists[$listno]["name"] . "@" . $ezmlm->mailing_lists[$listno]["host"];
		$listpath = $ezmlm->mailing_lists[$listno]["directory"];

		$ezmlm->deletelistentry($listno);
	} else {
		echo "List does not exists<br>";
	}
}
// }}}

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
