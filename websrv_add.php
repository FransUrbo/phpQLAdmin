<?php
// Add a webserver configuration to the LDAP db
// $Id: websrv_add.php,v 2.19.2.7 2006-11-25 12:16:17 turbo Exp $
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

	  if(!$_REQUEST["serverip"] and ($choosen_servers < 1)) {
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
	  $dn = pql_get_dn($_pql->ldap_linkid, $_REQUEST["server"], pql_get_define("PQL_ATTR_WEBSRV_SRV_URL").'='.$_REQUEST["serverurl"]);
	  if($dn[0]) {
		$error = true;
		$error_text["serverurl"] = $LANG->_('Already exists');
	  }
	}
}

if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);
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
	$server_reference = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["server"], pql_get_define("PQL_ATTR_CN"));
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a virtual host for %server%'), array('server' => $server_reference)); ?></span>
<?php }
// }}}
?>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($_REQUEST["virthost"]) {
		// {{{ Input for a Virtual Host Location Object
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual host location'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Location mount point')?></td>
          <td><?php echo pql_format_error_span($error_text["mountpoint"]); ?><input type="text" name="mountpoint" size="40" value="<?=$_REQUEST["mountpoint"]?>"></td>
        </tr>

        <!-- -------------------- -->

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Access order')?></td>
          <td>
            <select name="access_order">
              <option value="deny_allow"><?=$LANG->_('Deny, Allow')?></option>
              <option value="allow_deny"><?=$LANG->_('Allow, Deny')?></option>
            </select>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Access (Deny)')?></td>
          <td><?php echo pql_format_error_span($error_text["access_deny"]); ?><input type="text" name="access_deny" size="40" value="<?=$_REQUEST["access_deny"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Access (Allow)')?></td>
          <td><?php echo pql_format_error_span($error_text["access_allow"]); ?><input type="text" name="access_allow" size="40" value="<?=$_REQUEST["access_allow"]?>"></td>
        </tr>

        <!-- -------------------- -->

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td>
            <!-- See http://httpd.apache.org/docs/1.3/mod/core.html -->
            <?php echo pql_format_error_span($error_text["options"]); ?>
            <input type="checkbox" name="all" CHECKED><?=$LANG->_('All')?><br>
            <input type="checkbox" name="index"><?=$LANG->_('Indexes')?><br>
            <input type="checkbox" name="multiview"><?=$LANG->_('MultiViews')?><br>
            <input type="checkbox" name="execcgi"><?=$LANG->_('ExecCGI')?><br>
            <input type="checkbox" name="followsym"><?=$LANG->_('FollowSymlinks')?><br>
            <input type="checkbox" name="includes"><?=$LANG->_('Includes')?><br>
            <input type="checkbox" name="includesnoexec"><?=$LANG->_('IncludesNOEXEC')?><br>
            <input type="checkbox" name="symlinksifownermatch"><?=$LANG->_('SymLinksIfOwnerMatch')?><br>
          </td>
        </tr>

        <input type="hidden" name="type"     value="location">
        <input type="hidden" name="server"   value="<?=urlencode($_REQUEST["server"])?>">
        <input type="hidden" name="virthost" value="<?=urlencode($_REQUEST["virthost"])?>">
<?php
		// }}}

	  } elseif($_REQUEST["server"] or ($_REQUEST["ref"] == 'domain')) {
		// {{{ Input for Virtual Host Object
		if(empty($_REQUEST["serverip"])) {
		  // Get the IP from the physical server
		  $serverip = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["host"], pql_get_define("PQL_ATTR_IPHOSTNUMBER"));

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
		  $physical = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter, 'ONELEVEL');
		  if(is_array($physical)) {
			// For each physical host, get all its web servers
			$servers = array();
			for($i=0; $physical[$i]; $i++) {
			  $tmp = pql_websrv_find_servers($_pql->ldap_linkid, $physical[$i]);
			  if(is_array($tmp))
				$servers = $servers + $tmp;
			}
		  }
		}
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual host'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server FQDN')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?=$serverurl?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server IP[:PORT]')?></td>
          <td><?php echo pql_format_error_span($error_text["serverip"]); ?><input type="text" name="serverip" size="40" value="<?=$serverip?>"></td>
        </tr>
<?php if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"] and is_array($servers) and (count($servers) > 1)) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('The IP:PORT input is ignore if \imore\I than one\n\bAdd to existing server\B option below is selected!')?></td>
        </tr>
<?php } ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server Administrator')?></td>
          <td><?php echo pql_format_error_span($error_text["serveradmin"]); ?><input type="text" name="serveradmin" size="40" value="<?=$_REQUEST["serveradmin"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Document root')?></td>
          <td><?php echo pql_format_error_span($error_text["documentroot"]); ?><input type="text" name="documentroot" size="40" value="<?=$_REQUEST["documentroot"]?>"></td>
        </tr>
