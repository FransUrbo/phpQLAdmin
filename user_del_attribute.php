<?php
// delete attribute of a user
// user_del_attribute.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

switch ($attrib) {
  case "mailalternateaddress":
    $attrib = $config["PQL_GLOB_ATTR_MAILALTERNATE"];
    break;	
    
  case "mailforwardingaddress";
    $attrib = $config["PQL_GLOB_ATTR_FORWARDS"];
    break;
    
  default:
    die("Unknown attribute '$attrib' in ". __FILE__);
}

include("./header.html");
?>
  <span class="title1"><?php pql_complete_constant(PQL_LANG_USER_DEL_ATTRIBUTE_TITLE, array("value" => $value));?></span>
<?php
if(isset($ok) || !PQL_CONF_VERIFY_DELETE) {
    $_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
    
    // delete the user attribute
    if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib, $value)){
	$msg = PQL_LANG_USER_DEL_ATTRIBUTE_OK;
	$success = true;
    } else {
    	$msg = PQL_LANG_USER_DEL_ATTRIBUTE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	$success = false;
    }
    
    if ($attrib == 'mailalternateaddress' and $success and isset($delete_forwards)) {
	// does another account forward to this alias?
	$sr = ldap_search($_pql->ldap_linkid, "(|(" . $config["PQL_GLOB_ATTR_FORWARDS"] ."=" . $value . "))");
	if (ldap_count_entries($_pql->ldap_linkid,$sr) > 0) {
	    
	    $results = ldap_get_entries($_pql->ldap_linkid, $sr);
	    foreach($results as $key => $result){
		if ((string)$key != "count") {
		    $ref = $result[$config["PQL_CONF_REFERENCE_USERS_WITH"][pql_get_rootdn($user)]][0];
		    $domain = pql_strip_username($result[$config["PQL_GLOB_ATTR_MAIL"]][0]);
		    $forwarders[]  = array("domain" => $domain, "reference" => $ref, "cn" => $cn,  "email" => $result["mail"][0]);
		}
	    }
	    var_dump($forwarders);
	    foreach($forwarders as $forward) {
		// we found a forward -> remove it 
		pql_replace_userattribute($_pql->ldap_linkid, $forward['reference'], $config["PQL_GLOB_ATTR_FORWARDS"], $value);
	    }
	}
    }
    
    // redirect to users detail page
    $msg = urlencode($msg);
    $url = "user_detail.php?domain=$domain&msg=$msg&user=" . urlencode($user);
    header("Location: " . $config["PQL_GLOB_URI"] . "$url");
    echo $value;
} else {
?>
<br>
<br>
<?php echo PQL_LANG_SURE; ?>
<br>
  <form action="<?php echo $PHP_SELF; ?>" method="GET">
    <input type="hidden" name="user" value="<?php echo $user; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="value" value="<?php echo $value; ?>">
<?php
  if ($attrib == 'mailalternateaddress') {
?>	
    <input type="checkbox" name="delete_forwards" checked> <?php echo PQL_LANG_MAILALTERNATEADDRESS_DEL_FORWARDS; ?><br><br>
<?php
  }
?>
    <input type="submit" name="ok" value="<?php echo PQL_LANG_YES; ?>">
    <input type="button" name="back" value="<?php echo PQL_LANG_NO; ?>" onClick="history.back();">
  </form>
  <br>
<?php
} // end of if
?>
</body>
</html>
