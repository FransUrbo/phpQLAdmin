<?php
// navigation bar
// $Id: left.php,v 2.121 2006-11-12 21:43:20 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/left-head.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);
if($_pql->ldap_error) {
  unset($_SESSION["USER_ID"]);
  unset($_SESSION["USER_PASS"]);
  unset($_SESSION["USER_DN"]);
  
  die("$_pql->ldap_error<br><a href=\"index.php\" target=\"_top\">".$LANG->_('Login again')."</a>");
}
// }}}

// {{{ Find out if we're to run in ADVANCE/SIMPLE mode
if($_REQUEST["advanced"] == 1) {
  $checked  = " CHECKED";
  $_SESSION["ADVANCED_MODE"] = 1;
} else {
  $checked  = "";
  $_SESSION["ADVANCED_MODE"] = 0;
}
// }}}

// {{{ ---------------- LDAP host, user, login/logout information etc
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
<?php } elseif($_SESSION["lynx"]) { ?>
<?php   if($_REQUEST["advanced"] == 1) { ?>
      <input type="hidden" name="advanced" value="0">
      <input type="submit" value="<?=$LANG->_('Simple mode')."\n"?>">
<?php   } else { ?>
      <input type="hidden" name="advanced" value="1">
      <input type="submit" value="<?=$LANG->_('Advanced mode')."\n"?>">
<?php   } ?>
<?php } else { ?>
      <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?=$checked?>><?=$LANG->_('\uA\Udvanced mode')."\n"?>
<?php } ?>
    </form>
  </div>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"] and $_SESSION["ADVANCED_MODE"]) { ?>

  <!-- Add domain branch link -->
  <div id="el5Parent" class="parent">
    <a href="domain_add_form.php?rootdn=<?=$_SESSION["BASE_DN"][0]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain branch'))); ?></a><br>
  </div>

<?php }
// }}}
?>

  <!-- Home branch -->
<?php
  // {{{ ---------------- HOME BRANCH (PROJECT URLS ETC)
  // {{{ Level 1:      phpQLAdmin configuration etc
  $div_counter = 10; // Initialize the global counter
pql_format_tree("<b>".$LANG->_('Home')."</b>", 'home.php');
// }}}

// {{{ Level 2[abc]: Search links, configuration and tests and setup
// Level 2a: Search links
$links = array($LANG->_('Directory search') => 'user_search.php');
pql_format_tree($LANG->_('Search'), 0, $links, 1);

if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
  // Level 2b: Configuration and tests
  $links = array($LANG->_('phpQLAdmin Configuration')	=> 'config_detail.php',
				 $LANG->_('Test LDAP-Connection')		=> 'config_ldaptest.php');
  pql_format_tree($LANG->_('phpQLAdmin Configuration'), 0, $links, 1);
  
  // Level 2c: LDAP server setup etc
  $links = array($LANG->_('LDAP Syntaxes')		=> 'config_ldap.php?type=ldapsyntaxes',
				 $LANG->_('LDAP Matching rules')	=> 'config_ldap.php?type=matchingrules',
				 $LANG->_('LDAP Attribute types')	=> 'config_ldap.php?type=attributetypes',
				 $LANG->_('LDAP Object classes')	=> 'config_ldap.php?type=objectclasses');
  pql_format_tree($LANG->_('LDAP Server Configuration'), 0, $links, 1);
  
  if($_SESSION["MONITOR_BACKEND_ENABLED"] and $_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
	// Level 2c2: LDAP Server status
	$links = array($LANG->_('LDAP Server Status')		=> 'status_ldap.php?type=basics',
				   $LANG->_('LDAP Connection Status')	=> 'status_ldap.php?type=connections',
				   $LANG->_('LDAP Database Status')		=> 'status_ldap.php?type=databases');
	pql_format_tree($LANG->_('LDAP Server Statistics'), 0, $links, 1);
  }
}
// }}}

