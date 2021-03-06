<?php
// edit attributes of a webserver configuration
// $Id: websrv_edit_attributes.php,v 2.17 2007-03-14 12:10:53 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_websrv.inc");

include($_SESSION["path"]."/header.html");
include($_SESSION["path"]."/include/attrib.websrv.inc");

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);
}
// }}}

// {{{ Forward back to domain detail page
function attribute_forward($msg) {
	$url["domain"] = pql_format_urls($_REQUEST["domain"]);
	$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

    $server = ldap_explode_dn(urldecode($_REQUEST["server"]), 0);
    $server = preg_replace("cn=", "", $server[0]);

    // URL Encode the DN values
    $msg    = urlencode($msg);

//    $LINK_URL  = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&server=$server";
 	$LINK_URL = "host_detail.php?host=".$_REQUEST["host"]."&server=".$_REQUEST["server"];
	if($_REQUEST["virthost"]) $LINK_URL .= "&virthost=".$_REQUEST["virthost"];
	if($_REQUEST["hostdir"])  $LINK_URL .= "&hostdir=".$_REQUEST["hostdir"];
	$LINK_URL .= "&view=".$_REQUEST["view"]."&msg=$msg";

	pql_header($LINK_URL);
}
// }}}
?>
    <span class="title1"><?php echo $LANG->_('Change a weberver configuration value')?></span>
    <br><br>

<?php
// {{{ Select what to do
if(@$_REQUEST["submit"] or ($_REQUEST["action"] == 'del')) {
    if(attribute_check())
      attribute_save($_REQUEST["action"]);
    else
      attribute_print_form();
} elseif(!empty($_REQUEST["toggle"])) {
  // No point in checking a toggle...
  attribute_save('toggle');
} else {
    attribute_print_form();
}
// }}}
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
