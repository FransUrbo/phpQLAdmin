<?php
// navigation bar
// $Id: left.php,v 2.130 2007-07-13 11:12:01 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/left-head.html");
// }}}

// {{{ Server control header
$div_counter = 0;
?>
    <!-- Server Control -->
    <div id="el<?=$div_counter?>Parent" class="parent">
      <a class="item" href="control.php">
        <font color="black" class="heada"><b>Users</b></font>
      </a>
    </div>

<?php
  $div_counter++;

if($_SESSION["ALLOW_BRANCH_CREATE"] and $_SESSION["ADVANCED_MODE"]) {
?>

  <!-- Add domain branch link -->
  <div id="el5Parent" class="parent">
    <a href="domain_add_form.php?rootdn=<?=$_SESSION["BASE_DN"][0]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain branch'))); ?></a><br>
  </div>

<?php
}
// }}}

// {{{ ---------------- GET THE DOMAINS/BRANCHES
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  // This is a 'super-admin'. Should be able to read EVERYTHING!
  $domains = pql_get_domains();
} else {
  // {{{ Get ALL domains we have access to.
  //     I.e., all DN's with 'administrator: USER_DN'
  foreach($_SESSION["BASE_DN"] as $dn)  {
	$dom = $_pql->get_dn($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR")."=".$_SESSION["USER_DN"]);
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
  
  $cn = $_pql->get_attribute($_SESSION["USER_DN"], pql_get_define("PQL_ATTR_CN"));
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
	$defaultdomain = $_pql->get_attribute($DN, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
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
	echo "<br>Domains:";
	printr($domains);
  }

  foreach($domains as $key => $domain) {
	// {{{ Get domain name part from the DN
	// Three steps because pql_get_domains() and pql_get_dn()
	// returns normalized DN's which isn't as pretty.
	// 1. Get each part of the DN
	$dnparts = split(',', $domain);
	
	// 2. Extract the attribute from the first RDN
	$tmp = split('=', $dnparts[0]);
	$attrib = $tmp[0];
	
	// 3. Get the attribute value from the object
	$d = $_pql->get_attribute($domain, $attrib);
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

	// Create a user search filter
	if(pql_get_define("PQL_CONF_USER_FILTER", $rootdn))
	  $filter = pql_get_define("PQL_CONF_USER_FILTER", $rootdn);
	else
	  $filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)."=*";
	
	// Get the subbranches in this domain
	$branches = pql_unit_get($domain);
	if((count($branches) > 1)) {
	  // More than one subbranch
	  $links = array(pql_complete_constant($LANG->_('Add %what%'),
										   array('what' => $LANG->_('sub unit')))
					 => "unit_add.php?rootdn=$rootdn&domain=$domain",
					 
					 pql_complete_constant($LANG->_('Add %what%'),
										   array('what' => $LANG->_('user')))
					 => "user_add.php?rootdn=$rootdn&domain=$domain");
	  
	  // Just incase there's user(s) at the base of the domain branch...
	  $users = $_pql->get_dn($domain, $filter, 'ONELEVEL');
	  if(is_array($users))
		// We have users in this domain
		pql_left_htmlify_userlist($rootdn, $domain, $subbranch, $users, $links);
		
	  pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
	} elseif((count($branches) <= 1))
	  // This branch don't have any sub units (flat structure)
	  // -> make sure we still jump into the for loop!
	  $branches[0] = $domain;

	// Just to make sure that the branches variable is really count()'able...
	if(!@$branches[0])
	  $branches = array();

	if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS")) {
	  echo "<br>Branches ($domain): ";
	  printr($branches);
	}

	for($i=0; $i < count($branches); $i++) {
	  $subbranch = 0;

	  // Zero out the variables, othervise we won't get users in
	  // specified domain, but also in the PREVIOUS domain shown!
	  unset($users); unset($links); unset($cns);
	  
	  // Get all users (their DN) in this domain (sub)branch
	  if(file_exists($_SESSION["path"]."/.DEBUG_DOMAINS"))
		echo "Retreiving users in branch '".$branches[$i]."'<br>";
	  $users = $_pql->get_dn($branches[$i], $filter);
	  
	  // Level 2: The users
	  if(count($branches) > 1) {
		// We're only interested in the 'People', 'Users' etc value,
		// not the complete DN.
		// Three steps because pql_get_domains() and pql_get_dn()
		// returns normalized DN's which isn't as pretty.
		$dnparts		= ldap_explode_dn($branches[$i], 0);
		$tmp			= split('=', $dnparts[0]);
		$attrib		= $tmp[0];
		$subbranch	= $_pql->get_attribute($branches[$i], $attrib);
		
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
		pql_left_htmlify_userlist($rootdn, $domain, $subbranch, $users, $links);
	
	  // Level 1: The domain name with it's users
	  if((count($branches) > 1) and $subbranch)
		pql_format_tree(pql_maybe_decode(urldecode($subbranch)), $url, $links, 1);
	  else
		pql_format_tree($d, "domain_detail.php?rootdn=$rootdn&domain=$domain", $links, 0);
	} // end foreach ($branches)
	
	// This an ending for the domain tree
	pql_format_tree_end();
	unset($links);
  } // end foreach ($domains)
  
  // }}}
} // end if(is_array($domains))
// }}}

if(file_exists($_SESSION["path"]."/.DEBUG_PROFILING")) {
  $now = pql_format_return_unixtime();
  echo "Now: <b>$now</b><br>";

  $time = $now - $start;
  echo "Time: '$time second";
  if($time > 1)
	echo "s";
  echo "'<br>";
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
