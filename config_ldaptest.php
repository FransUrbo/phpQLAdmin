<?php
// make some simple tests on ldap connection
// $Id: config_ldaptest.php,v 1.4 2002-12-13 13:47:10 turbo Exp $
//
require("pql.inc");
require("pql_control.inc");

if(!function_exists("ldap_connect")){
	$ldap_ext = PQL_TEST_LDAP_EXT_NA;
	$connection = "-";
	$connection_control = "-";
} else {
	$ldap_ext = PQL_TEST_LDAP_EXT_OK;
	
	// user directory connection
	$_pql = new pql('', '', true);
	if(!$_pql->connect()){
		$connection = PQL_TEST_CONNECTION_FAILED;
		
		// do additional tests
		if(!gethostbyname(PQL_LDAP_HOST)){
			// not resolved
			$connection .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_RESOLVE_ERR ,array("host" => PQL_LDAP_HOST ));
		} else {
			// try to open a connection
			if(!fsockopen(PQL_LDAP_HOST, 389)){
				// impossible to connect
				$connection .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_CONNECT_ERR ,array("host" => PQL_LDAP_HOST ));
			}
		}
	} else {
		if(!$_pql->bind('', '')){
			$connection = PQL_TEST_CONNECTION_USER_BIND_ERR;
		} else {
			$connection = PQL_TEST_CONNECTION_CONNECT_OK;
		}
	}


	if(PQL_LDAP_CONTROL_USE){
		// control directory connection
		$_pql_control = new pql_control('', '', true);
		if(!$_pql_control->connect()) {
			$connection_control = PQL_TEST_CONNECTION_FAILED;

			// do additional tests
			if(!gethostbyname(PQL_LDAP_CONTROL_HOST)){
				// not resolved
				$connection_control .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_RESOLVE_ERR ,array("host" => PQL_LDAP_CONTROL_HOST ));
			} else {
				// try to open a connection
				if(!fsockopen(PQL_LDAP_CONTROL_HOST, 389)){
					// impossible to connect
					$connection_control .= ", " . pql_complete_constant(PQL_TEST_CONNECTION_CONNECT_ERR ,array("host" => PQL_LDAP_HOST ));
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

include("header.html");
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
