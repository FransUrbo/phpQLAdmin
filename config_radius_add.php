<?php
// Add a RADIUS Profile
// $Id: config_radius_add.php,v 1.2 2007-09-20 15:15:45 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/include/attrib.config.inc");
include($_SESSION["path"]."/include/attrib.config.radius.inc");
include($_SESSION["path"]."/header.html");

require($_SESSION["path"]."/include/texts.radius.inc");
// }}}

// {{{ Forward back to configuration detail page
function attribute_forward($msg) {
  pql_header("config_detail.php?view=radius&msg=".urlencode($msg));
}
// }}}

// {{{ Verify all submitted values and show form or save
$error = attribute_check_radprofile();
if(isset($error)) {
  // Errors or we've been asked to add a policy - show form.
  attribute_print_form_radprofile($error);
} else {
  // No errors. We're good to go!
  attribute_save_radprofile();
}
// }}}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
