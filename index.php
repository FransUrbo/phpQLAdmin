<?php
// logins to the system
// $Id: index.php,v 2.31 2004-02-14 14:01:00 turbo Exp $
//
// Start debuging
// http://www.linuxjournal.com/article.php?sid=7213&mode=thread&order=0
//apd_set_pprof_trace();

require_once("./dlw_porting.inc");

session_start();
require("./include/pql_config.inc");

// DLW: I'm not sure if $msg ever gets set in a _POST, but for now I'll play it safe.
if (!empty($_POST["msg"])) {
		$log = date("M d H:i:s");
		$log .= " : Unexpected _POST[msg])\n";
		error_log($log, 3, "phpQLadmin.log");
}

// DLW: I think !empty($_GET["logout"]) will work here better than $_GET["logout"] == 1.
if ($_GET["logout"] == 1 or !empty($_GET["msg"])) {
	if ($_GET["logout"] == 1) {
		$log = date("M d H:i:s");
		$log .= " : Logged out (" . $_SESSION["USER_DN"] . ")\n";
		error_log($log, 3, "phpQLadmin.log");
	}

	$_SESSION = array();
	session_destroy();			// DLW: Should you destroy the session if there is a $_POST["msg"]?

	if ($_GET["logout"] == 1) {
		header("Location:index.php");
	}
}

// DLW: Does this ever get set?
if ($LOGIN_PASS == 1) {
	Header("Location:index2.php");
}

if (empty($_POST["uname"]) or empty($_POST["passwd"])) {
	include("./header.html");

	if(!$_SESSION["USER_HOST"]) {
		if(! eregi('\+', pql_get_define("PQL_GLOB_HOST"))) {
			$host = split(';', pql_get_define("PQL_GLOB_HOST"));
			$_SESSION["USER_HOST"] = $host[0] . ";" . $host[1] . ";" . $host[2];
		}
	}

	// print status message, if one is available
	if(isset($_GET["msg"])) {
?>

  <table cellpadding="3" cellspacing="0" border="0" width="100%">
    <tr>
      <td class="message"><img src="images/info.png" width="16" height="16" border="0"> <?php echo $_GET["msg"]; ?></td>
    </tr>
  </table>

<?php
	}
?>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0" align=center>
    <tr>
      <td bgcolor="#D0DCE0"><FONT size=3><?php echo pql_complete_constant($LANG->_('Welcome to \b%whoarewe%\B'), array('whoarewe' => pql_get_define("PQL_GLOB_WHOAREWE"))); ?></FONT></td>
    </tr>

    <tr align="center">
      <td class="title1"><?=$LANG->_('Please login')?></td>
    </tr>
  </table>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method=post name="phpqladmin" accept-charset="UTF-8">
    <table cellspacing="0" cellpadding="3" border="0" align=center>
      <tr>
        <td><?=$LANG->_('LDAP server')?>:</td>
        <td align="left">
<?php
	if(eregi('\+', pql_get_define("PQL_GLOB_HOST"))) {
		$servers = split('\+', pql_get_define("PQL_GLOB_HOST"));
?>
          <select name="server">
<?php	foreach($servers as $server) {
			$host = split(';', $server);
?>
            <option value="<?=$server?>"><?=$host[0]?>:<?=$host[1]?></option>
<?php	} ?>
          </select>
<?php
	} else {
		$server = split(';', $_SESSION["USER_HOST"]);
?>
        <b><?=$server[0].":".$server[1]?></b>
        <input type="hidden" name="server" value="<?php echo pql_get_define("PQL_GLOB_HOST"); ?>">
<?php
	}
?>
        </td>
      <tr>

          <tr>
            <td bgcolor="#D0DCE0"><b><?=$LANG->_('Login ID')?>:</b></td>
            <td><input type=text name="uname" size="30"></td>
          </tr>

          <tr>
             <td bgcolor="#D0DCE0"><b><?=$LANG->_('Password')?>:</b></td>
             <td><input type=password name="passwd" size="30" onChange="this.form.submit()" autocomplete="OFF"></td>
          </tr>

          <tr><td></td></tr>

          <tr>
            <td></td>
            <td>
              <input type=submit name=action value=submit>
              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
              <input type=reset name=action value=reset>
            </td>
          </tr>
     </table>
  </form>

  <script language="JavaScript">
  <!--
    document.phpqladmin.uname.focus();
	// -->
  </script>
</body>
</html>

<?php
} else {
	// -------------------------------------
	// Get the LDAP server
	if(!$_SESSION["USER_HOST"]) {
	    // DLW: This code assumes that $_POST["server"] is set, but lower down it check to see if $_POST["server"] is set.
		$host = split(';', $_POST["server"]);
		$_SESSION["USER_HOST"] = $host[0] . ";" . $host[1];
	} elseif(is_array($_SESSION["USER_HOST"])) {
		$host = $_SESSION["USER_HOST"][0];
		$_SESSION["USER_HOST"] = $host;
	} elseif($_POST["server"]) {
		$_SESSION["USER_HOST"]=$_POST["server"];
	}

	// -------------------------------------
	// Get the search base - controls database
	if(!$_SESSION["USER_SEARCH_DN_CTR"]) {
		// Get first entry -> default server:port
		$host = split('\+', $_POST["server"]);

		// Get hostname and base DN
		$dn   = split(';', $host[0]);
		$_SESSION["USER_SEARCH_DN_CTR"] = $dn[2];
	} elseif(is_array($_SESSION["USER_SEARCH_DN_CTR"])) {
		$host = $_SESSION["USER_HOST"][2];
		$_SESSION["USER_SEARCH_DN_CTR"] = $host;
	}

	// NOTE:
	// These variables will be NULL the first time,
	// so we will bind anonymously... 
	// We must have read access (to the DN and CN/UID =>
	// the PQL_CONF_REFERENCE_USERS_WITH define entry) as
	// anonymous here!
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	// -------------------------------------
	// Get DN of user
	// TODO: This is wrong. There might (?) be multiple
	//       users with the same uid in the database
	//       (under different branches/trees).
	$rootdn = pql_get_dn($_pql, $_POST["uname"], 1);
	if(!$rootdn and !is_array($rootdn))
	  die($LANG->_('Can\'t find you in the database')."!");	// DLW: This should be a $msg error.
	elseif(is_array($rootdn)) {
		// We got multiple DN's. Try to bind as each one, keeping
		// the one that succeeded.

		$got_rootdn = 0;		// DLW: Do we need this variable?
		foreach($rootdn as $dn) {
			$_pql->bind($dn, $_POST["passwd"]);
			$error = ldap_errno($_pql->ldap_linkid);
			if(!$error and !$got_rootdn){
				// That worked, keep it!
				$got_rootdn = 1;
				$rootdn = $dn;
				break;
			}
		}
	}

	if($error) {
	    // DLW: Maybe hand the uname back through so the user doesn't have to type it again.
		$msg = $LANG->_('Error') . ": " . ldap_err2str($error);
		header("Location:index.php?msg=" . urlencode($msg));
		exit;
	}

	// We made it, so set all the session variables.
	if($_POST["passwd"] and !$_SESSION["USER_PASS"])
	  $_SESSION["USER_PASS"] = $_POST["passwd"];

	$_SESSION["USER_ID"]	= $_POST["uname"];
	$_SESSION["USER_DN"]	= $rootdn;

	$log = date("M d H:i:s");
	$log .= " : Logged in ($rootdn)\n";
	error_log($log, 3, "phpQLadmin.log");
	Header("Location:index2.php");
}

// Closing connection

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
