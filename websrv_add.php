<?php
// Add a webserver configuration to the LDAP db
// $Id: websrv_add.php,v 2.17.2.1 2006-03-14 14:46:30 turbo Exp $
//
// {{{ Setup session
require("./libs/pql_session.inc");
require($_SESSION["path"]."/libs/pql_config.inc");
require($_SESSION["path"]."/libs/pql_control.inc");
require($_SESSION["path"]."/libs/pql_websrv.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

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
	  if(!$_REQUEST["serverip"]) {
		$error = true;
		$error_text["serverip"] = $LANG->_('Missing');
	  }
	  
	  if(!$_REQUEST["serverurl"]) {
		$error = true;
		$error_text["serverurl"] = $LANG->_('Missing');
	  } elseif(!ereg('\/$', $_REQUEST["serverurl"]))
		$_REQUEST["serverurl"] .= '/';
	  
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
// }}}

if(($error == 'true') or !$_REQUEST["type"] or
   (($_REQUEST["type"] == "websrv")   and (!$_REQUEST["serverport"] or !$_REQUEST["serverip"]  or !$_REQUEST["serverurl"])) or
   (($_REQUEST["type"] == "vrtsrv")   and (!$_REQUEST["serverip"]   or !$_REQUEST["serverurl"] or !$_REQUEST["serveradmin"] or !$_REQUEST["documentroot"])) or
   (($_REQUEST["type"] == "location") and  !$_REQUEST["mountpoint"]))
{
  // Get servers
  $servers = pql_websrv_find_servers($_pql->ldap_linkid, $_REQUEST["domain"]);

  // {{{ Show the input form
  if($_REQUEST["virthost"]) {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a virtual host location for %virthost%'), array('virthost' => $_REQUEST["virthost"])); ?></span>
<?php
  } elseif($_REQUEST["server"]) {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a virtual host for %server%'), array('domain' => $_REQUEST["server"])); ?></span>
<?php } else { ?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a web server in branch %domain%'), array('domain' => $_REQUEST["domain"])); ?></span>
<?php } ?>

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
        <input type="hidden" name="virthost" value="<?=$_REQUEST["virthost"]?>">
<?php
		// }}}
	  } elseif($_REQUEST["server"]) {
		// {{{ Input for Virtual Host Object
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual host'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server FQDN')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?=$_REQUEST["serverurl"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server IP[:PORT]')?></td>
          <td><?php echo pql_format_error_span($error_text["serverip"]); ?><input type="text" name="serverip" size="40" value="<?=$_REQUEST["serverip"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('The IP:PORT input is only relevant if only one \bAdd to server\B option below is added!')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server Administrator')?></td>
          <td><?php echo pql_format_error_span($error_text["serveradmin"]); ?><input type="text" name="serveradmin" size="40" value="<?=$_REQUEST["serveradmin"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Document root')?></td>
          <td><?php echo pql_format_error_span($error_text["documentroot"]); ?><input type="text" name="documentroot" size="40" value="<?=$_REQUEST["documentroot"]?>"></td>
        </tr>
<?php 	if(count($servers) > 1) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Add to server')?></td>
          <td>
            <?php
			echo pql_format_error_span($error_text["documentroot"])."\n";
			$i = 1;
			foreach($servers as $server => $dn) {
?>
            <input type="checkbox" name="host_<?=$i?>" value="<?=urlencode($server)?>"<?php if($dn == $_REQUEST["server"]) {?> CHECKED<?php } ?>>
            <b><?=$server?></b><br>

<?php		$i++;
	 		}
?>
            <input type="hidden" name="hosts" value="<?=count($servers)?>">
          </td>
        </tr>
<?php	} else { ?>
        <input type="hidden" name="host_1" value="<?=urlencode($_REQUEST["server"])?>">
        <input type="hidden" name="hosts" value="1">
<?php	} ?>
        <input type="hidden" name="type" value="vrtsrv">

<?php	if(pql_get_define("PQL_CONF_BIND9_USE")) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Create DNS object')?></td>
          <td><input type="checkbox" name="dns"></td>
        </tr>

<?php	}
		// }}}
	  } else {
		// {{{ Input for Web Server Device
		// Set a default port
		if(!$_REQUEST["serverport"])
		  $_REQUEST["serverport"] = 80;
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('web server'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server FQDN')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?=$_REQUEST["serverurl"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('IP Address')?></td>
          <td><?php echo pql_format_error_span($error_text["serverip"]); ?><input type="text" name="serverip" size="40" value="<?=$_REQUEST["serverip"]?>"></td>
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
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">
    <br>
    <input type="submit" value="Create">
  </form>
<?php
// }}}
} else {
  // {{{ No errors (i.e. no missing values). We're good to go!
  if($_REQUEST["type"] == "vrtsrv") {
	// {{{ Create a virtual host
	// {{{ Extract all hosts we want to add the virtual server to
	$amount = $_REQUEST["hosts"];
	if($amount) {
	  if($_REQUEST["host_".$amount] != 'on') {
		for($i=0; $i <= $amount; $i++)
		  if($_REQUEST["host_".$i])
			$hosts[] = urldecode($_REQUEST["host_".$i]);
	  }
	}
	// }}}
	
	foreach($hosts as $host) {
	  // {{{ Setup the entry array
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]		= $_REQUEST["serverurl"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]	= $_REQUEST["serveradmin"];
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]		= $_REQUEST["documentroot"];

	  // {{{ Get the IP for this virtual host on this web server
	  if(count($hosts) > 1) {
		// As of phpQLAdmin > 20060304, a web server object have it's
		// IP address in the 'description' attribute ('device' object class).
		// Let's see if this is availible...
		$dn = pql_get_define("PQL_ATTR_CN")."=".$host.",ou=WEB,".$_REQUEST["domain"];
		$ip = pql_get_attribute($_pql->ldap_linkid, $dn, 'description');
		if(!$ip) {
		  // Try again. This time doing a DNS query
		  $tmp{'host'} = eregi_replace(':.*', '', $host);
		  $tmp{'port'} = eregi_replace('.*:', '', $host);
		  if(!$tmp{'port'})
			$tmp{'port'} = '80';
		  $ip = gethostbyname($tmp{'host'}).":".$tmp{'port'};
		  if(!$ip) {
			// BUMMER! Can't find an IP address for this host!
			// Fall back to user input. It's wrong, but what the hell!
			$ip = $_REQUEST["serverip"];
		  }
		}

		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]	= $ip;
	  } else
		$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]	= $_REQUEST["serverip"];
	  // }}}

	  // {{{ Extract the host FQDN from the URL. A little crude...
	  if(eregi('.*://', $_REQUEST["serverurl"]))
		$fqdn = eregi_replace('.*://', '', $_REQUEST["serverurl"]);
	  else
		$fqdn = $_REQUEST["serverurl"];
	  if(eregi(':', $fqdn)) { $fqdn = eregi_replace(':.*', '', $fqdn); }
	  if(eregi('/', $fqdn)) { $fqdn = eregi_replace('/.*', '', $fqdn); }
	  // }}}
	  
	  // {{{ Add the host FQDN to the entry array.
	  $entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]	= $fqdn;
	  $entry[pql_get_define("PQL_ATTR_CN")] = $fqdn;
	  // }}}
	  // }}}
	  
	  // {{{ Add the web server object
	  $dn = pql_get_define("PQL_ATTR_CN")."=".$host.",ou=WEB,".$_REQUEST["domain"];
	  if(pql_websrv_add_server($_pql->ldap_linkid, $_REQUEST["domain"], $dn, $entry, $_REQUEST["type"]))
		$msg = "Successfully added webserver configuration ".$_REQUEST["serverurl"];
	  else
		$msg = "Failed to add webserver configuration ".$_REQUEST["serverurl"];
	  // }}}
	}

	// {{{ Create the DNS object(s)
	echo "<br>----------------------------<br>";
	if($_REQUEST["dns"] and pql_get_define("PQL_CONF_BIND9_USE")) {
	  require($_SESSION["path"]."/libs/pql_bind9.inc");
	  
	  // Separate the domainname and hostname from the FQDN by removing the FIRST part of the FQDN.
	  $fqdn = ereg_replace(':.*', '', $_REQUEST["serverurl"]);
	  $tmp = split('\.', $fqdn);
	  $domainname = ''; $hostname = $tmp[0];

	  for($i=1; $i < count($tmp); $i++) {
		$domainname .= $tmp[$i];
		if($tmp[$i+1])
		  $domainname .= ".";
	  }
	  
	  // First make sure that the zone exists.
	  if(pql_bind9_add_zone($_pql->ldap_linkid, $_REQUEST["domain"], $domainname)) {
		$msg .= "<br>Successfully added domain $domainname";
		
		// Create a host entry
		unset($entry);
		$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($hostname);
		$entry[pql_get_define("PQL_ATTR_ZONENAME")]				= $domainname;
		$entry[pql_get_define("PQL_ATTR_DNSTTL")]				= 604800;
		$entry[pql_get_define("PQL_ATTR_ARECORD")]				= $_REQUEST["serverip"];
		
		if(pql_bind9_add_host($_pql->ldap_linkid, $_REQUEST["domain"], $entry))
		  $msg .= "<br>Successfully added host $hostname";
		else
		  $msg .= "<br>Failed to add host $hostname";
	  } else
		$msg .= "<br>Failed to add domain $domainname";
	}
	// }}}
	// }}}
  } elseif($_REQUEST["type"] == "websrv") {
	// {{{ Create a webserver device
	if($_REQUEST["serverport"])
	  $_REQUEST["serverurl"] .= ":".$_REQUEST["serverport"];

	// {{{ Setup the DN and the entry array
	$dn = pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["serverurl"].",ou=WEB,".$_REQUEST["domain"];
	$entry[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["serverurl"];
	$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = "device";
	$entry[pql_get_define("PQL_ATTR_DESCRIPTION")] = $_REQUEST["serverip"];
	// }}}

	if(pql_websrv_add_server($_pql->ldap_linkid, $_REQUEST["domain"], $dn, $entry, $_REQUEST["type"]))
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

	if(pql_websrv_add_server($_pql->ldap_linkid, $_REQUEST["domain"], $dn, $entry, $_REQUEST["type"]))
	  $msg = "Successfully added the virtual host location ".$_REQUEST["mountpoint"];
	else
	  $msg = "Failed to add webserver configuration ".$_REQUEST["mountpoint"];
	// }}}
  } else {
	$msg = "Wrong type.";
  }

  // {{{ Redirect to domain details page
  $url  = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
  $url .= "&server=".$_REQUEST["server"];
  if($_REQUEST["virthost"]) $url .= "&virthost=".$_REQUEST["virthost"];
  if($_REQUEST["hostdir"])  $url .= "&hostdir=".$_REQUEST["hostdir"];
  $url .= "&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
  
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
