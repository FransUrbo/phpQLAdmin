<?php
// adds an attribute 
// user_add_attribute.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// Make sure we can have a ' in branch (also affects the user DN).
$user   = eregi_replace("\\\'", "'", $user);
$domain = eregi_replace("\\\'", "'", $domain);

// forward back to users detail page
function attribute_forward($msg) {
    global $domain, $user, $rootdn, $view;

    // URL Encode some of the most important information
    // (root DN, domain/branch DN and user DN)
    $domain = urlencode($domain);
    $user   = urlencode($user);
    $rootdn = urlencode($rootdn);
    
    $url = "user_detail.php?domain=$domain&user=".urlencode($user)."&view=$view&msg=".urlencode($msg);
    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql, $domain, "defaultdomain");

// Get the username. Prettier than the DN
$username = pql_get_userattribute($_pql->ldap_linkid, $user, 'cn');
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_userattribute($_pql->ldap_linkid, $user, 'uid');
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
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change user data for %user%'), array('user', $username)); ?></span>
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
