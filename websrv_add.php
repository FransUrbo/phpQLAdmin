<?php
// Add a webserver configuration to the LDAP db
// $Id: websrv_add.php,v 2.24 2007-03-14 12:10:53 turbo Exp $
//
// {{{ Setup session
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_websrv.inc");

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Verify all submitted values
if($_REQUEST["submit"]) {
	$error = false;
	$error_text = array();
	
	if($_REQUEST["type"] == "location") {
	  if(!$_REQUEST["mountpoint"]) {
		$error = true;
		$error_text["mountpoint"] = $LANG->_('Missing');
	  }
	} else {
	  // {{{ Calculate number of _choosen_ servers
	  unset($hosts);
	  $choosen_servers = 0;
	  $amount = $_REQUEST["hosts"];
	  if($amount) {
		if($_REQUEST["host_".$amount] != 'on') {
		  for($i=0; $i <= $amount; $i++)
			if($_REQUEST["host_".$i]) {
			  $choosen_servers++;
			  $hosts[] = urldecode($_REQUEST["host_".$i]);
			}
		}
	  }

	  if(@$_REQUEST["host"])
		$choosen_servers++;
	  // }}}

	  if(!@$_REQUEST["serverip"] and ($choosen_servers < 1)) {
		// Error if: We've only choosen one server
		$error = true;
		$error_text["serverip"] = $LANG->_('Missing');
	  }
	  
	  if(!$_REQUEST["serverurl"]) {
		$error = true;
		$error_text["serverurl"] = $LANG->_('Missing');
	  }
	  // ----------------------------------------------------
	  //	  } elseif(!ereg('\/$', $_REQUEST["serverurl"]))
	  //		$_REQUEST["serverurl"] .= '/';
	  // BUG: Have no idea where this came from!
	  // 		Entered 'www.bayour.com' in the 'Server FQDN'
	  //		question and ended up with a server URL
	  //		such as 'www.bayour.com/'!
	  // ----------------------------------------------------
	  
	  if($_REQUEST["type"] != 'websrv') {
		if(!$_REQUEST["serveradmin"]) {
		  $error = true;
		  $error_text["serveradmin"] = $LANG->_('Missing');
		} elseif(!ereg('@', $_REQUEST["serveradmin"])) {
		  $error = true;
		  $error_text["serveradmin"] = $LANG->_('Invalid');
		}
		
		if(!$_REQUEST["documentroot"]) {
		  $error = true;
		  $error_text["documentroot"] = $LANG->_('Missing');
		} elseif(!ereg('^\/', $_REQUEST["documentroot"])) {
		  $error = true;
		  $error_text["documentroot"] = $LANG->_('Invalid');
		} elseif(!ereg('\/$',  $_REQUEST["documentroot"]))
		  $_REQUEST["documentroot"] .= '/';
	  }
	}

	if($_REQUEST["type"] == "vrtsrv") {
	  // Last, but not least, check to see that the virtual host doesn't already exist
	  $dn = $_pql->get_dn($_REQUEST["server"], pql_get_define("PQL_ATTR_WEBSRV_SRV_URL").'='.$_REQUEST["serverurl"]);
	  if($dn[0]) {
		$error = true;
		$error_text["serverurl"] = $LANG->_('Already exists');
	  }
	}
}

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);

  if($error) {
	echo "Errors:";
	printr($error_text);
  }
}
// }}}

