<?php
// View information about physical host object
// (mainly Host ACL's)
// $Id: host_detail.php,v 2.7 2007-03-14 12:10:52 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_websrv.inc");
require($_SESSION["path"]."/left-head.html");
// }}}

// {{{ Retreive and setup physical hosts array
if($_REQUEST["host"] == 'Global') {
  $hosts = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
					  '(&('.pql_get_define("PQL_ATTR_CN").'=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))',
					  'ONELEVEL');
  $host = 'Global';
} else {
  $hosts = array($_REQUEST["host"]);
  $host = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
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
if($_REQUEST["host"] == 'Global') {
  // {{{ Retreive all physical hosts
  $hosts = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
						 '(&('.pql_get_define("PQL_ATTR_CN").'=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))',
						 'ONELEVEL');
// }}}

  // {{{ Add a 'Global' branch as first branch
  if($_SESSION["ALLOW_BRANCH_CREATE"] or $controls_admin) {
	// Only do this if:
	//	1. User is super admin
	//	2. User is global controls administrator
	$tmp[] = "Global";
	foreach($hosts as $host)
	  $tmp[] = $host;
	$hosts = $tmp;
  }
// }}}
} else {
  $hosts = array($_REQUEST["host"]);
}

// {{{ Check if user is controls admin in any of the root DN's
$controls_admin = 0;
foreach($_pql->ldap_basedn as $dn)  {
  $dn = pql_format_normalize_dn($dn);
  if(pql_validate_administrator($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"), $_SESSION["USER_DN"]))
	$controls_admin = 1;
}
// }}}

// {{{ For each physical host, find web containers and their virtual hosts
$DATA = array();
foreach($hosts as $host_dn) {
  $tmp = pql_websrv_get_data($host_dn);
  if(is_array($tmp)) {
	foreach($tmp as $host => $data)
	  $DATA[$host] = $data;
  } else {
	// This host does not have any web containers, but we still want it
	// in the list - add it 'empty'
	$host = $_pql->get_attribute($host_dn, pql_get_define("PQL_ATTR_CN"));
	$DATA[$host_dn] = array();
  }
}
// }}}

// {{{ Setup default view if not set
if(empty($_REQUEST["view"])) {
  if(pql_get_define("PQL_CONF_HOSTACL_USE")) {
	$_REQUEST["view"] = 'hostacl';
  } elseif(pql_get_define("PQL_CONF_AUTOMOUNT_USE")) {
	$_REQUEST["view"] = 'automount';
  } elseif(pql_get_define("PQL_CONF_CONTROL_USE") and ($_SESSION["ALLOW_BRANCH_CREATE"] or $controls_admin)) {
	// Only do this if:
	//	1.  Mail server administration is on
	//	2a. User is super admin
	//	2b. User is global controls administrator
	$_REQUEST["view"] = 'mailsrv';
  } elseif(pql_get_define("PQL_CONF_WEBSRV_USE") and ($_SESSION["ALLOW_BRANCH_CREATE"] or is_array($DATA))) {
	// Only do this if:
	//	1.  Mail server administration is on
	//	2a. User is controls administrator OR
	//	2b. User is super admin
	$_REQUEST["view"] = 'websrv';

  } elseif(pql_get_define("PQL_CONF_RADIUS_USE")) {
	$_REQUEST["view"] = 'radius';
  }
}
// }}}

// {{{ Setup nav buttons
if(($_REQUEST["server"] != 'Global') and
   (($_REQUEST["ref"] == 'physical') or (($_REQUEST["host"] == 'Global') and ($_REQUEST["ref"] == 'global'))))
{
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

  if(pql_get_define("PQL_CONF_CONTROL_USE") and ($_SESSION["ALLOW_BRANCH_CREATE"] or $controls_admin)) {
	// Only do this if:
	//	1.  Mail server administration is on
	//	2a. User is super admin
	//	2b. User is global controls administrator
	$new = array('mailsrv'   => 'Mailserver Administration');
	$buttons = $buttons + $new;
  }

  if(pql_get_define("PQL_CONF_WEBSRV_USE") and ($_SESSION["ALLOW_BRANCH_CREATE"] or $DATA["access"])) {
	$new = array('websrv' => 'Webserver Administration');
	$buttons = $buttons + $new;
  }				 

  if(pql_get_define("PQL_CONF_RADIUS_USE")) {
	$new = array('radius' => 'RADIUS Administration');
	$buttons = $buttons + $new;
  }				 

  if(pql_get_define("PQL_CONF_SUDO_USE")) {
	$new = array('sudo' => 'Sudo Administration');
	$buttons = $buttons + $new;
  }				 

  if(pql_get_define("PQL_ATTR_BIND9_USE") and ($_REQUEST["host"] == 'Global')) {
	$new = array('dns' => 'DNS Administration');
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
  pql_header("control_detail.php?host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"], 1);
} elseif($_REQUEST["view"] == 'websrv') {
  include("./tables/host_details-websrv.inc");
} elseif($_REQUEST["view"] == 'radius') {
  include("./tables/host_details-radius.inc");
} elseif($_REQUEST["view"] == 'sudo') {
  include("./sudo_detail.php");
} elseif($_REQUEST["view"] == 'dns') {
  include("./tables/host_details-dns.inc");
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
