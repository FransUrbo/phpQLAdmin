<?php
// add a domain
// $Id: unit_add.php,v 1.1 2002-12-12 06:23:19 turbo Exp $
//
require("pql.inc");
require("pql_control.inc");

?>
<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="normal.css" type="text/css">
</head>

<body bgcolor="#e7e7e7" background="images/bkg.png">
<span class="title1"><?php echo PQL_DOMAIN_ADD; ?>: <?php echo $domain ?></span>
<br><br>
<?php
$_pql = new pql();
$_pql_control = new pql_control();

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
if(pql_unit_exist($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $unit)){
	$msg = urlencode(PQL_DOMAIN_EXISTS);
	header("Location: home.php?msg=$msg");
	exit();
}

if(pql_add_unit($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $unit)){
	// update locals if control patch is enabled
	if(pql_control_update_domains($_pql->ldap_linkid, PQL_LDAP_BASEDN, $_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN)){
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
