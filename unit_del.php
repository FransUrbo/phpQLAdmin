<?php
// delete a domain and all users within
// unit_del.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant(PQL_LANG_DOMAIN_DEL_TITLE, array("domain" => $domain))?></span>
<?php
	if($ok != 1){
?>
<br>
<br>
<img src="images/info.png" width="16" height="16" border="0">
<?php echo PQL_LANG_DOMAIN_DEL_WARNING; ?>
<br>
<br>
<?php echo PQL_LANG_SURE; ?>
<br>
<a href="domain_del.php?domain=<?php echo $domain; ?>&unit=<?php echo $unit; ?>&ok=1"><?php echo PQL_LANG_YES; ?></a>, <a href="javascript:history.back()"><?php echo PQL_LANG_NO; ?></a>
<br>
<?php
  } else {
      $_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
      $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

      // delete the unit 
      if(pql_remove_unit($_pql->ldap_linkid, $domain, $unit)){
	  // update locals if control patch is enabled
	  if(pql_control_update_domains($_pql, $USER_SEARCH_DN_CTR)) {
	      // message ??
	  }
	  
	  // redirect to home page
	  $msg = PQL_LANG_DOMAIN_DEL_OK;
	  $msg = urlencode($msg);
	  header("Location: home.php?msg=$msg&rlnb=1");
      } else {
	  $msg = PQL_LANG_DOMAIN_DEL_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	  // redirect to domain detail page
	  $msg = urlencode($msg);
	  header("Location: domain_detail.php?domain=$domain&unit=$unit&msg=$msg");
      }
  } // end of if
?>
</body>
</html>
