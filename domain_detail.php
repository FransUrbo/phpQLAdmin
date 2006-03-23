<?php
// shows details of a domain
// $Id: domain_detail.php,v 2.102.2.2 2006-03-23 09:29:03 turbo Exp $
//
// {{{ Setup session etc
require("./libs/pql_session.inc");
require($_SESSION["path"]."/libs/pql_config.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
// }}}

// {{{ Include control api if control is used
if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    include($_SESSION["path"]."/libs/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}
// }}}

// {{{ Print status message, if one is available
include($_SESSION["path"]."/header.html");
if(isset($_REQUEST["msg"])) {
    pql_format_status_msg($_REQUEST["msg"]);
}
// }}}

// {{{ Reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}
// }}}

// {{{ Check if domain exist
if(!pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], '(objectclass=*)', 'BASE')) {
  $smarty->assign('domain', $_REQUEST["domain"]);
  $smarty->display($_SESSION["path"]."/templates/".pql_get_define("PQL_CONF_GUI_TEMPLATE")."/dontexists_domain.tpl");
  exit();
}
// }}}

// {{{ Get the organization name, or show 'Not set' with an URL to set it
$domainname = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
if(!$domainname) {
  // TODO: Resonable default!
  $domainname = '';				// DLW: Just to shut off some warnings.
}
// }}}

if(empty($_REQUEST["view"])) {
  $_REQUEST["view"] = 'default';
}

// {{{ Get all needed default values for this domain
// Some of these (everything after the 'o' attribute)
// uses 'objectClass: dcOrganizationNameForm' -> http://rfc-2377.rfcindex.net/

// {{{ Setup attributes to retreive
// Array Format:	key = array(attribute, text[, [[bool][multi][integer]]]
if(($_REQUEST["view"] == 'default') and $_SESSION["ADVANCED_MODE"]) {
  $smarty->assign('title', $LANG->_('Default domain values'));
  $show[] = array('title' => $LANG->_('Branch DN'), 'value' => $_REQUEST["domain"]);
  
  $attribs = array("defaultdomain"				=> array(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
														 $LANG->_('Default domain name')));
  
  if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	$new = array(  "basehomedir"				=> array(pql_get_define("PQL_ATTR_BASEHOMEDIR"),
														 $LANG->_('Base home directory for users')),
				   "basemaildir"				=> array(pql_get_define("PQL_ATTR_BASEMAILDIR"),
														 $LANG->_('Base mail directory for users')),
				   "basequota"					=> array(pql_get_define("PQL_ATTR_BASEQUOTA"),
														 $LANG->_('Default quota'),
														 'integer'),
				   "maximumdomainusers"			=> array(pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"),
														 $LANG->_('Maximum allowed users in branch'),
														 'integer'),
				   "maximummailinglists"		=> array(pql_get_define("PQL_ATTR_MAXIMUM_MAILING_LISTS"),
														 $LANG->_('Maximum allowed mailinglists in branch'),
														 'integer'),
				   "defaultpasswordscheme"		=> array(pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"),
														 $LANG->_('Default password scheme')),
				   "autocreateusername"			=> array(pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
														 pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('username'))),
														 'bool'),
				   "usernameprefix"				=> array(pql_get_define("PQL_ATTR_USERNAME_PREFIX"),
														 $LANG->_('Username prefix')),
				   "usernameprefixlength"		=> array(pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"),
														 $LANG->_('Length of user suffix'),
														 'integer'),
				   "autocreatemailaddress"		=> array(pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
														 pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('email address'))),
														 'bool'),
				   "autocreatepassword"			=> array(pql_get_define("PQL_ATTR_AUTOCREATE_PASSWORD"),
														 pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('password'))),
														 'bool'));
	$attribs = $attribs + $new;
	
	if(pql_get_define("PQL_CONF_EZMLM_USE")) {
	  $new = array("ezmlmvirtualuser"			=> array(pql_get_define("PQL_ATTR_EZMLM_USER"),
														 $LANG->_('EZMLM Virtual User')));
	  $attribs = $attribs + $new;
	}
	
	$new = array(  "usehostacl"					=> array(pql_get_define("PQL_ATTR_HOSTACL_USE"),
														 $LANG->_('Use Host ACL'),
														 'bool'),
				   "usesudo"					=> array(pql_get_define("PQL_ATTR_SUDO_USE"),
														 $LANG->_('Use Sudo'),
														 'bool'));
	$attribs = $attribs + $new;
  }
} elseif(($_REQUEST["view"] == 'default') or ($_REQUEST["view"] == 'owner')) {
  $smarty->assign('title', $LANG->_('Branch owner'));
  $attribs = array("o"							=> array(pql_get_define("PQL_ATTR_O"),
														 $LANG->_('Organization name')),
				   "vatnumber"					=> array(pql_get_define("PQL_ATTR_VAT_NUMBER"),
														 $LANG->_('VAT number')),
				   "postalcode"					=> array(pql_get_define("PQL_ATTR_POSTALCODE"),
														 $LANG->_('Postal code')),
				   "postofficebox"				=> array(pql_get_define("PQL_ATTR_POSTOFFICEBOX"),
														 $LANG->_('Post box')),
				   "postaladdress"				=> array(pql_get_define("PQL_ATTR_POSTALADDRESS"),
														 $LANG->_('Postal address')),
				   "streetaddress"				=> array(pql_get_define("PQL_ATTR_STREETADDRESS"),
														 $LANG->_('Street address')),
				   "street"						=> array(pql_get_define("PQL_ATTR_STREET"),
														 $LANG->_('Street address')),
				   "l"							=> array(pql_get_define("PQL_ATTR_L"),
														 $LANG->_('City')),
				   "st"							=> array(pql_get_define("PQL_ATTR_ST"),
														 $LANG->_('State')),
				   "telephonenumber"			=> array(pql_get_define("PQL_ATTR_TELEPHONENUMBER"),
															 $LANG->_('Telephone number')),
				   "facsimiletelephonenumber"	=> array(pql_get_define("PQL_ATTR_FACSIMILETELEPHONENUMBER"),
														 $LANG->_('Fax number')),
				   "mobile"						=> array(pql_get_define("PQL_ATTR_MOBILE"),
														 $LANG->_('Telephone number')),
				   "info"						=> array(pql_get_define("PQL_ATTR_INFO"),
														 $LANG->_(''),
														 'multi'));
} else {
  $attribs = array();
}
// }}}

