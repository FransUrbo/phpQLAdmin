<?php
// edit attributes of a webserver configuration
// $Id: websrv_edit_attributes.php,v 2.4 2004-04-02 12:41:34 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_websrv.inc");

// forward back to domain detail page
function attribute_forward($msg) {
	$url["domain"] = pql_format_urls($_REQUEST["domain"]);
	$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

    $server = ldap_explode_dn($_REQUEST["server"], 0);
    $server = ereg_replace("cn=", "", $server[0]);

    // URL Encode the DN values
    $msg    = urlencode($msg);

    $url  = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&server=$server";
	$url .= "&view=".$_REQUEST["view"]."&msg=$msg";
    header("Location: " . pql_get_define("PQL_CONF_URI") . "$url");
}

include("./header.html");
include("./include/attrib.websrv.inc");
?>
    <span class="title1">Change a weberver configuration value</span>
    <br><br>

<?php
// Select what to do
if($action == 'del') {
    attribute_save($action);
} elseif($submit == 1) {
    if(attribute_check())
      attribute_save($action);
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
