<?php
// start page
// $Id: home.php,v 2.31 2003-11-14 11:55:52 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
<?php
  // Reload different nav frame depending on value of $rlnb
  if($rlnb == 2) {
?>
	parent.frames.pqlnavctrl.location.reload();
<?php
  } elseif($rlnb == 3) {
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
    $ADVANCED_MODE = 0;
    session_register("ADVANCED_MODE");

    // Change LDAP server
    if($ldapserver) {
	$host = split(';', $ldapserver);

	$USER_HOST = $host[0] . ";" . $host[1] . ";" . $host[2];

	session_register("USER_HOST", "USER_SEARCH_DN_CTR");
    }

    // We need to disable advanced mode so that only the user frame
    // is shown, hence no 'advanced=...' in the url.
    header("Location: " . pql_get_define("PQL_GLOB_URI") . "index2.php");
}
?>

  <br><span class="title1"><?php echo pql_get_define("PQL_GLOB_WHOAREWE"); ?><?php if($ADVANCED_MODE) { ?><br>Version: <?=$VERSION?><?php } ?></span><br>

  <ul>
<?php
	if($ADVANCED_MODE == 1) {
	    // Should we show the 'change server' choices
	    if(pql_get_define("PQL_GLOB_CHANGE_SERVER") and eregi('\+', pql_get_define("PQL_GLOB_HOST"))) {
		$servers = split('\+', pql_get_define("PQL_GLOB_HOST"));
?>
    <li>
      <form action="<?=$PHP_SELF?>" target="_top">
	Change LDAP server<br>
        <select name="ldapserver">
<?php		foreach($servers as $server) {
			$host = split(';', $server);
?>
          <option value="<?=$server?>"><?=$host[0]?>:<?=$host[1]?></option>
<?php		} ?>
        </select>
        <input type="submit" value="<?="--&gt;&gt;"?>" name="submit">
      </form>
    </li>

<?php
	    }
	}
?>
  </ul>
  <br>

<?php
if($ALLOW_BRANCH_CREATE and $ADVANCED_MODE) {
    include("./trailer.html");
}
?>
</body>
</html>
