<?php
// edit attributes of all users of the domain
// $Id: domain_edit_attributes.php,v 2.33 2003-11-19 16:25:06 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/config_plugins.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// forward back to users detail page
function attribute_forward($msg) {
    global $domain, $user, $rootdn;

    $msg = urlencode($msg);
	if($user)
	  $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=".urlencode($user)."&view=$view&msg=$msg";
	elseif($administrator)
	  $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=$administrator&view=$view&msg=$msg";
	else
	  $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&view=$view&msg=$msg";

    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Look for a URL encoded '=' (%3D). If one isn't found, encode the DN
// These variables ISN'T encoded "the first time", but they are after
// the attribute_print_form() have been executed, so we don't want to
// encode them twice!
if(! ereg("%3D", $rootdn)) {
	// URL encode namingContexts
	$rootdn = urlencode($rootdn);
}
if(! ereg("%3D", $domain)) {
	// .. and/or domain DN
	$domain = urlencode($domain);
}

// Select which attribute have to be included
include("./include/".pql_plugin_get_filename(pql_plugin_get($attrib)));

// Get the organization name, or the DN if it's unset
$orgname = pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($domain);
}

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default values'), 'domain' => $orgname)); ?>
  </span>

  <br><br>

<?php
if(!$type) {
	$type = 'fulldomain';
}
	 
// select what to do
if($submit == 1) {
	if($attrib == 'basequota') {
		attribute_save("modify");
	} else {
	    if(attribute_check($type))
		  attribute_save($type);
		else
		  attribute_print_form($type);
	}
} elseif($submit == 2) {
    // Support for changing domain defaults
	if($type != 'delete') {
		if(attribute_check())
		  attribute_save("modify");
		else
		  attribute_print_form();
	} else
	  attribute_save("delete");
} elseif($submit == 3) {
	// Support for changing domain administrator
	attribute_print_form($action);
} elseif($submit == 4) {
	// SAVE change of domain administrator, mailinglist admin and contact person
	attribute_save($action);
} else {
	if($attrib == pql_get_define("PQL_GLOB_ATTR_BASEQUOTA"))
	  attribute_print_form();
	elseif(($attrib == pql_get_define("PQL_GLOB_ATTR_AUTOCREATEUSERNAME")) or
		   ($attrib == pql_get_define("PQL_GLOB_ATTR_AUTOCREATEMAILADDRESS")))
	  attribute_save();
	else
	  attribute_print_form($type);
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
