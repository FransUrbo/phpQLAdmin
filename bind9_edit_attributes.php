<?php
// edit attributes of a BIND9 DNS zone
// $Id: bind9_edit_attributes.php,v 2.2 2003-06-25 07:06:24 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_bind9.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// forward back to domain detail page
function attribute_forward($msg) {
    global $domain, $rootdn, $rdn, $view;

    $msg = urlencode($msg);
    $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&view=$view&msg=$msg";

    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

include("./header.html");
include("./include/attrib.dnszone.inc");
?>
    <span class="title1">Change DNS zone value</span>
    <br><br>

<?php
// Translate the TYPE to dNSZone objectclass attribute
switch($type) {
  case "a":
  case "host":
	$attrib = 'relativeDomainName';
	break;
  case "ttl":
	$attrib = 'dNSTTL';
	break;
  case "ns":
	$attrib = 'nSRecord';
	break;
  case "mx":
	$attrib = 'mXRecord';
	break;
  case "cname":
	$attrib = 'cNAMERecord';
	break;
  case "srv":
	$attrib = 'sRVRecord';
	break;
  case "txt":
	$attrib = 'tXTRecord';
	break;
  default:
	die("unknown zone type '$type'.");
}


// select what to do
if(($action == 'del') && $oldvalue) {
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
