<?php
// add a domain
// $Id: domain_add.php,v 2.41.2.2 2003-12-17 21:50:46 dlw Exp $
//
session_start();

// Make sure we can have a ' in branch
$domain = ereg_replace("\\\'", "'", $_REQUEST["domain"]);
$rootdn = $_REQUEST["rootdn"];
$defaultdomain = $_REQUEST["defaultdomain"];

require("./include/pql_config.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?=$LANG->_('Create domain')?>: <?=$domain?></span>
  <br><br>
<?php
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// Should we force a dot in the domainname or not?
if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc" or
   pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "ou" or
   pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "o")
{
	// We're using a domain or organization object, which
	// means we should allow a domain name to be without dot.
	$force_dot = 0;
} else {
	$force_dot = 1;
}

// check if domain is valid
//if(!pql_check_hostaddress($domain, $force_dot)) {
//	$msg = urlencode($LANG->_('Invalid domain name! Use: domain.tld (e.g. adfinis.com)'));
//	header("Location: " . pql_get_define("PQL_GLOB_URI") . "home.php?msg=$msg");
//	exit();
//}

// check if domain exist
if(pql_domain_exist($_pql, $domain, $rootdn)) {
	$msg = urlencode($LANG->_('This domain already exists'));
	header("Location: " . pql_get_define("PQL_GLOB_URI") . "home.php?msg=$msg");
	exit();
}

$dns = pql_domain_add($_pql->ldap_linkid, $rootdn, $domain);
if($dns[0]) {
	// Can't have ' in the branch (used when creating default{mail,home}dir)
	$domain = ereg_replace("'", "", $domain);

	$entry["BRANCH_NAME"] = $domain;

	if($defaultdomain != "") {
		// update locals if control patch is enabled
		pql_control_update_domains($_pql, $_SESSION["USER_SEARCH_DN_CTR"], '*', array('', $defaultdomain));
	}

	// Default values we can easily figure out
	$defaultmaildir = pql_format_international(user_generate_mailstore('', '', '', $entry, 'branch'));
	$defaulthomedir = pql_format_international(user_generate_homedir('', '', '', $entry, 'branch'));

	// Replace spaces with underscore - can't create dirs with spaces,
	// it's bound to break SOMEWHERE!
	$defaultmaildir = preg_replace('/ /', '_', $defaultmaildir);
	$defaulthomedir = preg_replace('/ /', '_', $defaulthomedir);

	$msg = "";
	
	// Save the attributes - Default domain
	if($defaultdomain && !pql_set_domain_value($_pql->ldap_linkid,
											   $dns[0], 'defaultDomain',
											   pql_maybe_idna_encode($defaultdomain)))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// Save the attributes - Default home directory
	if($defaulthomedir && !pql_set_domain_value($_pql->ldap_linkid,
												$dns[0], 'baseHomeDir',
												$defaulthomedir))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// Save the attributes - Default mail directory
	if($defaultmaildir && !pql_set_domain_value($_pql->ldap_linkid,
												$dns[0], 'baseMailDir',
												$defaultmaildir))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// Save the attributes - Default quota
	if($defaultquota && !pql_set_domain_value($_pql->ldap_linkid, $dns[0], 'baseQuota', $defaultquota))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// The creator is by default the administrator
	if(! pql_set_domain_value($_pql->ldap_linkid, $dns[0], pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"]))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	// Create a template DNS zone
	if($template && $defaultdomain && pql_get_define("PQL_GLOB_BIND9_USE")) {
		require("./include/pql_bind9.inc");

		if(! pql_bind9_add_zone($_pql->ldap_linkid, $dns[0], $defaultdomain))
		  $msg = pql_complete_constant($LANG->_("Failed to add domain %domainname%"), array("domainname" => $defaultdomain));
	}

	// redirect to domain-details
	if($msg == "")
	  $msg = urlencode(pql_complete_constant($LANG->_('Domain %domain% successfully created'),
											 array("domain" => pql_maybe_decode($dns[0])))) . ".";
	$url = "domain_detail.php?rootdn=$rootdn&domain=$dns[0]&msg=$msg&rlnb=1";

	// Now it's time to run the special adduser script if defined...
	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $rootdn)) {
		// Setup the environment with the user details
		putenv("PQL_DOMAIN=$domain");
		putenv("PQL_DOMAINNAME=$defaultdomain");
		putenv("PQL_HOMEDIRS=$defaulthomedir");
		putenv("PQL_MAILDIRS=$defaultmaildir");
		putenv("PQL_QUOTA=$defaultquota");
		putenv("PQL_WEBUSER=".posix_getuid());
		
		// Execute the domain add script (0 => show output)
		if(pql_execute(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $rootdn), 0)) {
			echo pql_complete_constant($LANG->_('The %what% add script failed'),
									   array('what' => $LANG->_('domain'))) . "!";
			$msg .= " " . urlencode(pql_complete_constant($LANG->_('The %what% add script failed'),
														  array('what' => $LANG->_('domain'))) . "!");
		} else {
			echo "<b>" . pql_complete_constant($LANG->_('Successfully executed the %what% add script'),
											   array('what' => $LANG->_('domain'))) . "</b><br>";
			$msg .= " " . urlencode(pql_complete_constant($LANG->_('Successfully executed the %what% add script'),
														  array('what' => $LANG->_('domain'))));
		}

		$url = "domain_detail.php?rootdn=$rootdn&domain=$dns[0]";
?>

    <form action="<?=$url?>&msg=<?=$msg?>&rlnb=1" method="post">
      <input type="submit" value="<?=$LANG->_('Continue')?>">
    </form>
<?php

		die();
	} else
	  header("Location: " . pql_get_define("PQL_GLOB_URI") . $url);
} else {
	$msg = urlencode($LANG->_('Failed to create the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: " . pql_get_define("PQL_GLOB_URI") . "home.php?msg=$msg");
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
