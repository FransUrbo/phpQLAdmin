<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
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
?>
  <span class="title1"><?=$LANG->_('phpQLAdmin configuration')?></span>

  <br><br>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td valign="bottom" align="left" width="100%" colspan="2"><a href="<?=$PHP_SELF."?view=default"?>"><img alt="/ <?=$LANG->_('Global configuration')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Global configuration')?>"></a><?php $i=1; foreach($_pql->ldap_basedn as $dn) { $dn = urldecode($dn); if(!($i%2)) { ?><br><?php } ?><a href="<?=$PHP_SELF."?view=branch&branch=$dn"?>"><img alt="/ <?=$LANG->_('Top branch')?> <?=$dn?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$dn?>"></a><?php $i++; } ?>
      </td>
    </tr>
  </table>

  <br>

<?php
if($view == '')
     $view = 'default';

if($view == 'default')
     include("./tables/config_details-global.inc");

if(($view == 'branch') and $branch)
     include("./tables/config_details-branch.inc");
?>
</body>
</html>
