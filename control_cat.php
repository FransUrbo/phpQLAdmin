<?php
// shows details of specified category of attributes
// $Id: control_cat.php,v 2.27 2008-03-19 12:17:47 turbo Exp $
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

// {{{ Setup the controls view buttons
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

// {{{ Setup the host view buttons
if($_REQUEST["ref"]) {
  if(@$_REQUEST["view"])
	// Save this so it doesn't disappear
	$tmp = $_REQUEST["view"];
  
  $_REQUEST["view"] = 'mailsrv';

  $buttons = array();

  if(pql_get_define("PQL_CONF_HOSTACL_USE")) {
	$new = array('hostacl' => 'Host Control');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_AUTOMOUNT_USE")) {
	$new = array('automount' => 'Automount Information');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_CONTROL_USE")) {
	$new = array('mailsrv'   => 'Mailserver Administration');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
	$new = array('websrv' => 'Webserver Administration');
	$buttons = $buttons + $new;
  }				 

  if(pql_get_define("PQL_CONF_RADIUS_USE")) {
	$new = array('radius' => 'RADIUS Administration');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_BIND9_USE")) {
	$new = array('dns' => 'DNS Administration');
	$buttons = $buttons + $new;
  }
  
  pql_generate_button($buttons, "host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"], 'host_detail.php'); echo "    <br>\n";
  
  if(@$tmp)
	// Restore the view value
	$_REQUEST["view"] = $tmp;
  else
	// Unset the view value (so it can be set below)
	unset($_REQUEST["view"]);
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
