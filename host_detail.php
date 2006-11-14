<?php
// View information about physical host object
// (mainly Host ACL's)
// $Id: host_detail.php,v 1.1.2.4 2006-11-14 21:19:19 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/left-head.html");
// }}}

// {{{ Retreive and setup physical hosts array
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

if(($_REQUEST["server"] != 'Global') and !@$_REQUEST["virthost"] and @($_REQUEST["ref"] != 'left2')) {
  // Do NOT show if:
  //	Left host frame/Webserver - Global
  //	Left host frame/[physical]/Webserver - Port xx
  //	Left host frame/[physical]/Webserver - Port xx/Virtual host
  $buttons = array('hostacl'   => 'Host Control',
				   'automount' => 'Automount Information',
				   'mailsrv'   => 'Mailserver Administration',
				   'websrv'    => 'Webserver Administration');
  pql_generate_button($buttons, "host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"]); echo "    <br>\n";
}
// }}}

// {{{ Load the requested domain details page
if($_REQUEST["view"] == 'hostacl') {
  include("./tables/host_details-acl.inc");
} elseif($_REQUEST["view"] == 'automount') {
  include("./tables/host_details-automount.inc");
} elseif($_REQUEST["view"] == 'mailsrv') {
  pql_header("control_detail.php?host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"]);
} elseif($_REQUEST["view"] == 'websrv') {
  include("./tables/host_details-websrv.inc");
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
