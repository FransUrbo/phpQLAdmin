<?php
// View everything about web servers - virtual hosts and virtual host locations
// $Id: host_details-websrv.inc,v 1.1.2.7 2006-11-17 14:02:29 turbo Exp $

// {{{ Include Webserver API etc
require($_SESSION["path"]."/include/pql_websrv.inc");

if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);
}
// }}}

// {{{ ---------------- GET WEB SERVER CONTAINERS
if($_REQUEST["host"] == 'Global') {
  // {{{ Called from the left frame, GLOBAL
  $servers = array();
  foreach($hosts as $host) {
	$tmp = pql_websrv_find_servers($_pql->ldap_linkid, $host);
	if(is_array($tmp))
	  $servers = $servers + $tmp;
  }
  
  if($_REQUEST["server"] and ($_REQUEST["server"] != 'Global'))
	$server_dn = $_REQUEST["server"];
  else
	// Take the first server as default one.
	foreach($servers as $server_name => $server_dn) break;
// }}}

} elseif($_REQUEST["ref"]) {
  // {{{ Originally called from the physical host link - Get all web servers below this physical host
  // NOTE: The physical host is called with 'ref=left', the web server container 'ref=left1'
  //       and the virtual host 'ref=left2' in the URL.
  if(!$_REQUEST["virthost"] or ($_REQUEST["ref"] == 'left')) {
	// Left host frame/[physical]->Webserver administration->Webserver
	// Left host frame/[physical]->Webserver administration->{Webserver->}virtual host
	$servers = pql_websrv_find_servers($_pql->ldap_linkid, $_REQUEST["host"]);

	if($_REQUEST["server"])
	  $server_dn = $_REQUEST["server"];
	else {
	  // Take the first server as default one.
	  foreach($servers as $server_name => $server_dn) break;
	  $_REQUEST["server"] = $server_dn; // Just in case it's used below (can't remember)
	}
  } else {
	// Left host frame/[physical]/Webserver/Virtual host
	$servers = array($_REQUEST["virthost"] => $_REQUEST["server"]);
	$server_dn = $_REQUEST["server"];
  }
// }}}

} elseif($_REQUEST["server"]) {
  // {{{ Called with specific web server
  // The variable '$host' here is wrong, I know (it's actually the physical FQDN),
  // but it doesn't matter - I'm only using the DN ($_REQUEST["server"]).
  // It's just so that the array is in the right format for setting up buttons and
  // retreiving virtual hosts further down....
  $servers = array($host => $_REQUEST["server"]);
  $server_dn = $_REQUEST["server"];
// }}}

}
$server_reference = pql_get_attribute($_pql->ldap_linkid, $server_dn, pql_get_define("PQL_ATTR_CN"));
// }}}

