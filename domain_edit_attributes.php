<?php
// edit attributes of all users of the domain
// $Id: domain_edit_attributes.php,v 2.41.12.1 2004-12-19 10:12:54 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/config_plugins.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
$url["user"]   = pql_format_urls($_REQUEST["user"]);

// forward back to users detail page
function attribute_forward($msg) {
	global $url;
	$msg = urlencode($msg);

	if($_REQUEST["user"])
	  $link = "user_detail.php?rootdn=" . $url["rootdn"]
		. "&domain=" . $url["domain"]
		. "&user=". $url["user"]
		. "&view=" . $_REQUEST["view"] . "&msg=$msg";
	elseif($_REQUEST["administrator"]) {
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

    header("Location: " . pql_get_define("PQL_CONF_URI") . $link);
}

// Select which attribute have to be included
include("./include/".pql_plugin_get_filename(pql_plugin_get($_REQUEST["attrib"])));

// Get the organization name, or the DN if it's unset
$orgname = pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_O"));
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
	if($_REQUEST["action"])
	  attribute_save($_REQUEST["action"]);
	elseif($_REQUEST["type"])
	  attribute_save($_REQUEST["type"]);
} else {
	if($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_BASEQUOTA"))
	  attribute_print_form();
	elseif(($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_AUTOCREATE_USERNAME")) or
		   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_AUTOCREATE_MAILADDRESS")))
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
