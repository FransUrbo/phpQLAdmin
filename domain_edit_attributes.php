<?php
// edit attributes of all users of the domain
// $Id: domain_edit_attributes.php,v 2.50 2005-03-17 09:13:10 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/config_plugins.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
$url["user"]   = pql_format_urls($_REQUEST["user"]);

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to users detail page
function attribute_forward($msg) {
	global $url;

	if($_REQUEST["user"]) {
		$link = "user_detail.php?rootdn=" . $url["rootdn"]
		  . "&domain=" . $url["domain"]
		  . "&user=". $url["user"]
		  . "&view=" . $_REQUEST["view"] . "&msg=$msg";
	} elseif(ereg('^config_detail', $_REQUEST["view"])) {
		// Very special sircumstances - we've deleted an administrator from the config_details/ROOTDN page!
		$tmp  = split('/', $_REQUEST["view"]);
		$link = "config_detail.php?view=".$tmp[1];
	} elseif($_REQUEST["administrator"]) {
		// Administrators is _always_ added/change here, NOT from user_edit_attribute.php
		// as one would think. I.e. when giving a user access to branch from the
		// 'User details->User access' page...
		$url["user"]			= pql_format_urls($_REQUEST["user"]);
		$url["administrator"]	= pql_format_urls($_REQUEST["administrator"]);
		
		$link = "user_detail.php?rootdn=" . $url["rootdn"]
		  . "&domain=" . $url["domain"]
		  . "&user=" . $url["administrator"]
		  . "&view=" . $_REQUEST["view"] . "&msg=$msg";
	} else
	  $link = "domain_detail.php?rootdn=" . $url["rootdn"]
		. "&domain=" . $url["domain"]
		. "&view=" . $_REQUEST["view"] . "&msg=$msg";

	if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
	  echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	  die("<b>".$_SESSION["URI"].$link."</b>");
	} else
	  pql_header($link);
}
// }}}

// {{{ Select and load attribute plugin file
$plugin = pql_plugin_get_filename(pql_plugin_get($_REQUEST["attrib"]));
if(!$plugin) {
    die("<span class=\"error\">ERROR: No plugin file defined for attribute '<i>".$_REQUEST["attrib"]."</i>'</span>");
} elseif(!file_exists($_SESSION["path"]."/include/$plugin")) {
    die("<span class=\"error\">ERROR: Plugin file defined for attribute '<i>".$_REQUEST["attrib"]."</i>' does not exists!</span>");
}
include($_SESSION["path"]."/include/$plugin");
// }}}

// {{{ Get the organization name, or the DN if it's unset
$orgname = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($_REQUEST["domain"]);
}
if(is_array($orgname)) {
	$orgname = $orgname[0];
}
$_REQUEST["orgname"] = $orgname;
// }}}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default values'), 'domain' => $_REQUEST["orgname"])); ?>
  </span>

  <br><br>

<?php
// {{{ Pages that call us
// * with 'type=':
//		domain_detail.php
//		tables/domain_details-aci.inc
//
// * with 'submit=':
//		domain_detail.php
//		tables/config_details-branch.inc
//		tables/domain_details-aci.inc
//		tables/domain_details-default.inc
//		tables/domain_details-owner.inc
//		tables/user_details-access.inc
//
// * with 'action=':
//		tables/config_details-branch.inc
//		tables/domain_details-default.inc
//		tables/domain_details-owner.inc
//		tables/user_details-access.inc
//
// * with 'set=':
//		tables/domain_details-users_chval.inc
//
// * Values for type/submit/action:
//		type={edit,delete,moveup,movedown,host,del}
//		submit=[1-4]
//		action={modify,delete,add}
//
// * Values for attrib:
//		OpenLDAPaci					autoCreateUserName				mobile
//		accountStatus				baseHomeDir						o
//		additionalDomainName		baseMailDir						postalAddress
//		administrator				baseQuota						postalCode
//		deliveryMode				defaultDomain					postOfficeBox
//		ezmlmAdministrator			defaultPasswordScheme			registeredAddress
//		mailHost					ezmlmVirtualUser				st
//		mailQuota					facsimileTelePhoneNumber		street
//		seeAlso						info							streetAddress
//		startWithAdvancedMode		l								telePhoneNumber
//		autoCreateMailAddress		maximumDomainUsers				userNamePrefix
//		autoCreatePassWord			maximumMailingLists				userNamePrefixLength
//		vatNumber
//
// * Files that manages the attribs above:
//		include/attrib.accountstatus.inc
//		include/attrib.aci.inc
//		include/attrib.additionaldomainname.inc
//		include/attrib.administrator.inc
//		include/attrib.basehomedir.inc
//		include/attrib.basemaildir.inc
//		include/attrib.defaultdomain.inc
//		include/attrib.defaultpasswordscheme.inc
//		include/attrib.deliverymode.inc
//		include/attrib.domaintoggle.inc
//		include/attrib.ezmlmuser.inc
//		include/attrib.mailhost.inc
//		include/attrib.mailquota.inc
//		include/attrib.maximumdomainusers.inc
//		include/attrib.maximummailinglists.inc
//		include/attrib.outlook.inc
//		include/attrib.usernameprefix.inc
//		include/attrib.usertoggle.inc
// }}}

if(!$_REQUEST["type"]) {
  if (!empty($_REQUEST["action"])) { // DLW: I'm wingining it here.
	$_REQUEST["type"] = $_REQUEST["action"];
  } else {
	$_REQUEST["type"] = 'fulldomain';
  }
}

// {{{ Select what to do
if(@$_REQUEST["submit"] == 1) {
	// Called from:
	//	tables/domain_details-dnszone.inc
	//	tables/domain_details-options.inc

	if($_REQUEST["attrib"] == 'basequota') {
		attribute_save("modify");
	} else {
	    if(attribute_check($_REQUEST["type"]))
		  attribute_save($_REQUEST["type"]);
		else
		  attribute_print_form($_REQUEST["type"]);
	}
} elseif(@$_REQUEST["submit"] == 2) {
    // Support for changing domain defaults
	// Called from:
	//	tables/domain_details-aci.inc

	if($_REQUEST["type"] != 'delete') {
		if(attribute_check())
		  attribute_save("modify");
		else
		  attribute_print_form();
	} else
	  attribute_save("delete");
} elseif(@$_REQUEST["submit"] == 3) {
	// Support for changing domain administrator
	// Called from:
	//	tables/domain_details-aci.inc
	//	tables/domain_details-default.inc
	//	tables/domain_details-owner.inc

	attribute_print_form($_REQUEST["type"]);
} elseif(@$_REQUEST["submit"] == 4) {
	// SAVE change of domain administrator, mailinglist admin and contact person
	// Called from:
	//	tables/domain_details-aci.inc
	//	tables/domain_details-default.inc
	//	tables/domain_details-owner.inc

	if($_REQUEST["action"])
	  attribute_save($_REQUEST["action"]);
	elseif($_REQUEST["type"])
	  attribute_save($_REQUEST["type"]);
} else {
	if($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_BASEQUOTA"))
	  attribute_print_form();
	elseif(($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME")) or
		   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS")) or
		   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_AUTOCREATE_PASSWORD")))
	  attribute_save();
	else
	  attribute_print_form($_REQUEST["type"]);
}
// }}}
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
