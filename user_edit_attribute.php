<?php
// edit an attribute of user
// $Id: user_edit_attribute.php,v 2.34 2004-02-21 12:57:38 turbo Exp $
//
// This file gets iterated through at least 2 times for any attribute (sequenced by "$submit"):
//   1) $submit is unset: Set the default value of the attribute (usually from "$oldvalue")
//      and print out the form.
//   2) $submit is 1 (or 2?): Validate the input.  The name of the input variable changes depending on
//      which attribute is being edited.
//      If the input is valid, save it, else print out the form again and return to step 2.
session_start();
require("./include/pql_config.inc");
require("./include/config_plugins.inc");

require_once("./dlw_porting.inc");
// These variable are "_GET" the first time, and "_POST" the other times.
if (empty($session)) {
  dlw_expect_from(__FILE__, __LINE__, '_REQUEST', array('domain', 'user', 'rootdn', 'oldvalue', 'view', 'attrib', 'PHPSESSID'));
  dlw_expect_from(__FILE__, __LINE__, '_POST', array());
} else {
  dlw_expect_from(__FILE__, __LINE__, '_POST', array('domain', 'user', 'rootdn', 'oldvalue', 'view', 'attrib', 'submit'));
}

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

if(!$_REQUEST["domain"] && $_REQUEST["user"]) {
    // We're called without branchname - try to reconstruct it
    
    $tmpdn = split(',', $_REQUEST["user"]);
    if($tmpdn[1]) {
	unset($tmpdn[0]);
	$_REQUEST["domain"] = implode(",", $tmpdn);
    } else
      $_REQUEST["domain"] = $tmpdn[count($tmpdn)-1];
}

$url["domain"] = pql_format_urls($_GET["domain"]);
$url["rootdn"] = pql_format_urls($_GET["rootdn"]);
$url["user"]   = pql_format_urls($_GET["user"]);

// Get default domain name for this domain
$defaultdomain = pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));

// Get the username. Prettier than the DN
$username = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_GLOB_ATTR_CN"));
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_GLOB_ATTR_UID"));
}
$username = $username[0];

// forward back to users detail page (called by attribute_save).
function attribute_forward($msg, $rlnb = false) {
    // URL Encode some of the most important information
    // (root DN, domain/branch DN and user DN)
    $_REQUEST["domain"] = urlencode($_REQUEST["domain"]);
    $_REQUEST["user"]   = urlencode($_REQUEST["user"]);
    $_REQUEST["rootdn"] = urlencode($_REQUEST["rootdn"]);

    $url = "user_detail.php?rootdn=" . $url["rootdn"] . "&domain=" . $url["domain"]
      . "&user=" . $url["user"] . "&view=" . $_REQUEST["view"] . "&msg=".urlencode($msg);
    if ($rlnb)
      $url .= "&rlnb=2";

    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Select which attribute have to be included
include("./include/".pql_plugin_get_filename(pql_plugin_get($_REQUEST["attrib"])));

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change user data for %user%'), array('user' => $username)); ?></span>
  <br><br>
<?php
// select what to do
if(($_POST["submit"] == 1) or ($_POST["submit"] == 2)) {
    if(attribute_check("modify")){
	attribute_save("modify");
    } else {
	attribute_print_form();
    }
} elseif($submit == 4) {
	// SAVE change directly, no need for a form
	attribute_save($action);
} else {
    attribute_init();
    attribute_print_form();
}
?>
</body>
</html>
