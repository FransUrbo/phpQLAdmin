<?php
// edit attributes of a BIND9 DNS zone
// $Id: bind9_edit_attributes.php,v 2.8 2005-02-24 17:04:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_bind9.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// {{{ Forward back to domain detail page
function attribute_forward($msg) {
    $msg  = urlencode($msg);
    $url  = "domain_detail.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"]."&view=".$_REQUEST["view"];
	$url .= "&dns_domain_name=".$_REQUEST["dns_domain_name"]."&msg=$msg";

    header("Location: " . $_SESSION["URI"] . "$url");
}
// }}}

include($_SESSION["path"]."/header.html");
include($_SESSION["path"]."/include/attrib.dnszone.inc");
?>
    <span class="title1">Change DNS zone value</span>
    <br><br>

<?php
// {{{ Translate the TYPE to dNSZone objectclass attribute
switch($_REQUEST["type"]) {
  case "a":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_ARECORD");
	break;
  case "host":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME");
	break;
  case "ttl":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_DNSTTL");
	break;
  case "ns":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_NSRECORD");
	break;
  case "mx":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_MXRECORD");
	break;
  case "cname":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_CNAMERECORD");
	break;
  case "srv":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_SRVRECORD");
	break;
  case "txt":
	$_REQUEST["attrib"] = pql_get_define("PQL_ATTR_TXTRECORD");
	break;
  default:
	if($_REQUEST["action"] != "del") {
	  die("unknown zone type '".$_REQUEST["type"]."'.");
	}
}
// }}}

// {{{ Select what to do
if(($_REQUEST["action"] == 'del') && $_REQUEST["rdn"]) {
	attribute_save($_REQUEST["action"]);
} elseif($_REQUEST["submit"] == 1) {
    if(attribute_check())
      attribute_save($_REQUEST["action"]);
    else
      attribute_print_form();
} else {
    attribute_print_form();
}
// }}}
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
