<?php
// delete a domain and all users within
// $Id: domain_del.php,v 2.34 2005-03-04 11:55:32 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require("./include/pql_config.inc");

include($_SESSION["path"]."/header.html");

$domain = $_REQUEST["domain"];
// }}}

// {{{ Make sure we can have a ' in branch
$domain = eregi_replace("\\\'", "'", $domain);
$domain = urldecode($domain);
// }}}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove domain %domain%'),
														array("domain" => pql_maybe_decode($domain)))?></span>
  <br><br>
<?php
if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
	// {{{ Delete a branch and it's users etc from the database
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	$delete_forwards = (isset($_REQUEST["delete_forwards"]) || pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) ? true : false;

	// Before we delete the domain/branch, we need to get the defaultDomain, additionalDomainName
	// and smtpRoutes value(s) so that we can remove it from the QmailLDAP/Controls object(s)
	$domainname  = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	$additionals = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
	$routes		 = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_SMTPROUTES"));

	// Delete the domain
	if(pql_domain_del($_pql, $domain, $delete_forwards)) {
	    // {{{ Deletion of the branch was successfull - Update the QmailLDAP/Controls object(s)
		if(pql_get_define("PQL_CONF_CONTROL_USE")) {
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
// }}}
	    
		// {{{ Now it's time to run the special branch removal script
		if(pql_get_define("PQL_CONF_SCRIPT_DELETE_DOMAIN", $_REQUEST["rootdn"])) {
		  // Setup the environment with the user details
		  putenv("PQL_CONF_DOMAIN=$domain");
		  putenv("PQL_CONF_WEBUSER=".posix_getuid());
		  
		  if(pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH") and 
			 pql_get_define("PQL_CONF_KRB5_REALM") and
			 pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL") and
			 pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER") and 
			 pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB")) {
			putenv("PQL_KADMIN_CMD=".pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH")."/kadmin");
			putenv("PQL_KADMIN_REALM=".pql_get_define("PQL_CONF_KRB5_REALM"));
			putenv("PQL_KADMIN_PRINC=".pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL"));
			putenv("PQL_KADMIN_SERVR=".pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER"));
			putenv("PQL_KADMIN_KEYTB=".pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB"));
		  }
		  
		  // Execute the domain removal script (0 => show output)
		  if(pql_execute(pql_get_define("PQL_CONF_SCRIPT_DELETE_DOMAIN", $_REQUEST["rootdn"]), 0)) {
			echo pql_complete_constant($LANG->_('The %what% removal script failed'),
									   array('what' => $LANG->_('branch'))) . "!<br>";
			$msg = urlencode(pql_complete_constant($LANG->_('The %what% removal script failed'),
												   array('what' => $LANG->_('branch'))) ."!") . ".&nbsp;<br>";
		  } else {
			echo "<b>" . pql_complete_constant($LANG->_('Successfully executed the %what% removal script'),
											   array('what' => $LANG->_('branch'))) . "</b><br>";
			$msg = urlencode(pql_complete_constant($LANG->_('Successfully executed the %what% removal script'),
												   array('what' => $LANG->_('branch')))) . ".&nbsp;<br>";
		  }
		}
// }}}
	
	    // {{{ Redirect to home page
	    $msg = urlencode($LANG->_('Successfully removed the domain'));
		$link = "home.php?msg=$msg&rlnb=1";

		if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
		  echo "<br>If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
		  die("<b>".$link."<b>");
		} else
		  header("Location: " . $_SESSION["URI"] . $link);
		// }}}
	} else {
	    $msg = $LANG->_('Failed to remove the domain') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		
	    // redirect to domain detail page
	    $msg = urlencode($msg);
	    header("Location: " . $_SESSION["URI"] . "domain_detail.php?domain=$domain&msg=$msg");
	}
// }}}
} else {
	// {{{ Get confirmation and ask HOW to delete the branch
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
	// }}}
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
