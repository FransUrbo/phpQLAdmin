<?php
// edit attributes of a webserver configuration
// $Id: websrv_edit_attributes.php,v 2.4.16.1 2004-12-20 08:41:12 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_websrv.inc");

// forward back to domain detail page
function attribute_forward($msg) {
	$url["domain"] = pql_format_urls($_REQUEST["domain"]);
	$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

    $server = ldap_explode_dn(urldecode($_REQUEST["server"]), 0);
    $server = ereg_replace("cn=", "", $server[0]);

    // URL Encode the DN values
    $msg    = urlencode($msg);

    $link  = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&server=$server";
	$link .= "&view=".$_REQUEST["view"]."&msg=$msg";
	header("Location: " . pql_get_define("PQL_CONF_URI") . "$link");
}

include("./header.html");
include("./include/attrib.websrv.inc");
?>
    <span class="title1">Change a weberver configuration value</span>
    <br><br>

<?php
// Select what to do
if($_REQUEST["action"] == 'del') {
    attribute_save($_REQUEST["action"]);
} elseif($_REQUEST["submit"] == 1) {
    if(attribute_check())
      attribute_save($_REQUEST["action"]);
    else
      attribute_print_form();
} else {
    attribute_print_form();
}
?>
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
