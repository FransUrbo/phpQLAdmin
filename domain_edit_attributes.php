<?php
// edit attributes of all users of the domain
// $Id: domain_edit_attributes.php,v 2.34.2.3 2003-12-17 16:11:38 dlw Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/config_plugins.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// forward back to users detail page
function attribute_forward($msg) {
  //global $domain, $user, $rootdn;

    $msg = urlencode($msg);
	if($user)
	  $url = "user_detail.php?rootdn=" . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"]
		. "&user=". urlencode($user) . "&view=" . $_REQUEST["view"] . "&msg=$msg";
	elseif($administrator)		// DLW: Bug? $administrator isn't visable.
	  $url = "user_detail.php?rootdn=" . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"]
	    . "&user=$administrator&view=" . $_REQUEST["view"] . "&msg=$msg";
	else
	  $url = "domain_detail.php?rootdn=" . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"]
		. "&view=" . $_REQUEST["view"] . "&msg=$msg";

    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Look for a URL encoded '=' (%3D). If one isn't found, encode the DN
// These variables ISN'T encoded "the first time", but they are after
// the attribute_print_form() have been executed, so we don't want to
// encode them twice!
if(! ereg("%3D", $_REQUEST["rootdn"])) {
	// URL encode namingContexts
	$_REQUEST["rootdn"] = urlencode($_REQUEST["rootdn"]);
}
if(! ereg("%3D", $_REQUEST["domain"])) {
	// .. and/or domain DN
	$_REQUEST["domain"] = urlencode($_REQUEST["domain"]);
}

// Select which attribute have to be included
include("./include/".pql_plugin_get_filename(pql_plugin_get($_REQUEST["attrib"])));

// Get the organization name, or the DN if it's unset
$orgname = pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($_REQUEST["domain"]);
}

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default values'), 'domain' => $orgname)); ?>
  </span>

  <br><br>

<?php

  // DLW: I'm guessing that $type comes from a form.
  /* DLW: Type isn't always set, and some times it seems like "action" it taking it's place.
   *      Check this out in more detail. */
if(!$_REQUEST["type"]) {
  if (!empty($_REQUEST["action"])) { // DLW: I'm wingining it here.
	$_REQUEST["type"] = $_REQUEST["action"];
  } else {
	$_REQUEST["type"] = 'fulldomain';
  }
}

// select what to do
if(@$_REQUEST["submit"] == 1) {
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
	if($_REQUEST["type"] != 'delete') {
		if(attribute_check())
		  attribute_save("modify");
		else
		  attribute_print_form();
	} else
	  attribute_save("delete");
} elseif(@$_REQUEST["submit"] == 3) {
	// Support for changing domain administrator
	attribute_print_form($_REQUEST["type"]);
} elseif(@$_REQUEST["submit"] == 4) {
	// SAVE change of domain administrator, mailinglist admin and contact person
	attribute_save($_REQUEST["type"]);
} else {
	if($_REQUEST["attrib"] == pql_get_define("PQL_GLOB_ATTR_BASEQUOTA"))
	  attribute_print_form();
	elseif(($_REQUEST["attrib"] == pql_get_define("PQL_GLOB_ATTR_AUTOCREATEUSERNAME")) or
		   ($_REQUEST["attrib"] == pql_get_define("PQL_GLOB_ATTR_AUTOCREATEMAILADDRESS")))
	  attribute_save();
	else
	  attribute_print_form($_REQUEST["type"]);
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
