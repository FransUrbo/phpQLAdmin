<?php
// This file is a little uggly - it is basically only a _LOADER_
// for Sudoers information (with a little require at the bottom
// to actually SHOW this information as well).

if(@($_REQUEST["action"] != 'add')) {
  // {{{ Setup session etc
  if(!class_exists('pql')) {
	require("./include/pql_session.inc");
	require($_SESSION["path"]."/include/pql_config.inc");
  }
  require($_SESSION["path"]."/include/pql_sudoers.inc");
// }}}
}

// {{{ Retreive all users
if($_REQUEST["domain"]) {
  if(pql_get_define("PQL_CONF_SUBTREE_USERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_USERS") . ",";
  $userdn = $subrdn . $_REQUEST["domain"];
  $filter = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
  $users = $_pql->get_dn($userdn, $filter);

  // Extract 'human readable' name from the user DN's found
  $user_results = pql_left_htmlify_userlist($_REQUEST["rootdn"], $_REQUEST["domain"],
											$userdn, $users, ($links = NULL));
}
// }}}

// {{{ Retreive all computers
if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS")) {
  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ",";
}
$computer_results = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
								  '(&(cn=*)(objectclass=ipHost)(ipHostNumber=*))');
if(is_array($computer_results)) {
  sort($computer_results);
}
// }}}

// {{{ Retreive all sudo roles
if($_REQUEST["domain"]) {
  // {{{ Called from Users->[domain]->Sudo
  if(pql_get_define("PQL_CONF_SUBTREE_SUDOERS"))
	$subrdn =  pql_get_define("PQL_CONF_SUBTREE_SUDOERS") . ",";
  $sudodn = $subrdn . $_REQUEST["domain"];
  $filter = pql_get_define("PQL_ATTR_OBJECTCLASS").'=sudoRole';
  $sudo_results = $_pql->search($sudodn, $filter);
  if(is_array($sudo_results))
	asort($sudo_results);
  // }}}
} elseif(($_REQUEST["host"] == 'Global') and ($_REQUEST["ref"] == 'global')) {
  // {{{ Called from Computers->Global->Sudo Administration

  // Setup a filter containing all physical hosts we know about
  $filter = '(&(objectClass=sudoRole)(cn=*)(|';
  for($i=0; $computer_results[$i]; $i++) {
	// Get the FQDN from the host DN
	$physical = $_pql->get_attribute($computer_results[$i], pql_get_define("PQL_ATTR_CN"));
	$filter .= "(sudoHost=$physical)";
  }
  $filter .= '))';

  foreach($_pql->ldap_basedn as $dn)  {
	$dn  = pql_format_normalize_dn($dn);
	$tmp = $_pql->get_dn($dn, $filter);
	pql_add2array($sudo_results, $tmp);
  }

  if(is_array($sudo_results)) {
	// Get ALL information about these SUDO roles
	sort($sudo_results);
	for($i=0; $sudo_results[$i]; $i++) {
	  $roles[] = $_pql->search($sudo_results[$i], 'objectClass=*', 'BASE');
	}
  }

  $sudo_results = $roles;
  // }}}
} else {
  // {{{ Called from physical host details->Sudo Administration

  // Get the FQDN from the host DN
  $physical = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
  $filter = "(&(objectClass=sudoRole)(cn=*)(sudoHost=$physical))";
  $sudo_results = array();
  foreach($_pql->ldap_basedn as $dn)  {
	$dn  = pql_format_normalize_dn($dn);
	$tmp = $_pql->get_dn($dn, $filter);
	pql_add2array($sudo_results, $tmp);
  }

  if(is_array($sudo_results)) {
	// Get ALL information about these SUDO roles
	sort($sudo_results);
	for($i=0; $sudo_results[$i]; $i++) {
	  $roles[] = $_pql->search($sudo_results[$i], 'objectClass=*', 'BASE');
	}
  }

  $sudo_results = $roles;
  // }}}
}

if(is_array($sudo_results) and !@$sudo_results[0]) {
  // Make sure it's a numbered array...
  $tmp = $sudo_results;
  unset($sudo_results);
  $sudo_results[] = $tmp;
}
// }}}

if(@($_REQUEST["action"] != 'add')) {
  // Load the sudo details page
  require($_SESSION["path"]."/tables/domain_details-sudoers.inc");
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
