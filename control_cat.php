<?php
// shows details of specified category of attributes
// control_cat.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("include/pql.inc");
require("include/pql_control.inc");
require("include/pql_control_plugins.inc");

$_pql_control = new pql_control($USER_HOST_CTR, $USER_DN, $USER_PASS);

// register all attribute plugins here (require)
$control_plugins = pql_control_plugin_get_catplugins($cat);

// valid category ??
if(!is_array($control_plugins)){
	die("invalid category !!");
}

// include each defined plugin
foreach($control_plugins as $plugin){
	include("include/".pql_control_plugin_get_filename($plugin));
}

include("header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}
?>
  <span class="title1"><?php echo $cat; ?></span>
  <br><br>
<?php
// call print_view functions for each plugin
foreach($control_plugins as $plugin_name){
    $func = $plugin_name . "_print_view";
    if(function_exists($func)){
		call_user_func($func, $host);
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