// {{{ Go through each attribute, retreive it's value(s)
foreach($attribs as $key => $attrib) {
  // $attrib is an array, with the following dimensions:
  //	0: attribute
  //	1: text
  //	2: type			(optional!)

  // Get default value
  $value = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib[0]);
  if(is_array($value) and ($key != 'simscanattachmentsuffix'))
	$value = $value[0];
  $$key = $value;
  
  if(@$attrib[2] == 'multi') {
	// Special circumstances - multiple lines...
	$$key = eregi_replace("\n", "<br>", $$key);
  }
  
  if(($key != 'defaultdomain') and ($key != 'basehomedir') and ($key != 'basemaildir')) {
	if(!$value) {
	  if(@$attrib[2] == 'integer') {
		// No value, no toggle, but an integer.
		$value = 0;
		$$key = "<i>".$LANG->_('Unlimited')."</i>";
	  } else
		$$key = "<i>".$LANG->_('Not set')."</i>";
	} elseif(@$attrib[2] == 'bool') {
	  // A toggle value
	  if(!$value) {
		// No value
		$value = 0;
		$$key = pql_format_bool(0);
	  } else
		// Got a value
		$$key = pql_format_bool($value);
	}
	
	// A dcOrganizationNameForm attribute
	$alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
								  array('attribute' => $attrib[0], 'domainname' => $domainname));
	$alt2 = pql_complete_constant($LANG->_('Delete attribute %attribute% for %domainname%'),
								  array('attribute' => $attrib[0], 'domainname' => $domainname));
	
	if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	  // Setup edit links. If it's a dcOrganizationNameForm attribute, then
	  // we add a delete link as well.
	  $link  = $key . "_link";
	  $$link = "<a href=\"domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]
		. "&attrib=".$attrib[0]."&".$attrib[0]."=".urlencode($value)."&type=modify"."&view=".$_REQUEST["view"]
		. "\"><img src=\"images/edit.png\" width=\"12\" height=\"12\""."border=\"0\" alt=\"$alt1\">"
		. "</a>&nbsp;<a href=\"domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]
		. "type=delete&submit=2&attrib=".$attrib[0]."&".$attrib[0]."=".urlencode($value)."&view=".$_REQUEST["view"]
		. "\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt2."\"></a>";
	}
  } else {
	// A phpQLAdminBranch attribute
	$alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
								  array('attribute' => $attrib[0], 'domainname' => $domainname));
	
	if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	  // Setup edit links. If it's a dcOrganizationNameForm attribute, then
	  // we add a delete link as well.
	  $link  = $key . "_link";
	  $$link = "<a href=\"domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]
		. "attrib=".$attrib[0]."&".$attrib[0]."=$value&view=".$_REQUEST["view"]."\"><img src=\"images/edit.png\" "
		. "width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt1."\"></a>";
	}
  }

  // Put together the show array for Smarty template generator (below)
  $show[] = array('title' => $attrib[1],
				  'value' => ((@$attrib[2] == 'bool') ? ($$key ? $LANG->_('Yes') : $LANG->_('No')) : $$key),
				  'link'  => $$link);
}
// }}}

// {{{ Get additional multi valued attributes
$domain_admins      = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
$mailinglist_admins = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"));
$seealso            = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_SEEALSO"));

