<?php
// delete attribute of a user
// $Id: user_del_attribute.php,v 2.27 2004-03-11 18:13:32 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

switch ($_REQUEST["attrib"]) {
  case "mailalternateaddress":
    $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_MAILALTERNATE");
    break;	
    
  case "mailforwardingaddress";
    $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_FORWARDS");
    break;
    
  default:
    die(pql_complete_constant($LANG->_('Unknown attribute %attribute% in %file%'), array('attribute' => $_REQUEST["attrib"], 'file' => __FILE__)));
}

include("./header.html");

if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
    $_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
    
    // delete the user attribute
    if(pql_modify_userattribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $_REQUEST["oldvalue"], '')) {
	$msg = pql_complete_constant($LANG->_('Successfully removed alias %mail%'), array("mail" => pql_maybe_idna_decode($_REQUEST["oldvalue"])));
	$success = true;
    } else {
    	$msg = pql_complete_constant($LANG->_('Failed to removed alias %mail%'), array("mail" => pql_maybe_idna_decode($_REQUEST["oldvalue"]))) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	$success = false;
    }
    
    if (lc($_REQUEST["attrib"]) == 'mailalternateaddress' and $success and isset($_REQUEST["delete_forwards"])) {
	// does another account forward to this alias?
	$sr = ldap_search($_pql->ldap_linkid, "(|(" . pql_get_define("PQL_ATTR_FORWARDS") ."=" . $_REQUEST["oldvalue"] . "))");
	if (ldap_count_entries($_pql->ldap_linkid,$sr) > 0) {
	    $results = ldap_get_entries($_pql->ldap_linkid, $sr);
	    foreach($results as $key => $result){
		if ((string)$key != "count") {
		    $ref = $result[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH",
						  pql_get_rootdn($_REQUEST["user"], 'user_del_attribute.php'))][0];
		    $_REQUEST["domain"] = pql_strip_username($result[pql_get_define("PQL_ATTR_MAIL")][0]);
		    $forwarders[]  = array("domain"	=> $_REQUEST["domain"],
					   "reference"	=> $ref,
					   "cn"		=> $_REQUEST["cn"],
					   "email"	=> $result[pql_get_define("PQL_ATTR_MAIL")][0]);
		}
	    }
	    var_dump($forwarders);
	    foreach($forwarders as $forward) {
		// we found a forward -> remove it 
		pql_replace_userattribute($_pql->ldap_linkid,
					  $forward['reference'],
					  pql_get_define("PQL_ATTR_FORWARDS"),
					  $_REQUEST["oldvalue"]);
	    }
	}
    }
    
    // redirect to users detail page
    $url = "user_detail.php?rootdn=" . $_REQUEST["rootdn"] . "&domain=" . $_REQUEST["domain"]
      . "&user=" . urlencode($_REQUEST["user"]) . "&msg=" . urlencode($msg);
    header("Location: " . pql_get_define("PQL_CONF_URI") . "$url");
} else {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove attribute %attribute% for user %user%'), array('attribute' => $_REQUEST["attrib"], 'user' => $_REQUEST["user"])); ?></span>
  <br><br>
  <?=$LANG->_('Are you really sure')?>
  <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
    <input type="hidden" name="user" value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="domain" value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
<?php
  if ($_REQUEST["attrib"] == 'mailalternateaddress') {
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