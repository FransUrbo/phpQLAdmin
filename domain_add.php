<?php
// add a domain
// $Id: domain_add.php,v 1.2 2002-12-12 11:50:27 turbo Exp $
//
session_start();
require("pql.inc");
require("pql_control.inc");

include("header.html");
?>
  <span class="title1"><?php echo PQL_DOMAIN_ADD; ?>: <?php echo $domain ?></span>
  <br><br>
<?php
$_pql = new pql();
$_pql_control = new pql_control();

// convert domain to lowercase
$domain = strtolower($domain);

// Should we force a dot in the domainname or not?
if(PQL_LDAP_OBJECTCLASS_DOMAIN == "domain") {
    // We're using a domain object, which means we should allow
    // a domain name to be without dot.
    $nodot = 1;
} else {
    $nodot = 0;
}

// check if domain is valid
if(!check_hostaddress($domain, $nodot)){
    $msg = urlencode(PQL_DOMAIN_INVALID);
    header("Location: " . PQL_URI . "home.php?msg=$msg");
    exit();
}

// check if domain exist
if(pql_domain_exist($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain)){
    $msg = urlencode(PQL_DOMAIN_EXISTS);
    header("Location: " . PQL_URI . "home.php?msg=$msg");
    exit();
}

if(pql_add_domain($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain)){
    // NOTE: Don't execute this one if we're using 'domain' mode.
    //       The FQDN isn't know yet!
    if(PQL_LDAP_OBJECTCLASS_DOMAIN != "domain") {
		// update locals if control patch is enabled
		if(pql_control_update_domains($_pql->ldap_linkid, PQL_LDAP_BASEDN,
									  $_pql_control->ldap_linkid,
									  PQL_LDAP_CONTROL_BASEDN)) {
			// message ??
		}
    }
	
    // redirect to domain-details
    $msg = urlencode(pql_complete_constant(PQL_DOMAIN_ADD_OK, array("domain" => $domain)));
    header("Location: " . PQL_URI . "domain_detail.php?domain=$domain&msg=$msg&rlnb=1");
} else {
    $msg = urlencode(PQL_DOMAIN_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
    header("Location: " . PQL_URI . "home.php?msg=$msg");
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
