<?php
// Setup session etc
require("../include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

if((@$_SESSION["USER_DN"] or @$_SESSION["USER_ID"]) and @$_SESSION["USER_PASS"] and @$_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
  // We're logged in and we're administrator. Allow showing the PHP Information page.
  phpinfo();
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
