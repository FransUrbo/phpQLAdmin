<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.7 2003-06-18 12:41:05 turbo Exp $
//
session_start();

require("./include/pql_config.inc");
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

include("./include/attrib.config.inc");
include("./header.html");

// forward back to configuration detail page
function attribute_forward($msg, $rlnb = false) {
	global $attrib, $$attrib, $domain, $rootdn, $view, $delval;

    $msg = urlencode($msg);
	if(lc($attrib) == 'controlsadministrator') {
		if($$attrib)
		  $userdn = urlencode($$attrib);
		elseif($delval)
		  $userdn = urlencode($delval);

		$url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=$userdn&view=$view&msg=$msg";
	} else
	  $url = "config_detail.php?msg=$msg";

	if($rlnb and $config["PQL_GLOB_AUTO_RELOAD"])
	  $url .= "&rlnb=1";

    header("Location: " . $config["PQL_GLOB_URI"] . "$url");
}

// select what to do
if($submit == 1) {
    if(attribute_check()) {
		attribute_save();
    } else {
		attribute_print_form();
    }
} elseif($submit == 2) {
	attribute_save();
} elseif($delval) {
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
