<?php
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
  if(!$_REQUEST["virthost"] or ($_REQUEST["ref"] == 'left')) {
	// Left host frame/[physical]->Webserver administration->Webserver
	// Left host frame/[physical]->Webserver administration->{Webserver->}virtual host
	$servers = pql_websrv_find_servers($_pql->ldap_linkid, $_REQUEST["host"]);

	if($_REQUEST["server"])
	  $server_dn = $_REQUEST["server"];
	else
	  // Take the first server as default one.
	  foreach($servers as $server_name => $server_dn) break;
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

if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
  echo "DEBUG (server containers): default=<b>$server_dn</b>";
  printr($servers);
}
// }}}

if(count($servers)) {
  // {{{ -------------- SETUP THE BASE LINK URL AND IMAGES HTML
  $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
  $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';

  $LINK_URL    = "websrv_edit_attributes.php?host=".urlencode($server_dn)."&view=".$_REQUEST["view"];
// }}}

  // {{{ -------------- SETUP THE SERVER BUTTONS
  if(($_REQUEST["host"] == 'Global') or $_REQUEST["ref"]) {
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

  // Shortcut to simplify referencing the virtual host
  $virt_host = $_REQUEST["virthost"];
  $server_reference = $server_reference . " => " . $virt_host;

  if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
   	echo "DEBUG (virtual hosts): default=<b>$virt_host</b>";
   	printr($virt_hosts);
  }
// }}}

  if(count($virt_hosts) > 0) {
	$got_virt_hosts = 1; // if we should give option to create virtual server directory or not

	// {{{ -------------- SETUP THE VIRTUAL HOST BUTTONS
	if(count($virt_hosts) > 1) {
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

	// {{{ -------------- GET VIRTUAL HOST DIRECTORIES
	$virt_host_dirs = pql_websrv_get_virtual_host_directories($_pql->ldap_linkid, $server_dn, $virt_host);
	if(count($virt_host_dirs) > 1) {
	  // Shortcut to simplify referencing the virtual host
	  if(empty($_REQUEST["hostdir"])) {
		// Use the first virtual host directory as default (ie, if we're not called with a specific virtual host to show)
		foreach($virt_host_dirs as $_REQUEST["hostdir"] => $data) break;
	  }

	  $virt_host_dir = $_REQUEST["hostdir"];
	  $server_reference = $server_reference . " => " . $virt_host_dir;
	
	  if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
		echo "DEBUG (virtual host directories): default=<b>$virt_host_dir</b>";
		printr($virt_host_dirs);
	  }
	}
// }}}

	// {{{ -------------- SETUP THE VIRTUAL HOST DIRECTORY BUTTONS
	if(count($virt_host_dirs) > 1) {
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
?>
  <?=$LANG->_('No virtual hosts in this server')?>.
<?php
// }}}
  }
} else {
  // {{{ -------------- NO WEB SERVER OBJECTS
?>
  <?=$LANG->_('No web servers for this physical server defined.')?>.
<?php
// }}}
}

// {{{ -------------- TABLE FOOTER (Inc. 'Add ... links')
?>
  <p>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
<?php if($servers) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?view=<?=$_REQUEST["view"]?>&host=<?=urlencode($server_dn)?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server'))); ?></a></td>
      </tr>
<?php	if($got_virt_hosts) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?view=<?=$_REQUEST["view"]?>&host=<?=urlencode($server)?>&virthost=<?=urlencode($virt_host)?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server directory'))); ?></a></td>
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
