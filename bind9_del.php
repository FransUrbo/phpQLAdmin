<?php
// remove a domain from a bind9 ldap db
// $Id: bind9_del.php,v 2.2 2004-02-14 14:01:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_bind9.inc");

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove DNS zone %domainname%'), array('domainname' => $dns_domain_name)); ?></span>
  <br><br>

<?php
if(($action == 'del') and ($type == 'domain') and $dns_domain_name and $domain) {
    if(isset($ok) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn)) {
		if(pql_bind9_del_zone($_pql->ldap_linkid, $domain, $dns_domain_name)) {
			$msg = $LANG->_('Successfully removed DNS zone') . ": <b>" . $cn . "</b>";
		} else {
			$msg .= "<br>".$LANG->_('Sorry, could not delete zone');
		}
		
		// redirect to domain-detail page
		$msg = urlencode($msg);
		$url = "domain_detail.php?rootdn=".urlencode($rootdn)."&domain=".urlencode($domain)."&view=$view&msg=$msg";
		header("Location: " . pql_get_define("PQL_GLOB_URI") . $url);
    } else {
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="GET">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="dns_domain_name" value="<?=$dns_domain_name?>">
    <input type="hidden" name="action" value="<?=$action?>">
    <input type="hidden" name="type" value="<?=$type?>">
    <input type="hidden" name="view" value="<?=$view?>">

    <span class="title2"><?=$LANG->_('Are you really sure')?>?</span>
    <input type="submit" name="ok"   value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back" value="<?=$LANG->_('No')?>" onClick="history.back();">
  </form>
<?php        
    }
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
