<?php
// adds an attribute 
// $Id: user_add_attribute.php,v 2.35 2005-03-19 15:12:22 turbo Exp $
//
/* This file gets iterated through at least 2 times for any attribute (sequenced by "$submit"):
 *   1) $submit is unset: Set the default value of the attribute (usually from "$oldvalue")
 *      and print out the form.
 *   2) $submit is 1: Validate the input. The name of the input variable changes depending on
 *      which attribute is being edited.
 *      If the input is valid, save it, else print out the form again and return to step 2. */

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_GET["domain"]);
$url["user"]   = pql_format_urls($_GET["user"]);
$url["rootdn"] = pql_format_urls($_GET["rootdn"]);

include($_SESSION["path"]."/include/".$include);
include($_SESSION["path"]."/header.html");
// }}}

// {{{ If we're called without branchname - try to reconstruct it
if(empty($_REQUEST["domain"]) && !empty($_REQUEST["user"])) {
    $tmpdn = split(',', $_REQUEST["user"]);
    if($tmpdn[1]) {
        unset($tmpdn[0]);
        $_REQUEST["domain"] = implode(",", $tmpdn);
    } else
      $_REQUEST["domain"] = $tmpdn[count($tmpdn)-1];
}
// }}}

// {{{ Make sure we can have a ' in branch (also affects the user DN).
$_REQUEST["user"]   = eregi_replace("\\\'", "'", $_REQUEST["user"]);
$_REQUEST["domain"] = eregi_replace("\\\'", "'", $_REQUEST["domain"]);
// }}}

// {{{ Forward back to users detail page
function attribute_forward($msg) {
    // URL Encode some of the most important information
    // (root DN, domain/branch DN and user DN)
    if(!eregi('%3D', $_REQUEST["rootdn"]))  $_REQUEST["rootdn"] = urlencode($_REQUEST["rootdn"]);
    if(!eregi('%3D', $_REQUEST["domain"]))  $_REQUEST["domain"] = urlencode($_REQUEST["domain"]);
    if(!eregi('%3D', $_REQUEST["user"]))    $_REQUEST["user"]   = urlencode($_REQUEST["user"]);
    
    $url = "user_detail.php?rootdn=" . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"]
      . "&user=" . $_REQUEST["user"] . "&view=" . $_REQUEST["view"] . "&msg=".urlencode($msg);
    pql_header($url);
}
// }}}

// {{{ Get some default values
// ... domain name
$defaultdomain = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));

// ... user name
$username = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_CN"));
if(!$username) {
    // No common name, use uid field
    $username = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_UID"));
}

// {{{ Select which attribute have to be included
switch($_REQUEST["attrib"]) {
  case "mailalternateaddress":
    $include = "attrib.mailalternateaddress.inc";
    break;
  case "mailforwardingaddress":
    $include = "attrib.mailforwardingaddress.inc";
    break;
  default:
    die(pql_complete_constant($LANG->_('Unknown attribute %attribute% in %file%'),
			      array('attribute' => $_REQUEST["attrib"], 'file' => __FILE__)));
}
// }}}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change user data for %user%'),
							array('user' => $username)); ?></span>
  <br><br>
<?php
// {{{ Select what to do
if($_REQUEST["submit"] == 1) {
    if(attribute_check("add")) {
	attribute_save("add");
    } else {
	attribute_print_form();
    }
} else {
    attribute_print_form();
}
// }}}
?>
</body>
</html>
