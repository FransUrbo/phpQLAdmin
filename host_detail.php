<?php
// View information about physical host object
// (mainly Host ACL's)
// $Id: host_detail.php,v 1.1.2.8 2006-11-17 17:27:57 turbo Exp $

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

// {{{ Print status message, if one is available
if(isset($_REQUEST["msg"])) {
    pql_format_status_msg($_REQUEST["msg"]);
}
// }}}

// {{{ Reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
	if($_REQUEST["rlnb"] == 1) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    // This doesn't work as it's supposed to... Don't know enough java to figure it out either...
    //refreshFrames();

    // This work perfectly though...
    parent.frames.pqlnavctrl.location.reload();
  //--></script>
<?php
	} elseif($_REQUEST["rlnb"] == 2) {
?>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    parent.frames.pqlnav.location.reload();
  //--></script>
<?php   }
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

if(($_REQUEST["server"] != 'Global') and !@$_REQUEST["virthost"] and @($_REQUEST["ref"] == 'left')) {
  // Do NOT show if:
  //	Left host frame/Webserver - Global
  //	Left host frame/[physical]/Webserver - Port xx
  //	Left host frame/[physical]/Webserver - Port xx/Virtual host
  $buttons = array();

  if(pql_get_define("PQL_CONF_HOSTACL_USE")) {
	$new = array('hostacl' => 'Host Control');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_AUTOMOUNT_USE")) {
	$new = array('automount' => 'Automount Information');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_CONTROL_USE")) {
	$new = array('mailsrv'   => 'Mailserver Administration');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
	$new = array('websrv' => 'Webserver Administration');
	$buttons = $buttons + $new;
  }				 

  if($_REQUEST["host"] != 'Global') {
	$new = array('action'  => 'Action');
	$buttons = $buttons + $new;
  }

  pql_generate_button($buttons, "host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"]); echo "    <br>\n";
}
// }}}

// {{{ Load the requested host details page
if($_REQUEST["view"] == 'hostacl') {
  include("./tables/host_details-acl.inc");
} elseif($_REQUEST["view"] == 'automount') {
  include("./tables/host_details-automount.inc");
} elseif($_REQUEST["view"] == 'mailsrv') {
  pql_header("control_detail.php?host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"]);
} elseif($_REQUEST["view"] == 'websrv') {
  include("./tables/host_details-websrv.inc");
} elseif($_REQUEST["view"] == 'action') {
  include("./tables/host_details-action.inc");
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
