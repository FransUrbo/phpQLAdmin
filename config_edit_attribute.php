<?php
// Edit and set configuration values in the LDAP database
// $Id: config_edit_attribute.php,v 1.1 2003-01-18 11:29:43 turbo Exp $
//
session_start();

require("./include/pql.inc");
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// forward back to configuration detail page
function attribute_forward($msg) {
    $msg = urlencode($msg);
    $url = "config_detail.php&msg=$msg";
    header("Location: " . PQL_URI . "$url");
}

include("./include/attrib.config.inc");
include("./header.html");
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
?>
</body>
</html>

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
