<?php
// shows details of a domain
// $Id: domain_detail.php,v 2.70.2.3 2003-12-17 16:11:37 dlw Exp $
//
session_start();
require("./include/pql_config.inc");

if(pql_get_define("PQL_GLOB_CONTROL_USE")) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}

include("./header.html");

// print status message, if one is available
if(isset($_REQUEST["msg"])) {
    pql_format_status_msg($_REQUEST["msg"]);
}

// reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// Make sure we can have a ' in branch (also affects the user DN).
$_REQUEST["user"]   = eregi_replace("\\\'", "'", $_REQUEST["user"]);
$_REQUEST["domain"] = eregi_replace("\\\'", "'", $_REQUEST["domain"]);

// check if domain exist
if(!pql_domain_exist($_pql, $_REQUEST["domain"])) {
    echo "Domain &quot;" . $_REQUEST["domain"] . "&quot; does not exists<br><br>";
	echo "Is this perhaps a Top Level DN (namingContexts), and you haven't configured ";
	echo "how to reference domains/branches in this database!?<br><br>";
	echo "Please go to <a href=\"config_detail.php\">Show configuration</a> and double check.<br>";
	echo "Look at the config option 'Reference domains with'.";
    exit();
}

// Look for a URL encoded '=' (%3D). If one isn't found, encode the DN
// These variables ISN'T encoded "the first time", but they are after
// a successfull/failed modification, so we don't want to encode them twice!
if(! ereg("%3D", $_REQUEST["rootdn"])) {
	// URL encode namingContexts
	$_REQUEST["rootdn"] = urlencode($_REQUEST["rootdn"]);
}
if(! ereg("%3D", $_REQUEST["domain"])) {
	// .. and/or domain DN
	$_REQUEST["domain"] = urlencode($_REQUEST["domain"]);
}

// Get the organization name, or show 'Not set' with an URL to set it
$domainname = pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_O"));
if(!$domainname) {
  // TODO: Resonable default!
  $domainname = '';				// DLW: Just to shut off some warnings.
}

// Get some default values for this domain
// Some of these (everything after the 'o' attribute)
// uses 'objectClass: dcOrganizationNameForm' -> http://rfc-2377.rfcindex.net/
$attribs = array(pql_get_define("PQL_GLOB_ATTR_AUTOCREATEMAILADDRESS"),
				 pql_get_define("PQL_GLOB_ATTR_AUTOCREATEUSERNAME"),
				 pql_get_define("PQL_GLOB_ATTR_BASEHOMEDIR"),
				 pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR"),
				 pql_get_define("PQL_GLOB_ATTR_BASEQUOTA"),
				 pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"),
				 pql_get_define("PQL_GLOB_ATTR_DEFAULTPASSWORDSCHEME"),
				 pql_get_define("PQL_GLOB_ATTR_FACSIMILETELEPHONENUMBER"),
				 pql_get_define("PQL_GLOB_ATTR_L"),
				 pql_get_define("PQL_GLOB_ATTR_MAXIMUMDOMAINUSERS"),
				 pql_get_define("PQL_GLOB_ATTR_MAXIMUMMAILINGLISTS"),
				 pql_get_define("PQL_GLOB_ATTR_O"),
				 pql_get_define("PQL_GLOB_ATTR_POSTALADDRESS"),
				 pql_get_define("PQL_GLOB_ATTR_POSTALCODE"),
				 pql_get_define("PQL_GLOB_ATTR_POSTOFFICEBOX"),
				 pql_get_define("PQL_GLOB_ATTR_ST"),
				 pql_get_define("PQL_GLOB_ATTR_STREET"),
				 pql_get_define("PQL_GLOB_ATTR_TELEPHONENUMBER"),
				 pql_get_define("PQL_GLOB_ATTR_USERNAMEPREFIX"));
foreach($attribs as $attrib) {
	// Get default value
	$value = pql_domain_value($_pql, $_REQUEST["domain"], $attrib);
	$$attrib = $value;

	// Setup edit links. If it's a dcOrganizationNameForm attribute, then
	// we add a delete link as well.
	$link = $attrib . "_link";
	if(($attrib != 'defaultdomain') and ($attrib != 'basehomedir') and ($attrib != 'basemaildir')) {
		if(!$value and (($attrib == 'maximumdomainusers') or ($attrib == 'maximummailinglists') or
						($attrib == 'autocreateusername') or ($attrib == 'autocreatemailaddress')))
		  $value = 0;
		else
		  // We have a value
		  if(($attrib == 'autocreateusername') or ($attrib == 'autocreatemailaddress')) {
			  // It's a toggle. Convert the boolean value to an integer
			  if($value == 'FALSE') {
				  // It's false, set it to zero
				  $value = 0;
				  $$attrib = 0;
			  } else {
				  $value = 1;
				  $$attrib = 1;
			  }
		  }

		// A dcOrganizationNameForm attribute
		$alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
									  array('attribute' => $attrib, 'domainname' => $domainname));
		$alt2 = pql_complete_constant($LANG->_('Delete attribute %attribute% for %domainname%'),
									  array('attribute' => $attrib, 'domainname' => $domainname));

		$$link = "<a href=\"domain_edit_attributes.php?type=modify&attrib=$attrib&rootdn="
		  . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"] . "&$attrib=". urlencode($value)
		  . "\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\""
		  . $alt1 . "\"></a>&nbsp;<a href=\"domain_edit_attributes.php?type=delete&submit=2&attrib=$attrib&rootdn="
		  . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"] . "&$attrib=". urlencode($value)
		  . "\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt2."\"></a>";
	} else {
		$alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
									  array('attribute' => $attrib, 'domainname' => $domainname));

		// A phpQLAdminBranch attribute
		$$link = "<a href=\"domain_edit_attributes.php?attrib=$attrib&rootdn="
		  . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"]
		  . "&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt1."\"></a>";
	}
}
$domain_admins		= pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"));
$mailinglist_admins	= pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR"));
$seealso   			= pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_SEEALSO"));
$basequota			= pql_ldap_mailquota(pql_parse_quota($basequota));

$additionaldomainname = pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"));

// Setup the buttons
$buttons = array('default'	=> 'Default Branch Values',
				 'users'	=> 'Registred users',
				 'chval'	=> 'Change values of all users');

if($_SESSION["ADVANCED_MODE"]) {
	$new = array('owner'	=> 'Branch Owner',
				 'dnsinfo'	=> 'MX Information',
				 'aci'		=> 'Access Control Information');
	$buttons = $buttons + $new;

	if(pql_get_define("PQL_GLOB_CONTROL_USE")) {
		$new = array('options' => 'QmailLDAP/Controls Options');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_GLOB_BIND9_USE")) {
		$new = array('dnszone'	=> 'DNS Zone');
		$buttons = $buttons + $new;
	}

	if(pql_get_define("PQL_GLOB_WEBSRV_USE")) {
		$new = array('websrv'	=> 'Webserver Administration');
		$buttons = $buttons + $new;
	}
}

$new = array('action' => 'Actions');
$buttons = $buttons + $new;
?>
  <span class="title1"><?=$LANG->_('Organization name')?>: <?=urldecode($domainname)?></span>

  <br><br>
<?php
pql_generate_button($buttons, "domain=" . $_REQUEST["domain"]); echo "  <br>\n";

if(empty($_REQUEST["view"]))
  $_REQUEST["view"] = 'default';

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
	$users = pql_user_get($_pql->ldap_linkid, $_REQUEST["domain"]);
	include("./tables/domain_details-users.inc");
} elseif($_REQUEST["view"] == 'action') {
	include("./tables/domain_details-action.inc");
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
