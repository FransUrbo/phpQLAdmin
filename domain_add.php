<?php
// add a domain
// domain_add.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?=PQL_DOMAIN_ADD?>: <?=$domain?></span>
  <br><br>
<?php
$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);
$_pql_control = new pql_control($USER_HOST_CTR, $USER_DN, $USER_PASS);

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
if(pql_domain_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain)){
	$msg = urlencode(PQL_DOMAIN_EXISTS);
	header("Location: " . PQL_URI . "home.php?msg=$msg");
	exit();
}

// Defaultvalues we can easily figure out
$defaulthomedir = user_generate_homedir('', '', '', $domain, '');
$defaultmaildir = user_generate_mailstore('', '', '', $domain, '');

if(pql_add_domain($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain)) {
	if($defaultdomain != "") {
		// update locals if control patch is enabled
		if(pql_control_update_domains($_pql->ldap_linkid, $USER_SEARCH_DN_USR,
									  $_pql_control->ldap_linkid,
									  $USER_SEARCH_DN_CTR)) {
			// message ??
		}
	}
	
	$msg = "";
	
	// Save the attributes - Default domain
	if($defaultdomain && !pql_set_domain_value($_pql->ldap_linkid, $domain, 'defaultDomain', $defaultdomain)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default home directory
	if($defaulthomedir && !pql_set_domain_value($_pql->ldap_linkid, $domain, 'baseHomeDir', $defaulthomedir)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default mail directory
	if($defaultmaildir && !pql_set_domain_value($_pql->ldap_linkid, $domain, 'baseMailDir', $defaultmaildir)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default quota
	if($defaultquota && !pql_set_domain_value($_pql->ldap_linkid, $domain, 'baseQuota', $defaultquota)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// The creator is by default the administrator
	if(! pql_set_domain_value($_pql->ldap_linkid, $domain, 'administrator', $USER_DN)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}

	// Now it's time to run the special adduser script if defined...
	if(PQL_EXTRA_SCRIPT_CREATE_DOMAIN) {
		// Open a bi-directional pipe, so we can write AND read
		$fh = popen(escapeshellcmd(PQL_EXTRA_SCRIPT_CREATE_DOMAIN), "w+");
		if($fh) {
			
			// We're done, close the command file handle...
			pclose($fh);
		} else {
			$msg = urlencode(PQL_USER_DOMAIN_SCRIPT_FAILED);
			$url = "domain_detail.php?domain=$domain&msg=$msg";
			header("Location: " . PQL_URI . "$url");
		}
	}
	
	// redirect to domain-details
	if($msg == "") {
		$msg = urlencode(pql_complete_constant(PQL_DOMAIN_ADD_OK, array("domain" => $domain)));
	}
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
