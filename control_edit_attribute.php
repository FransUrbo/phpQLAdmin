<?php
// edit an attribute of a control option
// $Id: control_edit_attribute.php,v 2.22 2004-03-11 19:41:20 turbo Exp $
//
session_start();

// initial check
if($attrib == "") {
	die("no attribute requested !!");
}

if($type == "") {
	$type = "add";
}

require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_control_plugins.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);

// Register all attribute plugins here
$plugin = pql_plugin_get($attrib);
include("./include/".pql_plugin_get_filename($plugin));

// forward back to users detail page
function attribute_forward($msg) {
	global $domain, $user, $attrib, $mxhost, $domain, $rootdn, $view;

	$msg = urlencode($msg);
	$cat = pql_plugin_cat($attrib);
	if($mxhost)
	  $url = "control_detail.php?mxhost=$mxhost&view=$view&msg=$msg";
	else
	  $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&view=$view&msg=$msg";

	header("Location: " . pql_get_define("PQL_CONF_URI") . "$url");
}

include("./header.html");
?>
  <span class="title1">Change control values</span>
  <br><br>
<?php
if(function_exists($plugin . "_help")) {
    // if a help function is available for the plugin,
    // print additional help table and put the
    // displayed form of the plugin into a cell of this
    // table
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <tr>

<?php
}

// select what to do
if($submit == 1) {
    if(call_user_func($plugin . "_check", $type)) {
		call_user_func($plugin . "_save", $type, $mxhost);
    } else {
		call_user_func($plugin . "_print_form", $mxhost);
    }
} else {
    call_user_func($plugin . "_init", $mxhost);
    call_user_func($plugin . "_print_form", $mxhost);
}

if(function_exists($plugin . "_help")) {
    // this is the help table
?>

    <p>

    <td valign="top">
      <table cellspacing="0" cellpadding="3" border="0">
        <tr>
          <td valign="middle" class="helptitle"><img src="images/help.png" width="16" height="16" border="0">&nbsp;help on this attribute</td>
        </tr>

        <tr>
          <td height="0" class="helptext" valign="top">
            <?php call_user_func($plugin . "_help"); ?>
          </td>
        </tr>

<?php
	  if(function_exists($plugin . "_help_cr")) {
	      // this is the copyright message supplied with help text
?>

        <tr>
          <td height="0" class="helptextcr" valign="top">
            <?php call_user_func($plugin . "_help_cr"); ?>
          </td>
        </tr>
<?php
}
?>
      </table>
    </tr>
  </table>
<?php } ?>

</body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
