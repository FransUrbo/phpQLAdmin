<?php
// shows configuration of phpQLAdmin
// $Id: config_detail.php,v 2.48.2.2 2003-12-17 21:50:45 dlw Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// print status message, if one is available
if(isset($_REQUEST["msg"])) {
    pql_format_status_msg($_REQUEST["msg"]);
}

// reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}

foreach($_pql->ldap_basedn as $dn) {
    $dn = urldecode($dn);

    if(eregi('KERBEROS', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $dn)))
      $show_kerberos_info = 1;
}

// Create the button array with domain buttons
$buttons = array('default' => 'Global configuration');
foreach($_pql->ldap_basedn as $dn) {
    $button = array($dn => urldecode($dn));
    $buttons += $button;
}
?>
  <span class="title1"><?=$LANG->_('phpQLAdmin configuration')?></span>

  <br><br>

<?php
// Output the buttons to the browser
pql_generate_button($buttons);

if(empty($_REQUEST["view"]) or $_REQUEST["view"] == 'default') {
    include("./tables/config_details-global.inc");
} else {
    if (empty($_REQUEST["branch"])) {
      $_REQUEST["branch"] = $_REQUEST["view"];
    }
    include("./tables/config_details-branch.inc");
}
?>
</body>
</html>
