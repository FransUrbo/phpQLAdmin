<?php
// delete a domain and all users within
// $Id: domain_del.php,v 2.31.8.1 2005-02-12 05:19:12 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include($_SESSION["path"]."/header.html");

$domain = $_REQUEST["domain"];

// Make sure we can have a ' in branch
$domain = eregi_replace("\\\'", "'", $domain);
$domain = urldecode($domain);
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove domain %domain%'),
														array("domain" => pql_maybe_decode($domain)))?></span>
<?php
if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	$delete_forwards = (isset($_REQUEST["delete_forwards"]) || pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) ? true : false;

	// Before we delete the domain/branch, we need to get the defaultDomain, additionalDomainName
	// and smtpRoutes value(s) so that we can remove it from the QmailLDAP/Controls object(s)
	$domainname  = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	$additionals = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
	$routes		 = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_SMTPROUTES"));

	// delete the domain
	if(pql_domain_del($_pql, $domain, $delete_forwards)) {
	    // Deletion of the branch was successfull - Update the QmailLDAP/Controls object(s)

		if(pql_get_define("PQL_CONF_CONTROL_USE")) {
			// -------------------------
			// Include control API
			include($_SESSION["path"]."/include/pql_control.inc");
			$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

			// -------------------------
			// Update the QLC object(s):

			// ... remove the domain name
			if($domainname)
			  // NOTE: Even if autoreplication is disabled, this should still be done!
			  pql_control_update_domains($_pql, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
										 '*', array($domainname, ''));
			
			// ... remove the additional domain name(s)
			if(is_array($additionals)) {
				foreach($additionals as $additional)
				  pql_control_update_domains($_pql, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
											 '*', array($additional, ''));
			}
			
			// ... remove the SMTP route(s)
			if(is_array($routes)) {
				foreach($routes as $route)
				  pql_control_update_domains($_pql, $_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"],
											 '*', array($route, ''));
			}
		}
	    
	    $msg = $LANG->_('Successfully removed the domain');
		
	    // redirect to home page
	    $msg = urlencode($msg);
	    header("Location: " . $_SESSION["URI"] . "home.php?msg=$msg&rlnb=1");
	} else {
	    $msg = $LANG->_('Failed to remove the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		
	    // redirect to domain detail page
	    $msg = urlencode($msg);
	    header("Location: " . $_SESSION["URI"] . "domain_detail.php?domain=$domain&msg=$msg");
	}
} else {
?>
<br>
<br>
<img src="images/info.png" width="16" height="16" border="0">
<?php echo $LANG->_('Attention: If you delete a domain, all users, DNS zones etc within this domain will be deleted too'); ?>!
<br>
<br>
<?php echo $LANG->_('Are you really sure'); ?>
<br>
<form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
	<input type="hidden" name="domain" value="<?=urlencode($domain)?>">
	
	<input type="checkbox" name="delete_forwards" checked> <?php echo $LANG->_('Also delete forwards to users in this domain'); ?><br><br>
	<input type="submit" name="ok" value="<?php echo $LANG->_('Yes'); ?>">
	<input type="button" name="back" value="<?php echo $LANG->_('No'); ?>" onClick="history.back();">
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
