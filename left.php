<?php
// navigation bar
// $Id: left.php,v 2.90.8.1 2004-09-17 14:28:48 turbo Exp $
//
session_start();

require("./include/pql_config.inc");
require("./left-head.html");

function left_htmlify_userlist($linkid, $rootdn, $domain, $subbranch, $users, &$links) {
    // Iterate trough all users in this domain/branch
    foreach ($users as $dn) {
	unset($cn); unset($sn); unset($gecos);
	
	// From the user DN, get the CN and SN.
	$cn = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_GIVENNAME"));
	$sn = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_SN"));
	if($cn[0] && $sn[0]) {
	    // We have a givenName (first name) and a surName (last name) - combine the two
	    if($sn[0] != '_') {
		$cns[$dn] = $sn[0].", ".$cn[0];
	    } else {
		$cns[$dn] = $cn[0];
	    }
	} else {
	    // Probably don't have a givenName - get the commonName
	    $cn = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_CN"));
	    if($cn[0]) {
		// We have a commonName - split it up into two parts (which should be first and last name)
		$cn = split(" ", $cn[0]);
		if(!$cn[1]) {
		    // Don't have second part (last name) of the commonName - MUST be a system 'user'.
		    $cns[$dn] = "System - ".$cn[0];
		} else {
		    // We have two parts - combine into 'Lastname, Firstname'
		    $cns[$dn] = $cn[1].", ".$cn[0];
		}
	    } else {
		// No givenName, surName or commonName - last try, get the gecos
		$gecos = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_GECOS"));
		if($gecos[0])
		  // We have a gecos - use that as is
		  $cns[$dn] = $gecos[0];
		//			    else
		//			      // No gecos either. Now what!?
	    }
	}
    }

    if(is_array($cns)) {
	asort($cns);
    
	foreach($cns as $dn => $cn) {
	    $uid = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_UID"));
	    $uid = $uid[0];
	    
	    $uidnr = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_QMAILUID"));
	    $uidnr = $uidnr[0];
	    
	    if(($uid != 'root') or ($uidnr != '0')) {
		// Do NOT show root user(s) here! This should (for safty's sake)
		// not be availible to administrate through phpQLAdmin!
		if($subbranch)
		  $new = array($cn => "user_detail.php?rootdn=$rootdn&domain=$domain&subbranch=$subbranch&user=".urlencode($dn));
		else
		  $new = array($cn => "user_detail.php?rootdn=$rootdn&domain=$domain&user=".urlencode($dn));
		// Add the link to the main array
		$links = $links + $new;
	    }
	}
    }
}

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);
if($_pql->ldap_error) {
    unset($_SESSION["USER_ID"]);
    unset($_SESSION["USER_PASS"]);
    unset($_SESSION["USER_DN"]);

    die("$_pql->ldap_error<br><a href=\"index.php\" target=\"_top\">".$LANG->_('Login again')."</a>");
}

// find out if we're to run in ADVANCE/SIMPLE mode
if($_REQUEST["advanced"] == 1) {
    $checked  = " CHECKED";
    $_SESSION["ADVANCED_MODE"] = 1;
} else {
    $checked  = "";
    $_SESSION["ADVANCED_MODE"] = 0;
}
?>
  <!-- Link to the logged in user information page -->
  <div id="el1Parent" class="parent" style="margin-bottom: 5px">
    <?=$LANG->_('User')?>:
    <nobr>
      <a class="item" href="user_detail.php?rootdn=<?php echo pql_get_rootdn($_SESSION["USER_DN"], 'left.php')?>&user=<?=urlencode($_SESSION["USER_DN"])?>">
        <span class="heada"><b><?=$_SESSION["USER_ID"]?></b></span>
      </a>
    </nobr>
  </div>

<?php if($_SESSION["ADVANCED_MODE"]) {
	$host = split(';', $_SESSION["USER_HOST"]);

	// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
	$host[0] = urldecode($host[0]);
?>
  <!-- LDAP Server information -->
  <div id="el2Parent" class="parent" style="margin-bottom: 5px">
    <?=$LANG->_('LDAP Server')?>:
    <nobr>
      <span class="heada"><?=pql_maybe_idna_decode($host[0])?><?php if(!eregi('^ldapi:', $host[0])) { echo ":".$host[1]; } ?></span>
    </nobr>
  </div>

<?php } ?>

  <!-- logout link -->
  <div id="el3Parent" class="parent" style="margin-bottom: 5px">
    <nobr>
      <a class="item" href="index.php?logout=1" target="_parent">
        <span class="heada"><?=$LANG->_('Log out')?></span>
      </a>
    </nobr>
  </div>

  <!-- Advanced Mode selector -->
  <div id="el4Parent" class="parent">
    <form method=post action="index2.php" target="_top">
