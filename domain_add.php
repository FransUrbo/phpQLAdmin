<?php
// add a domain
// $Id: domain_add.php,v 2.58 2005-03-09 09:59:03 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

$url["domain"]		  = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"]		  = pql_format_urls($_REQUEST["rootdn"]);
$url["defaultdomain"] = pql_format_urls($_REQUEST["rootdn"]);

// {{{ Include control api if control is used
if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    include($_SESSION["path"]."/include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}

include($_SESSION["path"]."/header.html");
// }}}
?>
  <span class="title1"><?=$LANG->_('Create domain')?>: <?=$_REQUEST["domain"]?></span>
  <br><br>
<?php
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// {{{ Should we enforce a dot in the domainname or not?
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
// }}}

// {{{ Check if domain is valid
//if(!pql_check_hostaddress($_REQUEST["domain"], $force_dot)) {
//	$msg = urlencode($LANG->_('Invalid domain name! Use: domain.tld (e.g. adfinis.com)'));
//	header("Location: " . $_SESSION["URI"] . "home.php?msg=$msg");
//	exit();
//}
// }}}

// {{{ Check if domain exist
$filter  = '(&(objectclass=*)('.pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]);
$filter .= '='.$_REQUEST["domain"].'))';
if(pql_get_dn($_pql->ldap_linkid, $_REQUEST["rootdn"], $filter, 'ONELEVEL')) {
	$msg = urlencode($LANG->_('This domain already exists'));
	header("Location: " . $_SESSION["URI"] . "home.php?msg=$msg");
	exit();
}
// }}}

// {{{ Setup the 'LDIF'
// Setup RDN
$dn = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]).'='.$_REQUEST["domain"].','.$_REQUEST["rootdn"];
$entry[pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"])] = $_REQUEST["domain"];

// Setup objectclasses
$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $_REQUEST["rootdn"]);
if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "o") {
	$entry[pql_get_define("PQL_ATTR_O")] = $_REQUEST["domain"];
} else{
	$entry[pql_get_define("PQL_ATTR_O")] = 0;
}

// Add default domain to the object
if($_REQUEST["defaultdomain"]) {
	$entry[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")] = $_REQUEST["defaultdomain"];
}

// This is needed by the user_generate_{homedir,mailstore}() functions. It will be unset
// before adding the object to the database...
$entry["BRANCH_NAME"] = $_REQUEST["domain"];

// Add default home directory to the object
if(function_exists("user_generate_homedir")) {
  $defaulthomedir = pql_format_international(user_generate_homedir('', '', '', $entry, 'branch'));
  if($defaulthomedir) {
	$defaulthomedir = preg_replace('/ /', '_', $defaulthomedir);
	$defaulthomedir = preg_replace('/&_/', '', $defaulthomedir);
	$defaulthomedir = preg_replace('/&/', '',  $defaulthomedir);
	
	$entry[pql_get_define("PQL_ATTR_BASEHOMEDIR")] = $defaulthomedir;
  }
}

// Add default mail directory to the object
if(function_exists("user_generate_mailstore")) {
  $defaultmaildir = pql_format_international(user_generate_mailstore('', '', '', $entry, 'branch'));
  if($defaultmaildir) {
	$defaultmaildir = preg_replace('/ /', '_', $defaultmaildir);
	$defaultmaildir = preg_replace('/&_/', '', $defaultmaildir);
	$defaultmaildir = preg_replace('/&/', '',  $defaultmaildir);

	$entry[pql_get_define("PQL_ATTR_BASEMAILDIR")] = $defaultmaildir;
  }
}

// Add all the super admins with full access.
$admins = pql_get_attribute($_pql->ldap_linkid, urldecode($_SESSION["BASE_DN"][0]), pql_get_define("PQL_ATTR_ADMINISTRATOR"));
if(is_array($admins)) {
	for($j=0; $admins[$j]; $j++)
	  $entry[pql_get_define("PQL_ATTR_ADMINISTRATOR")][] = $admins[$j];
} else {
	$entry[pql_get_define("PQL_ATTR_ADMINISTRATOR")] = $admins;
}

// This was just a temporary value for the user_generate_{homedir,mailstore}() functions.
unset($entry["BRANCH_NAME"]);
// }}}

// Add the branch/domain to the database
if(pql_write_add($_pql->ldap_linkid, $dn, $entry, 'branch', 'domain_add.php')) {
    // {{{ Add the USER subtree if defined
    if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
		pql_unit_add($_pql->ldap_linkid, $dn, pql_get_define("PQL_CONF_SUBTREE_USERS"));
    }
	// }}}

    // {{{ Add the GROUPS subtree if defined
    if(pql_get_define("PQL_CONF_SUBTREE_GROUPS")) {
		pql_unit_add($_pql->ldap_linkid, $dn, pql_get_define("PQL_CONF_SUBTREE_GROUPS"));
    }
	// }}}

	// {{{ Update locals if control patch is enabled
	if(($_REQUEST["defaultdomain"] != "") and pql_get_define("PQL_CONF_CONTROL_USE") and
	   pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"])) {
		pql_control_update_domains($_pql, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
								   '*', array('', $_REQUEST["defaultdomain"]));
	}
	// }}}

	$msg = "";
	
	// {{{ Create a template DNS zone
	if($_REQUEST["template"] && $_REQUEST["defaultdomain"] && pql_get_define("PQL_CONF_BIND9_USE")) {
		require($_SESSION["path"]."/include/pql_bind9.inc");

		if(! pql_bind9_add_zone($_pql->ldap_linkid, $dn, $_REQUEST["defaultdomain"]))
		  $msg = pql_complete_constant($LANG->_("Failed to add domain %domainname%"), array("domainname" => $_REQUEST["defaultdomain"]));
	}
	// }}}

	// {{{ Prepare for redirecting to domain-details
	if($msg == "")
	  $msg = urlencode(pql_complete_constant($LANG->_('Domain %domain% successfully created'),
											 array("domain" => pql_maybe_decode($dn)))) . ".";
	$url = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".urlencode($dn)."&msg=$msg&rlnb=1";
	// }}}

	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["rootdn"])) {
		// {{{ Run the special adddomain script - Setup the environment with the user details
		putenv("PQL_DOMAIN=".$_REQUEST["domain"]);
		putenv("PQL_DOMAINNAME=".$_REQUEST["defaultdomain"]);
		putenv("PQL_HOMEDIRS=$defaulthomedir");
		putenv("PQL_MAILDIRS=$defaultmaildir");
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

		$url = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".urlencode($dn);
?>

    <form action="<?=$url?>&msg=<?=$msg?>&rlnb=1" method="post">
      <input type="submit" value="<?=$LANG->_('Continue')?>">
    </form>
<?php

		die();
		// }}}
	} elseif(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
		echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
		die("<b>$url</b>");
	} else {
		header("Location: " . $_SESSION["URI"] . $url);
	}
} else {
	$msg = urlencode($LANG->_('Failed to create the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: " . $_SESSION["URI"] . "home.php?msg=$msg");
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