// The quota value retreived from the object is a one liner.
// Split it up into it's parts (SIZE and AMOUNT) and
// create an array that pql_ldap_mailquota() understands.
if($basequota) {
  $temp		= split(',', $basequota);
  $temp[1]	= eregi_replace("C$", "", $temp[1]);
  $temp[0]	= eregi_replace("S$", "", $temp[0]);

  $quota			 = array();
  $quota["maxmails"] = $temp[1];
  $quota["maxsize"]	 = $temp[0];

  $basequota		 = pql_ldap_mailquota($quota);
}

$additionaldomainname = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
// }}}
// }}}

// {{{ Setup the buttons
$buttons = array('default'	=> 'Branch Defaults');

if($_SESSION["ADVANCED_MODE"]) {
	$new = array('owner'	=> 'Branch Owner Details');
	$buttons = $buttons + $new;
}

$new = array('users'	=> 'Registred Users',
			 'chval'	=> 'Change values of all users');
$buttons = $buttons + $new;

if($_SESSION["ADVANCED_MODE"]) {
	if($_SESSION["ACI_SUPPORT_ENABLED"]) {
		$new = array('dnsinfo'	=> 'MX Information');
		$buttons = $buttons + $new;

		if($_SESSION["ALLOW_BRANCH_CREATE"]) {
		  // This is a 'super-admin'.
		  $new = array('aci'	=> 'Access Control Information');
		  $buttons = $buttons + $new;
		}
	}

	if(pql_get_define("PQL_CONF_CONTROL_USE") and $_SESSION["ALLOW_CONTROL_CREATE"]) {
		$new = array('options' => 'QmailLDAP/Controls Options');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_CONF_BIND9_USE")) {
		$new = array('dnszone'	=> 'DNS Zone');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
		$new = array('websrv'	=> 'Webserver Administration');
		$buttons = $buttons + $new;
	}
}

if(pql_get_define("PQL_CONF_SIMSCAN_USE")) {
  $new = array('simscan' => 'SimScan config');
  $buttons = $buttons + $new;
}

$key = pql_get_define("PQL_ATTR_HOSTACL_USE");
if($$key) {
  $new = array('hostacl' => 'Host control');
  $buttons = $buttons + $new;
}

$key = pql_get_define("PQL_ATTR_SUDO_USE");
if($$key) {
  $new = array('sudo' => 'Sudoers access');
  $buttons = $buttons + $new;
}

$new = array('action' => 'Actions');
$buttons = $buttons + $new;

if($domainname) {
?>
  <span class="title1"><?=$LANG->_('Organization name')?>: <?=pql_maybe_idna_decode(urldecode($domainname))?></span>
<?php
} elseif($o) {
?>
  <span class="title1"><?=$LANG->_('Organization name')?>: <?=urldecode($o)?></span>
<?php
}
?>

  <br><br>
<?php
pql_generate_button($buttons, "domain=" . $url["domain"]); echo "  <br>\n";
// }}}

// {{{ Load the requested domain details page
if(@is_array($show)) {
  $smarty->assign('show', $show);
  $smarty->display($_SESSION["path"]."/templates/".pql_get_define("PQL_CONF_GUI_TEMPLATE")."/domain_details.tpl");
}

// Load additional stuff that can't be fixed with Smarty
if($_REQUEST["view"] == 'default') {
  if($_SESSION["ADVANCED_MODE"]) {
	include("./tables/domain_details-default.inc");
  } else {
	include("./tables/domain_details-owner.inc");
  }
}

if($_REQUEST["view"] == 'owner') {
  include("./tables/domain_details-owner.inc");
} elseif($_REQUEST["view"] == 'chval') {
	include("./tables/domain_details-users_chval.inc");
} elseif($_REQUEST["view"] == 'users') {
	$filter = "(&(" . pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*)(mail=*))";
	$users = pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], $filter);
	include("./tables/domain_details-users.inc");
} elseif($_REQUEST["view"] == 'action') {
	include("./tables/domain_details-action.inc");
} elseif($_REQUEST["view"] == 'simscan') {
	include("./tables/domain_details-simscan.inc");
} elseif($_SESSION["ADVANCED_MODE"] == 1) {
	if($_REQUEST["view"] == 'dnsinfo')
	  include("./tables/domain_details-dnsinfo.inc");
	elseif($_REQUEST["view"] == 'dnszone')
	  include("./tables/domain_details-dnszone.inc");
	elseif($_REQUEST["view"] == 'options')
	  include("./tables/domain_details-options.inc");
	elseif($_REQUEST["view"] == 'aci')
	  include("./tables/domain_details-aci.inc");
	elseif($_REQUEST["view"] == 'websrv')
	  include("./tables/domain_details-websrv.inc");
	elseif($_REQUEST["view"] == 'hostacl')
	  include($_SESSION["path"]."/host_modify.php");
	elseif($_REQUEST["view"] == 'sudo')
	  include($_SESSION["path"]."/sudo_modify.php");
}
// }}}
?>
</body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
