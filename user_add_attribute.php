<?php
// adds an attribute 
// user_add_attribute.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql.inc");
$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

// forward back to users detail page
function attribute_forward($msg){
    global $domain, $user;
    
    $msg = urlencode($msg);
    $url = "user_detail.php?domain=$domain&user=" . urlencode($user) . "&msg=$msg";
    header("Location: " . PQL_URI . "$url");
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
 
// select which attribute have to be included
switch($attrib){
  case "mailalternateaddress":
    $include = "attrib.mailalternateaddress.inc";
    break;
  case "mailforwardingaddress":
    $include = "attrib.mailforwardingaddress.inc";
    break;
  default:
    die("unknown attribute");
}

include($include);
include("./header.html");
?>
  <span class="title1"><?php echo PQL_USER_EDIT; ?></span>
  <br><br>
<?php
// select what to do
if($submit == 1){
    if(attribute_check("add")){
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
