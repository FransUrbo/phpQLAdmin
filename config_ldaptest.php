<?php
// make some simple tests on ldap connection
// $Id: config_ldaptest.php,v 2.42 2007-02-15 16:16:04 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");

// {{{ Check a domain value
function check_domain_value($dn, $attrib, $value) {
  global $_pql; $LANG;
  
  $debugging = pql_get_define("PQL_CONF_DEBUG_ME");
  if($debugging)
	// Debugging enabled - temporarily disable it!
	pql_set_define("PQL_CONF_DEBUG_ME", false);
  
  $entry[$attrib] = $value;
  if(!$_pql->modify($dn, $entry, "config_ldaptest.php:check_domain_value()/1")) {
	if(ldap_errno($linkid) == 21)
	  // Invalid syntax
	  $msg = $LANG->_('No. Reason:\n')."<b>".$LANG->_('Old phpQLAdmin schema')."</b>";
	else {
	  $LDIF = pql_create_ldif('config_ldaptest.php:pql_write_mod', $dn, $entry, 1);
	  
	  $msg = "<a href=\"javascript:ldifWindow('".$LDIF."')\">".
		$LANG->_('No. Reason:\n')."<b>'".ldap_error($linkid)."'</b></a>";
	}
  } else {
	// Success - delete it again
	unset($entry);
	$entry['test'] = array();
	$_pql->modify($dn, $entry, "config_ldaptest.php:check_domain_value()/2");
	
	$msg = 0;
  }
  
  if($debugging)
	// Debugging have been temporarily disabled - enable it again!
	pql_set_define("PQL_CONF_DEBUG_ME", true);
  
  return($msg);
}
// }}}

// {{{ Check some extension modules
if(class_exists('Memcache') and function_exists("memcache_connect")) {
  $memcache_ext = $LANG->_('Built in (loaded)');
} else {
  $memcache_ext = $LANG->_('Not available');
}
  
if(function_exists("idn_to_utf8")) {
  $idn_ext = $LANG->_('Built in (loaded)');
} else {
  $idn_ext = $LANG->_('Not available');
}

if(function_exists("mhash")) {
  $mhash_ext = $LANG->_('Built in (loaded)');
} else {
  $mhash_ext = $LANG->_('Not available');
}

if(extension_loaded('kadm5') and function_exists("kadm5_init_with_password")) {
  $krb5_ext = $LANG->_('Built in (loaded)');
} else {
  $krb5_ext = $LANG->_('Not available');
}

if((function_exists('ImageCreateFromPng')  && (imagetypes() & IMG_PNG)) or
   (function_exists('ImageCreateFromGif')  && (imagetypes() & IMG_GIF)) or
   (function_exists('ImageCreateFromJpeg') && (imagetypes() & IMG_JPG)))
{
  $gd_ext = $LANG->_('Built in (loaded)');
} else {
  $gd_ext = $LANG->_('Not available');
}
// }}}
  
