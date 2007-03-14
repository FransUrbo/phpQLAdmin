<?php
// start page
// $Id: home.php,v 2.45 2007-03-14 12:10:51 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

include($_SESSION["path"]."/header.html");

// print status message, if one is available
if(isset($msg)){
    pql_format_status_msg($msg);
}

// reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
<?php
  // Reload different nav frame depending on value of $rlnb
  if($_REQUEST["rlnb"] == 2) {
?>
	parent.frames.pqlnavctrl.location.reload();
<?php
  } elseif($_REQUEST["rlnb"] == 3) {
?>
	parent.frames.pqlnavezmlm.location.reload();
<?php
  } else {
?>
	parent.frames.pqlnav.location.reload();
<?php
  }
?>
  //--></script>
<?php
}

// find out which LDAP server(s) to use
if(isset($submit)) {
  $_SESSION["ADVANCED_MODE"] = 0;
  
  // Change LDAP server
  if($ldapserver) {
	$host = split(';', $ldapserver);
	
	$_SESSION["USER_HOST"] = $host[0] . ";" . $host[1] . ";" . $host[2];
  }
  
  // We need to disable advanced mode so that only the user frame
  // is shown, hence no 'advanced=...' in the url.
  pql_header("index2.php", 1);
}
?>

  <br><span class="title1"><?php echo pql_get_define("PQL_CONF_WHOAREWE"); ?><?php if($_SESSION["ADVANCED_MODE"]) { ?><br>Version: <?=$_SESSION["VERSION"]?><?php } ?></span><p>
<?php
if($_SESSION["ADVANCED_MODE"] == 1) {
  // Should we show the 'change server' choices
  if(pql_get_define("PQL_CONF_CHANGE_SERVER") and eregi('\+', pql_get_define("PQL_CONF_HOST"))) {
	$servers = split('\+', pql_get_define("PQL_CONF_HOST"));
?>

  <ul>
    <li>
      <form action="<?=$_SERVER["PHP_SELF"]?>" target="_top">
	Change LDAP server<br>
        <select name="ldapserver">
<?php	foreach($servers as $server) {
			$host = split(';', $server);
?>
          <option value="<?=$server?>"><?=pql_maybe_idna_decode(urldecode($host[0]))?><?php if(!eregi('^ldapi:', $host[0])) { echo ":".$host[1]; } ?></option>
<?php	} ?>
        </select>
        <input type="submit" value="<?="--&gt;&gt;"?>" name="submit">
      </form>
    </li>

  </ul>

  <p>
<?php
  }
}

if($_SESSION["ALLOW_BRANCH_CREATE"] and $_SESSION["ADVANCED_MODE"]) {
  include("./trailer.html");
}
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
