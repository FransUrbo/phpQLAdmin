<?php
// logins to the system
// $Id: index.php,v 2.39 2005-01-12 14:39:35 turbo Exp $
//
// Start debuging
// http://www.linuxjournal.com/article.php?sid=7213&mode=thread&order=0
//apd_set_pprof_trace();
require_once("./include/dlw_porting.inc");
session_start();

// DLW: I'm not sure if $msg ever gets set in a _POST, but for now I'll play it safe.
if (!empty($_POST["msg"])) {
	$log = date("M d H:i:s");
	$log .= " : Unexpected _POST[msg])\n";
	@error_log($log, 3, "phpQLadmin.log");
}

// DLW: I think !empty($_GET["logout"]) will work here better than $_GET["logout"] == 1.
if ($_GET["logout"] == 1 or !empty($_GET["msg"])) {
	if ($_GET["logout"] == 1) {
		$log = date("M d H:i:s");
		$log .= " : Logged out (" . $_SESSION["USER_DN"] . ")\n";
		@error_log($log, 3, "phpQLadmin.log");
	}
	
	if(!empty($_POST["msg"]))
	  $msg = $_POST["msg"];
	
	$_SESSION = array();
	session_destroy();

	if ($_GET["logout"] == 1) {
		if(!empty($_POST["msg"]))
		  header("Location:index.php?msg=".urlencode($msg));
		else
		  header("Location:index.php");
	}
}

require("./include/pql_config.inc");

if (empty($_POST["uname"]) or empty($_POST["passwd"])) {
	include("./header.html");

	if(!$_SESSION["USER_HOST"]) {
		if(! eregi('\+', pql_get_define("PQL_CONF_HOST"))) {
			$host = split(';', pql_get_define("PQL_CONF_HOST"));
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
      <td bgcolor="#D0DCE0"><FONT size=3><?php echo pql_complete_constant($LANG->_('Welcome to \b%whoarewe%\B'), array('whoarewe' => pql_get_define("PQL_CONF_WHOAREWE"))); ?></FONT></td>
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
	if(eregi('\+', pql_get_define("PQL_CONF_HOST"))) {
		$servers = split('\+', pql_get_define("PQL_CONF_HOST"));
?>
          <select name="server">
<?php	foreach($servers as $server) {
			$host = split(';', $server);

			// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
			$host[0] = urldecode($host[0]);
?>
            <option value="<?=$server?>"><?=$host[0]?><?php if(!eregi('^ldapi:', $host[0])) { ?>:<?=$host[1]?><?php } ?></option>
<?php	} ?>
          </select>
<?php
	} else {
		$server = split(';', $_SESSION["USER_HOST"]);

		// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
		$server[0] = urldecode($server[0]);
?>
        <b><?=$server[0].":".$server[1]?></b>
        <input type="hidden" name="server" value="<?php echo pql_get_define("PQL_CONF_HOST"); ?>">
<?php
	}
?>
        </td>
      <tr>

          <tr>
            <td bgcolor="#D0DCE0"><b><?=$LANG->_('Login ID')?>:</b></td>
            <td><input type=text name="uname" value="<?=$_GET["uname"]?>" size="30"></td>
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
<?php if(empty($_GET["uname"]) and empty($_POST["uname"])) { ?>
    document.phpqladmin.uname.focus();
<?php } else { ?>
    document.phpqladmin.passwd.focus();
<?php } ?>
	// -->
  </script>
</body>
</html>

<?php
} else {
	// -------------------------------------
	if($_POST["server"]) {
		// Get the LDAP server
		if(!$_SESSION["USER_HOST"]) {
			$host = split(';', $_POST["server"]);
			$_SESSION["USER_HOST"] = $host[0] . ";" . $host[1];
		}

		// Get the search base - controls database
		if(!$_SESSION["USER_SEARCH_DN_CTR"]) {
			// Get first entry -> default server:port
			$host = split('\+', $_POST["server"]);
			
			// Get hostname and base DN
			$dn   = split(';', $host[0]);
			$_SESSION["USER_SEARCH_DN_CTR"] = $dn[2];
		}
	} else
	  die("No LDAP server to login to is defined. Weird.");

	// NOTE:
	// User DN and password will be NULL the first time,
	// so we will bind anonymously... 
	// We must have read access (to the DN and CN/UID =>
	// the PQL_CONF_REFERENCE_USERS_WITH define entry) as
	// anonymous here!
echo "anonymous binding... (".$_SESSION["USER_DN"].")<br>";
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	// -------------------------------------
	// Get DN of user
	// TODO: This is wrong. There might (?) be multiple
	//       users with the same uid in the database
	//       (under different branches/trees).
	$user_found = 0;
	foreach($_pql->ldap_basedn as $base) {
		$base    = urldecode($base);
		$objects = pql_get_dn($_pql->ldap_linkid, $base,
							  pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $base).'='.$_POST["uname"]);
		foreach($objects as $userdn) {
			$_pql->bind($userdn, $_POST["passwd"]);
			$error = ldap_errno($_pql->ldap_linkid);
			if(!$error) {
				// User bound with correct DN with corresponding correct password.
				$user_found = 1;
				break;
			} else {
				// Authentication problem (probably!).
				$msg = $LANG->_('Error') . ": " . ldap_err2str($error);
				header("Location:index.php?msg=" . urlencode($msg) . "&uname=$uname");
				exit;
			}
		}
	}

	if(!$user_found) {
		$msg = urlencode($LANG->_('Error') . ": " . $LANG->_("Can't find you in the database"));
		header("Location: " . pql_get_define("PQL_CONF_URI") . "index.php?msg=$msg");
	}

	// We made it, so set all the session variables.
	if($_POST["passwd"] and !$_SESSION["USER_PASS"])
	  $_SESSION["USER_PASS"] = $_POST["passwd"];

	$_SESSION["USER_ID"]	= $_POST["uname"];
	$_SESSION["USER_DN"]	= $userdn;

	$log = date("M d H:i:s");
	$log .= " : Logged in ($userdn)\n";
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
