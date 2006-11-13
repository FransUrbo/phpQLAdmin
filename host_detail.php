<?php
// View information about physical host object
// (mainly Host ACL's)
// $Id: host_detail.php,v 1.1.2.1 2006-11-13 13:18:46 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/left-head.html");
// }}}

// {{{ Setup hosts array
if($_REQUEST["host"] == 'Global') {
  $hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
					  '(&(cn=*)(|(objectclass=ipHost)(objectclass=device)))',
					  'ONELEVEL');
  $host = 'Global';
} else {
  $hosts = array($_REQUEST["host"]);
  $host = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
}
// }}}
?>
    <link rel="stylesheet" href="tools/normal.css" type="text/css">
    <span class="title1"><?=$LANG->_('Computer')?>: <?=pql_maybe_idna_decode(urldecode($host))?></span>
    <p>
<?php
// {{{ Setup nav buttons
if(empty($_REQUEST["view"])) {
  $_REQUEST["view"] = 'hostacl';
}

$buttons = array();
if(pql_get_define("PQL_ATTR_SUDO_USE")) {
  $new = array('hostacl' => 'Host control');
  $buttons = $buttons + $new;
}
if(pql_get_define("PQL_ATTR_AUTOMOUNT_USE")) {
  $new = array('automount' => 'Automount information');
  $buttons = $buttons + $new;
}

pql_generate_button($buttons, "host=".urlencode($_REQUEST["host"])); echo "    <br>\n";
// }}}

include("./tables/host_details-acl.inc");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
