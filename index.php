<?php
// logins to the system
// $Id: index.php,v 2.42.2.3 2005-05-16 08:37:08 turbo Exp $
//
// Start debuging
// http://www.linuxjournal.com/article.php?sid=7213&mode=thread&order=0
//apd_set_pprof_trace();
require_once("./include/dlw_porting.inc");
require("./include/pql_session.inc");

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
		session_write_close();

		// To early (haven't loaded the API's) to use pql_header(), so we
		// do it the hard way...
		if(!empty($_POST["msg"])) {
			header("Location: index.php?msg=".urlencode($_POST["msg"]));
			exit;
		} elseif(!empty($_GET["msg"])) {
			header("Location: index.php?msg=".urlencode($_GET["msg"]));
			exit;
		} else {
			header("Location: index.php");
			exit;
		}
	}

	// Create a new session
	require("./include/pql_session.inc");
}

require("./include/pql_config.inc");

if (empty($_POST["uname"]) or empty($_POST["passwd"])) {
	include($_SESSION["path"]."/header.html");

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

if(!($whoarewe = pql_get_define("PQL_CONF_WHOAREWE")))
  $whoarewe = "<br>phpQLAdmin @ ".$_SERVER["SERVER_NAME"];
?>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0" align=center>
    <tr>
      <td bgcolor="#D0DCE0"><center><FONT size=3><?php echo pql_complete_constant($LANG->_('Welcome to \b%whoarewe%\B'), array('whoarewe' => $whoarewe)); ?></FONT></center></td>
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
        <input type="hidden" name="server" value="<?=$_SESSION["USER_HOST"]?>">
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
		if(!$_SESSION["USER_HOST"])
		  $_SESSION["USER_HOST"] = $_POST["server"];

		// Get the search base - controls database
		if(!$_SESSION["USER_SEARCH_DN_CTR"]) {
			$dn = split(';', $_SESSION["USER_HOST"]);
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
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	// -------------------------------------
	// Get DN of user
	// TODO: This is wrong. There might (?) be multiple
	//       users with the same uid in the database
	//       (under different branches/trees).
	$user_found = 0;
	foreach($_SESSION["BASE_DN"] as $base) {
		$objects = pql_get_dn($_pql->ldap_linkid, $base,
							  pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $base).'='.$_POST["uname"]);
		if(is_array($objects)) {
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

				session_write_close();
				pql_header("index.php?msg=" . urlencode($msg) . "&uname=$uname");
			}
		  }
		}
	}

	if(!$user_found) {
		$msg = urlencode($LANG->_('Error') . ": " . $LANG->_("Can't find you in the database"));

		unset($_POST);
		session_write_close();
		pql_header("index.php?msg=$msg");
	}

	// We made it, so set all the session variables.
	if($_POST["passwd"] and !$_SESSION["USER_PASS"])
	  $_SESSION["USER_PASS"] = $_POST["passwd"];

	$_SESSION["USER_ID"]	= $_POST["uname"];
	$_SESSION["USER_DN"]	= $userdn;

	$log = date("M d H:i:s");
	$log .= " : Logged in ($userdn)\n";
	error_log($log, 3, "phpQLadmin.log");

	session_write_close();
	if(pql_get_attribute($_pql->ldap_linkid, $_SESSION["USER_DN"], pql_get_define("PQL_ATTR_START_ADVANCED")))
	  pql_header("index2.php?advanced=1");
	else
	  pql_header("index2.php");
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
