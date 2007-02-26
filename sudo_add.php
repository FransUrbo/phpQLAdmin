<?php
// add sudo role
// $Id: sudo_add.php,v 2.1 2007-02-26 09:44:44 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_sudoers.inc");

include($_SESSION["path"]."/header.html");
require($_SESSION["path"]."/include/attrib.sudoers.inc");

// Load sudoers information
require($_SESSION["path"]."/sudo_detail.php");
// }}}

// {{{ Forward back to domain detail page
function attribute_forward($msg) {
  $url["domain"] = pql_format_urls($_REQUEST["domain"]);
  $url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
  
  // URL Encode the DN values
  $msg = urlencode($msg);
  
  $LINK_URL = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
  $LINK_URL .= "&view=".$_REQUEST["view"]."&msg=$msg";
  
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	die("<b>$LINK_URL</b>");
  } else
	pql_header($LINK_URL);
}
// }}}
?>
    <span class="title1"><?=pql_complete_constant($LANG->_('Add sudo role to branch %branch%'), array('branch' => $_REQUEST["domain"]))?></span>
    <br><br>

<?php
// {{{ Select what to do
if(@$_REQUEST["dosave"]) {
  if(attribute_check()) {
	attribute_save($_REQUEST["action"]);
  } else {
	attribute_print_form();
  }
} else {
  attribute_print_form();
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
