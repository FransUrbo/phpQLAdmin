<?php
// delete attribute of a user
// user_del_attribute.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

switch ($attrib) {
  case "mailalternateaddress":
    $attrib = pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE");
    break;	
    
  case "mailforwardingaddress";
    $attrib = pql_get_define("PQL_GLOB_ATTR_FORWARDS");
    break;
    
  default:
    die(pql_complete_constant($LANG->_('Unknown attribute %attribute% in %file%'), array('attribute' => $attrib, 'file' => __FILE__)));
}

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove attribute %attribute%'), array('attribute' => $oldvalue));?></span>
<?php
if(isset($ok) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn)) {
    $_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
    
    // delete the user attribute
    if(pql_modify_userattribute($_pql->ldap_linkid, $user, $attrib, $oldvalue, '')) {
	$msg = pql_complete_constant($LANG->_('Successfully removed alias %mail%'), array("mail" => $oldvalue));
	$success = true;
    } else {
    	$msg = pql_complete_constant($LANG->_('Failed to removed alias %mail%'), array("mail" => $oldvalue)) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	$success = false;
    }
    
    if (lc($attrib) == 'mailalternateaddress' and $success and isset($delete_forwards)) {
	// does another account forward to this alias?
	$sr = ldap_search($_pql->ldap_linkid, "(|(" . pql_get_define("PQL_GLOB_ATTR_FORWARDS") ."=" . $oldvalue . "))");
	if (ldap_count_entries($_pql->ldap_linkid,$sr) > 0) {
	    $results = ldap_get_entries($_pql->ldap_linkid, $sr);
	    foreach($results as $key => $result){
		if ((string)$key != "count") {
		    $ref = $result[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", pql_get_rootdn($user))][0];
		    $domain = pql_strip_username($result[pql_get_define("PQL_GLOB_ATTR_MAIL")][0]);
		    $forwarders[]  = array("domain" => $domain, "reference" => $ref, "cn" => $cn,  "email" => $result[pql_get_define("PQL_GLOB_ATTR_MAIL")][0]);
		}
	    }
	    var_dump($forwarders);
	    foreach($forwarders as $forward) {
		// we found a forward -> remove it 
		pql_replace_userattribute($_pql->ldap_linkid,
					  $forward['reference'],
					  pql_get_define("PQL_GLOB_ATTR_FORWARDS"),
					  $oldvalue);
	    }
	}
    }
    
    // redirect to users detail page
    $url = "user_detail.php?rootdn=$rootdn&domain=$domain&user=".urlencode($user)."&msg=".urlencode($msg);
    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
} else {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove attribute %attribute for user %user%'), array('attribute' => $attrib, 'user' => $username)); ?></span>
  <br><br>
  <?=$LANG->_('Are you really sure')?>
  <form action="<?php echo $PHP_SELF; ?>" method="GET">
    <input type="hidden" name="user" value="<?=$user?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
<?php
  if ($attrib == 'mailalternateaddress') {
?>	
    <input type="checkbox" name="delete_forwards" checked><?=$LANG->_('Also delete forwards to this alias')?><br><br>
<?php
  }
?>
    <input type="submit" name="ok" value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back" value="<?=$LANG->_('No')?>" onClick="history.back();">
  </form>
  <br>
<?php
} // end of if
?>
</body>
</html>
