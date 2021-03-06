<?php
// shows configuration of phpQLAdmin
// $Id: config_detail.php,v 2.69 2007-10-09 16:54:26 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");

// {{{ Possibly clear session
if(@$_REQUEST["action"] == "clear_session") {
    $view	= $_REQUEST["view"];

    // SOME variables should be remembered in the next session
    $user_host	= $_SESSION["USER_HOST"];
    $user_dn	= $_SESSION["USER_DN"];
    $user_pass	= $_SESSION["USER_PASS"];
    $user_ctrdn	= $_SESSION["USER_SEARCH_DN_CTR"];
    $advanced	= $_SESSION["ADVANCED_MODE"];

    // Destroy the current session
    $_SESSION = array();
    session_destroy();

	// Create a new session
	$_SESSION["initial_load"] = 1;
    require("./include/pql_session.inc");

	// Reset the values...
    $_SESSION["USER_HOST"]			= $user_host;
    $_SESSION["USER_DN"]			= $user_dn;
    $_SESSION["USER_PASS"]			= $user_pass;
    $_SESSION["USER_SEARCH_DN_CTR"]	= $user_ctrdn;
    $_SESSION["ADVANCED_MODE"]		= $advanced;

    $msg = "Successfully deleted the session variable. Will reload from scratch.";
    $link = "config_detail.php?view=$view&msg=".urlencode($msg);

	// TODO: Try to flush memcached cache...
	if(class_exists('Memcache') and function_exists("memcache_connect") and @$pql_cache)
	  $pql_cache->flush();

    pql_header($link);
}
// }}}

// {{{ Print status message, if one is available
if(isset($_REQUEST["msg"])) {
    pql_format_status_msg($_REQUEST["msg"]);
}
// }}}

// {{{ Reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}
// }}}

foreach($_SESSION["BASE_DN"] as $dn) {
    if(preg_match('/KERBEROS/i', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $dn)))
      $show_kerberos_info = 1;
}

// {{{ Create the button array with domain buttons
$buttons    = array('default'  => 'Global configuration');
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  $button   = array('template' => 'User templates');
  $buttons += $button;

  // {{{ Try to figure out if the ppolicy overlay is loaded
  require($_SESSION["path"]."/include/pql_status.inc");
  if(@$_pql->get_dn("cn=Overlay,".$_pql->ldap_monitor, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*')) {
	// OpenLDAP <2.4
	$overlays = pql_get_subentries($_pql->ldap_linkid, "cn=Overlay,".$_pql->ldap_monitor, "monitoredInfo", "cn=Overlay*");
  } elseif(@$_pql->get_dn("cn=Overlays,".$_pql->ldap_monitor, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*')) {
	// OpenLDAP >2.4
	$overlays = pql_get_subentries($_pql->ldap_linkid, "cn=Overlays,".$_pql->ldap_monitor, "monitoredInfo", "cn=Overlay*");
  } else
	$overlays = array();

  if(in_array('ppolicy', $overlays)) {
	$button   = array('ppolicy'  => 'Password Policies');
	$buttons += $button;
  }
  // }}}

  if(pql_get_define("PQL_CONF_RADIUS_USE")) {
	$button   = array('radius' => 'RADIUS Profiles');
	$buttons += $button;
  }
}
foreach($_SESSION["BASE_DN"] as $dn) {
  $button   = array($dn => $dn);
  $buttons += $button;
}
if($_SESSION["lynx"]) {
  $button   = array('index2' => 'Back to index');
  $buttons += $button;
}
?>
  <span class="title1"><?php echo $LANG->_('phpQLAdmin configuration')?></span>

  <br><br>

<?php
if(@empty($_REQUEST["view"])) {
  if(@$_REQUEST["rootdn"]) {
	// The branch links doesn't have the 'view=' value.
	// And instead of having the root DN twice (first
	// in the 'rootdn=' option and then in the 'view='),
	// 'convert' it here...
	$_REQUEST["view"] = $_REQUEST["rootdn"];
  } else {
	$_REQUEST["view"] = 'default';
  }
}

// Output the buttons to the browser
pql_generate_button($buttons);
// }}}

// {{{ Include table view
if($_REQUEST["view"] == 'default') {
    include("./tables/config_details-global.inc");
} elseif($_REQUEST["view"] == 'template') {
    include("./tables/config_details-template.inc");
} elseif($_REQUEST["view"] == 'ppolicy') {
    include("./tables/config_details-ppolicy.inc");
} elseif($_REQUEST["view"] == 'radius') {
    include("./tables/config_details-radius.inc");
} elseif($_SESSION["lynx"] and ($_REQUEST["view"] == 'index2')) {
    $link = "index2.php";
    if(isset($_SESSION["ADVANCED_MODE"]))
      $link .= "?advanced=1";
    else
      $link .= "?advanced=0";

    pql_header($link, 1);
} else {
    if (empty($_REQUEST["branch"])) {
      $_REQUEST["branch"] = $_REQUEST["view"];
    }
    include("./tables/config_details-branch.inc");
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
