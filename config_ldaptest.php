<?php
// make some simple tests on ldap connection
// config_ldaptest.php,v 1.4 2002/12/13 13:47:10 turbo Exp
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

function check_domain_value($linkid, $dn, $attrib, $value) {
	$entry[$attrib] = $value;

	if(! @ldap_mod_replace($linkid, $dn, $entry)) {
		return("No. Reason: '<b>".ldap_error($linkid)."</b>'");
	} else {
		// Success - delete it again
		unset($entry);
		$entry['test'] = array();
		ldap_mod_del($linkid, $dn, $entry);

		return(0);
	}
}

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

	// Test access rights etc.
	if($USER_DN and $USER_PASS) {
		$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
		$basedn = $_pql->ldap_basedn[0];

		// ----------------------
		// Try to set the attribute 'test' in the top DN
		$fail = check_domain_value($_pql->ldap_linkid, $basedn, 'test', '1');
		if($fail) {
			$TEST["basedn"] = $fail;
		} else {
			$TEST["basedn"] = "Yes, you have";
		}

		// ----------------------
		// Test to see if we have access to create domain/branches
		// by creating a subbranch
		unset($entry);
		$entry["objectClass"][] = "top";
		if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
			$entry["objectClass"][] = "domain";
		} else {
			$entry["objectClass"][] = "organizationalUnit";
			$entry["o"] = "test";
		}
		$entry[PQL_LDAP_REFERENCE_DOMAINS_WITH] = "phpQLAdmin_Branch_Test";
		$dn = PQL_LDAP_REFERENCE_DOMAINS_WITH."=phpQLAdmin_Branch_Test,".$basedn;
		if(!@ldap_add($_pql->ldap_linkid, $dn, $entry)) {
			$TEST["branches"][$basedn] = "No. Reason: '<b>".ldap_error($_pql->ldap_linkid)."</b>'";
		} else {
			// Success - delete it again
			ldap_delete($_pql->ldap_linkid, $dn);
			$TEST["branches"][$basedn] = "Yes, you have";
		}

		// ----------------------
		// Go through each domain/branch and check write access
		foreach($_pql->ldap_basedn as $dn)  {
			$sr = @ldap_list($_pql->ldap_linkid, $dn,
							 "(" . PQL_LDAP_REFERENCE_DOMAINS_WITH . "=*)"
							 ) or pql_errormsg($_pql->ldap_linkid);
			$info = @ldap_get_entries($_pql->ldap_linkid, $sr) or pql_errormsg($_pql->ldap_linkid);
			for ($i=0; $i<$info["count"]; $i++) {
				$domains[] = $info[$i]["dn"];
			}
			
		}

		if(is_array($domains)) {
			foreach($domains as $domain) {
				// Check write access
				$fail = check_domain_value($_pql->ldap_linkid, $domain, 'test', '1');
				if($fail) {
					$TEST["branches"][$domain] = $fail;
				} else {
					$TEST["branches"][$domain] = "Yes, you have";
				}
			}
		}
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

<?php
if($ADVANCED_MODE == 1) {
?>
<tr></tr>

<tr>
	<td class="title">LDAP server contains phpQLAdminConfig objectclass</td>
<?php
	if(pql_get_subschemas($_pql->ldap_linkid, "phpQLAdminConfig")) {
?>
	<td class="<?php table_bgcolor(); ?>"><?=PQL_TEST_CONNECTION_OCEXISTS_OK?></td>
<?php
	} else {
?>
	<td class="<?php table_bgcolor(); ?>">No, it doesn't. Please load <a href="phpQLAdmin.schema">phpQLAdmin.schema</a>&nbsp;</td>
<?php
	}
?>
</tr>
<tr>
	<td class="title">LDAP server contains phpQLAdminBranch objectclass</td>
<?php
	if(pql_get_subschemas($_pql->ldap_linkid, "phpQLAdminBranch")) {
?>
	<td class="<?php table_bgcolor(); ?>"><?=PQL_TEST_CONNECTION_OCEXISTS_OK?></td>
<?php
	} else {
?>
	<td class="<?php table_bgcolor(); ?>">No, it doesn't. Please load <a href="phpQLAdmin.schema">phpQLAdmin.schema</a>&nbsp;</td>
<?php
	}
?>
</tr>
<tr>
    <td class="title">LDAP server contains schema for <a href="http://www.ietf.org/rfc/rfc2377.txt" target="_new">RFC 2377</a></td>
<?php
	if(pql_get_subschemas($_pql->ldap_linkid, "dcOrganizationNameForm")) {
?>
	<td class="<?php table_bgcolor(); ?>"><?=PQL_TEST_CONNECTION_OCEXISTS_OK?></td>
<?php
	} else {
?>
	<td class="<?php table_bgcolor(); ?>">No, it doesn't. Please load <a href="rfc2377.schema">rfc2377.schema</a>&nbsp;</td>
<?php
	}
}
?>
</tr>

<?php
if($basedn) {
?>
<tr></tr>

<tr>
	<td class="title">Access to write phpQLAdmin configuration</td>
    <?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$TEST["basedn"]?></td>
</tr>
	

<tr>
	<td class="title">Access to create domains/branches</td>
    <?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$TEST["branches"][$basedn]?></td>
</tr>

<?php
	if(is_array($domains)) {
		asort($domains);
		foreach($domains as $key => $domain) {
?>
<tr>
	<td class="title">Access to modify DN <b><?=$domain?></b></td>
    <?php $class=table_bgcolor(0); ?>
	<td class="<?=$class?>"><?=$TEST["branches"][$domain]?></td>
</tr>
<?php
		}	
	}
}
?>

<tr class="subtitle">
  <td>
    <table>
      <th>
        <tr>
          <td><img src="images/info.png" width="16" height="16" border="0" align="right"></td>
          <td><?php echo PQL_TEST_HELP; ?>&nbsp;</td>
        </tr>
      </th>
    </table>
  </td>
  <td></td>
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
