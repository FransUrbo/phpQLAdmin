<?php
// Add a user template
// $Id: config_template_add.php,v 2.11 2007-03-14 12:10:50 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/include/attrib.config.inc");
include($_SESSION["path"]."/include/attrib.config.template.inc");
include($_SESSION["path"]."/header.html");

$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
// }}}

// {{{ Forward back to configuration detail page
function attribute_forward($msg) {
  pql_header("config_detail.php?view=template&msg=".urlencode($msg));
}
// }}}

// {{{ Verify all submitted values and show form or save
$error = attribute_check_templates();
if(isset($error)) {
  // Errors or we've been asked to add a template - show form.
  attribute_print_form_templates($error);
} else {
  // No errors. We're good to go!
  attribute_save_templates();
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
