<?php
// adds an attribute 
// $Id: user_add_attribute.php,v 2.24 2003-11-19 19:38:19 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

if(!$domain && $user) {
    // We're called without branchname - try to reconstruct it
    
    $tmpdn = split(',', $user);
    if($tmpdn[1]) {
        unset($tmpdn[0]);
        $domain = implode(",", $tmpdn);
    } else
      $domain = $tmpdn[count($tmpdn)-1];
}

// Make sure we can have a ' in branch (also affects the user DN).
$user   = eregi_replace("\\\'", "'", $user);
$domain = eregi_replace("\\\'", "'", $domain);

// forward back to users detail page
function attribute_forward($msg) {
    global $domain, $user, $rootdn, $view;

    // URL Encode some of the most important information
    // (root DN, domain/branch DN and user DN)
    if(!eregi('%3D', $rootdn))  $rootdn = urlencode($rootdn);
    if(!eregi('%3D', $domain))  $domain = urlencode($domain);
    if(!eregi('%3D', $user))    $user   = urlencode($user);
    
    $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=$user&view=$view&msg=".urlencode($msg);
    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Get default domain name for this domain
$defaultdomain = pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));

// Get the username. Prettier than the DN
$username = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_CN"));
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_UID"));
}
$username = $username[0];

// select which attribute have to be included
switch($attrib) {
  case "mailalternateaddress":
    $include = "attrib.mailalternateaddress.inc";
    break;
  case "mailforwardingaddress":
    $include = "attrib.mailforwardingaddress.inc";
    break;
  default:
    die(pql_complete_constant($LANG->_('Unknown attribute %attribute% in %file%'), array('attribute' => $attrib, 'file' => __FILE__)));
}

include("./include/".$include);
include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change user data for %user%'), array('user' => $username)); ?></span>
  <br><br>
<?php
// select what to do
if($submit == 1) {
    if(attribute_check("add")) {
	attribute_save("add");
    } else {
	attribute_print_form();
    }
} else {
    attribute_print_form();
}
?>
</body>
</html>
