<?php
// start page
// home.php,v 1.3 2002/12/13 13:50:12 turbo Exp
//
session_start();

require("./include/pql.inc");

include("./header.html");

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}
// reload navigation bar if needed
if(isset($rlnb) and PQL_AUTO_RELOAD) {
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

	$USER_HOST_USR = $host[0] . ";" . $host[1];
	$USER_SEARCH_DN_USR = $host[2];

	$USER_HOST_CTR = $host[0] . ";" . $host[1];
	$USER_SEARCH_DN_CTR = $host[3];

	session_register("USER_HOST_USR", "USER_HOST_CTR", "USER_SEARCH_DN_USR", "USER_SEARCH_DN_CTR");
    }

    // We need to disable advanced mode so that only the user frame
    // is shown, hence no 'advanced=...' in the url.
    header("Location: " . PQL_URI . "index2.php");
}

// Get release version
$fp = fopen(".version", "r");
$VERSION = fgets($fp, 20);
fclose($fp);
?>

  <br><span class="title1"><?=PQL_DESCRIPTION?> - v<?=$VERSION?></span><br>

  <ul>
<?php
	// TODO: How do we know if the user is allowed to add domains?
	//       In the domain description we don't have that info...
	// if(($ADVANCED_MODE == 1) && ($USER_BASE == 'everything')){
	if($ADVANCED_MODE == 1) {
	    // Should we show the 'change server' choices
	    if(PQL_LDAP_CHANGE_SERVER and eregi(" ", PQL_LDAP_HOST)) {
		$servers = split(" ", PQL_LDAP_HOST);
?>
    <li>
      <form action="<?=$PHP_SELF?>" target="_top">
	Change LDAP server<br>
        <select name="ldapserver">
<?php
		foreach($servers as $server) {
		    // We're only interssted in the HOST entry (othervise the list will be to long)
		    $host = split(';', $server);
?>
          <option value="<?=$server?>"><?=$host[0]?></option>
<?php
		}
?>
        </select>
        <input type="submit" value="<?="--&gt;&gt;"?>" name="submit">
      </form>
    </li>

<?php
	    }
	}
?>
    <!-- begin search engine snippet -->
    <li>
      <form method=post action="search.php">
        <?php echo PQL_SEARCH_TITLE; ?>
        <br>
        <select name=attribute>
          <option value=<?php echo PQL_LDAP_ATTR_UID; ?> SELECTED><?php echo PQL_SEARCH_UID; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_CN; ?>><?php echo PQL_SEARCH_CN; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_MAIL; ?>><?php echo PQL_SEARCH_MAIL; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_MAILALTERNATE; ?>><?php echo PQL_SEARCH_MAILALTERNATEADDRESS; ?></option>
          <option value=<?php echo PQL_LDAP_ATTR_FORWARDS; ?>><?php echo PQL_SEARCH_MAILFORWARDINGADDRESS; ?></option>
        </select>

        <select name=filter_type>
       	  <option value=contains SELECTED><?php echo PQL_SEARCH_CONTAINS; ?></option>
          <option value=is><?php echo PQL_SEARCH_IS; ?></option>
          <option value=starts_with><?php echo PQL_SEARCH_STARTSWITH; ?></option>
          <option value=ends_with><?php echo PQL_SEARCH_ENDSWITH; ?></option>
        </select>

	<br>

        <input type="text" name="search_string" size="32">
        <input type="submit" value="<?php echo PQL_SEARCH_FINDBUTTON; ?>">
      </form>
    </li>
    <!-- end of search engine snippet -->
  </ul>
  <br>

<?php include("./trailer.html"); ?>
</body>
</html>
