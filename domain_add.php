<?php
// add a domain
// domain_add.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?=PQL_DOMAIN_ADD?>: <?=$domain?></span>
  <br><br>
<?php
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

// convert domain to lowercase
$domain = strtolower($domain);

// Should we force a dot in the domainname or not?
if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
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
if(pql_domain_exist($_pql, $domain)) {
	$msg = urlencode(PQL_DOMAIN_EXISTS);
	header("Location: " . PQL_URI . "home.php?msg=$msg");
	exit();
}

// TODO: Currently puts the domain in the first base DN found.
$dns = pql_domain_add($_pql->ldap_linkid, $_pql->ldap_basedn[0], $domain);
if($dns[0]) {
	$entry[BRANCH_NAME] = $domain;

	// Default values we can easily figure out
	$defaultmaildir = user_generate_mailstore('', '', '', $entry);
	$defaulthomedir = user_generate_homedir('', '', '', $entry);

	if($defaultdomain != "") {
		// update locals if control patch is enabled
		if(pql_control_update_domains($_pql->ldap_linkid,
									  $_pql_control->ldap_linkid,
									  $USER_SEARCH_DN_CTR)) {
			// message ??
		}
	}
	
	$msg = "";
	
	// Save the attributes - Default domain
	if($defaultdomain && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'defaultDomain', $defaultdomain)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default home directory
	if($defaulthomedir && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseHomeDir', $defaulthomedir)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default mail directory
	if($defaultmaildir && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseMailDir', $defaultmaildir)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default quota
	if($defaultquota && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseQuota', $defaultquota)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// The creator is by default the administrator
	if(! pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'administrator', $USER_DN)) {
		$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}

	// redirect to domain-details
	if($msg == "") {
		$msg = urlencode(pql_complete_constant(PQL_DOMAIN_ADD_OK, array("domain" => $dns[0])));
	}
	$url = "domain_detail.php?domain=$dns[0]&msg=$msg&rlnb=1";

	// Now it's time to run the special adduser script if defined...
	if(PQL_EXTRA_SCRIPT_CREATE_DOMAIN) {
		// Setup the environment with the user details
		putenv("PQL_DOMAIN=$domain");
		putenv("PQL_DOMAINNAME=$defaultdomain");
		putenv("PQL_HOMEDIRS=$defaulthomedir");
		putenv("PQL_MAILDIRS=$defaultmaildir");
		putenv("PQL_QUOTA=$defaultquota");
		putenv("PQL_WEBUSER=".posix_getuid());
		
		// Execute the domain add script (0 => show output)
		if(pql_execute(PQL_EXTRA_SCRIPT_CREATE_DOMAIN, 0)) {
			echo PQL_DOMAIN_ADD_SCRIPT_FAILED;
			$msg .= urlencode(PQL_DOMAIN_ADD_SCRIPT_FAILED);
		} else {
			echo "<b>" . PQL_DOMAIN_ADD_SCRIPT_OK . "</b><br>";
			$msg .= urlencode(PQL_DOMAIN_ADD_SCRIPT_OK);
		}

		$url = "domain_detail.php?domain=$dns[0]&";
?>

    <form action="<?=$url?>&msg=<?=$msg?>&rlnb=1" method="post">
      <input type="submit" value="Continue">
    </form>
<?php

		die();
	} else {
		header("Location: " . PQL_URI . $url);
	}
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