// {{{ Level 2d:     Documentation etc
$links = array($LANG->_('Documentation')		=> 'doc/index.php');
if($_SESSION["ADVANCED_MODE"]) {
  $new = array($LANG->_('What\'s left todo')		=> 'TODO',
			   $LANG->_('What\'s been done')		=> 'CHANGES');
  $links = $links + $new;
  
  if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	$new = array($LANG->_('Language translator')	=> 'tools/update_translations.php');
	$links = $links + $new;
  }
}
pql_format_tree($LANG->_('Documentation'), 0, $links, 1);
// }}}

// {{{ Level 2[ef]:  Main site and misc QmailLDAP links
if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
  // Level 2e: Main site and general phpQLAdmin links
  $links = array('phpQLAdmin @ Bayour'			=> 'http://phpqladmin.com/',
				 $LANG->_('Bugtracker')			=> 'http://bugs.bayour.com/');
  pql_format_tree($LANG->_('phpQLAdmin Site Specifics'), 0, $links, 1);
  
  // Level 2f: Misc QmailLDAP/Controls links
  $links = array('Official Qmail-LDAP pages'			=> 'http://www.qmail-ldap.org/',
				 'Life with Qmail-LDAP'			=> 'http://www.lifewithqmail.org/ldap/',
				 'Life with Qmail'				=> 'http://www.lifewithqmail.org/');
  pql_format_tree($LANG->_('Misc Qmail & Qmail-LDAP'), 0, $links, 1);
}
// }}}

// This an ending for the initial parent (level 0)
pql_format_tree_end();
// }}}

