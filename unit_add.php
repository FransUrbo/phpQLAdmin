<?php
// add a domain
// $Id: unit_add.php,v 2.13.2.1 2003-11-24 18:07:02 dlw Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create domain %domain%'), array('domain' => $domain)); ?></span>
  <br><br>
<?php
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// convert domain to lowercase
$domain = strtolower($domain);

// check if domain is valid
if(!pql_check_hostaddress($domain)) {
	$msg = urlencode($LANG->_('Invalid domain name! Use: domain.tld (e.g. adfinis.com)'));
	header("Location: home.php?msg=$msg");
	exit();
}

// "
// check if unit exist
if(pql_unit_exist($_pql->ldap_linkid, $domain, $unit)){
	$msg = urlencode($LANG->_('This domain already exists'));
	header("Location: home.php?msg=$msg");
	exit();
}

if(pql_add_unit($_pql->ldap_linkid, $domain, $unit)){
	// update locals if control patch is enabled
	if(pql_control_update_domains($_pql, $_SESSION["USER_SEARCH_DN_CTR"])) {
	    // message ??
	}

	// redirect to domain-details
	$msg = urlencode(pql_complete_constant($LANG->_('Domain %domain% successfully created'), array("domain" => $domain)));
	header("Location: domain_detail.php?domain=$domain&unit=$unit&msg=$msg&rlnb=1");
} else {
	$msg = urlencode($LANG->_('Failed to create the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: home.php?msg=$msg");
}
?>

</body>
</html>
