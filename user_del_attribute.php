<?php
// delete attribute of a user
// user_del_attribute.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");

switch ($attrib) {
  case "mailalternateaddress":
    $attrib = PQL_LDAP_ATTR_MAILALTERNATE;
    break;	
    
  case "mailforwardingaddress";
    $attrib = PQL_LDAP_ATTR_FORWARDS;
    break;
    
  default:
    die("Unknown attribute '$attrib' in ". __FILE__);
}

include("./header.html");
?>
  <span class="title1"><?php pql_complete_constant(PQL_USER_DEL_ATTRIBUTE_TITLE, array("value" => $value));?></span>
<?php
if(isset($ok) || PQL_VERIFY_DELETE) {
    $_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);
    
    // delete the user attribute
    if(pql_remove_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, $attrib, $value)){
	$msg = PQL_USER_DEL_ATTRIBUTE_OK;
	$success = true;
    } else {
    	$msg = PQL_USER_DEL_ATTRIBUTE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	$success = false;
    }
    
    if ($attrib == 'mailalternateaddress' and $success and isset($delete_forwards)) {
	// does another account forward to this alias?
	$sr = ldap_search($_pql->ldap_linkid, $USER_SEARCH_DN_USR, "(|(" . PQL_LDAP_ATTR_FORWARDS ."=" . $value . "))");
	if (ldap_count_entries($_pql->ldap_linkid,$sr) > 0) {
	    
	    $results = ldap_get_entries($_pql->ldap_linkid, $sr);
	    foreach($results as $key => $result){
		if ((string)$key != "count") {
		    $ref = $result[PQL_LDAP_REFERENCE_USERS_WITH][0];
		    $domain = pql_strip_username($result[PQL_LDAP_ATTR_MAIL][0]);
		    $forwarders[]  = array("domain" => $domain, "reference" => $ref, "cn" => $cn,  "email" => $result["mail"][0]);
		}
	    }
	    var_dump($forwarders);
	    foreach($forwarders as $forward) {
		// we found a forward -> remove it 
		pql_remove_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $forward['domain'], $forward['reference'], PQL_LDAP_ATTR_FORWARDS, $value);
	    }
	}
    }
    
    // redirect to users detail page
    $msg = urlencode($msg);
    $url = "user_detail.php?domain=$domain&msg=$msg&user=" . urlencode($user);
    header("Location: " . PQL_URI . "$url");
    echo $value;
} else {
?>
<br>
<br>
<?php echo PQL_SURE; ?>
<br>
  <form action="<?php echo $PHP_SELF; ?>" method="GET">
    <input type="hidden" name="user" value="<?php echo $user; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="value" value="<?php echo $value; ?>">
<?php
  if ($attrib == 'mailalternateaddress') {
?>	
    <input type="checkbox" name="delete_forwards" checked> <?php echo PQL_LDAP_MAILALTERNATEADDRESS_DEL_FORWARDS; ?><br><br>
<?php
  }
?>
    <input type="submit" name="ok" value="<?php echo PQL_YES; ?>">
    <input type="button" name="back" value="<?php echo PQL_NO; ?>" onClick="history.back();">
  </form>
  <br>
<?php
} // end of if
?>
</body>
</html>
