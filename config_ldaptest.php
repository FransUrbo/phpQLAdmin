<?php
// make some simple tests on ldap connection
// config_ldaptest.php,v 1.4 2002/12/13 13:47:10 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

if(!function_exists("ldap_connect")){
	$ldap_ext = PQL_TEST_LDAP_EXT_NA;
	$connection = "-";
	$connection_control = "-";
} else {
	$ldap_ext = PQL_TEST_LDAP_EXT_OK;
	
	// user directory connection
	$_pql = new pql($USER_HOST, '', '', true);
	if(!$_pql->connect($USER_HOST)){
		$connection = PQL_TEST_CONNECTION_FAILED;
		
		// do additional tests
		if(!gethostbyname($USER_HOST)){
			// not resolved
			$connection .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_RESOLVE_ERR ,array("host" => $USER_HOST ));
		} else {
			// try to open a connection
			if(!fsockopen($USER_HOST, 389)){
				// impossible to connect
				$connection .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_CONNECT_ERR ,array("host" => $USER_HOST ));
			}
		}
	} else {
		if(!$_pql->bind('', '')){
			$connection = PQL_TEST_CONNECTION_USER_BIND_ERR;
		} else {
			$connection = PQL_TEST_CONNECTION_CONNECT_OK;
		}
	}


	if(defined("PQL_LDAP_CONTROL_USE")) {
		// control directory connection
		$_pql_control = new pql_control($USER_HOST, '', '', true);
		if(!$_pql_control->connect($USER_HOST)) {
			$connection_control = PQL_TEST_CONNECTION_FAILED;

			$host = split(' ', PQL_LDAP_HOST);
			$host = split(';', $host[0]);

			$fqdn = $host[0];
			$port = $host[1];

			// do additional tests
			if(!gethostbyname($fqdn)){
				// not resolved
				$connection_control .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_RESOLVE_ERR, array("host" => $fqdn ));
			} else {
				// try to open a connection
				if(!fsockopen($fqdn, $port)){
					// impossible to connect
					$connection_control .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_CONNECT_ERR, array("host" => $USER_HOST));
				}
			}
		} else {
			if(!$_pql_control->bind()){
				$connection_control = PQL_TEST_CONNECTION_CONTROL_BIND_ERR;
			} else {
				$connection_control = PQL_TEST_CONNECTION_CONNECT_OK;
			}
		}
	} else {
		$connection_control = PQL_TEST_CONNECTION_CONROL_INACTIVE;
	}
} // end if(function_exists...

include("./header.html");
?>
  <span class="title1"><?php echo PQL_TEST_TITLE; ?></span>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_TEST_TESTS; ?></th>
      <tr>
	<td class="title"><?php echo PQL_TEST_LDAP_EXT_TITLE; ?></td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $ldap_ext; ?>&nbsp;</td>
</tr>
<tr>
	<td class="title"><?php echo PQL_TEST_CONNECTION_USER_TITLE; ?></td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $connection; ?>&nbsp;</td>
</tr>
<tr>
	<td class="title"><?php echo PQL_TEST_CONNECTION_CONROL_TITLE; ?></td>
	<td class="<?php table_bgcolor(); ?>"><?php echo $connection_control; ?>&nbsp;</td>
</tr>
<tr class="subtitle">
	<td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo PQL_TEST_HELP; ?>&nbsp;</td>
</tr>
</table>

</body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
