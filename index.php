<?php
// logins to the system
// index.php,v 1.4 2002/12/13 13:55:38 turbo Exp
//
// Start debuging
// http://www.linuxjournal.com/article.php?sid=7213&mode=thread&order=0
//apd_set_pprof_trace();

session_start();
require("./include/pql_config.inc");

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

if (empty($uname) or empty($passwd)) {
	include("./header.html");

	if(!$USER_HOST) {
		if(! eregi('\+', pql_get_define("PQL_GLOB_HOST"))) {
			$host = split(';', pql_get_define("PQL_GLOB_HOST"));
			$USER_HOST = $host[0] . ";" . $host[1] . ";" . $host[2];
			
			session_register("USER_HOST");
		}
	}

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
      <td bgcolor="#D0DCE0"><FONT size=3><?php echo pql_complete_constant($LANG->_('Welcome to \b%whoarewe%\B'), array('whoarewe' => pql_get_define("PQL_GLOB_WHOAREWE"))); ?></FONT></td>
    </tr>

    <tr align="center">
      <td class="title1"><?=$LANG->_('Please login')?></td>
    </tr>
  </table>

  <form action="<?=$PHP_SELF?>" method=post name="phpqladmin" accept-charset="UTF-8">
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
		$server = split(';', $USER_HOST);
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
	if(!$USER_HOST) {
		$host = split(';', $server);
		$USER_HOST = $host[0] . ";" . $host[1];
		
		session_register("USER_HOST");
	} elseif(is_array($USER_HOST)) {
		$host = $USER_HOST[0];
		$USER_HOST = $host;
		
		session_register("USER_HOST");
	} elseif($server) {
		$USER_HOST=$server;
		session_register("USER_HOST");
	}

	// -------------------------------------
	// Get the search base - controls database
	if(!$USER_SEARCH_DN_CTR) {
		// Get first entry -> default server:port
		$host = split('\+', $server);

		// Get hostname and base DN
		$dn   = split(';', $host[0]);
		$USER_SEARCH_DN_CTR = $dn[2];

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
	// the PQL_CONF_REFERENCE_USERS_WITH define entry) as
	// anonymous here!
	$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

	// -------------------------------------
	// Get DN of user
	// TODO: This is wrong. There might (?) be multiple
	//       users with the same uid in the database
	//       (under different branches/trees).
	$rootdn = pql_get_dn($_pql, $uname, 1);
	if(!$rootdn and !is_array($rootdn))
	  die($LANG->_('Can\'t find you in the database')."!");
	elseif(is_array($rootdn)) {
		// We got multiple DN's. Try to bind as each one, keeping
		// the one that succeeded.

		$got_rootdn = 0;
		foreach($rootdn as $dn) {
			$_pql->bind($dn, $passwd);
			$error = ldap_errno($_pql->ldap_linkid);
			if(!$error and !$got_rootdn){
				// That worked, keep it!
				$got_rootdn = 1;
				$rootdn = $dn;
				break;
			}
		}
	}

	if($passwd and !$USER_PASS)
	  $USER_PASS = $passwd;

	if($error) {
		$msg = $LANG->_('Error') . ": " . ldap_err2str($error);
		header("Location:index.php?msg=" . urlencode($msg));
		exit;
	}

	$USER_ID	= $uname;
	$USER_DN	= $rootdn;

	if(! session_register("USER_ID", "USER_PASS", "USER_DN"))
	  die($LANG->_('Could not register session variables'));

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
