<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.19 2005-03-17 09:13:10 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
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
		elseif($_REQUEST[$attrib])
		  $userdn = urlencode($_REQUEST[$attrib]);

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
if(@$_REQUEST["submit"] or @$_REQUEST["toggle"] or ($_REQUEST["action"] == 'del'))
{
    if(attribute_check()) {
		attribute_save();
    } else {
		attribute_print_form();
    }
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