<?php if($_SESSION["konqueror"]) { ?>
      <input type="checkbox" name="advanced" accesskey="a" onClick="this.form.submit()"<?=$checked?>><?=$LANG->_('\uA\Udvanced mode')."\n"?>
<?php } else { ?>
      <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?=$checked?>><?=$LANG->_('\uA\Udvanced mode')."\n"?>
<?php } ?>
    </form>
  </div>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"] and $_SESSION["ADVANCED_MODE"]) { ?>

  <!-- Add domain branch link -->
  <div id="el5Parent" class="parent">
    <a href="domain_add_form.php?rootdn=<?=$_pql->ldap_basedn[0]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain branch'))); ?></a><br>
  </div>

<?php } ?>

  <!-- Home branch -->
<?php
// ---------------- HOME BRANCH (PROJECT URLS ETC)
// Level 1: phpQLAdmin configuration etc
$div_counter = 10; // Initialize the global counter
pql_format_tree("<b>".$LANG->_('Home')."</b>", 'home.php');

// Level 2a: Search links
$links = array($LANG->_('Find user') => 'user_search.php');
pql_format_tree($LANG->_('Search'), 0, $links, 1);

if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
    // Level 2b: Configuration and tests
    $links = array($LANG->_('phpQLAdmin Configuration')	=> 'config_detail.php',
		   $LANG->_('Test LDAP-Connection')	=> 'config_ldaptest.php',
		   $LANG->_('Translate phpQLAdmin')	=> 'update_translations.php');
    pql_format_tree($LANG->_('phpQLAdmin Configuration'), 0, $links, 1);
    
    // Level 2c: LDAP server setup etc
    $links = array($LANG->_('LDAP Syntaxes')		=> 'config_ldap.php?type=ldapsyntaxes',
		   $LANG->_('LDAP Matching rules')	=> 'config_ldap.php?type=matchingrules',
		   $LANG->_('LDAP Attribute types')	=> 'config_ldap.php?type=attributetypes',
		   $LANG->_('LDAP Object classes')	=> 'config_ldap.php?type=objectclasses');
    if($_SESSION["MONITOR_BACKEND_ENABLED"] and $_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
	$new = array(0					=> 0,
		     $LANG->_('LDAP Server Status')	=> 'status_ldap.php?type=basics',
		     $LANG->_('LDAP Connection Status')	=> 'status_ldap.php?type=connections',
		     $LANG->_('LDAP Database Status')	=> 'status_ldap.php?type=databases');
	$links = $links + $new;
    }
    pql_format_tree($LANG->_('LDAP Server Configuration'), 0, $links, 1);
}

// Level 2d: Documentation etc
$links = array($LANG->_('Documentation')		=> 'doc/index.php');
if($_SESSION["ADVANCED_MODE"]) {
    $new = array($LANG->_('What\'s left todo')		=> 'TODO',
		 $LANG->_('What\'s been done')		=> 'CHANGES');
    $links = $links + $new;

    if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	$new = array($LANG->_('Language translator')	=> 'update_translations.php');
	$links = $links + $new;
    }
}
pql_format_tree($LANG->_('Documentation'), 0, $links, 1);

if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
    // Level 2e: Main site and general phpQLAdmin links
    $links = array('phpQLAdmin @ Bayour'			=> 'http://phpqladmin.bayour.com/',
		   $LANG->_('Bugtracker')			=> 'http://apache.bayour.com/anthill/');
    pql_format_tree($LANG->_('phpQLAdmin Site Specifics'), 0, $links, 1);

    // Level 2f: Misc QmailLDAP/Controls links
    $links = array('Official Qmail-LDAP pages'			=> 'http://www.nrg4u.com/',
		   'Life with Qmail-LDAP'			=> 'http://www.lifewithqmail.org/ldap/',
		   'Life with Qmail'				=> 'http://www.lifewithqmail.org/');
    pql_format_tree($LANG->_('Misc Qmail & Qmail-LDAP'), 0, $links, 1);
}

// This an ending for the initial parent (level 0)
pql_format_tree_end();

