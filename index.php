<?php
// logins to the system
// $Id: index.php,v 1.2 2002-12-12 12:00:43 turbo Exp $
//
session_start();

require("pql.inc");
require("pql_control.inc");
require("pql_control_plugins.inc");

$_pql = new pql();

if ($logout == 1 or !empty($msg)) {
	if ($logout == 1) {
		$log = date("M d H:i:s");
		$log .= " : Logged out ($USER_DN)\n";
		error_log($log, 3, "phpQLadmin.log");
	}
	session_unset();
	session_destroy();
	if ($logout == 1) {
		header("Location:index.php");
	}
}

if ($LOGIN_PASS == 1) {
	Header("Location:index2.php");
}

// Get all domains
$domains = pql_get_domains($_pql->ldap_linkid, PQL_LDAP_BASEDN);
if(is_array($domains)){
	asort($domains);
}

if (empty($uname) or empty($passwd)) {
	include("header.html");

	// print status message, if one is available
	if(isset($msg)) {
?>
  <table cellpadding="3" cellspacing="0" border="0" width="100%">
    <tr>
      <td class="message"><img src="images/info.png" width="16" height="16" border="0"> <?php echo $msg; ?></td>
    </tr>
  </table>
<?php
	}
?>
<br><br>
<table cellspacing="0" cellpadding="3" border="0" align=center>
  <tr>
    <td bgcolor="#D0DCE0"><FONT size=3><?php echo PQL_WELCOME . " <B>" . PQL_DESCRIPTION; ?></B></FONT></td>
  </tr>

  <tr>
    <td class="title1"><? echo PQL_LOGIN; ?></td>
  </tr>
</table>
<form action="<?php echo $PHP_SELF; ?>" method=post>
  <table cellspacing="0" cellpadding="3" border="0" align=center>
<?php
	if (!empty($invalid)) {
?>
    <tr>
      <td>
        <tr>
          <td><FONT color=red><B>Error:</B><?php echo $invalid; ?></FONT></td>
          <td></td>
        </tr>

<?php
	}
?>
        <tr>
          <td>
            <tr>
              <td bgcolor="#D0DCE0"><b><?=PQL_USERNAME?>:</b></td>
              <td><input type=text name="uname"></td>
            </tr>

            <tr>
              <td bgcolor="#D0DCE0"><b><?=PQL_USERPASS?>:</b></td>
              <td><input type=password name="passwd"></td>
            </tr>

            <tr>
              <td bgcolor="#D0DCE0"><b><?=PQL_ORG_BASE?>:</b></td>
              <td>
                <select name="base">
<?php
    if(is_array($domains)) {
		foreach($domains as $key => $domain) {
?>
                  <option value="<?php echo $domain; ?>"><?php echo $domain; ?></option>
<?php
		}
	}
?>
                  <option value="everything">Whole tree</option>
                </select>
              </td>
            </tr>

            <tr><td></td></tr>

            <tr>
              <td align=center><input type=submit name=action value=submit></td>
              <td align=center><input type=reset name=action value=reset></td>
            </tr>
          </td>
        </tr>
      </td>
    </tr>
  </table>
</form>
</body></html>

<?php
} else {
	$uname		= strtolower($uname);	

	// Get DN of user
	$rootdn = pql_get_dn($_pql->ldap_linkid, PQL_LDAP_BASEDN, $USER_ID);
	$_pql->close();

	// Connecting, selecting database
	$_pql = new pql($rootdn, $USER_PASS);
	$_pql->close();

	$_pql = new pql();
	$error = ldap_errno($_pql->ldap_linkid);
	if( $error != 0 ){
		$msg = PQL_ERROR . ": " . ldap_err2str($error);
		header("Location:index.php?msg=" . urlencode($msg));
		exit;
	}

	$USER_ID	= $uname;
	$USER_PASS	= $passwd;
	$USER_DN	= $rootdn;
	$USER_BASE	= $base;

	if (!session_register("USER_ID", "USER_PASS", "USER_DN", "USER_BASE")) {
		die (PQL_SESSION_REG);
	}

	$log = date("M d H:i:s");
	$log .= " : Logged in ($USER_DN)\n";
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
