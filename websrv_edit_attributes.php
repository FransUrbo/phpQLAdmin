<?php
// edit attributes of a webserver configuration
// $Id: websrv_edit_attributes.php,v 2.3 2004-03-11 18:13:32 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_websrv.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// forward back to domain detail page
function attribute_forward($msg) {
    global $rootdn, $domain, $view, $server;

    $server = ldap_explode_dn($server, 0);
    $server = ereg_replace("cn=", "", $server[0]);

    // URL Encode the DN values
    $rootdn = urlencode($rootdn);
    $domain = urlencode($domain);
    $msg    = urlencode($msg);

    $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&server=$server&view=$view&msg=$msg";
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
