<?php
// shows details of a domain
// domain_detail.php,v 2.2 2002/12/17 06:28:26 turbo Exp
//
session_start();
require("./include/pql_config.inc");

if($config["PQL_GLOB_CONTROL_USE"]) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
}

include("./header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and $config["PQL_GLOB_AUTO_RELOAD"]) {
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
$dc = ldap_explode_dn($domain, 0); $dc = split('=', $dc[0]);
if(!pql_domain_exist($_pql, $dc[1])){
    echo "Domain &quot;$domain&quot; does not exists<br><br>";
	echo "Is this perhaps a Top Level DN (namingContexts), and you haven't configured ";
	echo "how to reference domains/branches in this database!?<br><br>";
	echo "Please go to <a href=\"config_detail.php\">Show configuration</a> and double check.<br>";
	echo "Look at the config option 'Reference domains with'.";
    exit();
}

// Get some default values for this domain
// Some of these (everything after the 'o' attribute)
// uses 'objectClass: dcOrganizationNameForm' -> http://rfc-2377.rfcindex.net/
$attribs = array('defaultdomain', 'basehomedir', 'basemaildir', 'o', 'l',
				 'postalcode', 'postaladdress', 'telephonenumber', 'street',
				 'facsimiletelephonenumber', 'postofficebox', 'st', 'basequota',
				 'maximumdomainusers', 'defaultpasswordscheme');
foreach($attribs as $attrib) {
	// Get default value
	$value = pql_get_domain_value($_pql, $domain, $attrib);
	$$attrib = maybe_decode($value);

	// Setup edit links. If it's a dcOrganizationNameForm attribute, then
	// we add a delete link as well.
	$link = $attrib . "_link";
	if(($attrib != 'defaultdomain') and ($attrib != 'basehomedir') and ($attrib != 'basemaildir')) {
		if(($attrib == 'maximumdomainusers') and !$value)
		  $value = 0;

		// A dcOrganizationNameForm attribute
		$$link = "<a href=\"domain_edit_attributes.php?type=modify&attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=". urlencode($value) ."\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify attribute $attrib for $domain\"></a>&nbsp;<a href=\"domain_edit_attributes.php?type=delete&submit=2&attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=". urlencode($value) ."\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Delete attribute $attrib for $domain\"></a>";
	} else {
		// A phpQLAdminBranch attribute
		$$link = "<a href=\"domain_edit_attributes.php?attrib=$attrib&rootdn=$rootdn&domain=$domain&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify $attrib for $domain\"></a>";
	}
}
$admins	   = pql_get_domain_value($_pql, $domain, "administrator");
$seealso   = pql_get_domain_value($_pql, $domain, "seealso");
$basequota = pql_ldap_mailquota(pql_parse_quota($basequota));

// Get the organization name, or show 'Not set' with an URL to set it
$domainname = pql_get_domain_value($_pql, $domain, 'o');
if(!$domainname) {
	$domainname = "<a href=\"domain_edit_attributes.php?type=modify&attrib=o&rootdn=$rootdn&domain=$domain\">".PQL_LANG_UNSET."</a>";
}

$additionaldomainname = pql_get_domain_value($_pql, $domain, "additionaldomainname");
?>
  <span class="title1">Organization: <?=urldecode($domainname)?></span>

  <br><br>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><?php if($ADVANCED_MODE) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=default"?>"><img alt="/ Default Branch Values \" vspace="0" hspace="0" border="0" src="navbutton.php?Default Branch Values"></a><?php } ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=owner"?>"><img alt="/ Branch Owner \" vspace="0" hspace="0" border="0" src="navbutton.php?Branch Owner"></a><?php if($ADVANCED_MODE) { ?><br><?php } ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=users"?>"><img alt="/ Registred Users \" vspace="0" hspace="0" border="0" src="navbutton.php?Registred Users"></a><?php if(!$ADVANCED_MODE) { ?><br><?php } ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=chval"?>"><img alt="/ Change values of all users in this branch \" vspace="0" hspace="0" border="0" src="navbutton.php?Change values of all users"></a><?php if($ADVANCED_MODE) { ?><br><?php } ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=dnsinfo"?>"><img alt="/ MX Information \" vspace="0" hspace="0" border="0" src="navbutton.php?MX Information"></a><?php if(!$ADVANCED_MODE) { ?><br><?php } ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=options"?>"><img alt="/ Control Options \" vspace="0" hspace="0" border="0" src="navbutton.php?Control Options"></a><?php if($ADVANCED_MODE) { ?><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=dnszone"?>"><img alt="/ DNS Zone \" vspace="0" hspace="0" border="0" src="navbutton.php?DNS Zone"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=action"?>"><img alt="/ Actions \" vspace="0" hspace="0" border="0" src="navbutton.php?Actions"></a><?php } ?></td>
  </tr>
</table>

<?php
if(!$view)
  $view = 'default';

if($ADVANCED_MODE and ($view == 'default')) {
	include("./tables/domain_details-default.inc");
}

if($view == 'owner') {
	include("./tables/domain_details-owner.inc");
}

if(($view == 'users') or ($view == 'chval')) {
	$users = pql_get_user($_pql->ldap_linkid, $domain);

	if($view == 'users')
		include("./tables/domain_details-users.inc");

	if(is_array($users) and ($view == 'chval'))
	  include("./tables/domain_details-users_chval.inc");
	else {
?>
  <br><br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">No users in this branch!</th>
  </table>
<?php
    }
}

if($ADVANCED_MODE == 1) {
	if($view == 'dnsinfo')
		include("./tables/domain_details-dnsinfo.inc");

	if($view == 'dnszone')
		include("./tables/domain_details-dnszone.inc");

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
