<?php
// shows configuration of phpQLAdmin
// $Id: config_detail.php,v 2.47 2003-11-16 09:05:17 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// print status message, if one is available
if(isset($msg)) {
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
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

if(($view == '') or ($view == 'default')) {
    include("./tables/config_details-global.inc");
} else {
    $branch = $view;
    include("./tables/config_details-branch.inc");
}
?>
</body>
</html>
