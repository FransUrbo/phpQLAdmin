<?php
// Modify Sudo roles
// $Id: sudo_modify.php,v 2.9 2007-02-26 09:44:44 turbo Exp $

// {{{ Setup session etc
// Called directly - as in modifying something with a specific sudo role
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/attrib.sudoers.inc");
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to users detail page
function attribute_forward($msg) {
  $url  = "domain_detail.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
  $url .= "&view=".$_REQUEST["view"]."&msg=$msg";
  
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
    echo "<p>If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
    die("<b>".$_SESSION["URI"].$url."</b>");
  } else
    pql_header($url);
}
// }}}

// Get the role name for the header
$role_name = $_pql->get_attribute($_REQUEST["sudorole"], pql_get_define("PQL_ATTR_CN"));
?>
    <span class="title1"><?=pql_complete_constant($LANG->_('Modify sudo role %role%'), array('role' => $role_name))?></span>
    <br><br>
<?php
// {{{ Select what to do
if(!@$_REQUEST["action"]) {
  attribute_print_view($_REQUEST["sudorole"]);
} else {
  if($_REQUEST["action"] == 'del') {
    // Remove 'something' ($_REQUEST["type"] and $_REQUEST[$type]) from a sudo role
    attribute_save("del");
  } else {
    if(!attribute_check())
      attribute_print_form();
    else
      attribute_save("mod");
  }
}
// }}}
?>
  </body>
</html>

<?php
/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
