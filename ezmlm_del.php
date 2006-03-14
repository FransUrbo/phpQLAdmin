<?php
// $Id: ezmlm_del.php,v 1.29.2.1 2006-03-14 14:46:30 turbo Exp $
//
// {{{ Setup session etc
require("./libs/pql_session.inc");
require($_SESSION["path"]."/libs/pql_config.inc");
require($_SESSION["path"]."/libs/pql_ezmlm.inc");

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
if(!($basedir = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die("Can't get ".pql_get_define("PQL_ATTR_BASEMAILDIR")." path from domain '".$_REQUEST["domain"]."'!");
}
// }}}

// {{{ Load list of mailinglists
$user = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
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
