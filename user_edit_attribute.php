<?php
// edit an attribute of user
// user_edit_attribute.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");

// forward back to users detail page
function attribute_forward($msg, $rlnb = false){
    global $domain, $user;
    
    $msg = urlencode($msg);
    $url = "user_detail.php?domain=$domain&user=" . urlencode($user) . "&msg=$msg";
    if ($rlnb) {
	$url .= "&rlnb=1";
    }
    
    header("Location: " . PQL_URI . "$url");
}

// select which attribute have to be included
switch($attrib){
  case "mailalternateaddress":
    $include = "attrib.mailalternateaddress.inc";
    break;
  case "mailforwardingaddress":
    $include = "attrib.mailforwardingaddress.inc";
    break;
  case "userpassword":
    $include = "attrib.userpassword.inc";
    break;
  case "mail":
    $include = "attrib.mail.inc";
    break;
  case "accountstatus":
    $include = "attrib.accountstatus.inc";
    break;
  case "uid":
    $include = "attrib.uid.inc";
    break;
  case "deliverymode":
    $include = "attrib.deliverymode.inc";
    break;
  case "mailquota":
    $include = "attrib.mailquota.inc";
    break;
  case "mailhost":
    $include = "attrib.mailhost.inc";
    break;
  case "mailmessagestore":
    $include = "attrib.mailmessagestore.inc";
    break;
  case "homedirectory";
    $include = "attrib.homedirectory.inc";
    break;
  case "qmaildotmode":
    $include = "attrib.qmaildotmode.inc";
    break;
  case "deliveryprogrampath":
    $include = "attrib.deliveryprogrampath.inc";
    break;
  case "homePhone":
  case "mobile":
  case "o":
  case "postalAddress":
  case "l":
  case "st":
  case "postalCode":
  case "c":
  case "title":
  case "physicalDeliveryOfficeName":
  case "telephoneNumber":
  case "pager":
  case "info":
    $include = "attrib.outlook.inc";
    break;
  case "loginshell";
    $include = "attrib.loginshell.inc";
    break;
  case "cn";
    $include = "attrib.cn.inc";
    break;
  default:
    die("unknown attribute");
}

include("./include/".$include);
include("./header.html");
?>
  <span class="title1"><?php echo PQL_USER_EDIT; ?> - <u><?=$user?></u></span>
  <br><br>
<?php
// select what to do
if($submit == 1){
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
