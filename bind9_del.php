<?php
// remove a domain from a bind9 ldap db
// $Id: bind9_del.php,v 2.13 2007-02-15 12:07:09 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_bind9.inc");

include($_SESSION["path"]."/header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove DNS zone %domainname%'), array('domainname' => $_REQUEST["dns_domain_name"])); ?></span>
  <br><br>

<?php
if(($_REQUEST["action"] == 'del') and ($_REQUEST["type"] == 'domain') and $_REQUEST["dns_domain_name"] and $_REQUEST["domain"]) {
    if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
		if(pql_bind9_del_zone($_REQUEST["domain"], $_REQUEST["dns_domain_name"])) {
			$msg = $LANG->_('Successfully removed DNS zone') . ": <b>" . $cn . "</b>";
		} else {
			$msg .= "<br>".$LANG->_('Sorry, could not delete zone');
		}
		
		// redirect to domain-detail page
		$url  = "domain_detail.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
		$url .= "&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
		pql_header($url);
    } else {
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>"      method="GET">
    <input type="hidden" name="rootdn"          value="<?php echo $_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain"          value="<?php echo $_REQUEST["domain"]?>">
    <input type="hidden" name="dns_domain_name" value="<?php echo $_REQUEST["dns_domain_name"]?>">
    <input type="hidden" name="action"          value="<?php echo $_REQUEST["action"]?>">
    <input type="hidden" name="type"            value="<?php echo $_REQUEST["type"]?>">
    <input type="hidden" name="view"            value="<?php echo $_REQUEST["view"]?>">

    <span class="title2"><?php echo $LANG->_('Are you really sure')?>?</span>
    <input type="submit" name="ok"              value="<?php echo $LANG->_('Yes')?>">
    <input type="button" name="back"            value="<?php echo $LANG->_('No')?>" onClick="history.back();">
  </form>
<?php        
    }
}
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
