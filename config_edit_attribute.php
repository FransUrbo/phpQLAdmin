<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.5 2003-04-04 15:16:47 turbo Exp $
//
session_start();

require("./include/pql_config.inc");
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

include("./include/attrib.config.inc");
include("./header.html");

// forward back to configuration detail page
function attribute_forward($msg, $rlnb = false) {
    $msg = urlencode($msg);
    $url = "config_detail.php?msg=$msg";
	if($rlnb and $config["PQL_GLOB_AUTO_RELOAD"])
	  $url .= "&rlnb=1";

    header("Location: " . $config["PQL_GLOB_URI"] . "$url");
}
?>
  <span class="title1">Change configuration value</span>

  <br><br>

<?php
// select what to do
if($submit == 1) {
    if(attribute_check()) {
		attribute_save();
    } else {
		attribute_print_form();
    }
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
