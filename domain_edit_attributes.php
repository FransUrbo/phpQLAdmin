<?php
// edit attributes of all users of the domain
// domain_edit_attributes.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// forward back to users detail page
function attribute_forward($msg){
    global $domain;
    
    $msg = urlencode($msg);
    $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&msg=$msg";
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
	break;
  case "administrator";
  case "seealso":
	$include = "attrib.administrator.inc";
	break;
  case "o":
  case "postalcode":
  case "postaladdress":
  case "l":
  case "st":
  case "c":
  case "telephonenumber":
  case "facsimiletelephonenumber":
  case "postofficebox":
    $include = "attrib.outlook.inc";
    break;
  default:
    die("unknown attribute");
}

include("./include/".$include);
include("./header.html");
?>
  <span class="title1"><?php
  if(eregi("defaultdomain|basehomedir|basemaildir|administrator", $include)) {
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
	if($type != 'delete') {
		if(attribute_check()) {
			attribute_save("modify");
		} else {
			attribute_print_form();
		}
	} else {
		attribute_save("delete");
	}
} elseif($submit == 3) {
	// Support for changing domain administrator
	attribute_print_form($action);
} elseif($submit == 4) {
	// SAVE change of domain administrator
	attribute_save($action);
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
