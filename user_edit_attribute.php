<?php
// edit an attribute of user
// $Id: user_edit_attribute.php,v 2.29 2003-11-19 19:38:19 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/config_plugins.inc");

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
$domain = eregi_replace("\\\'", "'", $domain);
$user   = eregi_replace("\\\'", "'", $user);

// Get default domain name for this domain
$defaultdomain = pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));

// Get the username. Prettier than the DN
$username = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_CN"));
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_UID"));
}
$username = $username[0];

// forward back to users detail page
function attribute_forward($msg, $rlnb = false) {
    global $domain, $user, $rootdn, $view;

    // URL Encode some of the most important information
    // (root DN, domain/branch DN and user DN)
    $domain = urlencode($domain);
    $user   = urlencode($user);
    $rootdn = urlencode($rootdn);
    
    $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=$user&view=$view&msg=".urlencode($msg);
    if ($rlnb)
      $url .= "&rlnb=2";

    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Select which attribute have to be included
include("./include/".pql_plugin_get_filename(pql_plugin_get($attrib)));

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Change user data for %user%'), array('user' => $username)); ?></span>
  <br><br>
<?php
// select what to do
if(($submit == 1) or ($submit == 2)) {
    if(attribute_check("modify")){
	attribute_save("modify");
    } else {
	attribute_print_form();
    }
} else {
    attribute_init();
    attribute_print_form();
}
?>
</body>
</html>
