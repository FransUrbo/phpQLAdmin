<?php
// Add a user template
// $Id: config_template_add.php,v 2.10 2007-03-05 10:21:07 turbo Exp $
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
  $url =  "config_detail.php?view=template";
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	die("<b>$url</b>");
  } else
	pql_header($url);
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
