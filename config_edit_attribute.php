<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.16.2.3 2005-04-27 12:25:17 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");

require("./include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/include/attrib.config.inc");
include($_SESSION["path"]."/header.html");
if($_REQUEST["view"] == "template") {
  include($_SESSION["path"]."/left-head.html");
}
// }}}

// {{{ Forward back to configuration detail page
function attribute_forward($msg, $rlnb = false) {
    $attrib = $_REQUEST["attrib"];

    $msg = urlencode($msg);
	if(lc($attrib) == 'controlsadministrator') {
		if($_REQUEST[$attrib])
		  $userdn = urlencode($_REQUEST[$attrib]);
		elseif($_REQUEST["delval"])
		  $userdn = urlencode($_REQUEST["delval"]);

		$url = "user_detail.php?rootdn=" . $_REQUEST["rootdn"]
		  . "&domain=" . $_REQUEST["domain"] . "&user=$userdn&view=" . $_REQUEST["view"] . "&msg=$msg";
	} else {
		if($_REQUEST["view"] == "template")
		  $url = "config_detail.php?branch=" . $_REQUEST["rootdn"] . "&view=template&msg=$msg";
		elseif($_REQUEST["rootdn"])
		  $url = "config_detail.php?branch=" . $_REQUEST["rootdn"] . "&view=branch&msg=$msg";
		else
		  $url = "config_detail.php?msg=$msg";
	}

	if($rlnb and pql_get_define("PQL_CONF_AUTO_RELOAD"))
	  $url .= "&rlnb=1";

	if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
		echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
		die("<b>$url</b>");
	} else
	  pql_header($url);
}
// }}}

// {{{ Select what to do
$attrib = $_REQUEST["attrib"];
if(@$_REQUEST["submit"] == 1) {
    if(attribute_check()) {
		attribute_save();
    } else {
		attribute_print_form();
    }
} elseif(@$_REQUEST["submit"] == 2) {
	attribute_save();
} elseif(!empty($_REQUEST[$attrib]) or !empty($_REQUEST["toggle"])) {
	attribute_save();
} else {
    attribute_print_form();
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
