<?php
// shows details of specified category of attributes
// $Id: control_cat.php,v 1.2 2002-12-12 11:50:27 turbo Exp $
//
require("pql.inc");
require("pql_control.inc");
require("pql_control_plugins.inc");

$_pql_control = new pql_control();

// register all attribute plugins here (require)
$control_plugins = pql_control_plugin_get_catplugins($cat);

// valid category ??
if(!is_array($control_plugins)){
	die("invalid category !!");
}

// include each defined plugin
foreach($control_plugins as $plugin){
	include(pql_control_plugin_get_filename($plugin));
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
	call_user_func($func);
	echo "<br><br>";
    }
}

?>
</body>
</html>
