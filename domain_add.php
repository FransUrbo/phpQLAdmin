<?php
// add a domain
// $Id: domain_add.php,v 2.47.10.1 2004-04-19 12:49:31 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

$url["domain"]		  = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"]		  = pql_format_urls($_REQUEST["rootdn"]);
$url["defaultdomain"] = pql_format_urls($_REQUEST["rootdn"]);

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}

include("./header.html");
?>
  <span class="title1"><?=$LANG->_('Create domain')?>: <?=$_REQUEST["domain"]?></span>
  <br><br>
<?php
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// Should we force a dot in the domainname or not?
if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "dc" or
   pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "ou" or
   pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "o")
{
	// We're using a domain or organization object, which
	// means we should allow a domain name to be without dot.
	$force_dot = 0;
} else {
	$force_dot = 1;
}

// check if domain is valid
//if(!pql_check_hostaddress($_REQUEST["domain"], $force_dot)) {
//	$msg = urlencode($LANG->_('Invalid domain name! Use: domain.tld (e.g. adfinis.com)'));
//	header("Location: " . pql_get_define("PQL_CONF_URI") . "home.php?msg=$msg");
//	exit();
//}

// check if domain exist
if(pql_domain_exist($_pql, $_REQUEST["domain"], $_REQUEST["rootdn"])) {
	$msg = urlencode($LANG->_('This domain already exists'));
	header("Location: " . pql_get_define("PQL_CONF_URI") . "home.php?msg=$msg");
	exit();
}

$dns = pql_domain_add($_pql->ldap_linkid, $_REQUEST["rootdn"], $_REQUEST["domain"]);
if($dns[0]) {
	$entry["BRANCH_NAME"] = $_REQUEST["domain"];

	if(($_REQUEST["defaultdomain"] != "") and pql_get_define("PQL_CONF_CONTROL_USE")) {
		// update locals if control patch is enabled
		pql_control_update_domains($_pql, $_SESSION["USER_SEARCH_DN_CTR"], '*', array('', $_REQUEST["defaultdomain"]));
	}

	// Default values we can easily figure out
	$defaultmaildir = pql_format_international(user_generate_mailstore('', '', '', $entry, 'branch'));
	$defaulthomedir = pql_format_international(user_generate_homedir('', '', '', $entry, 'branch'));

	// Replace spaces with underscore - can't create dirs with spaces,
	// it's bound to break SOMEWHERE!
	$defaultmaildir = preg_replace('/ /', '_', $defaultmaildir);
	$defaulthomedir = preg_replace('/ /', '_', $defaulthomedir);

	// Remove any occurences of '&'
	$defaultmaildir = preg_replace('/&_/', '', $defaultmaildir);
	$defaultmaildir = preg_replace('/&/', '',  $defaultmaildir);
	$defaulthomedir = preg_replace('/&_/', '', $defaulthomedir);
	$defaulthomedir = preg_replace('/&/', '',  $defaulthomedir);

	$msg = "";
	
	// Save the attributes - Default domain
	if($_REQUEST["defaultdomain"] && !pql_domain_set_value($_pql->ldap_linkid,
											   $dns[0], 'defaultDomain',
											   pql_maybe_idna_encode($_REQUEST["defaultdomain"])))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// Save the attributes - Default home directory
	if($defaulthomedir && !pql_domain_set_value($_pql->ldap_linkid,
												$dns[0], 'baseHomeDir',
												$defaulthomedir))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// Save the attributes - Default mail directory
	if($defaultmaildir && !pql_domain_set_value($_pql->ldap_linkid,
												$dns[0], 'baseMailDir',
												$defaultmaildir))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// Save the attributes - Default quota
	if($defaultquota && !pql_domain_set_value($_pql->ldap_linkid, $dns[0], 'baseQuota', $defaultquota))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	
	// The creator is by default the administrator
	if(! pql_domain_set_value($_pql->ldap_linkid, $dns[0], pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"]))
	  $msg = $LANG->_('Failed to change the default domainname') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	// Create a template DNS zone
	if($template && $_REQUEST["defaultdomain"] && pql_get_define("PQL_CONF_BIND9_USE")) {
		require("./include/pql_bind9.inc");

		if(! pql_bind9_add_zone($_pql->ldap_linkid, $dns[0], $_REQUEST["defaultdomain"]))
		  $msg = pql_complete_constant($LANG->_("Failed to add domain %domainname%"), array("domainname" => $_REQUEST["defaultdomain"]));
	}

	// redirect to domain-details
	if($msg == "")
	  $msg = urlencode(pql_complete_constant($LANG->_('Domain %domain% successfully created'),
											 array("domain" => pql_maybe_decode($dns[0])))) . ".";
	$url = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".urlencode($dns[0])."&msg=$msg&rlnb=1";

	// Now it's time to run the special adduser script if defined...
	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["rootdn"])) {
		// Setup the environment with the user details
		putenv("PQL_DOMAIN=".$_REQUEST["domain"]);
		putenv("PQL_DOMAINNAME=".$_REQUEST["defaultdomain"]);
		putenv("PQL_HOMEDIRS=$defaulthomedir");
		putenv("PQL_MAILDIRS=$defaultmaildir");
		putenv("PQL_QUOTA=$defaultquota");
		putenv("PQL_WEBUSER=".posix_getuid());
		
		// Execute the domain add script (0 => show output)
		if(pql_execute(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["rootdn"]), 0)) {
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

		$url = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".urlencode($dns[0]);
?>

    <form action="<?=$url?>&msg=<?=$msg?>&rlnb=1" method="post">
      <input type="submit" value="<?=$LANG->_('Continue')?>">
    </form>
<?php

		die();
	} else
	  header("Location: " . pql_get_define("PQL_CONF_URI") . $url);
} else {
	$msg = urlencode($LANG->_('Failed to create the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: " . pql_get_define("PQL_CONF_URI") . "home.php?msg=$msg");
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
