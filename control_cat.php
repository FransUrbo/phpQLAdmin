<?php
// shows details of specified category of attributes
// $Id: control_cat.php,v 2.21 2005-06-22 13:58:27 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_control_plugins.inc");

$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Register all attribute plugins here (require)
$plugins = pql_plugin_get_catplugins($_REQUEST["cat"]);
if(!is_array($plugins)) {
  die($LANG->_('Invalid category')."!");
}
// }}}

// {{{ Setup the buttons
$buttons = array();
foreach($plugins as $plugin) {
  $new = array($plugin => $plugin);
  $buttons = $buttons + $new;
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

// Output the buttons to the browser
pql_generate_button($buttons, 'mxhost='.$_REQUEST["mxhost"].'&cat='.urlencode($_REQUEST["cat"])); echo "  <br>\n";

// {{{ Load the requested control category
if(!$_REQUEST["view"]) {
  $_REQUEST["view"] = $plugins[0];
}

// {{{ Show the view for this plugin
if($_REQUEST["view"]) {
  $file = $_SESSION["path"]."include/".pql_plugin_get_filename($_REQUEST["view"]);
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
