<?php
// shows details of a domain
// domain_detail.php,v 2.2 2002/12/17 06:28:26 turbo Exp
//
session_start();
require("./include/pql_config.inc");

if(pql_get_define("PQL_GLOB_CONTROL_USE")) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
}

include("./header.html");

// print status message, if one is available
if(isset($msg)) {
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// check if domain exist
if(!pql_domain_exist($_pql, $domain)) {
    echo "Domain &quot;$domain&quot; does not exists<br><br>";
	echo "Is this perhaps a Top Level DN (namingContexts), and you haven't configured ";
	echo "how to reference domains/branches in this database!?<br><br>";
	echo "Please go to <a href=\"config_detail.php\">Show configuration</a> and double check.<br>";
	echo "Look at the config option 'Reference domains with'.";
    exit();
}

// Look for a URL encoded '=' (%3D). If one isn't found, encode the DN
// These variables ISN'T encoded "the first time", but they are after
// a successfull/failed modification, so we don't want to encode them twice!
if(! ereg("%3D", $rootdn)) {
	// URL encode namingContexts
	$rootdn = urlencode($rootdn);
}
if(! ereg("%3D", $domain)) {
	// .. and/or domain DN
	$domain = urlencode($domain);
}

// Get the organization name, or show 'Not set' with an URL to set it
$domainname = pql_get_domain_value($_pql, $domain, 'o');
//if(!$domainname) {
// TODO: Resonable default!
//}

// Get some default values for this domain
// Some of these (everything after the 'o' attribute)
// uses 'objectClass: dcOrganizationNameForm' -> http://rfc-2377.rfcindex.net/
$attribs = array('defaultdomain', 'basehomedir', 'basemaildir', 'o', 'l',
				 'postalcode', 'postaladdress', 'telephonenumber', 'street',
				 'facsimiletelephonenumber', 'postofficebox', 'st', 'basequota',
				 'maximumdomainusers', 'defaultpasswordscheme', 'maximummailinglists',
				 'autocreateusername', 'autocreatemailaddress', 'usernameprefix');
foreach($attribs as $attrib) {
	// Get default value
	$value = pql_get_domain_value($_pql, $domain, $attrib);
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

		$$link = "<a href=\"domain_edit_attributes.php?type=modify&attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=". urlencode($value) ."\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt1."\"></a>&nbsp;<a href=\"domain_edit_attributes.php?type=delete&submit=2&attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=". urlencode($value) ."\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt2."\"></a>";
	} else {
		$alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %domainname%'),
									  array('attribute' => $attrib, 'domainname' => $domainname));

		// A phpQLAdminBranch attribute
		$$link = "<a href=\"domain_edit_attributes.php?attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt1."\"></a>";
	}
}
$domain_admins		= pql_get_domain_value($_pql, $domain, "administrator");
$mailinglist_admins	= pql_get_domain_value($_pql, $domain, "ezmlmadministrator");
$seealso   			= pql_get_domain_value($_pql, $domain, "seealso");
$basequota			= pql_ldap_mailquota(pql_parse_quota($basequota));

$additionaldomainname = pql_get_domain_value($_pql, $domain, "additionaldomainname");
?>
  <span class="title1"><?=$LANG->_('Organization name')?>: <?=urldecode($domainname)?></span>

  <br><br>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=default"?>"><img alt="/ <?=$LANG->_('Default Branch Values')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Default Branch Values')?>"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=users"?>"><img alt="/ <?=$LANG->_('Registred users')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Registred users')?>"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=chval"?>"><img alt="/ <?=$LANG->_('Change values of all users')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Change values of all users')?>"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=dnszone"?>"><img alt="/ <?=$LANG->_('DNS Zone')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('DNS Zone')?>"></a><?php if($ADVANCED_MODE) { ?><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=owner"?>"><img alt="/ <?=$LANG->_('Branch Owner')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Branch Owner')?>"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=dnsinfo"?>"><img alt="/ <?=$LANG->_('MX Information')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('MX Information')?>"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=options"?>"><img alt="/ <?=$LANG->_('Control Options')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Control Options')?>"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=action"?>"><img alt="/ <?=$LANG->_('Actions')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Actions')?>"></a><?php } ?></td>
  </tr>
</table>

<br>
<?php
if(!$view)
  $view = 'default';

if($view == 'default') {
	if($ADVANCED_MODE) {
		include("./tables/domain_details-default.inc");
	} else {
		include("./tables/domain_details-owner.inc");
	}
}

if($view == 'owner') {
	include("./tables/domain_details-owner.inc");
}

if(($view == 'users') or ($view == 'chval')) {
	$users = pql_get_user($_pql->ldap_linkid, $domain);

	if(is_array($users)) {
		if($view == 'chval')
		  include("./tables/domain_details-users_chval.inc");
		elseif($view == 'users')
		  include("./tables/domain_details-users.inc");
	} else {
?>
  <br><br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('No users in this branch')?>!</th>
  </table>
<?php
	}
}

if($ADVANCED_MODE == 1) {
	if($view == 'dnsinfo')
		include("./tables/domain_details-dnsinfo.inc");
}

if($view == 'dnszone')
	 include("./tables/domain_details-dnszone.inc");

if($ADVANCED_MODE == 1) {
	if($view == 'options')
		include("./tables/domain_details-options.inc");

	if($view == 'action')
		include("./tables/domain_details-action.inc");
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