if(!function_exists("ldap_connect")){
  // {{{ Not availible
  $ldap_ext = '<font color="red">'.$LANG->_('Not available')."</font>";
  $connection = "-";
  $connection_control = "-";
// }}}
} else {
  $ldap_ext = $LANG->_('Built in (loaded)');

  // {{{ User directory connection
  $_pql = new pql($_SESSION["USER_HOST"], '', '', true);
  if(!$_pql->connect($_SESSION["USER_HOST"])) {
	$connection = $LANG->_('Failed');
	
	$server = split(';', $_SESSION["USER_HOST"]);
	$server = urldecode($server[0]); 	// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
	
	// do additional tests
	if(!eregi('^ldap', $_SESSION["USER_HOST"])) {
	  if(!gethostbyname($_SESSION["USER_HOST"]))
		// not resolved
		$connection .= ", " . pql_complete_constant($LANG->_('The hostname %host% could not be resolved'),
													array("host" => $_SESSION["USER_HOST"] ));
	  else {
		// try to open a connection
		if(!fsockopen($_SESSION["USER_HOST"], 389))
		  // impossible to connect
		  $connection .= ", " . pql_complete_constant($LANG->_('Could not connect to port 389 at %host%, please make sure the service is up and that it\'s not blocked by a firewall'),
													  array("host" => $_SESSION["USER_HOST"] ));
	  }
	}
  } else {
	if(!$_pql->bind())
	  $connection = $LANG->_('Connection ok, but could not bind to the directory anonymously');
	else
	  $connection = $LANG->_('Yes');
  }
// }}}
  
  // {{{ Control directory connection
  if(pql_get_define("PQL_CONF_CONTROL_USE")) {
	$_pql_control = new pql_control($_SESSION["USER_HOST"], '', '', true);
	if(!$_pql_control->connect($_SESSION["USER_HOST"])) {
	  $connection_control = $LANG->_('Failed');
	  
	  $host = split('\+', pql_get_define("PQL_CONF_HOST"));
	  $host = split(';', $host[0]);
	  
	  $fqdn = $host[0];
	  $port = $host[1];
	  
	  // do additional tests
	  if(!eregi('^ldap', $_SESSION["USER_HOST"])) {
		if(!gethostbyname($fqdn)) {
		  // not resolved
		  $connection_control .= ", " . pql_complete_constant($LANG->_('The hostname %host% could not be resolved'),
															  array("host" => $fqdn ));
		} else {
		  // try to open a connection
		  if(!fsockopen($fqdn, $port)) {
			// impossible to connect
			$connection_control .= ", " . pql_complete_constant($LANG->_('Could not connect to port 389 at %host%, please make sure the service is up and it is not blocked by a firewall'),
																array("host" => $_SESSION["USER_HOST"]));
		  }
		}
	  }
	} else {
	  if(!$_pql_control->bind())
		$connection_control = $LANG->_('Connection ok, but could not bind to the directory anonymously');
	  else
		$connection_control = $LANG->_('Yes');
	}
  } else
	$connection_control = $LANG->_('Control extension deactivated');
// }}}
  
  // {{{ Access rights
  if($_SESSION["USER_DN"] and $_SESSION["USER_PASS"]) {
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
	foreach($_SESSION["BASE_DN"] as $basedn) {
	  // {{{ Try to set the attribute 'test' in the top DN
	  $fail = check_domain_value($basedn, 'test', 'TRUE');
	  if($fail)
		$TEST["basedn"][$basedn] = $fail;
	  else
		$TEST["basedn"][$basedn] = $LANG->_('Yes');
// }}}
	  
	  // {{{ Test to see if we have access to create domain/branches by creating a subbranch
	  unset($entry);
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "top";
	  if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "dc") {
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "domain";
		$entry[pql_get_define("PQL_ATTR_DC")] = "test";
	  } elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "ou") {
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "organizationalUnit";
		$entry[pql_get_define("PQL_ATTR_OU")] = "test";
	  } elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "o") {
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "organization";
		$entry[pql_get_define("PQL_ATTR_O")] = "test";
	  }
	  $entry[pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)] = "phpQLAdmin_Branch_Test";
	  
	  // Setup the DN
	  $dn = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)."=phpQLAdmin_Branch_Test,".$basedn;
	  
	  if(!@ldap_add($_pql->ldap_linkid, $dn, $entry)) {
		$LDIF = pql_create_ldif("config_ldaptest.php:ldap_add", $dn, $entry, 1);
		
		$TEST["branches"][$basedn] = "<a href=\"javascript:ldifWindow('".$LDIF."')\">".
		  $LANG->_('No. Reason:\n')."<b>'".ldap_error($_pql->ldap_linkid)."'</b>";
	  } else {
		// Success - delete it again
		ldap_delete($_pql->ldap_linkid, $dn);
		
		$TEST["branches"][$basedn] = $LANG->_('Yes');
	  }
// }}}
	  
	  // {{{ Check write access (w/o ACI's)
	  $filter = "(&" . pql_setup_branch_objectclasses(1, $basedn)
		. "(" . pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) . "=*))";
	  
	  $sr = @ldap_list($_pql->ldap_linkid, $basedn, $filter)
		or pql_format_error(1);
	  $info = @ldap_get_entries($_pql->ldap_linkid, $sr)
		or pql_format_error(1);
	  
	  if(! $info["count"]) {
		// Didn't find anything on a one-level search, try a global one...
		$sr = @ldap_search($_pql->ldap_linkid, $basedn, $filter)
		  or pql_format_error(1);
		$info = @ldap_get_entries($_pql->ldap_linkid, $sr)
		  or pql_format_error(1);
	  }
	  
	  for ($i=0; $i<$info["count"]; $i++) {
		$domains[] = $info[$i]["dn"];
	  }
