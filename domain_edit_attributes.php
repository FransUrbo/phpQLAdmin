<?php
// edit attributes of all users of the domain
// domain_edit_attributes.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// forward back to users detail page
function attribute_forward($msg) {
    global $domain, $user, $rootdn;

    $msg = urlencode($msg);
	if($user)
	  $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=".urlencode($user)."&view=$view&msg=$msg";
	elseif($administrator)
	  $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=$administrator&view=$view&msg=$msg";
	else
	  $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&view=$view&msg=$msg";

    header("Location: " . $config["PQL_GLOB_URI"] . "$url");
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
	$include = "attrib.mailquota.inc";
	break;
  case "ezmlmadministrator";
  case "administrator";
  case "seealso":
	$include = "attrib.administrator.inc";
	break;
  case "maximumdomainusers":
	$include = "attrib.maximumdomainusers.inc";
	break;
  case "maximummailinglists":
	$include = "attrib.maximummailinglists.inc";
	break;
  case "o":
  case "postalcode":
  case "postaladdress":
  case "street":
  case "l":
  case "st":
  case "c":
  case "telephonenumber":
  case "facsimiletelephonenumber":
  case "postofficebox":
    $include = "attrib.outlook.inc";
    break;
  case "additionaldomainname":
	$include = "attrib.additionaldomainname.inc";
	break;
  case "defaultpasswordscheme":
	$include = "attrib.defaultpasswordscheme.inc";
	break;
  case "autocreateusername":
  case "autocreatemailaddress";
	$include = "attrib.domaintoggle.inc";
	break;
  case "usernameprefix";
	$include = "attrib.usernameprefix.inc";
	break;
  default:
    die("unknown attribute");
}

include("./include/".$include);
include("./header.html");
?>
  <span class="title1"><?php echo PQL_LANG_DOMAIN_DEFAULT_TITLE; ?>
  </span>

  <br><br>

<?php
// select what to do
if($submit == 1) {
	if($attrib == 'basequota') {
		attribute_save("modify");
	} else {
	    if(attribute_check("fulldomain"))
		  attribute_save("fulldomain");
		else
		  attribute_print_form("fulldomain");
	}
} elseif($submit == 2) {
    // Support for changing domain defaults
	if($type != 'delete') {
		if(attribute_check())
		  attribute_save("modify");
		else
		  attribute_print_form();
	} else
	  attribute_save("delete");
} elseif($submit == 3) {
	// Support for changing domain administrator
	attribute_print_form($action);
} elseif($submit == 4) {
	// SAVE change of domain administrator, mailinglist admin and contact person
	attribute_save($action);
} else {
	if($attrib == 'basequota')
	  attribute_print_form();
	elseif(($attrib == 'autocreateusername') or ($attrib == 'autocreatemailaddress'))
	  attribute_save();
	else
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
