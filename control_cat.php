<?php
// shows details of specified category of attributes
// $Id: control_cat.php,v 2.19 2005-03-09 09:59:03 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_control_plugins.inc");

$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// register all attribute plugins here (require)
$plugins = pql_plugin_get_catplugins($_REQUEST["cat"]);

// valid category ??
if(!is_array($plugins)) {
	die($LANG->_('Invalid category')."!");
}

// include each defined plugin
foreach($plugins as $plugin) {
	include($_SESSION["path"]."/include/".pql_plugin_get_filename($plugin));
}

include($_SESSION["path"]."/header.html");

// print status message, if one is available
if(isset($msg)) {
    pql_format_status_msg($msg);
}
?>
  <span class="title1"><?=$_REQUEST["cat"]?></span>
  <br><br>
<?php
// call print_view functions for each plugin
foreach($plugins as $name) {
    $func = $name . "_print_view";

    if(function_exists($func)) {
		call_user_func($func, $_REQUEST["mxhost"]);
		echo "<br><br>";
    }
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