// }}}
	}
	
	if(is_array($domains)) {
	  foreach($domains as $domain) {
		// Check write access
		$fail = check_domain_value($domain, 'test', 'TRUE');
		if($fail) {
		  $TEST["branches"][$domain] = $fail;
		} else {
		  $TEST["branches"][$domain] = $LANG->_('Yes');
		}
	  }
	}
	
	// {{{ Check write access (w/ ACI's)
	if(!empty($_SESSION["ACI_SUPPORT_ENABLED"])) {
	  foreach($_SESSION["BASE_DN"] as $basedn) {
		// Setup the LDIF we're adding
		unset($entry);
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "top";
		if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "dc") {
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "domain";
		  $entry[pql_get_define("PQL_ATTR_DC")] = "test";
		} elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "ou") {
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "organizationalUnit";
		  $entry[pql_get_define("PQL_ATTR_OU")] = "test";
		} elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn) == "o") {
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = "organization";
		  $entry[pql_get_define("PQL_ATTR_O")] = "test";
		}
		$entry[pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)] = "phpQLAdmin_Branch_Test";
		
		// Add the ACI entries to the object
		$ol_ver = $_pql->find_ldap_version();
		if(ereg('^2\.[34]', $ol_ver))
		  // Actually this is a little wrong. There was a bug on <2.3.32 which was
		  // fixed 20070214 (re23), so it should be ok in 2.3.33 when/if that is
		  // released. Also 2.4 will use this format (fix actually backported from 2.4)...
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")][] =  "0#entry#grant;w,r,s,c;entry#access-id#".$_SESSION["USER_DN"];
		else
		  $entry[pql_get_define("PQL_ATTR_LDAPACI")][] =  "0#entry#grant;w,r,s,c;[entry]#access-id#".$_SESSION["USER_DN"];
		$entry[pql_get_define("PQL_ATTR_LDAPACI")][] =  "1#entry#grant;w,r,s,c;[all]#access-id#".$_SESSION["USER_DN"];
		
		// Setup the DN
		$dn = pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $basedn)."=phpQLAdmin_Branch_Test,".$basedn;
		
		if(!@ldap_add($_pql->ldap_linkid, $dn, $entry)) {
		  $LDIF = pql_create_ldif("config_ldaptest.php:ldap_add", $dn, $entry, 1);
		  
		  $TEST["acis"][$basedn] = "<a href=\"javascript:ldifWindow('".$LDIF."')\">".
			$LANG->_('No. Reason:\n')."<b>'".ldap_error($_pql->ldap_linkid)."'</b>";
		} else {
		  // Success - delete it again
		  ldap_delete($_pql->ldap_linkid, $dn);
		  $TEST["acis"][$basedn] = $LANG->_('Yes');
		}
	  }
	} else {
	  foreach($_SESSION["BASE_DN"] as $basedn)
		$TEST["acis"][$basedn] = "".$LANG->_("ACI's deactivated")."";
	}
// }}}
 }
// }}}
} // end if(function_exists...
  
include($_SESSION["path"]."/header.html");
?>
  <script type="text/javascript" language="javascript"><!--
    function ldifWindow(string) {
      myWindow = window.open("", "LDIFWindow", 'toolbar=no,menubar=no,status=no,dependent=yes,width=350,height=200');

	  myWindow.document.write('<html>\n  <head>\n      <title>phpQLAdmin LDIF</title>\n  </head>\n');
	  myWindow.document.write('  <body>\n');
      myWindow.document.write('    '+string+'\n');
	  myWindow.document.write('  </body>\n</html>');

      myWindow.document.bgColor="white";
      myWindow.document.close();
    }
  //--></script>

  <span class="title1"><?=$LANG->_('phpQLAdmin ldap connection')?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('PHP Extensions needed')?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$ldap_ext?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('IDN')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$idn_ext?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('MHASH')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$idn_ext?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('MemCache')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$memcache_ext?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('GD')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$gd_ext?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('KRB5')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$krb5_ext?>&nbsp;</td>
      </tr>
    </th>

