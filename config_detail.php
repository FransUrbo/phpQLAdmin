<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 2.51 2004/03/11 18:13:32 turbo Exp
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

if($_REQUEST["action"] == "clear_session") {
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

    // Start a new session
    session_start();
    $_SESSION["USER_HOST"]		= $user_host;
    $_SESSION["USER_DN"]		= $user_dn;
    $_SESSION["USER_PASS"]		= $user_pass;
    $_SESSION["USER_SEARCH_DN_CTR"]	= $user_ctrdn;
    $_SESSION["ADVANCED_MODE"]		= $advanced;

    $msg = "Successfully deleted the session variable. Will reload from scratch.";
    $link = "config_detail.php?view=$view&msg=".urlencode($msg);

    header("Location: " . pql_get_define("PQL_CONF_URI") . $link);
}

// print status message, if one is available
if(isset($_REQUEST["msg"])) {
    pql_format_status_msg($_REQUEST["msg"]);
}

// reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
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
if($_SESSION["lynx"]) {
    $button = array('index2' => 'Back to index');
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
} elseif($_SESSION["lynx"] and ($_REQUEST["view"] == 'index2')) {
    $link = "index2.php";
    if(isset($_SESSION["ADVANCED_MODE"]))
      $link .= "?advanced=1";

    header("Location: " . pql_get_define("PQL_CONF_URI") . $link);
} else {
    if (empty($_REQUEST["branch"])) {
      $_REQUEST["branch"] = $_REQUEST["view"];
    }
    include("./tables/config_details-branch.inc");
}
?>
</body>
</html>
