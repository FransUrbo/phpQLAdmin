<?php
// add a domain
// unit_add.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?php echo PQL_DOMAIN_ADD; ?>: <?php echo $domain ?></span>
  <br><br>
<?php
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

// convert domain to lowercase
$domain = strtolower($domain);

// check if domain is valid
if(!check_hostaddress($domain)){
	$msg = urlencode(PQL_DOMAIN_INVALID);
	header("Location: home.php?msg=$msg");
	exit();
}

// "
// check if unit exist
if(pql_unit_exist($_pql->ldap_linkid, $domain, $unit)){
	$msg = urlencode(PQL_DOMAIN_EXISTS);
	header("Location: home.php?msg=$msg");
	exit();
}

if(pql_add_unit($_pql->ldap_linkid, $domain, $unit)){
	// update locals if control patch is enabled
	if(pql_control_update_domains($_pql->ldap_linkid, $_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR)){
		// message ??
  }

	// redirect to domain-details
	$msg = urlencode(pql_complete_constant(PQL_DOMAIN_ADD_OK, array("domain" => $domain)));
	header("Location: domain_detail.php?domain=$domain&unit=$unit&msg=$msg&rlnb=1");
} else {
	$msg = urlencode(PQL_DOMAIN_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: home.php?msg=$msg");
}
?>

</body>
</html>
