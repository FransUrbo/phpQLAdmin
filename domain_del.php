<?php
// delete a domain and all users within
// $Id: domain_del.php,v 1.2 2002-12-12 11:50:27 turbo Exp $
//
session_start();
require("pql.inc");
require("pql_control.inc");

include("header.html");
?>
  <span class="title1"><?php echo pql_complete_constant(PQL_DOMAIN_DEL_TITLE, array("domain" => $domain))?></span>
<?php
    if(isset($ok) || PQL_VERIFY_DELETE){
	$_pql = new pql();
	$_pql_control = new pql_control();
	
	$delete_forwards = (isset($delete_forwards)) ? true : false;
	
	// delete the domain
	if(pql_remove_domain($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $delete_forwards)) {
	    // update locals if control patch is enabled
	    if(pql_control_update_domains($_pql->ldap_linkid, PQL_LDAP_BASEDN, $_pql_control->ldap_linkid,
					  PQL_LDAP_CONTROL_BASEDN)){
		// message ??
	    }
	    
	    $msg = PQL_DOMAIN_DEL_OK;

	    // redirect to home page
	    $msg = urlencode($msg);
	    header("Location: " . PQL_URI . "home.php?msg=$msg&rlnb=1");
	} else {
	    $msg = PQL_DOMAIN_DEL_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	    // redirect to domain detail page
	    $msg = urlencode($msg);
	    header("Location: " . PQL_URI . "domain_detail.php?domain=$domain&msg=$msg");
	}
    } else {
?>
<br>
<br>
<img src="images/info.png" width="16" height="16" border="0">
<?php echo PQL_DOMAIN_DEL_WARNING; ?>
<br>
<br>
<?php echo PQL_SURE; ?>
<br>
<form action="<?php echo $PHP_SELF; ?>" method="GET">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	
	<input type="checkbox" name="delete_forwards" checked> <?php echo PQL_DOMAIN_DEL_FORWARDS; ?><br><br>
	<input type="submit" name="ok" value="<?php echo PQL_YES; ?>">
	<input type="button" name="back" value="<?php echo PQL_NO; ?>" onClick="history.back();">
</form>
<br>
<?php
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
