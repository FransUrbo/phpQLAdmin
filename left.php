<?php
// navigation bar
// $Id: left.php,v 2.65 2003-11-24 09:05:49 turbo Exp $
//
session_start();

require("./include/pql_config.inc");
require("./left-head.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS, false, 0);
if($_pql->ldap_error) {
    session_unregister("USER_ID");
    session_unregister("USER_PASS");
    session_unregister("USER_DN");

    die("$_pql->ldap_error<br><a href=\"index.php\" target=\"_top\">".$LANG->_('Login again')."</a>");
}

// find out if we're to run in ADVANCE/SIMPLE mode
if($advanced == 1) {
    $checked  = " CHECKED";
    $ADVANCED_MODE = 1;

    session_register("ADVANCED_MODE");
} else {
    $checked  = "";
    $ADVANCED_MODE = 0;

    session_register("ADVANCED_MODE");
}
?>
  <font color="black" class="heada">
    <?=$LANG->_('User')?>: <b><a href="user_detail.php?rootdn=<?php echo pql_get_rootdn($USER_DN)?>&user=<?=$USER_DN?>"><?=$USER_ID?></a></b>
  </font>
  <br>
<?php if($ADVANCED_MODE) {
	$host = split(';', $USER_HOST);
?>

  <font color="black" class="heada"><?=$LANG->_('LDAP Server')?>: <b><?=$host[0]?>:<?=$host[1]?></b></font>
  <br>
<?php } ?>

  <font color="black" class="heada">
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="index.php?logout=1" target="_parent"><?=$LANG->_('Log out')?></a>
  </font>

  <br>

  <form method=post action="index2.php" target="_top">
    <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?=$checked?>><?=$LANG->_('\uA\Udvanced mode')."\n"?>
  </form>

<?php if($ALLOW_BRANCH_CREATE and $ADVANCED_MODE) { ?>

  <div id="el2Parent" class="parent">
    <nobr><a href="domain_add_form.php?rootdn=<?=$_pql->ldap_basedn[0]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain branch'))); ?></a></nobr>
  </div>

<?php } ?>


  <!-- Home branch -->
<?php
// ---------------- HOME BRANCH (PROJECT URLS ETC)
// Level 1: phpQLAdmin configuration etc
$div_counter = 3; // Initialize the global counter
pql_format_tree("<b>".$LANG->_('Home')."</b>", 'home.php');

// Level 2a: Search links
$links = array('user_search.php' => $LANG->_('Find user'));
pql_format_tree($LANG->_('Search'), 0, $links, 1);

if($ADVANCED_MODE) {
    if($ALLOW_BRANCH_CREATE) {
	// Level 2b: Configuration and tests
	$links = array('config_detail.php'		=> $LANG->_('Show configuration'),
		       'config_ldaptest.php'		=> $LANG->_('Test LDAP-Connection'),
		       'config_ldap.php'		=> $LANG->_('LDAP server configuration'));
	pql_format_tree($LANG->_('Configuration'), 0, $links, 1);
    }

    // Level 2c: Documentation etc
    $links = array('doc/index.php'			=> $LANG->_('Documentation'),
		   'TODO'				=> $LANG->_('What\'s left todo'),
		   'CHANGES'				=> $LANG->_('What\'s been done'),
		   'update_translations.php'		=> $LANG->_('Language translator'));
    pql_format_tree($LANG->_('Documentation'), 0, $links, 1);

    // Level 2d: Main site and general phpQLAdmin links
    $links = array('http://phpqladmin.bayour.com/'	=> 'phpqladmin.bayour.com',
		   'http://apache.bayour.com/anthill/'	=> $LANG->_('Bugtracker'));
    pql_format_tree($LANG->_('phpQLAdmin'), 0, $links, 1);
}

// This an ending for the initial parent (level 0)
pql_format_tree_end();

// ---------------- GET THE DOMAINS/BRANCHES
if($ALLOW_BRANCH_CREATE) {
    // This is a 'super-admin'. Should be able to read EVERYTHING!
    $domains = pql_domain_get($_pql);
} else {
    // Get ALL domains we have access.
    //	'administrator: USER_DN'
    // in the domain object
    foreach($_pql->ldap_basedn as $dn)  {
	$dn = urldecode($dn);

	$dom = pql_domain_value($_pql, $dn, pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"), $USER_DN);
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
    $SINGLE_USER = 1; session_register("SINGLE_USER");

    $cn = pql_get_attribute($_pql->ldap_linkid, $USER_DN, pql_get_define("PQL_GLOB_ATTR_CN")); $cn = $cn[0];

    // Try to get the DN of the domain
    $dnparts = ldap_explode_dn($USER_DN, 0);
    for($i=1; $dnparts[$i]; $i++) {
	// Traverse the users DN backwards
	$DN = $dnparts[$i];
	for($j=$i+1; $dnparts[$j]; $j++)
	  $DN .= "," . $dnparts[$j];
	
	// Look in DN for attribute 'defaultdomain'.
	$defaultdomain = pql_domain_value($_pql, $DN, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));
	if($defaultdomain) {
	    // A hit. This is the domain under which the user is located.
	    $domain = $DN;
	    break;
	}
    }

    $links = array("user_detail.php?rootdn=$rootdn&domain=$domain&user=".urlencode($USER_DN) => $cn);
    pql_format_tree_span($cn, $links, -1);
} else {
    $SINGLE_USER = 0; session_register("SINGLE_USER");
?>


  <!-- Domain branches -->
<?php
    // We got at least one domain - get it's users
    asort($domains);
    foreach($domains as $key => $domain) {
	// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
	$d = split(',', urldecode($domain)); $d = split('=', $d[0]); $d = $d[1];
	$d = pql_maybe_decode($d);

	$domain = urlencode($domain);

	// Get Root DN
	$rootdn = pql_get_rootdn($domain);

	// Get the subbranches in this domain
	$branches = pql_get_subbranch($_pql->ldap_linkid, $domain);

	if((count($branches) > 1))
	  pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain");

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

		if(!is_array($users)) {
		    // No user available in this domain
		    
		    // Level 2: The users
		    $links = array("user_add.php?rootdn=$rootdn&domain=$domain" => $LANG->_('No users defined'));
		} else {
		    // We have users in this domain

		    // Level 2: The users
		    if(count($branches) > 1) {
			// We're only interested in the 'People', 'Users' etc value,
			// not the complete DN.
			$dnparts = ldap_explode_dn($branches[$i], 0);
			$dnparts = split('=', $dnparts[0]);
			$subbranch = $dnparts[1];
			
			$links = array("user_add.php?rootdn=$rootdn&domain=$domain&subbranch=$subbranch" =>
				       pql_complete_constant($LANG->_('Add %what%'),
							     array('what' => $LANG->_('user'))));
		    } else {
			$links = array("user_add.php?rootdn=$rootdn&domain=$domain" =>
				       pql_complete_constant($LANG->_('Add %what%'),
							     array('what' => $LANG->_('user'))));
		    }
		    
		    // Iterate trough all users in this domain/branch
		    foreach ($users as $dn) {
			unset($cn); unset($sn); unset($gecos);
			
			// From the user DN, get the CN and SN.
			$cn = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_GIVENNAME"));
			$sn = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_SN"));
			if($cn[0] && $sn[0]) {
			    // We have a givenName (first name) and a surName (last name) - combine the two
			    if($sn[0] != '_') {
				$cns[$dn] = $sn[0].", ".$cn[0];
			    } else {
				$cns[$dn] = $cn[0];
			    }
			} else {
			    // Probably don't have a givenName - get the commonName
			    $cn = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_CN"));
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
				$gecos = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_GECOS"));
				if($gecos[0])
				  // We have a gecos - use that as is
				  $cns[$dn] = $gecos[0];
				//			    else
				//			      // No gecos either. Now what!?
			    }
			}
		    }
		    asort($cns);
		    
		    foreach($cns as $dn => $cn) {
			$uid = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_UID"));
			$uid = $uid[0];
			
			$uidnr = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_QMAILUID"));
			$uidnr = $uidnr[0];
			
			if(($uid != 'root') or ($uidnr != '0')) {
			    // Do NOT show root user(s) here! This should (for safty's sake)
			    // not be availible to administrate through phpQLAdmin!
			    $new = array("user_detail.php?rootdn=$rootdn&domain=$domain&subbranch=$subbranch&user=".urlencode($dn) => $cn);
			    
			    // Add the link to the main array
			    $links = $links + $new;
			}
		    }
		}
	    }

	    // Level 1: The domain name with it's users
	    if((count($branches) > 1) and $subbranch) {
		pql_format_tree($subbranch, $url, $links, 1);
	    } else {
		pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
	    }
	} // end foreach ($branches)

	// This an ending for the initial parent (level 0)
	pql_format_tree_end();
    } // end foreach ($domains)
} // end if(is_array($domains))

require("./left-trailer.html");
?>
