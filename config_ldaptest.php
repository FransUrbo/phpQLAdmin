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
		if(ldap_errno($linkid) == 21)
		  // Invalid syntax
		  return($LANG->_('No. Reason:')."<b>".$LANG->_('Old phpQLAdmin schema')."</b>");
		else {
			$LDIF = pql_create_ldif(NULL, $dn, $entry);

			return("<a href=\"javascript:ldifWindow('".$LDIF."')\">".
				   $LANG->_('No. Reason:')."<b>".ldap_error($linkid)."</b>'</a>");
		}
	} else {
		// Success - delete it again
		unset($entry);
		$entry['test'] = array();
		ldap_mod_del($linkid, $dn, $entry);

		return(0);
	}
}

if(!function_exists("ldap_connect")){
	$ldap_ext = $LANG->_('Not available');
	$connection = "-";
	$connection_control = "-";
} else {
	$ldap_ext = $LANG->_('Built in (loaded)');
	
	// user directory connection
	$_pql = new pql($USER_HOST, '', '', true);
	if(!$_pql->connect($USER_HOST)){
		$connection = $LANG->_('Failed');
		
		// do additional tests
		if(!gethostbyname($USER_HOST)) {
			// not resolved
			$connection .= ", " . pql_complete_constant($LANG->_('The hostname %host% could not be resolved') ,array("host" => $USER_HOST ));
		} else {
			// try to open a connection
			if(!fsockopen($USER_HOST, 389)) {
				// impossible to connect
				$connection .= ", " . pql_complete_constant($LANG->_('Could not connect to port 389 at %host%, please make sure the service is up and that it's not blocked with a firewall') ,array("host" => $USER_HOST ));
			}
		}
	} else {
		if(!$_pql->bind('', '')) {
			$connection = $LANG->_('Connection ok, but could not bind to the directory');
		} else {
			$connection = $LANG->_('Yes');
		}
	}


	if(pql_get_define("PQL_GLOB_CONTROL_USE")) {
		// control directory connection
		$_pql_control = new pql_control($USER_HOST, '', '', true);
		if(!$_pql_control->connect($USER_HOST)) {
			$connection_control = $LANG->_('Failed');

			$host = split('\+', pql_get_define("PQL_GLOB_HOST"));
			$host = split(';', $host[0]);

			$fqdn = $host[0];
			$port = $host[1];

			// do additional tests
			if(!gethostbyname($fqdn)) {
				// not resolved
				$connection_control .= ", " . pql_complete_constant($LANG->_('The hostname %host% could not be resolved'), array("host" => $fqdn ));
			} else {
				// try to open a connection
				if(!fsockopen($fqdn, $port)) {
					// impossible to connect
					$connection_control .= ", " . pql_complete_constant($LANG->_('Could not connect to port 389 at %host%, please make sure the service is up and it is not blocked with a firewall'), array("host" => $USER_HOST));
				}
			}
		} else {
			if(!$_pql_control->bind()) {
				$connection_control = $LANG->_('Connection ok, but could not bind to the directory');
			} else {
				$connection_control = $LANG->_('Yes');
			}
		}
	} else {
		$connection_control = $LANG->_('Control extension deactivated');
	}

	// Test access rights etc.
	if($USER_DN and $USER_PASS) {
		$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
		foreach($_pql->ldap_basedn as $basedn) {
			// ----------------------
			// Try to set the attribute 'test' in the top DN
			$fail = check_domain_value($_pql->ldap_linkid, $basedn, 'test', 'TRUE');
			if($fail) {
				$TEST["basedn"][$basedn] = $fail;
			} else {
				$TEST["basedn"][$basedn] = $LANG->_('Yes');
			}
			
			// ----------------------
			// Test to see if we have access to create domain/branches
			// by creating a subbranch
			unset($entry);
			$entry["objectClass"][] = "top";
			if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "dc") {
				$entry["objectClass"][] = "domain";
				$entry["dc"] = "test";
			} elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "ou") {
				$entry["objectClass"][] = "organizationalUnit";
				$entry["ou"] = "test";
			} elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "o") {
				$entry["objectClass"][] = "organizationL";
				$entry["o"] = "test";
			}
			$entry[pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)] = "phpQLAdmin_Branch_Test";
			$dn = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)."=phpQLAdmin_Branch_Test,".$basedn;
			if(!@ldap_add($_pql->ldap_linkid, $dn, $entry)) {
				$LDIF = pql_create_ldif("config_ldaptest.php", $dn, $entry);
				$TEST["branches"][$basedn] = "<a href=\"javascript:ldifWindow('".$LDIF."')\">".
                                             $LANG->_("No. Reason:")."<b>".ldap_error($_pql->ldap_linkid)."</b>'";
			} else {
				// Success - delete it again
				ldap_delete($_pql->ldap_linkid, $dn);
				$TEST["branches"][$basedn] = $LANG->_('Yes');
			}
			
			// ----------------------
			// Check write access
			$filter = "(&" . pql_setup_branch_objectclasses(1, $basedn)
			  . "(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) . "=*))";
			
			$sr = @ldap_list($_pql->ldap_linkid, $basedn, $filter)
			  or pql_errormsg($_pql->ldap_linkid);
			$info = @ldap_get_entries($_pql->ldap_linkid, $sr)
			  or pql_errormsg($_pql->ldap_linkid);

			if(! $info["count"]) {
				// Didn't find anything on a one-level search, try a global one...
				$sr = @ldap_search($_pql->ldap_linkid, $basedn, $filter)
				  or pql_errormsg($_pql->ldap_linkid);
				$info = @ldap_get_entries($_pql->ldap_linkid, $sr)
				  or pql_errormsg($_pql->ldap_linkid);
			}

			for ($i=0; $i<$info["count"]; $i++) {
				$domains[] = $info[$i]["dn"];
			}
		}

		if(is_array($domains)) {
			foreach($domains as $domain) {
				// Check write access
				$fail = check_domain_value($_pql->ldap_linkid, $domain, 'test', 'TRUE');
				if($fail) {
					$TEST["branches"][$domain] = $fail;
				} else {
					$TEST["branches"][$domain] = $LANG->_('Yes');
				}
			}
		}
	}
} // end if(function_exists...