<?=pql_format_table_empty(2)?>

    <th colspan="3" align="left"><?=$LANG->_('LDAP server connection and setup tests')?>
      <tr>
        <td class="title"><?=$LANG->_('User directory connection')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$connection?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Control directory connection')?></td>
        <td class="<?php pql_format_table(); ?>"><?=$connection_control?>&nbsp;</td>
      </tr>
<?php if($_SESSION["ADVANCED_MODE"] == 1) {
		// Just so that we don't have to call pql_get_subschemas() multiple times here - takes time!
		$ldap = pql_get_subschemas();
?>

      <tr></tr>

      <tr>
        <td class="title"><?=$LANG->_('LDAP server contains phpQLAdminConfig objectclass')?></td>
<?php	 if($ldap["objectclasses"]["phpqladminconfig"]) { ?>
        <td class="<?php pql_format_table(); ?>"><?=$LANG->_('Yes')?></td>
<?php    } else { ?>
        <td class="<?php pql_format_table(); ?>"><?=$LANG->_('No')?>. <?=$LANG->_('Please load file')?> <a href="phpQLAdmin.schema">phpQLAdmin.schema</a>&nbsp;</td>
<?php    } ?>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('LDAP server contains phpQLAdminBranch objectclass')?></td>
<?php	 if($ldap["objectclasses"]["phpqladminbranch"]) { ?>
        <td class="<?php pql_format_table(); ?>"><?=$LANG->_('Yes')?></td>
<?php    } else { ?>
	<td class="<?php pql_format_table(); ?>"><?=$LANG->_('No')?>. <?=$LANG->_('Please load file')?> <a href="phpQLAdmin.schema">phpQLAdmin.schema</a>&nbsp;</td>
<?php    } ?>
      </tr>

      <tr>
        <td class="title">LDAP server contains schema for <a href="http://www.ietf.org/rfc/rfc2377.txt" target="_new">RFC 2377</a></td>
<?php	 if($ldap["objectclasses"]["dcorganizationnameform"]) { ?>
        <td class="<?php pql_format_table(); ?>"><?=$LANG->_('Yes')?></td>
<?php    } else { ?>
        <td class="<?php pql_format_table(); ?>"><?=$LANG->_('No')?>. <?=$LANG->_('Please load file')?> <a href="rfc2377.schema">rfc2377.schema</a>&nbsp;</td>
<?php    }
      }
?>
      </tr>
    </th>
<?php if($basedn) { ?>

<?=pql_format_table_empty(2)?>

    <th colspan="3" align="left"><?=$LANG->_('Modification access - phpQLAdmin configuration')?>
<?php    foreach($_SESSION["BASE_DN"] as $dn) { ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to write phpQLAdmin configuration in DN %dn%'), array('dn' => $dn)); ?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$TEST["basedn"][$dn]?></td>
      </tr>

<?php    } ?>
    </th>

<?=pql_format_table_empty(2)?>

    <th colspan="3" align="left"><?=$LANG->_('Branch creation access')?>
<?php    foreach($_SESSION["BASE_DN"] as $dn) { ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to create branches in DN %dn%'), array('dn' => $dn)); ?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$TEST["branches"][$dn]?></td>
      </tr>

      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to create branch with ACIs in DN %dn%'), array('dn' => $dn)); ?></td>
        <?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><?=$TEST["acis"][$dn]?></td>
      </tr>

      <tr></tr>

<?php    } ?>
    </th>

<?=pql_format_table_empty(2)?>

    <th colspan="3" align="left"><?=$LANG->_('DN modification access - domain DN\'s')?>
<?php    if(is_array($domains)) {
            asort($domains);
		    foreach($domains as $key => $domain) {
?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Access to modify DN \b%dn%\B'), array('dn' => $domain)); ?></b></td>
        <?php $class=pql_format_table(0); ?>
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
                <td>
                  <ul>
                    <li><?=$LANG->_('This is a simple test to show if the ldap extension is loaded\nand that the connections are working. It also does some\nrudimentary ACL tests')?>.</li>
                    <li><?=$LANG->_("NOTE: If you get denied access to create branches but when<br>trying with ACIs, it works. This doesn't mean that you don't<br>have access to create branches, it means that you \umust\U use<br>ACIs, otherwise you will not be allowed to create an object<br>(you will receive 'Insufficient access').")?></li>
                  </ul>
                </td>
              </tr>
          </table>
        </td>

        <td></td>
      </tr>
    </th>
  </table>
</body>
</html>

<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