<?php if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"] and is_array($servers) and (count($servers) > 1)) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Add to existing server')?></td>
          <td>
            <?php
			echo pql_format_error_span($error_text["documentroot"])."\n";
			$i = 1;
			foreach($servers as $server_fqdn => $server_dn) {
			  if($server_dn != $_REQUEST["server"]) {
?>
            <input type="checkbox" name="host_<?=$i?>" value="<?=urlencode($server_dn)?>"><b><?=$server_fqdn?></b><br>
<?php		  } else { ?>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b><?=$server_fqdn?></b>&nbsp;<font color=red>(<?=$LANG->_('Preselected')?>)</font><br>
            <input type="hidden"   name="host_<?=$i?>" value="<?=urlencode($server_dn)?>">
<?php		  }

			  $i++;
	 		}
?>
            <input type="hidden"   name="hosts"  value="<?=count($servers)?>">
          </td>
        </tr>
<?php	} else { ?>
        <input type="hidden" name="host_1" value="<?=urlencode($_REQUEST["server"])?>">
        <input type="hidden" name="hosts" value="1">
<?php	} ?>
        <input type="hidden" name="type" value="vrtsrv">

<?php	if(pql_get_define("PQL_CONF_BIND9_USE") and $_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
			// Only offer this if super admin - we might need write access to some other domain/branch (i.e. a domain/branch
			// where the specific DNS domain is located)
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Create DNS object')?></td>
          <td><input type="checkbox" name="dns">&nbsp;<?=$LANG->_('Yes')?></td>
        </tr>

<?php	}
		// }}}

	  } else {
		// {{{ Input for Web Server Device
		// Set a default port
		if(!$_REQUEST["serverport"])
		  $_REQUEST["serverport"] = 80;

		// Retreive the FQDN of physical host
		$server_reference = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('web server'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server FQDN')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?php if($_REQUEST["serverurl"]) { echo $_REQUEST["serverurl"]; } else { echo "$server_reference"; } ?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Listen port')?></td>
          <td><?php echo pql_format_error_span($error_text["serverport"]); ?><input type="text" name="serverport" size="40" value="<?=$_REQUEST["serverport"]?>"></td>
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
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="host"   value="<?=urlencode($_REQUEST["host"])?>">
    <input type="hidden" name="server" value="<?=urlencode($_REQUEST["server"])?>">
    <br>
    <input type="submit" value="Create">
  </form>
<?php
// }}}
} else {
  // {{{ No errors (i.e. no missing values). We're good to go!
  if($_REQUEST["type"] == "vrtsrv") {
	// {{{ Create a virtual host
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
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]	= $_REQUEST["serverurl"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]	= $_REQUEST["serveradmin"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]	= $_REQUEST["documentroot"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]	= $_REQUEST["serverip"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]	= $fqdn;
	  // }}}
	  
	  // {{{ Add the web server object
	  $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")."=".$fqdn.",".$hosts[$i];
	  if(pql_websrv_add_server($_pql->ldap_linkid, $dn, $entry, $_REQUEST["type"]))
		$msg = "Successfully added webserver configuration ".$_REQUEST["serverurl"];
	  else
		$msg = "Failed to add webserver configuration ".$_REQUEST["serverurl"];
	  // }}}

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
		$soadn = pql_get_dn($_pql->ldap_linkid, $rootdn, $filter);
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
		  else
			$serverip = $_REQUEST["serverip"];
		  
		  // Create a host entry
		  unset($entry);
		  $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($hostname);
		  $entry[pql_get_define("PQL_ATTR_ZONENAME")]			= $domainname;
		  $entry[pql_get_define("PQL_ATTR_DNSTTL")]				= 604800;
		  $entry[pql_get_define("PQL_ATTR_ARECORD")]			= $serverip;
		  
		  if(pql_bind9_add_host($_pql->ldap_linkid, $branch, $entry))
			$msg .= "<br>Successfully added host $hostname";
		  else
			$msg .= "<br>Failed to add host $hostname";
		} else
		  $msg .= "<br>Could not find branch";
	  }
// }}}
	}

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

	if(pql_websrv_add_server($_pql->ldap_linkid, $dn, $entry, $_REQUEST["type"]))
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

	if(pql_websrv_add_server($_pql->ldap_linkid, $dn, $entry, $_REQUEST["type"]))
	  $msg = "Successfully added the virtual host location ".$_REQUEST["mountpoint"];
	else
	  $msg = "Failed to add webserver configuration ".$_REQUEST["mountpoint"];
	// }}}

  } else
	$msg = "Wrong type.";

  // {{{ Redirect to host details page
  if(!ereg('%3D', $_REQUEST["host"]))
	$url = "host_detail.php?host=".urlencode($_REQUEST["host"]);
  else
	$url = "host_detail.php?host=".$_REQUEST["host"];

  if(!ereg('%3D', $_REQUEST["server"]))
	$url .= "&server=".urlencode($_REQUEST["server"]);
  else
	$url .= "&server=".$_REQUEST["server"];

  if($_REQUEST["virthost"]) $url .= "&virthost=".$_REQUEST["virthost"];
  if($_REQUEST["hostdir"])  $url .= "&hostdir=".$_REQUEST["hostdir"];
  $url .= "&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
  if(($_REQUEST["type"] == "websrv") or ($_REQUEST["type"] == "vrtsrv"))
	// Only reload left frame if adding a web server container or a virtual host
	$url .= "&rlnb=1";
  
  if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
	echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	die("<b>$url</b>");
  } else
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
