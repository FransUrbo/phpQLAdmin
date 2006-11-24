<?php
// View and manage web servers accessible from this domain/branch.
// $Id: domain_details-websrv.inc,v 1.21.2.3 2006-11-24 13:53:58 turbo Exp $

// {{{ Include Webserver API etc
require($_SESSION["path"]."/include/pql_websrv.inc");
// }}}

// {{{ Get all domain names - primary and additional - this domain 'own'
$domain_additional = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
if(is_array($domain_additional)) {
  $domains = $domain_additional;
} elseif($domain_additional) {
  $domains = array($domain_additional);
}

$domain_default    = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
if($domain_default) {
  $domains[] = $domain_default;
}
// }}}

// {{{ Get all virtual hosts which this domain/branch 'own'.
$filter = "(|";
for($i=0; $domains[$i]; $i++) {
  $filter .= "(".pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")."=*".$domains[$i].")";
}
$filter .= ")";
$virt_hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter);
// }}}

// {{{ Extract the physical host(s) from the virtual host(s) (if any)
if(is_array($virt_hosts)) {
  for($i=0; $virt_hosts[$i]; $i++) {
	$dn_parts = ldap_explode_dn($virt_hosts[$i], 0);

	$virt_host = $dn_parts[0];
	$container = $dn_parts[1];

	// Remove the attribute from the values
	$virt_host = preg_replace('/.*=/', '', $virt_host);
	$container = preg_replace('/.*=/', '', $container);

	// Setup an array, ordered firstly on virtual host, then on web container
	$hosts[$virt_host][$container] = $virt_hosts[$i];
  }
}

//if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
  echo "Hosts:";
  printr($hosts);
//}
// }}}

// TODO: Display!
die("I've retreived all virtual hosts (and their web servers). Have no idea what to do next!");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
