<?php
// logins to the system
// index.php,v 1.4 2002/12/13 13:55:38 turbo Exp
//
session_start();

// Include this for completness (and to get an early
// warning if it's missing - browser goes blank, no
// error!).
require("./include/config.inc");

require("./include/pql.inc");
require("./include/pql_control.inc");
require("./include/pql_control_plugins.inc");

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

if(! eregi(' ', PQL_LDAP_HOST)) {
	$host = split(';', PQL_LDAP_HOST);
	$USER_HOST = $host[0] . ";" . $host[1];
	
	session_register("USER_HOST");
}

if ($LOGIN_PASS == 1) {
	Header("Location:index2.php");
}

if (empty($uname) or empty($passwd)) {
	include("./header.html");

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

    <tr align="center">
      <td class="title1"><? echo PQL_LOGIN; ?></td>
    </tr>
  </table>

  <form action="<?php echo $PHP_SELF; ?>" method=post name="phpqladmin">
    <table cellspacing="0" cellpadding="3" border="0" align=center>
      <tr>
        <td>LDAP Server:</td>
        <td align="left">
<?php
	if(eregi(' ', PQL_LDAP_HOST)) {
		$servers = split(' ', PQL_LDAP_HOST);
?>
          <select name="server">
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
<?php
	} else {
		$server = $USER_HOST;
?>
        <b><?=$server?></b>
<?php
	}
?>
        </td>
      <tr>

          <tr>
            <td bgcolor="#D0DCE0"><b><?=PQL_USERNAME?>:</b></td>
            <td><input type=text name="uname"></td>
          </tr>

          <tr>
             <td bgcolor="#D0DCE0"><b><?=PQL_USERPASS?>:</b></td>
             <td><input type=password name="passwd" onChange="this.form.submit()" autocomplete="OFF"></td>
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
	$uname = strtolower($uname);	

	// -------------------------------------
	// Get the LDAP server
	if(!$USER_HOST) {
		$host = split(';', $server);
		$USER_HOST = $host[0] . ";" . $host[1];
		
		session_register("USER_HOST");
	} elseif(is_array($USER_HOST)) {
		$host = $USER_HOST[0];
		$USER_HOST = $host;
		
		session_register("USER_HOST");
	}

	// -------------------------------------
	// Get the search base - user database
	if(!$USER_SEARCH_DN_USR) {
		$host = split(' ', $server);
		$dn   = split(';', $host[0]);
		$USER_SEARCH_DN_USR = $dn[2];

		session_register("USER_SEARCH_DN_USR");
	} elseif(is_array($USER_SEARCH_DN_USR)) {
		$host = $USER_SEARCH_DN_USR[2];
		$USER_SEARCH_DN_USR = $host;

		session_register("USER_SEARCH_DN_USR");
	}

	// -------------------------------------
	// Get the search base - controls database
	if(!$USER_SEARCH_DN_CTR) {
		// Get first entry -> default server:port
		$host = split(' ', $server);

		// Get hostname and base DN
		$dn   = split(';', $host[0]);
		$USER_SEARCH_DN_CTR = $dn[3];

		session_register("USER_SEARCH_DN_CTR");
	} elseif(is_array($USER_SEARCH_DN_CTR)) {
		$host = $USER_HOST[2];
		$USER_SEARCH_DN_CTR = $host;

		session_register("USER_SEARCH_DN_CTR");
	}

	// NOTE:
	// These variables will be NULL the first time,
	// so we will bind anonymously... 
	// We must have read access (to the DN and CN/UID =>
	// the PQL_LDAP_REFERENCE_USERS_WITH define entry) as
	// anonymous here!
	$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

	// -------------------------------------
	// Get DN of user
	$rootdn = pql_get_dn($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $uname);

	// Rebind as user
	$_pql->bind($rootdn, $USER_PASS);
	$error = ldap_errno($_pql->ldap_linkid);
	if( $error != 0 ){
		$msg = PQL_ERROR . ": " . ldap_err2str($error);
		header("Location:index.php?msg=" . urlencode($msg));
		exit;
	}

	$USER_ID	= $uname;
	$USER_PASS	= $passwd;
	$USER_DN	= $rootdn;

	if (!session_register("USER_ID", "USER_PASS", "USER_DN")) {
		die (PQL_SESSION_REG);
	}

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
