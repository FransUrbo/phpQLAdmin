<?php
// shows details of a domain
// $Id: domain_detail.php,v 2.108 2006-12-26 19:06:56 aaron Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

include($_SESSION["path"]."/header.html");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
// }}}

// {{{ Include control api if control is used
if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    include($_SESSION["path"]."/include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}
// }}}

// {{{ Print status message, if one is available
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
    echo "Domain &quot;" . $_REQUEST["domain"] . "&quot; does not exists<br><br>";
	echo "Is this perhaps a Top Level DN (namingContexts), and you haven't configured ";
	echo "how to reference domains/branches in this database!?<br><br>";
	echo "Please go to <a href=\"config_detail.php\">Show configuration</a> and double check.<br>";
	echo "Look at the config option 'Reference domains with'.";
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

if(empty($_REQUEST["view"]))
  $_REQUEST["view"] = 'default';

// {{{ Get all needed default values for this domain
// Some of these (everything after the 'o' attribute)
// uses 'objectClass: dcOrganizationNameForm' -> http://rfc-2377.rfcindex.net/
$attribs = array("autocreatemailaddress"	=> pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS"),
				 "autocreateusername"		=> pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME"),
				 "autocreatepassword"		=> pql_get_define("PQL_ATTR_AUTOCREATE_PASSWORD"),
				 "basehomedir"				=> pql_get_define("PQL_ATTR_BASEHOMEDIR"),
				 "basemaildir"				=> pql_get_define("PQL_ATTR_BASEMAILDIR"),
				 "basequota"				=> pql_get_define("PQL_ATTR_BASEQUOTA"),
				 "defaultdomain"			=> pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
				 "defaultpasswordscheme"	=> pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"),
				 "defaultaccounttype"		=> pql_get_define("PQL_ATTR_DEFAULT_ACCOUNTTYPE"),
				 "facsimiletelephonenumber"	=> pql_get_define("PQL_ATTR_FACSIMILETELEPHONENUMBER"),
				 "l"						=> pql_get_define("PQL_ATTR_L"),
				 "maximumdomainusers"		=> pql_get_define("PQL_ATTR_MAXIMUM_DOMAIN_USERS"),
				 "maximummailinglists"		=> pql_get_define("PQL_ATTR_MAXIMUM_MAILING_LISTS"),
				 "o"						=> pql_get_define("PQL_ATTR_O"),
				 "postaladdress"			=> pql_get_define("PQL_ATTR_POSTALADDRESS"),
				 "streetaddress"			=> pql_get_define("PQL_ATTR_STREETADDRESS"),
				 "registeredaddress"		=> pql_get_define("PQL_ATTR_REGISTEREDADDRESS"),
				 "postalcode"				=> pql_get_define("PQL_ATTR_POSTALCODE"),
				 "postofficebox"			=> pql_get_define("PQL_ATTR_POSTOFFICEBOX"),
				 "st"						=> pql_get_define("PQL_ATTR_ST"),
				 "street"					=> pql_get_define("PQL_ATTR_STREET"),
				 "telephonenumber"			=> pql_get_define("PQL_ATTR_TELEPHONENUMBER"),
				 "mobile"					=> pql_get_define("PQL_ATTR_MOBILE"),
				 "usernameprefix"			=> pql_get_define("PQL_ATTR_USERNAME_PREFIX"),
				 "usernameprefixlength"		=> pql_get_define("PQL_ATTR_USERNAME_PREFIX_LENGTH"),
				 "vatnumber"				=> pql_get_define("PQL_ATTR_VAT_NUMBER"),
				 "ezmlmvirtualuser"			=> pql_get_define("PQL_ATTR_EZMLM_USER"),
				 "info"						=> pql_get_define("PQL_ATTR_INFO"),
				 "lockusername"				=> pql_get_define("PQL_ATTR_LOCK_USERNAME"),
				 "lockemailaddress"			=> pql_get_define("PQL_ATTR_LOCK_EMAILADDRESS"),
				 "lockdomainaddress"		=> pql_get_define("PQL_ATTR_LOCK_DOMAINADDRESS"),
				 "lockpassword"				=> pql_get_define("PQL_ATTR_LOCK_PASSWORD"),
				 "lockaccounttype"			=> pql_get_define("PQL_ATTR_LOCK_ACCOUNTTYPE"));
foreach($attribs as $key => $attrib) {
	// Get default value
	$value = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib);
	if(is_array($value) and ($key != 'simscanattachmentsuffix'))
	  $value = $value[0];
	$$key = $value;

	if($attrib == pql_get_define("PQL_ATTR_INFO")) {
		// Special circumstance - multiple lines...
		$$key = eregi_replace("\n", "<br>", $$key);
	}

	// Setup edit links. If it's a dcOrganizationNameForm attribute, then
	// we add a delete link as well.
	$link = $key . "_link";
	if(($key != 'defaultdomain') and ($key != 'basehomedir') and ($key != 'basemaildir')) {
	  if(!$value and ($key == 'maximumdomainusers') or ($key == 'maximummailinglists'))
		// No value, no toggle
		$value = 0;
	  elseif(($key == 'autocreateusername') or ($key == 'autocreatemailaddress') or
			 ($key == 'autocreatepassword') or ($key == 'usehostacl') or
			 ($key == 'usesudo') or ($key == 'lockusername') or ($key == 'lockemailaddress') or
			 ($key == 'lockdomainaddress') or ($key == 'lockpassword') or
			 ($key == 'lockaccounttype'))
	  {
		// A toggle value
		if(!$value)
		  // No value
		  $value = 0;
		else {
		  // Got a value
		  $$key = pql_format_bool($value);
		}
	  }
	  
	  // A dcOrganizationNameForm attribute
	  $alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
									array('attribute' => $attrib, 'domainname' => $domainname));
	  $alt2 = pql_complete_constant($LANG->_('Delete attribute %attribute% for %domainname%'),
									array('attribute' => $attrib, 'domainname' => $domainname));
	  
	  $$link = "<a href=\"domain_edit_attributes.php?type=modify&attrib=$attrib&rootdn="
		. $url["rootdn"] . "&domain=" . $url["domain"] . "&$attrib=". urlencode($value)
		. "&view=" . $_REQUEST["view"] . "\"><img src=\"images/edit.png\" width=\"12\" height=\"12\""
		. "border=\"0\" alt=\"$alt1\"></a>&nbsp;<a href=\"domain_edit_attributes.php?type=delete&"
		. "submit=2&attrib=$attrib&rootdn=" . $url["rootdn"] . "&domain=" . $url["domain"]
		. "&$attrib=". urlencode($value) . "&view=" . $_REQUEST["view"] . "\"><img src=\"images/del.png\""
		. "width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt2."\"></a>";
	} else {
	  $alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
									array('attribute' => $attrib, 'domainname' => $domainname));
	  
	  // A phpQLAdminBranch attribute
	  $$link = "<a href=\"domain_edit_attributes.php?attrib=$attrib&rootdn="
		. $url["rootdn"] . "&domain=" . $url["domain"] . "&$attrib=$value&view="
		. $_REQUEST["view"] . "\"><img src=\"images/edit.png\" width=\"12\" height=\"12\""
		. "border=\"0\" alt=\"".$alt1."\"></a>";
	}
}
if($_REQUEST["view"] == 'access') {
  // Get domain administrators
  $domain_admins      = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
  if($domain_admins and !is_array($domain_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$domain_admins = array($domain_admins);
  }
  
  // Get mailinglist administrators
  $mailinglist_admins = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"],
										  pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"));
  if($mailinglist_admins and !is_array($mailinglist_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$mailinglist_admins = array($mailinglist_admins);
  }

  // Get webserver administrators
  $websrv_admins = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"],
									 pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"));
  if($websrv_admins and !is_array($websrv_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$websrv_admins = array($websrv_admins);
  }

  // Get DNS administrators
  $bind9_admins = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"],
									pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"));
  if($bind9_admins and !is_array($bind9_admins)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$bind9_admins = array($bind9_admins);
  }

}

$seealso            = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_SEEALSO"));
if($seealso and !is_array($seealso)) {
	// It's defined, but it's not an array. Convert it so we don't get into trouble below.
	$seealso = array($seealso);
}

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

// {{{ Setup the buttons
$buttons = array('default'	=> 'Branch Defaults');

if($_SESSION["ADVANCED_MODE"]) {
  $new = array('access' => 'Branch access',
			   'owner'	=> 'Branch Owner Details');
  $buttons = $buttons + $new;
}

$new = array('users'	=> 'Registred Users',
			 'chval'	=> 'Change values of all users');
$buttons = $buttons + $new;

if($_SESSION["ADVANCED_MODE"]) {
	if(pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"), $_SESSION["USER_DN"]) or
	   $_SESSION["ALLOW_BRANCH_CREATE"])
	{
		$new = array('dnsinfo'	=> 'MX Information');
		$buttons = $buttons + $new;
	}

	if($_SESSION["ACI_SUPPORT_ENABLED"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
		// ACI enabled and this is a 'super-admin'.
		$new = array('aci'	=> 'Access Control Information');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_CONF_CONTROL_USE") and $_SESSION["ALLOW_CONTROL_CREATE"]) {
		$new = array('options' => 'Mailserver Administration');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_CONF_BIND9_USE") and
	   (pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"), $_SESSION["USER_DN"]) or
		pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["rootdn"], pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"])))
	{
		// * DNS administration is enabled
		// * User is either webSrvAdministrator for domain OR:
		// * User is super admin
		$new = array('dnszone'	=> 'DNS Zone');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_CONF_WEBSRV_USE") and
	   (pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"), $_SESSION["USER_DN"]) or
		pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["rootdn"], pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"])))
	{
		// * Webserver administration is enabled
		// * User is either webSrvAdministrator for domain OR:
		// * User is super admin
		$new = array('websrv'	=> 'Webserver Administration');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_CONF_HOSTACL_USE")) {
	  $new = array('hostacl'	=> 'Host Control');
	  $buttons = $buttons + $new;
	}
        if(pql_get_define("PQL_CONF_SUDO_USE")) {
          $new = array('sudo'   => 'Sudo');
          $buttons = $buttons + $new;
        }
}

if(pql_get_define("PQL_CONF_SIMSCAN_USE")) {
  $new = array('simscan' => 'SimScan config');
  $buttons = $buttons + $new;
}

if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  $new = array('action' => 'Actions');
  $buttons = $buttons + $new;
}

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
	elseif($_REQUEST["view"] == 'access')
	  include("./tables/domain_details-access.inc");
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