include("./header.html");
?>
  <script type="text/javascript" language="javascript"><!--
    function ldifWindow(string) {
      myWindow = window.open("", "LDIFWindow", 'toolbar,width=350,height=200');
      myWindow.document.write(string);
      myWindow.document.bgColor="white";
      myWindow.document.close();
    }
  //--></script>

  <span class="title1"><?=$LANG->_('phpQLAdmin ldap connection')?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('LDAP server connection and setup tests')?>
      <tr>
        <td class="title"><?=$LANG->_('PHP LDAP extension')?></td>
        <td class="<?php table_bgcolor(); ?>"><?=$ldap_ext?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('User directory connection')?></td>
        <td class="<?php table_bgcolor(); ?>"><?=$connection?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Control directory connection')?></td>
        <td class="<?php table_bgcolor(); ?>"><?=$connection_control?>&nbsp;</td>
      </tr>
<?php if($ADVANCED_MODE == 1) { ?>

      <tr></tr>

      <tr>
        <td class="title"><?=$LANG->_('LDAP server contains phpQLAdminConfig objectclass')?></td>
<?php    if(pql_get_subschemas($_pql->ldap_linkid, "phpQLAdminConfig")) { ?>
        <td class="<?php table_bgcolor(); ?>"><?=$LANG->_('Yes')?></td>
<?php    } else { ?>
        <td class="<?php table_bgcolor(); ?>"><?=$LANG->_('No')?>. <?=$LANG->_('Please load file')?> <a href="phpQLAdmin.schema">phpQLAdmin.schema</a>&nbsp;</td>
<?php    } ?>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('LDAP server contains phpQLAdminBranch objectclass')?></td>
<?php    if(pql_get_subschemas($_pql->ldap_linkid, "phpQLAdminBranch")) { ?>
        <td class="<?php table_bgcolor(); ?>"><?=$LANG->_('Yes')?></td>
<?php    } else { ?>
	<td class="<?php table_bgcolor(); ?>"><?=$LANG->_('No')?>. <?=$LANG->_('Please load file')?> <a href="phpQLAdmin.schema">phpQLAdmin.schema</a>&nbsp;</td>
<?php    } ?>
      </tr>

      <tr>
        <td class="title">LDAP server contains schema for <a href="http://www.ietf.org/rfc/rfc2377.txt" target="_new">RFC 2377</a></td>
<?php    if(pql_get_subschemas($_pql->ldap_linkid, "dcOrganizationNameForm")) { ?>
        <td class="<?php table_bgcolor(); ?>"><?=$LANG->_('Yes')?></td>
<?php    } else { ?>
        <td class="<?php table_bgcolor(); ?>"><?=$LANG->_('No')?>. <?=$LANG->_('Please load file')?> <a href="rfc2377.schema">rfc2377.schema</a>&nbsp;</td>
<?php    }
      }
?>
      </tr>
    </th>
<?php if($basedn) { ?>

    <th colspan="3" align="left"><?=$LANG->_('Modification access - phpQLAdmin configuration')?>
<?php    foreach($_pql->ldap_basedn as $dn) { ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to write phpQLAdmin configuration in DN %dn%'), array('dn' => $dn)); ?></td>
        <?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"><?=$TEST["basedn"][$dn]?></td>
      </tr>

<?php    } ?>
    </th>

    <th colspan="3" align="left"><?=$LANG->_('Domain modification access')?>
<?php    foreach($_pql->ldap_basedn as $dn) {
 ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to create branches in DN %dn%'), array('dn' => $dn)); ?></td>
        <?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"><?=$TEST["branches"][$dn]?></td>
      </tr>

<?php    } ?>
    </th>

    <th colspan="3" align="left"><?=$LANG->_('DN modification access - domain DN's')?>
<?php    if(is_array($domains)) {
            asort($domains);
		    foreach($domains as $key => $domain) {
?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to modify DN \b%dn\B'), array('dn' => $domain)); ?></b></td>
        <?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"><?=$TEST["branches"][$domain]?></td>
      </tr>

<?php       }
         }
      }
?>
    </th>

    <th colspan="3" align="left">
      <tr class="subtitle">
        <td>
          <table>
            <th>
              <tr>
                <td><img src="images/info.png" width="16" height="16" border="0" align="right"></td>
                <td><?=$LANG->_('This is a simple test to show if the ldap extension is loaded and that the connections are working. It also does some rudimentary ACL tests')?>.&nbsp;</td>
              </tr>
            </th>
          </table>
        </td>

        <td></td>
      </tr>
    </th>
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
