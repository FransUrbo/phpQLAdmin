<?php
// delete attribute of a user
// $Id: user_del_attribute.php,v 2.40 2006-12-16 12:02:09 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Retreive the _LDAP_ attribute name
switch ($_REQUEST["attrib"]) {
  case "mailalternateaddress":
    $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_MAILALTERNATE");
    break;	
    
  case "mailforwardingaddress":
    $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_FORWARDS");
    break;

  case "confirmtext":
    $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_GROUP_CONFIRM_TEXT");
    break;

  case "moderatortext":
    $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_GROUP_MODERATOR_TEXT");
    break;
    
  case "add_header":
	// Hate this part, but it's the easiest way...
	$_REQUEST["attrib"] = "add_header";
	break;

  default:
    die(pql_complete_constant($LANG->_('Unknown attribute %attribute% in %file%'), array('attribute' => $_REQUEST["attrib"], 'file' => __FILE__)));
}
// }}}

if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
    $_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
    
    // {{{ Setup the information header
    if(lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR"))
      $what = $LANG->_('group moderator');
    elseif(lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER"))
      $what = $LANG->_('group member');
    elseif(lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_GROUP_DN_SENDER"))
      $what = $LANG->_('group allowed sender');
    elseif(lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_GROUP_CONFIRM_TEXT"))
      $what = $LANG->_('QmailGroup confirmation text');
    elseif(lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_GROUP_MODERATOR_TEXT"))
      $what = $LANG->_('QmailGroup moderator text');
    elseif(lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_SPAMASSASSIN"))
      $what = $LANG->_('Additional mail header');
    else
      $what = $LANG->_('alias');
// }}}

    // {{{ Delete the user attribute
	// Hate this part, but it's the easiest way...
	if($_REQUEST["attrib"] == 'add_header')
	  $_REQUEST["attrib"] = pql_get_define("PQL_ATTR_SPAMASSASSIN");

    if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $_REQUEST["oldvalue"], '')) {
	  $msg = pql_complete_constant($LANG->_('Successfully removed %what% %oldvalue%'), array("what" => $what, "oldvalue" => pql_maybe_idna_decode($_REQUEST["oldvalue"])));
	  $success = true;

	  if($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_MAILALTERNATE")) {
		// {{{ Make sure we enable local delivery (again).
		// Retreive the 'deliveryMode attribute(s) and remove the 'noprogram' value if it's there
		$modes = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_MODE"));
		if(!is_array($modes)) $modes = array($modes);
		if(in_array('noprogram', $modes))
		  pql_modify_attribute($_pql->ldap_linkid, urldecode($_REQUEST["user"]), pql_get_define("PQL_ATTR_MODE"), 'noprogram', '');
		
		// Retreive the 'qmailDotMode' attribute(s) and remove the 'none' value if it's there
		$dotmode = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"));
		if(!is_array($dotmode)) $dotmode = array($dotmode);
		if(in_array('none', $dotmode))
		  pql_modify_attribute($_pql->ldap_linkid, urldecode($_REQUEST["user"]), pql_get_define("PQL_ATTR_DOTMODE"), 'none', '');
// }}}
	  }
    } else {
	  $msg = pql_complete_constant($LANG->_('Failed to removed %what% %oldvalue%'), array("what" => $what, "oldvalue" => pql_maybe_idna_decode($_REQUEST["oldvalue"]))) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	  $success = false;
    }
// }}}
    
    // {{{ Remove any forwarders TO this mail address
    if (lc($_REQUEST["attrib"]) == pql_get_define("PQL_ATTR_MAILALTERNATE") and $success and isset($_REQUEST["delete_forwards"])) {
	  // does another account forward to this alias?
	  $sr = ldap_search($_pql->ldap_linkid, $_REQUEST["user"], "(|(" . pql_get_define("PQL_ATTR_FORWARDS") ."=" . $_REQUEST["oldvalue"] . "))");
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
// }}}
    
    // {{{ Redirect to users detail page
    $url = "user_detail.php?rootdn=" . urlencode($_REQUEST["rootdn"]) . "&domain=" . urlencode($_REQUEST["domain"])
      . "&user=" . urlencode($_REQUEST["user"]) . "&msg=" . urlencode($msg) . "&view=" . $_REQUEST["view"];
	
    if(pql_get_define("PQL_CONF_DEBUG_ME")) {
      echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
      die("<b>$url</b>");
    }
	
    pql_header($url);
// }}}
} else {
  // {{{ Verify attribute removal
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove attribute %attribute% for user %user%'), array('attribute' => $_REQUEST["attrib"], 'user' => $_REQUEST["user"])); ?></span>
  <br><br>
  <?=$LANG->_('Are you really sure')?>
  <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
    <input type="hidden" name="user" value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="domain" value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
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
// }}}
} // end of if
?>
</body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
