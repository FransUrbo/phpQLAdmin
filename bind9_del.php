<?php
// remove a domain from a bind9 ldap db
// $Id: bind9_del.php,v 2.4 2004-11-17 07:30:06 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_bind9.inc");

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove DNS zone %domainname%'), array('domainname' => $_REQUEST["dns_domain_name"])); ?></span>
  <br><br>

<?php
if(($_REQUEST["action"] == 'del') and ($_REQUEST["type"] == 'domain') and $_REQUEST["dns_domain_name"] and $_REQUEST["domain"]) {
    if(isset($ok) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
		if(pql_bind9_del_zone($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["dns_domain_name"])) {
			$msg = $LANG->_('Successfully removed DNS zone') . ": <b>" . $cn . "</b>";
		} else {
			$msg .= "<br>".$LANG->_('Sorry, could not delete zone');
		}
		
		// redirect to domain-detail page
		$url  = "domain_detail.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
		$url .= "&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
		header("Location: " . pql_get_define("PQL_CONF_URI") . $url);
    } else {
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>"      method="GET">
    <input type="hidden" name="rootdn"          value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain"          value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="dns_domain_name" value="<?=$_REQUEST["dns_domain_name"]?>">
    <input type="hidden" name="action"          value="<?=$_REQUEST["action"]?>">
    <input type="hidden" name="type"            value="<?=$_REQUEST["type"]?>">
    <input type="hidden" name="view"            value="<?=$_REQUEST["view"]?>">

    <span class="title2"><?=$LANG->_('Are you really sure')?>?</span>
    <input type="submit" name="ok"              value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back"            value="<?=$LANG->_('No')?>" onClick="history.back();">
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
