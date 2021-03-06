<?php
// Edit attribute of a physical host
// $Id: host_edit_attribute.php,v 2.9 2007-09-13 17:57:16 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/config_plugins.inc");
if(pql_get_define("PQL_CONF_DHCP3_USE")) {
  require($_SESSION["path"]."/include/pql_dhcp3.inc");
}

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to host detail page
function attribute_forward($msg) {
	global $_pql, $url;

	$link = "host_detail.php?host=".urlencode($_REQUEST["host"])."&view=".$_REQUEST["view"]."&msg=$msg";
	if($_REQUEST["server"])
	  $link .= "&server=".$_REQUEST["server"];
	if($_REQUEST["virthost"])
	  $link .= "&virthost=".$_REQUEST["virthost"];
	if($_REQUEST["ref"])
	  $link .= "&ref=".$_REQUEST["ref"];
	if(!$_REQUEST["server"] and !$_REQUEST["virthost"] and !$_REQUEST["ref"])
	  $link .= "&ref=physical";
	if($_REQUEST["subnet"])
	  $link .= "&subnet=".$_REQUEST["subnet"];

	if(pql_get_define("PQL_CONF_DEBUG_ME") and file_exists($_SESSION["path"]."/.DEBUG_PROFILING")) {
	  $now = pql_format_return_unixtime();
	  echo "Now: <b>$now</b><br>";
	}

	pql_header($link);
}
// }}}

// {{{ Select and load attribute plugin file
$plugin = pql_plugin_get_filename(pql_plugin_get($_REQUEST["attrib"]));
if(!$plugin) {
    die("<span class=\"error\">ERROR: No plugin file defined for attribute '<i>".$_REQUEST["attrib"]."</i>'</span>");
} elseif(!file_exists($_SESSION["path"]."/include/$plugin")) {
    die("<span class=\"error\">ERROR: Plugin file defined for attribute '<i>".$_REQUEST["attrib"]."</i>' does not exists!</span>");
}

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "include: include/$plugin<br>";
}

include($_SESSION["path"]."/include/$plugin");
// }}}

// {{{ Select what to do
$error_text = array();
if(@$_REQUEST["submit"] or $_REQUEST["dosave"]) {
  if(attribute_check())
	attribute_save($_REQUEST["action"]);
  else
	attribute_print_form();
} else {
  if($_REQUEST["action"] == 'delete')
	attribute_save($_REQUEST["action"]);
  else
	attribute_print_form();
}
// }}}

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
