<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.9.2.2 2003-12-15 20:33:03 dlw Exp $
//
session_start();

require("./include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include("./include/attrib.config.inc");
include("./header.html");

// forward back to configuration detail page
function attribute_forward($msg, $rlnb = false) {
  //global $attrib, $$attrib, $domain, $rootdn, $view, $delval;
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
		if($_REQUEST["rootdn"])
		  $url = "config_detail.php?branch=" . $_REQUEST["rootdn"] . "&view=branch&msg=$msg";
		else
		  $url = "config_detail.php?msg=$msg";
	}

	if($rlnb and pql_get_define("PQL_GLOB_AUTO_RELOAD"))
	  $url .= "&rlnb=1";

    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// select what to do
if($_REQUEST["submit"] == 1) {
    if(attribute_check()) {
		attribute_save();
    } else {
		attribute_print_form();
    }
} elseif($_REQUEST["submit"] == 2) {
	attribute_save();
} elseif($delval or $toggle) {
    // DLW: I'm not sure how $deval or $toggle can be seen from here.
    trigger_error('DLW: How did this happen?', E_USER_WARNING);
	attribute_save();
} else {
    attribute_print_form();
}

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
