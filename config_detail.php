<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 2.51 2004/03/11 18:13:32 turbo Exp
//
session_start();
require("./include/pql_config.inc");

include($_SESSION["path"]."/header.html");

// {{{ Possibly clear session
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

    header("Location: " . $_SESSION["URI"] . $link);
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
    if(eregi('KERBEROS', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $dn)))
      $show_kerberos_info = 1;
}

// {{{ Create the button array with domain buttons
$buttons    = array('default'  => 'Global configuration');
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  $button   = array('template' => 'User templates');
  $buttons += $button;
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
  <span class="title1"><?=$LANG->_('phpQLAdmin configuration')?></span>

  <br><br>

<?php
// Output the buttons to the browser
pql_generate_button($buttons);
// }}}

// {{{ Include table view
if(empty($_REQUEST["view"]) or $_REQUEST["view"] == 'default') {
    include("./tables/config_details-global.inc");
} elseif($_REQUEST["view"] == 'template') {
    include("./tables/config_details-template.inc");
} elseif($_SESSION["lynx"] and ($_REQUEST["view"] == 'index2')) {
    $link = "index2.php";
    if(isset($_SESSION["ADVANCED_MODE"]))
      $link .= "?advanced=1";
    else
      $link .= "?advanced=0";

    header("Location: " . $_SESSION["URI"] . $link);
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
