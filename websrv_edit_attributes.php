<?php
// edit attributes of a webserver configuration
// $Id: websrv_edit_attributes.php,v 2.6.2.1 2005-03-04 11:59:45 turbo Exp $
//
require("./include/pql_session.inc");
require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_websrv.inc");

// forward back to domain detail page
function attribute_forward($msg) {
	$url["domain"] = pql_format_urls($_REQUEST["domain"]);
	$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

    $server = ldap_explode_dn(urldecode($_REQUEST["server"]), 0);
    $server = ereg_replace("cn=", "", $server[0]);

    // URL Encode the DN values
    $msg    = urlencode($msg);

    $LINK_URL  = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&server=$server";
	$LINK_URL .= "&view=".$_REQUEST["view"]."&msg=$msg";

	if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
		echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
		die("<b>$LINK_URL</b>");
	} else
	  header("Location: " . $_SESSION["URI"] . $LINK_URL);
}

include($_SESSION["path"]."/header.html");
include($_SESSION["path"]."/include/attrib.websrv.inc");
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
