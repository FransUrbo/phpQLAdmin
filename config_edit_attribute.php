<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.3 2003-01-21 13:32:38 turbo Exp $
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
	if($rlnb)
	  $url .= "&rlnb=1";

    header("Location: " . PQL_URI . "$url");
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