// {{{ ---------------- GET THE DOMAINS/BRANCHES
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  // This is a 'super-admin'. Should be able to read EVERYTHING!
  $domains = pql_get_domains($_pql);
} else {
  // {{{ Get ALL domains we have access to.
  //     I.e., all DN's with 'administrator: USER_DN'
  foreach($_SESSION["BASE_DN"] as $dn)  {
	$dom = pql_get_dn($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR")."=".$_SESSION["USER_DN"]);
	if($dom) {
	  foreach($dom as $d) {
		$domains[] = $d;
	  }
	}
  }
  // }}}
}
// }}}

// {{{ ---------------- GET THE USERS OF THE BRANCH
if(!isset($domains) or !is_array($domains)) {
  // {{{ No domain defined -> 'ordinary' user (only show this user)
  $_SESSION["SINGLE_USER"] = 1;
  
  $cn = pql_get_attribute($_pql->ldap_linkid, $_SESSION["USER_DN"], pql_get_define("PQL_ATTR_CN"));
  if(is_array($cn))
	$cn = $cn[0];
  
  // Try to get the DN of the domain
  $dnparts = ldap_explode_dn($_SESSION["USER_DN"], 0);
  for($i=1; $i < count($dnparts); $i++) {
	// Traverse the users DN backwards
	$DN = $dnparts[$i];
	for($j=$i+1; $j < count($dnparts); $j++)
	  $DN .= "," . $dnparts[$j];
	
	// Look in DN for attribute 'defaultdomain'.
	$defaultdomain = pql_get_attribute($_pql->ldap_linkid, $DN, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
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
  // }}}
} else {
  // {{{ We got at least one domain - get it's users
  $_SESSION["SINGLE_USER"] = 0;
?>

  <!-- Domain branches -->
<?php
  $domains = pql_uniq($domains);
  if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
	echo "<br>Domains:"; printr($domains);
  }
  foreach($domains as $key => $domain) {
	// {{{ Get domain name part from the DN
	// Three steps because pql_get_domains() and pql_get_dn()
	// returns normalized DN's which isn't as pretty.
	// 1. Get each part of the DN
	$dnparts = ldap_explode_dn($domain, 0);
	
	// 2. Extract the attribute from the first RDN
	$tmp = split('=', $dnparts[0]);
	$attrib = $tmp[0];
	
	// 3. Get the attribute value from the object
	$d = pql_get_attribute($_pql->ldap_linkid, $domain, $attrib);
	if(is_array($d))
	  // Happens if the object have '> 1' attribute values in it's reference.
	  // It's impossible to figure out WHICH of the values to use, so we
	  // take the first. Better than nothing...
	  $d = $d[0];
	// }}}
	
	if(!eregi('%3D', $domain))
	  $domain = urlencode($domain);
	
	// Get Root DN
	$rootdn = pql_get_rootdn($domain, 'left.php');
	
	// Create a user search filter (only look for mail users - !?!? - or Samba accounts).
	$filter  = "(&(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)."=*)(|(";
	$filter .= pql_get_define("PQL_ATTR_MAIL")."=*)(sambaSID=*)))";
	
	// Get the subbranches in this domain
	$branches = pql_unit_get($_pql->ldap_linkid, $domain);
	if((count($branches) > 1)) {
	  // More than one subbranch
	  $links = array(pql_complete_constant($LANG->_('Add %what%'),
										   array('what' => $LANG->_('sub unit')))
					 => "unit_add.php?rootdn=$rootdn&domain=$domain",
					 
					 pql_complete_constant($LANG->_('Add %what%'),
										   array('what' => $LANG->_('user')))
					 => "user_add.php?rootdn=$rootdn&domain=$domain");
	  
	  // Just incase there's user(s) at the base of the domain branch...
	  $users = pql_get_dn($_pql->ldap_linkid, $domain, $filter, 'ONELEVEL');
	  if(is_array($users))
		// We have users in this domain
		pql_left_htmlify_userlist($_pql->ldap_linkid, $rootdn, $domain, $subbranch, $users, $links);
	  
	  pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
	} elseif((count($branches) < 1))
	  // This branch don't have any sub units (flat structure)
	  // -> make sure we still jump into the for loop!
	  $branches[0] = $domain;

	// Just to make sure that the branches variable is really count()'able...
	if(!@$branches[0])
	  $branches = array();

	if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
	  echo "<br>Branches ($domain): "; printr($branches);
	}

	// Show users is either 'Yes', unset (same thing) or 'No'.
	$show_users = pql_get_define("PQL_CONF_SHOW_USERS", $rootdn);

	for($i=0; $i < count($branches); $i++) {
	  $subbranch = 0;
	  
	  if($show_users or !isset($show_users)) {
		// Zero out the variables, othervise we won't get users in
		// specified domain, but also in the PREVIOUS domain shown!
		unset($users); unset($links); unset($cns);
		
		// Get all users (their DN) in this domain (sub)branch
		if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS"))
		  echo "Retreiving users in branch '".$branches[$i]."'<br>";
		$users = pql_get_dn($_pql->ldap_linkid, $branches[$i], $filter);
		
		// Level 2: The users
		if(count($branches) > 1) {
		  // We're only interested in the 'People', 'Users' etc value,
		  // not the complete DN.
		  // Three steps because pql_get_domains() and pql_get_dn()
		  // returns normalized DN's which isn't as pretty.
		  $dnparts		= ldap_explode_dn($branches[$i], 0);
		  $tmp			= split('=', $dnparts[0]);
		  $attrib		= $tmp[0];
		  $subbranch	= pql_get_attribute($_pql->ldap_linkid, $branches[$i], $attrib);
		  
		  $url			= '';
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
		  pql_left_htmlify_userlist($_pql->ldap_linkid, $rootdn, $domain, $subbranch, $users, $links);
	  }
	  
	  // Level 1: The domain name with it's users
	  if((count($branches) > 1) and $subbranch)
		pql_format_tree(pql_maybe_decode(urldecode($subbranch)), $url, $links, 1);
	  else
		pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
	} // end foreach ($branches)
	
	// This an ending for the domain tree
	pql_format_tree_end();
  } // end foreach ($domains)
  
  // }}}
} // end if(is_array($domains))
// }}}

if(file_exists($_SESSION["path"]."/.DEBUG_ME") and file_exists($_SESSION["path"]."/.DEBUG_PROFILING")) {
  $now = pql_format_return_unixtime();
  echo "Now: <b>$now</b><br>";
}

require("./left-trailer.html");

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