if(($error == 'true') or !$_REQUEST["type"] or
   (($_REQUEST["type"] == "websrv")   and (!$_REQUEST["serverport"] or !$_REQUEST["serverurl"])) or 
   (($_REQUEST["type"] == "vrtsrv")   and (!$_REQUEST["serverurl"] or !$_REQUEST["serveradmin"] or !$_REQUEST["documentroot"])) or
   (($_REQUEST["type"] == "location") and  !$_REQUEST["mountpoint"]))
{
  // {{{ Show the input form
  if($_REQUEST["virthost"]) {
	// {{{ Page title
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a virtual host location for %virthost%'), array('virthost' => $_REQUEST["virthost"])); ?></span>
<?php
  } elseif($_REQUEST["server"]) {
	$server_reference = $_pql->get_attribute($_REQUEST["server"], pql_get_define("PQL_ATTR_CN"));
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a virtual host for %server%'), array('server' => $server_reference)); ?></span>
<?php }
// }}}
?>

  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($_REQUEST["virthost"]) {
		// {{{ Input for a Virtual Host Location Object
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual host location'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Location mount point')?></td>
          <td><?php echo pql_format_error_span($error_text["mountpoint"]); ?><input type="text" name="mountpoint" size="40" value="<?php echo $_REQUEST["mountpoint"]?>"></td>
        </tr>

        <!-- -------------------- -->

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Access order')?></td>
          <td>
            <select name="access_order">
              <option value="deny_allow"><?php echo $LANG->_('Deny, Allow')?></option>
              <option value="allow_deny"><?php echo $LANG->_('Allow, Deny')?></option>
            </select>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Access (Deny)')?></td>
          <td><?php echo pql_format_error_span($error_text["access_deny"]); ?><input type="text" name="access_deny" size="40" value="<?php echo $_REQUEST["access_deny"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Access (Allow)')?></td>
          <td><?php echo pql_format_error_span($error_text["access_allow"]); ?><input type="text" name="access_allow" size="40" value="<?php echo $_REQUEST["access_allow"]?>"></td>
        </tr>

        <!-- -------------------- -->

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Options')?></td>
          <td>
            <!-- See http://httpd.apache.org/docs/1.3/mod/core.html -->
            <?php echo pql_format_error_span($error_text["options"]); ?>
            <input type="checkbox" name="all" CHECKED><?php echo $LANG->_('All')?><br>
            <input type="checkbox" name="index"><?php echo $LANG->_('Indexes')?><br>
            <input type="checkbox" name="multiview"><?php echo $LANG->_('MultiViews')?><br>
            <input type="checkbox" name="execcgi"><?php echo $LANG->_('ExecCGI')?><br>
            <input type="checkbox" name="followsym"><?php echo $LANG->_('FollowSymlinks')?><br>
            <input type="checkbox" name="includes"><?php echo $LANG->_('Includes')?><br>
            <input type="checkbox" name="includesnoexec"><?php echo $LANG->_('IncludesNOEXEC')?><br>
            <input type="checkbox" name="symlinksifownermatch"><?php echo $LANG->_('SymLinksIfOwnerMatch')?><br>
          </td>
        </tr>

        <input type="hidden" name="type"     value="location">
        <input type="hidden" name="server"   value="<?php echo urlencode($_REQUEST["server"])?>">
        <input type="hidden" name="virthost" value="<?php echo urlencode($_REQUEST["virthost"])?>">
<?php
		// }}}

	  } elseif($_REQUEST["server"] or ($_REQUEST["ref"] == 'domain')) {
		// {{{ Input for Virtual Host Object
		if(empty($_REQUEST["serverip"])) {
		  // Get the IP from the physical server
		  $serverip = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_IPHOSTNUMBER"));

		  // Extract port (if any) from the webserver
		  $port = preg_replace('/.*:/', '', $server_reference);
		  if($port)
			$serverip .= ":$port";
		} else
		  $serverip = $_REQUEST["serverip"];

		if(empty($_REQUEST["serverurl"]))
		  $serverurl = preg_replace('/:.*/', '', $server_reference); // Remove the port (if any) from the web server
		else
		  $serverurl = $_REQUEST["serverurl"];

		if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
		  // Super admin in advanced mode - First get all physical servers
		  $filter = '(&(cn=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))';
		  $physical = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"], $filter, 'ONELEVEL');
		  if(is_array($physical)) {
			// For each physical host, get all its web servers
			$servers = array();
			for($i=0; $physical[$i]; $i++) {
			  $tmp = pql_websrv_find_servers($physical[$i]);
			  if(is_array($tmp))
				$servers = $servers + $tmp;
			}
		  }
		}
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual host'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Server FQDN')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?php echo $serverurl?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Server IP[:PORT]')?></td>
          <td><?php echo pql_format_error_span($error_text["serverip"]); ?><input type="text" name="serverip" size="40" value="<?php echo $serverip?>"></td>
        </tr>