// ---------------- GET THE DOMAINS/BRANCHES
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
    // This is a 'super-admin'. Should be able to read EVERYTHING!
    $domains = pql_domain_get($_pql);
} else {
    // Get ALL domains we have access.
    //	'administrator: USER_DN'
    // in the domain object
    foreach($_pql->ldap_basedn as $dn)  {
	$dn = urldecode($dn);

	$dom = pql_domain_get_value($_pql, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"]);
	if($dom) {
	    foreach($dom as $d) {
		$domains[] = urlencode($d);
	    }
	}
    }
}

// ---------------- GET THE USERS OF THE BRANCH
if(!isset($domains)) {
    // No domain defined -> 'ordinary' user (only show this user)
    $_SESSION["SINGLE_USER"] = 1;

    $cn = pql_get_attribute($_pql->ldap_linkid, $_SESSION["USER_DN"], pql_get_define("PQL_ATTR_CN")); $cn = $cn[0];

    // Try to get the DN of the domain
    $dnparts = ldap_explode_dn($_SESSION["USER_DN"], 0);
    for($i=1; $dnparts[$i]; $i++) {
	// Traverse the users DN backwards
	$DN = $dnparts[$i];
	for($j=$i+1; $dnparts[$j]; $j++)
	  $DN .= "," . $dnparts[$j];
	
	// Look in DN for attribute 'defaultdomain'.
	$defaultdomain = pql_domain_get_value($_pql, $DN, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	if($defaultdomain) {
	    // A hit. This is the domain under which the user is located.
	    $domain = $DN;
	    break;
	}
    }

    if(empty($domain) and $_REQUEST["domain"])
      $domain = $_REQUEST["domain"];

    if(empty($rootdn) and empty($_REQUEST["rootdn"]))
      $rootdn = pql_get_rootdn($_SESSION["USER_DN"], 'left.php');

    $links = array($cn => "user_detail.php?rootdn=".$rootdn."&domain=$domain&user=".urlencode($_SESSION["USER_DN"]));
    echo "<br>";
    pql_format_tree_span($cn, $links, -1);
} else {
    $_SESSION["SINGLE_USER"] = 0;
?>

  <!-- Domain branches -->
<?php
    // We got at least one domain - get it's users
    asort($domains);
    foreach($domains as $key => $domain) {
	// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
	$d = split(',', urldecode($domain)); $d = split('=', $d[0]); $d = $d[1];
	$d = pql_maybe_decode($d);

	if(!eregi('%3D', $domain))
	  $domain = urlencode($domain);

	// Get Root DN
	$rootdn = pql_get_rootdn($domain, 'left.php');

	// Get the subbranches in this domain
	$branches = pql_unit_get($_pql->ldap_linkid, $domain);

	if((count($branches) > 1)) {
	    $links = array(pql_complete_constant($LANG->_('Add %what%'),
						 array('what' => $LANG->_('sub unit')))
			   => "unit_add.php?rootdn=$rootdn&domain=$domain",

			   pql_complete_constant($LANG->_('Add %what%'),
						 array('what' => $LANG->_('user')))
			   => "user_add.php?rootdn=$rootdn&domain=$domain");

	    // Just incase there's user(s) at the base of the domain branch...
	    $users = pql_user_get($_pql->ldap_linkid, $domain, 'ONELEVEL');
	    if(is_array($users)) {
		// We have users in this domain
		left_htmlify_userlist($_pql->ldap_linkid, $rootdn, $domain, $subbranch, $users, $links);
	    }

	    pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
	} else
	  // This branch don't have any sub units (flat structure)
	  // -> make sure we still jump into the for loop!
	  $branches[0] = $domain;

	for($i = 0; $branches[$i]; $i++) {
	    unset($subbranch);

	    if(pql_get_define("PQL_CONF_SHOW_USERS", $rootdn)) {
		// Zero out the variables, othervise we won't get users in
		// specified domain, but also in the PREVIOUS domain shown!
		$users = ""; $cns = ""; unset($links);
		
		// Get all users (their DN) in this domain (sub)branch
		if(count($branches) > 1) {
		    $users = pql_user_get($_pql->ldap_linkid, $branches[$i]);
		} else {
		    // We only have one subbranch, don't show the subbranch, list the users
		    // under the domain branch
		    $users = pql_user_get($_pql->ldap_linkid, $domain);
		}

		// Level 2: The users
		if(count($branches) > 1) {
		    // We're only interested in the 'People', 'Users' etc value,
		    // not the complete DN.
		    $dnparts = ldap_explode_dn($branches[$i], 0);
		    $dnparts = split('=', $dnparts[0]);
		    $subbranch = urlencode($dnparts[1]);
		    
		    $links = array(pql_complete_constant($LANG->_('Add %what%'),
							 array('what' => $LANG->_('user')))
				   => "user_add.php?rootdn=$rootdn&domain=$domain&subbranch=$subbranch");
		} else {
		    $links = array(pql_complete_constant($LANG->_('Add %what%'),
							 array('what' => $LANG->_('sub unit')))
				    => "unit_add.php?rootdn=$rootdn&domain=$domain",

				   pql_complete_constant($LANG->_('Add %what%'),
							 array('what' => $LANG->_('user')))
				   => "user_add.php?rootdn=$rootdn&domain=$domain");
		}

		if(is_array($users))
		  // We have users in this domain
		  left_htmlify_userlist($_pql->ldap_linkid, $rootdn, $domain, $subbranch, $users, $links);
	    }

	    // Level 1: The domain name with it's users
	    if((count($branches) > 1) and $subbranch)
		pql_format_tree(pql_maybe_decode(urldecode($subbranch)), $url, $links, 1);
	    else {
		pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
		echo "\n";
	    }
	} // end foreach ($branches)

	// This an ending for the domain tree
	pql_format_tree_end();
    } // end foreach ($domains)
} // end if(is_array($domains))

require("./left-trailer.html");
?>
