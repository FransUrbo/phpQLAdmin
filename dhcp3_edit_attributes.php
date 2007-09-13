<?php
// edit attributes of all users of the domain
// $Id: dhcp3_edit_attributes.php,v 1.1 2007-09-13 17:50:21 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/config_plugins.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to users detail page
function attribute_forward($msg) {
	global $url;

	$link = "domain_detail.php?rootdn=" . $url["rootdn"]
	  . "&domain=" . $url["domain"]
	  . "&view=" . $_REQUEST["view"] . "&msg=$msg";

	pql_header($link);
}
// }}}

include($_SESSION["path"]."/include/attrib.dhcp3.inc");

// {{{ Get the organization name, or the DN if it's unset
$orgname = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_O"));
if(!$orgname) {
	$orgname = urldecode($_REQUEST["domain"]);
}
if(is_array($orgname)) {
	$orgname = $orgname[0];
}
$_REQUEST["orgname"] = $orgname;
// }}}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('DHCP value'), 'domain' => $_REQUEST["orgname"])); ?>
  </span>

  <br><br>

<?php
if(@$_REQUEST["submit"]) {
  if(attribute_check())
	attribute_save($_REQUEST["action"]);
  else
	attribute_print_form($_REQUEST["data"]);
} elseif($_REQUEST["action"] == "delete") {
  attribute_save($_REQUEST["action"]);
} else {
  attribute_print_form($_REQUEST["data"]);
}
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