<?php if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"] and is_array($servers) and (count($servers) > 1)) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo $LANG->_('The IP:PORT input is ignore if \imore\I than one\n\bAdd to existing server\B option below is selected!')?></td>
        </tr>

        <?php echo pql_format_table_empty(2)?>
<?php } ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Server Administrator')?></td>
          <td><?php echo pql_format_error_span($error_text["serveradmin"]); ?><input type="text" name="serveradmin" size="40" value="<?php echo $_REQUEST["serveradmin"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Document root')?></td>
          <td><?php echo pql_format_error_span($error_text["documentroot"]); ?><input type="text" name="documentroot" size="40" value="<?php echo $_REQUEST["documentroot"]?>"></td>
        </tr>
<?php if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"] and is_array($servers) and (count($servers) > 1)) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Add to existing server')?></td>
          <td>
            <?php
			$i = 1;
			foreach($servers as $server_fqdn => $server_dn) {
			  if($server_dn != $_REQUEST["server"]) {

				// Mark any previously (as in missing value reloads the form) selected host(s).
				$mark = '';
				for($j=0; $hosts[$j]; $j++) {
				  if($server_dn == $hosts[$j])
					$mark = ' CHECKED';
				}
?>
            <input type="checkbox" name="host_<?php echo $i?>" value="<?php echo urlencode($server_dn)?>"<?php echo $mark?>><b><?php echo $server_fqdn?></b><br>
<?php		  } else { ?>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b><?php echo $server_fqdn?></b>&nbsp;<font color=red>(<?php echo $LANG->_('Preselected')?>)</font><br>
            <input type="hidden"   name="host_<?php echo $i?>" value="<?php echo urlencode($server_dn)?>">
<?php		  }

			  $i++;
	 		}
?>
            <input type="hidden"   name="hosts"  value="<?php echo count($servers)?>">
          </td>
        </tr>
<?php	} else { ?>
        <input type="hidden" name="host_1" value="<?php echo urlencode($_REQUEST["server"])?>">
        <input type="hidden" name="hosts" value="1">
<?php	} ?>
        <input type="hidden" name="type" value="vrtsrv">

<?php	if(pql_get_define("PQL_CONF_BIND9_USE") and $_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
			// Only offer this if super admin - we might need write access to some other domain/branch (i.e. a domain/branch
			// where the specific DNS domain is located)
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Create DNS object')?></td>
          <td><input type="checkbox" name="dns" CHECKED>&nbsp;<?php echo $LANG->_('Yes')?></td>
        </tr>

<?php	}

		if(pql_get_define("PQL_CONF_CONTROL_USE") and $_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
			// Only offer this if super admin - we might need write access to some other domain/branch (i.e. a domain/branch
			// where the specific DNS domain is located)
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Update QLC object(s)')?></td>
          <td><input type="checkbox" name="qlc" CHECKED>&nbsp;<?php echo $LANG->_('Yes')?></td>
        </tr>

<?php	}
		// }}}

	  } else {
		// {{{ Input for Web Server Device
		// Set a default port
		if(!$_REQUEST["serverport"])
		  $_REQUEST["serverport"] = 80;

		// Retreive the FQDN of physical host
		$server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('web server'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Server FQDN')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?php if($_REQUEST["serverurl"]) { echo $_REQUEST["serverurl"]; } else { echo "$server_reference"; } ?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Listen port')?></td>
          <td><?php echo pql_format_error_span($error_text["serverport"]); ?><input type="text" name="serverport" size="40" value="<?php echo $_REQUEST["serverport"]?>"></td>
        </tr>

        <input type="hidden" name="type" value="websrv">
<?php
// }}}
	  }
?>
      </th>
    </table>

    <input type="hidden" name="submit" value="submit">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <input type="hidden" name="host"   value="<?php echo urlencode($_REQUEST["host"])?>">
    <input type="hidden" name="server" value="<?php echo urlencode($_REQUEST["server"])?>">
