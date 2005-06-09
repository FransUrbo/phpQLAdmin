<?php
// edit an attribute of a control option
// $Id: control_edit_attribute.php,v 2.32 2005-06-09 15:05:35 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_control_plugins.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Initial check
if($_REQUEST["attrib"] == "") {
	die("no attribute requested !!");
}

if($_REQUEST["type"] == "") {
	$_REQUEST["type"] = "add";
}
// }}}

// {{{ Get and load the plugin file
$plugin = pql_plugin_get($_REQUEST["attrib"]);
if($plugin) {
  $file = $_SESSION["path"]."/include/".pql_plugin_get_filename($plugin);
  if(!file_exists($file)) {
	echo pql_format_error_span("Configuration error: ");
	echo pql_complete_constant($LANG->_('Plugin file for %attrib% does not exists'), array('attrib' => $_REQUEST["attrib"]));
	die();
  } else
	include($file);
} else {
  echo pql_format_error_span("Configuration error: ");
  echo pql_complete_constant($LANG->_('No plugin defined for %attrib%'), array('attrib' => $_REQUEST["attrib"]));
  die();
}
// }}}

// {{{ Forward back to users detail page
function attribute_forward($msg) {
	$msg = urlencode($msg);
	$cat = pql_plugin_cat($_REQUEST["attrib"]);
	if($_REQUEST["mxhost"])
	  $url = "control_detail.php?mxhost=".$_REQUEST["mxhost"]."&view=".$_REQUEST["view"]."&msg=$msg";
	else
	  $url = "domain_detail.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"]."&view=".$_REQUEST["view"]."&msg=$msg";

	if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
	  echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	  die("<b>".$_SESSION["URI"].$link."</b>");
	} else
	  pql_header($url);
}
// }}}
?>
  <span class="title1">Change control values</span>
  <br><br>
<?php
// {{{ Prepare for the help table
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
// }}}

// {{{ Select what to do
if(@$_REQUEST["submit"]) {
    if(call_user_func($plugin . "_check", $_REQUEST["type"])) {
		call_user_func($plugin . "_save", $_REQUEST["type"], $_REQUEST["mxhost"]);
    } else {
		call_user_func($plugin . "_print_form", $_REQUEST["mxhost"]);
    }
} else {
    call_user_func($plugin . "_init", $_REQUEST["mxhost"]);
    call_user_func($plugin . "_print_form", $_REQUEST["mxhost"]);
}
// }}}

// {{{ Show the help text frame
if(function_exists($plugin . "_help")) {
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

<?php if(function_exists($plugin . "_help_cr")) {
	      // this is the copyright message supplied with help text
?>

        <tr>
          <td height="0" class="helptextcr" valign="top">
            <?php call_user_func($plugin . "_help_cr"); ?>
          </td>
        </tr>
<?php } ?>
      </table>
    </tr>
  </table>
<?php
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