if(count($servers)) {
  // {{{ -------------- SETUP THE BASE LINK URL AND IMAGES HTML
  $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
  $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';

  $LINK_URL    = "websrv_edit_attributes.php?host=".urlencode($server_dn)."&view=".$_REQUEST["view"];
// }}}

  // {{{ -------------- GET VIRTUAL HOST OBJECTS
  // Get ALL virtual hosts for this server
  $virt_hosts = pql_websrv_get_virtual_host_objects($_pql->ldap_linkid, $server_dn);
  if(count($virt_hosts) >= 1) {
	if(!@$_REQUEST["virthost"]) {
	  // Use the first virtual host as default (ie, if we're not called with a specific virtual host to show)
	  foreach($virt_hosts as $_REQUEST["virthost"] => $data) break;
	} else {
	  // Extract the specific virtual host object
	  foreach($virt_hosts as $virt_host_name => $virt_host_data) {
		if($_REQUEST["ref"] == 'left2') {
		  // Left host frame/[physical]/Webserver/Virtual host
		  if($virt_host_name != $_REQUEST["virthost"])
			// This should not be shown, remove it from display.
			unset($virt_hosts[$virt_host_name]);
		} else {
		  // Called from host details
		  if($virt_host_name == $_REQUEST["virthost"]) {
			// Mark this as default
			break;
		  }
		}
	  }
	}
  }
// }}}

  // {{{ -------------- SETUP THE SERVER BUTTONS
  // If called from base Global->Webserver Administration:		ref='left'
  // If called from Webserver - Global:							ref='left1'
  // If called from Physical host->Webserver Administration:	ref='left'
  // If called from Physical host/Webserver						ref='left1'
  // If called from Physical host/Webserver/Virtual host:		ref='left2'
  if(($_REQUEST["host"] == 'Global') or ($_REQUEST["ref"] and
										 ($_REQUEST["ref"] == 'left') and 
										 (count($virt_hosts) > 1)))
  {
	// Do not show these buttons if
	//	1. Called from virtual host
	//	2. Called from container, with one or less vitual hosts
	//	3. Called from container, with multiple containers
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($servers as $srv => $srv_dn) {
		if($srv_dn == $server_dn) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?=$_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($srv_dn)."&ref=".$_REQUEST["ref"]?>"><img alt="// <?=$srv?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?><?=$mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
  }
// }}}

  if(count($virt_hosts) > 0) {
	$got_virt_hosts = 1; // if we should give option to create virtual server directory or not

	// {{{ Shortcut to simplify referencing the virtual host
	$virt_host = $_REQUEST["virthost"];
	$server_reference = $server_reference . " => " . $virt_host;
	
	if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
	  echo "DEBUG (virtual hosts): default=<b>$virt_host</b>";
	  printr($virt_hosts);
	}
// }}}

	// {{{ -------------- GET VIRTUAL HOST DIRECTORIES
	$virt_host_dirs = pql_websrv_get_virtual_host_directories($_pql->ldap_linkid, $server_dn, $virt_host);
	if(count($virt_host_dirs) >= 1) {
	  // NOTE: We should NOT have a default virtual host location!
	  //	   If we did, it wouldn't be possible to access the actual
	  //	   web server container and it's setup!
	  if(@$_REQUEST["hostdir"]) {
		$virt_host_dir = $_REQUEST["hostdir"];
		$server_reference = $server_reference . " => " . $virt_host_dir;
	  }
	
	  if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
		echo "DEBUG (virtual host directories): default=<b>$virt_host_dir</b>";
		printr($virt_host_dirs);
	  }
	}
// }}}

	// {{{ -------------- SETUP THE VIRTUAL HOST BUTTONS
	// If called from web server container:	virt_hosts     >= 1, ref=left1
	// If called from virtual host:			virt_host_dirs == 1, ref=left2
	if((count($virt_hosts) >= 1) and
	   ((($_REQUEST["ref"] == 'left1') or ($_REQUEST["ref"] == 'left'))
		or
		((count($virt_hosts) >= 1) and
		 (count($virt_host_dirs) >= 1) and
		 ($_REQUEST["ref"] == 'left2'))))
	{
	  // Show these buttons if:
	  //	1. Called from container with one or more virtual host(s)						TODO(?)
	  //	2. Called from container with one or more virtual host(s), where the default
	  //	   (or the one) virtual host have one or more virtual host location(s)
	  //	3. Called from virtual host, with one or more virtual host location(s)
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($virt_hosts as $srv => $data) {
		if($srv == $_REQUEST["virthost"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?=$_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($server_dn)."&virthost=".urlencode($srv)."&ref=".$_REQUEST["ref"]?>"><img alt="// <?=$srv?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?><?=$mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
	}
	$LINK_URL .= "&virthost=".urlencode($virt_host);
// }}}

	// {{{ -------------- SETUP THE VIRTUAL HOST DIRECTORY BUTTONS
	if(count($virt_host_dirs) >= 1) {
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($virt_host_dirs as $srv => $data) {
		if($srv == $_REQUEST["hostdir"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?=$_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($server_dn)."&virthost=".urlencode($virt_host)."&hostdir=$srv"."&ref=".$_REQUEST["ref"]?>"><img alt="// <?=$srv?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?><?=$mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
	  if($_REQUEST["hostdir"]) {
		$LINK_URL .= "&hostdir=$virt_host_dir";
	  }
	}
// }}}

	// {{{ -------------- Include tables/host_details-websrv_virt{dir,host}.inc
?>

  <table cellspacing="0" cellpadding="3" border="0">
<?php
if($_REQUEST["hostdir"]) {
  require($_SESSION["path"]."/tables/host_details-websrv_virtdir.inc");
} else {
  require($_SESSION["path"]."/tables/host_details-websrv_virthost.inc");
}
?>
  </table>

<?php
// }}}
  } else {
	// {{{ -------------- NO VIRTUAL HOSTS DEFINED
	$got_virt_hosts = 0;

	if(count($servers) == 1) {
	  // We got no virtual host and only ONE web server container.
	  // Output special message, so that isn't 'lost'.
	  echo pql_complete_constant($LANG->_('Got \ione\I web server (\b%container%\B), but no virtual hosts'), array('container' => $server_reference)).".<br>";
	} else
	  echo $LANG->_('No virtual hosts in this server').".<br>";
// }}}
  }
} else {
  // {{{ -------------- NO WEB SERVER OBJECTS
  echo $LANG->_('No web servers for this physical server defined.').".<br>";
// }}}
}

// {{{ -------------- TABLE FOOTER (Inc. 'Add ... links')
?>
  <p>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
<?php if($servers) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?view=<?=$_REQUEST["view"]?>&host=<?=urlencode($_REQUEST["host"])?>&server=<?=urlencode($server_dn)?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server'))); ?></a></td>
      </tr>
<?php	if($got_virt_hosts) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?view=<?=$_REQUEST["view"]?>&host=<?=urlencode($_REQUEST["host"])?>&server=<?=urlencode($server_dn)?>&virthost=<?=urlencode($virt_host)?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server directory'))); ?></a></td>
      </tr>
<?php	}
	  }
?>
    </th>
  </table>
<?php
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