<?php if(@$_REQUEST["rootdn"]) { ?>
    <input type="hidden" name="rootdn" value="<?php echo urlencode($_REQUEST["rootdn"])?>">
<?php } ?>
<?php if(@$_REQUEST["domain"]) { ?>
    <input type="hidden" name="domain" value="<?php echo urlencode($_REQUEST["domain"])?>">
<?php } ?>
<?php if(@$_REQUEST["ref"]) { ?>
    <input type="hidden" name="ref"    value="<?php echo urlencode($_REQUEST["ref"])?>">
<?php } ?>
    <br>
    <input type="submit" value="Create">
  </form>
<?php
// }}}
} else {
  // {{{ No errors (i.e. no missing values). We're good to go!
  if($_REQUEST["type"] == "vrtsrv") {
	// {{{ Create a virtual host
	$IPs = array();
	for($i=0; $hosts[$i]; $i++) {
	  // {{{ Extract the host FQDN from the URL. A little crude...
	  if(eregi('.*://', $_REQUEST["serverurl"]))
		$fqdn = eregi_replace('.*://', '', $_REQUEST["serverurl"]);
	  else
		$fqdn = $_REQUEST["serverurl"];
	  if(eregi(':', $fqdn)) { $fqdn = eregi_replace(':.*', '', $fqdn); }
	  if(eregi('/', $fqdn)) { $fqdn = eregi_replace('/.*', '', $fqdn); }
	  // }}}
	  
	  // {{{ Setup the entry array
	  $entry = array();
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]		= $_REQUEST["serverurl"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]	= $_REQUEST["serveradmin"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]		= $_REQUEST["documentroot"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]	= $fqdn;

	  if(count($hosts) > 1) {
		$tmp = split(',', $hosts[$i]);	// Extract the webserver container
		$tmp = split('=', $tmp[0]);		// Extract the FQDN:PORT of the webserver container
		$tmp = split(':', $tmp[1]);		// Separate FQDN and PORT from the webserver container
		$hostname = $tmp[0]; $port = $tmp[1];

		// Find the IP address of this host.
		$ip = gethostbyname(trim($hostname));
		$IPs[] = $ip;

		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]		= $ip;
		if(@$port)
		  // Got a port. Remember that!
		  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]	   .= ':'.$port;
	  } else
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]		= $_REQUEST["serverip"];
	  // }}}
	  
	  // {{{ Add the web server object
	  $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$fqdn.",".$hosts[$i];
	  if(pql_websrv_add_server($dn, $entry, $_REQUEST["type"]))
		$msg = "Successfully added webserver configuration ".$_REQUEST["serverurl"];
	  else
		$msg = "Failed to add webserver configuration ".$_REQUEST["serverurl"];
	  // }}}
	}

	// {{{ Create the DNS object(s)
	if($_REQUEST["dns"] and pql_get_define("PQL_CONF_BIND9_USE")) {
	  require($_SESSION["path"]."/include/pql_bind9.inc");
	  
	  // Separate the domainname and hostname from the FQDN
	  $tmp = pql_separate_host_domain($_REQUEST["serverurl"]);
	  $hostname = $tmp[0]; $domainname = $tmp[1];
	  
	  // Get the root DN
	  $rootdn = pql_get_rootdn($_SESSION["USER_SEARCH_DN_CTR"], 'websrv_add.php'); $rootdn = urldecode($rootdn);
	  
	  // Find out which domain/branch that have this zone (if any).
	  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME")."=$domainname)(".pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")."=@))";
	  $soadn = $_pql->get_dn($rootdn, $filter);
	  if(@soadn and is_array($soadn)) {
		// soa => 'dNSTTL=3600 relativeDomainName=@,dc=bayour,dc=com,ou=DNS,o=Bayour.COM,c=SE'
		$soadn = $soadn[0];
		
		// From the SOA DN, remove the zone stuff and get the domain/branch
		$dn_parts = split(',', $soadn);
		
		// Put toghether the branch DN. It SHOULD start at position 4 (after the 'ou=DNS')
		// BUG: This is most likley NOT true....
		$branch = '';
		for($i=4; $dn_parts[$i]; $i++) {
		  $branch .= $dn_parts[$i];
		  if($dn_parts[$i+1])
			$branch .= ',';
		}
		
		// Remove any port from the server IP
		if(ereg(':', $_REQUEST["serverip"]))
		  $serverip =  ereg_replace(':.*', '', $_REQUEST["serverip"]);
		elseif(@$_REQUEST["serverip"])
		  $serverip = $_REQUEST["serverip"];
		
		// Create a host entry
		unset($entry);
		$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($hostname);
		$entry[pql_get_define("PQL_ATTR_ZONENAME")]				= $domainname;
		$entry[pql_get_define("PQL_ATTR_DNSTTL")]				= 604800;

		if($serverip)
		  $entry[pql_get_define("PQL_ATTR_ARECORD")]			= $serverip;
		elseif(is_array($IPs)) {
		  asort($IPs);
		  for($i=0; $IPs[$i]; $i++)
			$entry[pql_get_define("PQL_ATTR_ARECORD")][]		= $IPs[$i];
		}
		
		$dn = pql_bind9_add_host($branch, $entry);
		if($dn) {
		  $msg .= "<br>Successfully added host $hostname to DNS";

		  // Update the DNS domain SOA record!
		  if(!pql_get_define("PQL_CONF_DEBUG_ME")) {
			// We can't do this if we're debuging. The object don't exists in the db, hence
			// we can't figure out zone name etc...
			if(!pql_bind9_update_serial($dn))
			  die("failed to update SOA serial number");
		  } else
			echo $LANG->_("\bCan't update SOA since we're running in DEBUG_ME mode!\B")."<p>";
		} else
		  $msg .= "<br>Failed to add host $hostname";
	  } else
		$msg .= "<br>Could not find branch";
	}
