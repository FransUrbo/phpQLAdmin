<?php
// add a domain
// domain_add.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?=PQL_LANG_DOMAIN_ADD?>: <?=$domain?></span>
  <br><br>
<?php
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

// Should we force a dot in the domainname or not?
if($config["PQL_CONF_REFERENCE_DOMAINS_WITH"][$rootdn] == "dc" or
   $config["PQL_CONF_REFERENCE_DOMAINS_WITH"][$rootdn] == "o") {
	// We're using a domain or organization object, which
	// means we should allow a domain name to be without dot.
	$force_dot = 0;
} else {
	$force_dot = 1;
}

// check if domain is valid
if(!check_hostaddress($domain, $force_dot)) {
	$msg = urlencode(PQL_LANG_DOMAIN_INVALID);
	header("Location: " . $config["PQL_GLOB_URI"] . "home.php?msg=$msg");
	exit();
}

// check if domain exist
if(pql_domain_exist($_pql, $domain)) {
	$msg = urlencode(PQL_LANG_DOMAIN_EXISTS);
	header("Location: " . $config["PQL_GLOB_URI"] . "home.php?msg=$msg");
	exit();
}

// TODO: Currently puts the domain in the first base DN found.
$dns = pql_domain_add($_pql->ldap_linkid, $_pql->ldap_basedn[0], $domain);
if($dns[0]) {
	$entry[BRANCH_NAME] = $domain;

	// Default values we can easily figure out
	$defaultmaildir = user_generate_mailstore('', '', '', $entry);
	$defaulthomedir = user_generate_homedir('', '', '', $entry);

	// Replace spaces with underscore - can't create dirs with spaces,
	// it's bound to break SOMEWHERE!
	$defaultmaildir = preg_replace('/ /', '_', $defaultmaildir);
	$defaulthomedir = preg_replace('/ /', '_', $defaulthomedir);

	if($defaultdomain != "") {
		// update locals if control patch is enabled
		if(pql_control_update_domains($_pql, $USER_SEARCH_DN_CTR, '*', array('', $defaultdomain))) {
			// message ??
		}
	}
	
	$msg = "";
	
	// Save the attributes - Default domain
	if($defaultdomain && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'defaultDomain', $defaultdomain)) {
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default home directory
	if($defaulthomedir && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseHomeDir', $defaulthomedir)) {
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default mail directory
	if($defaultmaildir && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseMailDir', $defaultmaildir)) {
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// Save the attributes - Default quota
	if($defaultquota && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseQuota', $defaultquota)) {
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// The creator is by default the administrator
	if(! pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'administrator', $USER_DN)) {
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}

	// redirect to domain-details
	if($msg == "") {
		$msg = urlencode(pql_complete_constant(PQL_LANG_DOMAIN_ADD_OK, array("domain" => $dns[0])));
	}
	$url = "domain_detail.php?domain=$dns[0]&msg=$msg&rlnb=1";

	// Now it's time to run the special adduser script if defined...
	if($config["PQL_CONF_SCRIPT_CREATE_DOMAIN"][$rootdn]) {
		// Setup the environment with the user details
		putenv("PQL_DOMAIN=$domain");
		putenv("PQL_DOMAINNAME=$defaultdomain");
		putenv("PQL_HOMEDIRS=$defaulthomedir");
		putenv("PQL_MAILDIRS=$defaultmaildir");
		putenv("PQL_QUOTA=$defaultquota");
		putenv("PQL_WEBUSER=".posix_getuid());
		
		// Execute the domain add script (0 => show output)
		if(pql_execute($config["PQL_CONF_SCRIPT_CREATE_DOMAIN"][$rootdn], 0)) {
			echo PQL_LANG_DOMAIN_ADD_SCRIPT_FAILED;
			$msg .= " " . urlencode(PQL_LANG_DOMAIN_ADD_SCRIPT_FAILED);
		} else {
			echo "<b>" . PQL_LANG_DOMAIN_ADD_SCRIPT_OK . "</b><br>";
			$msg .= " " . urlencode(PQL_LANG_DOMAIN_ADD_SCRIPT_OK);
		}

		$url = "domain_detail.php?domain=$dns[0]&";
?>

    <form action="<?=$url?>&msg=<?=$msg?>&rlnb=1" method="post">
      <input type="submit" value="Continue">
    </form>
<?php

		die();
	} else {
		header("Location: " . $config["PQL_GLOB_URI"] . $url);
	}
} else {
	$msg = urlencode(PQL_LANG_DOMAIN_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: " . $config["PQL_GLOB_URI"] . "home.php?msg=$msg");
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
