<?php
// edit attributes of all users of the domain
// domain_edit_attributes.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("pql.inc");
$_pql = new pql($USER_DN, $USER_PASS);

// forward back to users detail page
function attribute_forward($msg){
    global $domain;
    
    $msg = urlencode($msg);
    $url = "domain_detail.php?domain=$domain&msg=$msg";
    header("Location: " . PQL_URI . "$url");
}

// select which attribute have to be included
switch($attrib){
  case "accountstatus";
    $include = "attrib.accountstatus.inc";
    break;
  case "deliverymode";
    $include = "attrib.deliverymode.inc";
    break;
  case "mailquota";
    $include = "attrib.mailquota.inc";
    break;
  case "mailhost";
    $include = "attrib.mailhost.inc";
    break;
  case "defaultdomain":
    $include = "attrib.defaultdomain.inc";
    break;
  case "basehomedir":
    $include = "attrib.basehomedir.inc";
    break;
  case "basemaildir";
    $include = "attrib.basemaildir.inc";
    break;
  case "basequota";
	$include = "attrib.basequota.inc";
  default:
    die("unknown attribute");
}

include($include);
include("header.html");
?>
  <span class="title1">
<?php
  if(eregi("defaultdomain|basehomedir|basemaildir", $include)) {
      echo PQL_DOMAIN_DEFAULT_TITLE;
  } else {
      echo PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE;
  }
?>
</span>
<br><br>

<?php
// select what to do
if($submit == 1){
    if(attribute_check("fulldomain")){
	attribute_save("fulldomain");
    } else {
	attribute_print_form("fulldomain");
    }
} elseif($submit == 2) {
    // Support for changing domain defaults
    if(attribute_check()){
	attribute_save("modify");
    } else {
	attribute_print_form();
    }
} else {
    attribute_print_form("fulldomain");
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