// }}}

	// {{{ Update additional domain name(s) and QLC object(s)
	if($_REQUEST["qlc"] and pql_get_define("PQL_CONF_CONTROL_USE")) {
	  // Separate the domainname and hostname from the FQDN
	  $tmp = pql_separate_host_domain($_REQUEST["serverurl"]);
	  $server_domain = $tmp[1];

	  // {{{ Fetch old (attrib) values from DB
	  unset($entry);
	  $oldvalues = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
	  if($oldvalues) {
		if(!is_array($oldvalues))
		  $oldvalues = array($oldvalues);
		
		foreach($oldvalues as $val)
		  $entry[pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")][] = $val;
	  } else
		// No previous values
		$entry[pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")][] = $server_domain;
	  // }}}

	  if(pql_modify_attribute($_REQUEST["domain"], '', '', $entry)) {
		$msg .= "<br>".pql_complete_constant($LANG->_('Successfully changed %what%'),
											 array('what' => $LANG->_('Additional domain name')));
	  
		// {{{ Update locals and/or rcptHosts
		$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		$attribs = array(pql_get_define("PQL_ATTR_LOCALS")    => pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"]),
						 pql_get_define("PQL_ATTR_RCPTHOSTS") => pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"]));
		foreach($attribs as $attrib => $autoadd) {
		  if($autoadd)
			$entry = array('', $server_domain);
		}

		pql_control_update_domains($_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"], '*', $entry);
		// }}}
	  } else
		$msg .= "<br>".pql_complete_constant($LANG->_('Failed to change %what%'),
											 array('what' => $LANG->_('Additional domain name')))
		  . ": " . ldap_error($_pql->ldap_linkid);
	}
	// }}}

	// Fix the DN so that it works with the redirect below
	$_REQUEST["virthost"] = $_REQUEST["serverurl"];
	$dn = urldecode($_REQUEST["host_1"]);
// }}}

  } elseif($_REQUEST["type"] == "websrv") {
	// {{{ Create a webserver device
	if($_REQUEST["serverport"])
	  $_REQUEST["serverurl"] .= ":".$_REQUEST["serverport"];

	// {{{ Setup the DN and the entry array
	$dn = pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["serverurl"].",".$_REQUEST["host"];
	$entry[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["serverurl"];
	// }}}

	if(pql_websrv_add_server($dn, $entry, $_REQUEST["type"]))
	  $msg = "Successfully added webserver configuration ".$_REQUEST["serverurl"];
	else
	  $msg = "Failed to add webserver configuration ".$_REQUEST["serverurl"];
	// }}}

  } elseif($_REQUEST["type"] == "location") {
	// {{{ Create a virtual host location
	// {{{ Setup the DN
	$dn  = pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")."=".$_REQUEST["mountpoint"].",";
	$dn .= pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$_REQUEST["virthost"].",";
	$dn .= urldecode($_REQUEST["server"]);

	// For the redirect below
	$_REQUEST["hostdir"] = $_REQUEST["mountpoint"];
	// }}}

	// {{{ Setup the entry array
	$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")] = $_REQUEST["mountpoint"];

	// {{{ Add the Allow/Deny options
	if($_REQUEST["access_deny"] or $_REQUEST["access_allow"]) {
	  if($_REQUEST["deny_allow"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")] = "deny,allow";
	  else
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")] = "allow,deny";

	  // Deny
	  if(isset($_REQUEST["access_deny"])) {
		if(eregi('\*', $_REQUEST["access_deny"]))
		  $_REQUEST["access_deny"] = "all";

		if(!eregi('^from', $_REQUEST["access_deny"]))
		  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY")]  = "from ".$_REQUEST["access_deny"];
		else
		  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY")]  = $_REQUEST["access_deny"];
	  }

	  // Allow
	  if(isset($_REQUEST["access_allow"])) {
		if(eregi('\*', $_REQUEST["access_allow"]))
		  $_REQUEST["access_allow"] = "all";

		if(!eregi('^from', $_REQUEST["access_allow"]))
		  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW")] = "from ".$_REQUEST["access_allow"];
		else
		  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW")] = $_REQUEST["access_allow"];
	  }
	}
	// }}}

	// {{{ Add the additional option(s)
	if($_REQUEST["all"])
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")] = "All";
	else {
	  if($_REQUEST["index"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "Indexes";
	  if($_REQUEST["multiview"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "MultiViews";
	  if($_REQUEST["execcgi"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "ExecCGI";
	  if($_REQUEST["followsym"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "FollowSymlinks";
	  if($_REQUEST["includes"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "Includes";
	  if($_REQUEST["includesnoexec"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "IncludesNOEXEC";
	  if($_REQUEST["symlinksifownermatch"])
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")][] = "SymLinksIfOwnerMatch";
	}
	// }}}
	// }}}

	if(pql_websrv_add_server($dn, $entry, $_REQUEST["type"]))
	  $msg = "Successfully added the virtual host location ".$_REQUEST["mountpoint"];
	else
	  $msg = "Failed to add webserver configuration ".$_REQUEST["mountpoint"];
	// }}}

  } else
	$msg = "Wrong type.";

  // {{{ Redirect to host details page
  if(@$_REQUEST["rootdn"] and @$_REQUEST["domain"]) 
	// Values already URL encoded...
	$url = "domain_detail.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"];
  elseif(@$_REQUEST["host"] and !ereg('%3D', $_REQUEST["host"]))
	$url = "host_detail.php?host=".urlencode($_REQUEST["host"]);
  else
	$url = "host_detail.php?host=".$_REQUEST["host"];

  if(@$_REQUEST["server"] and !ereg('%3D', $_REQUEST["server"]))
	$url .= "&server=".urlencode($_REQUEST["server"]);
  elseif(!@$_REQUEST["rootdn"] and !@$_REQUEST["domain"])
	$url .= "&server=".$_REQUEST["server"];

  if($_REQUEST["virthost"]) {
	if(!@$_REQUEST["domain"])
	  $url .= "&virthost=".$_REQUEST["virthost"];
	else
	  $url .= "&host=".$_REQUEST["virthost"];
  }


  if($_REQUEST["hostdir"])  $url .= "&hostdir=".$_REQUEST["hostdir"];
  $url .= "&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
  if(($_REQUEST["type"] == "websrv") or ($_REQUEST["type"] == "vrtsrv"))
	// Only reload left frame if adding a web server container or a virtual host
	$url .= "&rlnb=1";
  
  pql_header($url);
  // }}}
// }}}
}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
