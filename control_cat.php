<?php
// shows details of specified category of attributes
// $Id: control_cat.php,v 2.13 2003-11-20 08:01:28 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_control_plugins.inc");

$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

// register all attribute plugins here (require)
$plugins = pql_plugin_get_catplugins($cat);

// valid category ??
if(!is_array($plugins)) {
	die($LANG->_('Invalid category')."!");
}

// include each defined plugin
foreach($plugins as $plugin) {
	include("./include/".pql_plugin_get_filename($plugin));
}

include("./header.html");

// print status message, if one is available
if(isset($msg)) {
    pql_format_status_msg($msg);
}
?>
  <span class="title1"><?=$cat?></span>
  <br><br>
<?php
// call print_view functions for each plugin
foreach($plugins as $name) {
    $func = $name . "_print_view";

    if(function_exists($func)) {
		call_user_func($func, $mxhost);
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
