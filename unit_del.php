<?php
// delete a domain and all users within
// $Id: unit_del.php,v 2.15.2.1 2005-03-04 11:59:45 turbo Exp $
//
require("./include/pql_session.inc");
require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");

include($_SESSION["path"]."/header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove the domain %domain%'), array("domain" => $domain))?></span>
<?php
	if($ok != 1) {
?>
<br>
<br>
<img src="images/info.png" width="16" height="16" border="0">
<?=$LANG->_('Attention: If you deleted a domain, all users within this domain will be deleted too')?>!
<br>
<br>
<?=$LANG->_('Are you really sure')?>?
<br>
<a href="domain_del.php?domain=<?=$domain?>&unit=<?=$unit?>&ok=1"><?=$LANG->_('Yes')?></a>, <a href="javascript:history.back()"><?=$LANG->_('No')?></a>
<br>
<?php
  } else {
      $_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
      $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

      // delete the unit 
      if(pql_remove_unit($_pql->ldap_linkid, $domain, $unit)) {
	  // update locals if control patch is enabled
	  if(pql_control_update_domains($_pql, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"])) {
	      // message ??
	  }
	  
	  // redirect to home page
	  $msg = $LANG->_('Successfully removed the domain');
	  $msg = urlencode($msg);
	  header("Location: home.php?msg=$msg&rlnb=1");
      } else {
	  $msg = $LANG->_('Failed to remove the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	  // redirect to domain detail page
	  $msg = urlencode($msg);
	  header("Location: domain_detail.php?domain=$domain&unit=$unit&msg=$msg");
      }
  } // end of if
?>
</body>
</html>
