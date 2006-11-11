<?php
// shows details of specified category of attributes
// $Id: control_cat.php,v 2.23 2006-11-11 14:37:46 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_control_plugins.inc");

$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Load plugin categories
require($_SESSION["path"]."/include/pql_control_plugins.inc");
$cats = pql_plugin_get_cats();
asort($cats);
// }}}

// {{{ Load plugin attributes
$plugins = pql_plugin_get_catplugins($_REQUEST["cat"]);
if(!is_array($plugins)) {
  die($LANG->_('Invalid category')."!");
}
// }}}

// {{{ Setup the buttons
// ---- First the 'top' buttons (same as on control_detail.php)
$buttons1 = array('default' => 'Base values');
foreach($cats as $cat) {
  $new = array(urlencode($cat) => $cat);
  $buttons1 = $buttons1 + $new;
}
$new = array('action'  => 'Action');
$buttons1 = $buttons1 + $new;

// ---- Then the plugin attributes
$buttons2 = array();
foreach($plugins as $plugin) {
  $new = array($plugin => $plugin);
  $buttons2 = $buttons2 + $new;
}
// }}}

// {{{ Print status message, if one is available
if(isset($msg)) {
    pql_format_status_msg($msg);
}
// }}}
?>
  <span class="title1"><?=$_REQUEST["cat"]?></span>
  <br><br>
<?php
// {{{ Load the requested control category
if(@empty($_REQUEST["view"])) {
  $_REQUEST["view"] = $plugins[0];
}
// }}}

// {{{ Output the buttons to the browser
// The fiddeling with the '$_SERVER["PHP_SELF"]' value
// is because we have to fake it the first time we call
// pql_generate_button(). It uses the value to setup the
// URI, and only 'control_detail.php' knows how to convert
// the 'view=...' option to a 'cat=...' option.
$tmp = $_SERVER["PHP_SELF"];
$_SERVER["PHP_SELF"] = eregi_replace('control_cat.php', 'control_detail.php', $_SERVER["PHP_SELF"]);
pql_generate_button($buttons1, "mxhost=".$_REQUEST["mxhost"]); echo "  <br>\n";

$_SERVER["PHP_SELF"] = $tmp; unset($tmp);
pql_generate_button($buttons2, 'mxhost='.$_REQUEST["mxhost"].'&cat='.urlencode($_REQUEST["cat"])); echo "  <br>\n";
// }}}

// {{{ Show the view for this plugin
if($_REQUEST["view"]) {
  $file = $_SESSION["path"]."/"."include/".pql_plugin_get_filename($_REQUEST["view"]);
  include($file);

  $func = $_REQUEST["view"] . "_print_view";
  if(function_exists($func)) {
	call_user_func($func, $_REQUEST["mxhost"]);
	echo "<br><br>";
  }
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
