<?php
// View everything about web servers - virtual hosts and virtual host locations
// $Id: host_details-websrv.inc,v 1.9 2007-03-14 12:16:38 turbo Exp $

// {{{ Possibly show some debugging
if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "--- DEBUG: ---<br>";
  echo "_REQUEST:";
  printr($_REQUEST);
}
// }}}

// {{{ Figure out default web server container to show
$global = 0; // To avoid PHP warnings about undefined variables...
if(($_REQUEST["host"] == 'Global') or $_REQUEST["global"]) {
  // {{{ Originally called from the left frame - 'Global'
  // Figure out which server/container to show
  $KEEP["data"] = array();
  foreach($DATA as $tmp_physical_dn => $physical_data) {
	foreach($physical_data as $tmp_container_dn => $virt_hosts) {
	  if($_REQUEST["host"] == 'Global') {
		// Save the first values for later
		$server_name = $_pql->get_attribute($tmp_container_dn, pql_get_define("PQL_ATTR_CN"));

		if(!@$_REQUEST["global"]) {
		  // Overwrite the host/server values for code below.
		  $_REQUEST["host"]   = $tmp_physical_dn;
		  $_REQUEST["server"] = $tmp_container_dn;
		}
	  }

	  // Save this physical data for later
	  if(($tmp_physical_dn != 'Global') and is_array($physical_data))
		$KEEP["data"] = $KEEP["data"] + $physical_data;
	}
  }

  // For code further down
  $physical_dn  = $_REQUEST["host"];
  $container_dn = $_REQUEST["server"];

  // Since we're overwriting the $_REQUEST["host"], we need a way to remember
  // that we're 'Global'.
  $global             = 1;

  // Overwrite the physical data from one specific physical host with webserver
  // data from ALL the physical hosts - this for the 'host=Global' stuff to work.
  $physical_data      = $KEEP["data"];

  unset($KEEP); // We're SO done with this! Juck!
// }}}

} elseif($_REQUEST["ref"] == 'physical') {
  // {{{ Originally called from the left frame - '<physical host>'
  // NOTE: The physical host is called with 'ref=left', the web server container 'ref=left1'
  //       and the virtual host 'ref=left2' in the URL.
  // Take the first server as default one.
  $physical_dn = $_REQUEST["host"];
  $physical_data = $DATA[$physical_dn];

  if(!@$_REQUEST["server"]) {
	foreach($physical_data as $container_dn => $virt_hosts) {
	  $server_name = $_pql->get_attribute($container_dn, pql_get_define("PQL_ATTR_CN"));
	  break;
	}
	
	$_REQUEST["server"] = $container_dn;
  } else
	$container_dn = $_REQUEST["server"];
// }}}

} elseif($_REQUEST["server"]) {
  // {{{ Called with specific web server
  $physical_dn   = $_REQUEST["host"];
  $container_dn  = $_REQUEST["server"];

  $physical_data = $DATA[$physical_dn];
// }}}

} // else called with virtual host (nothing special needed)

$server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
// }}}

// {{{ Possibly show some debugging
if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "<b>Physical DN: '$physical_dn'</b><br>";
  echo "<b>Container DN: '$container_dn'</b><br>";
  echo "--- DEBUG ---<p>\n";
}
// }}}

if(is_array($physical_data) and count($physical_data)) {
  // {{{ Setup the base link url and images html
  // - Used in 'tables/host_details-websrv_virt{dir,host}.inc'
  $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
  $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';

  $LINK_URL    = "websrv_edit_attributes.php?host=".urlencode($_REQUEST["host"]);
  $LINK_URL   .= "&server=".urlencode($_REQUEST["server"])."&view=".$_REQUEST["view"];
// }}}

  // {{{ Figure out what virtual host to show
  $virt_hosts = $physical_data[$container_dn];
  if(!@$_REQUEST["virthost"]) {
	// We're not called with a specific virtual host to show
	foreach($virt_hosts as $virthost_dn => $virthost_name)
	  break;
	$_REQUEST["virthost"] = $virthost_name;
  }
  // else: Already specified
  //	   Still need to init virt_hosts array!
// }}}

  // {{{ Setup the server container buttons
  // If called from base Global->Webserver Administration:		ref='left'
  // If called from Webserver - Global:							ref='left1'
  // If called from Physical host->Webserver Administration:	ref='left'
  // If called from Physical host/Webserver						ref='left1'
  // If called from Physical host/Webserver/Virtual host:		ref='left2'
  if($global or ($_REQUEST["ref"] and ($_REQUEST["ref"] == 'physical') and (count($virt_hosts) > 1))) {
	// Do not show these buttons if
	//	1. Called from virtual host
	//	2. Called from container, with one or less vitual hosts
	//	3. Called from container, with multiple containers
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($physical_data as $tmp_container_dn => $tmp_virt_hosts) {
		$server_name = $_pql->get_attribute($tmp_container_dn, pql_get_define("PQL_ATTR_CN"));

		if($tmp_container_dn == $_REQUEST["server"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($tmp_container_dn)."&ref=".$_REQUEST["ref"]?><?php if($global) { echo "&global=1"; } ?>"><img alt="// <?php echo $server_name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $server_name?><?php echo $mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
		// Non polution PHP (tm)
		unset($tmp_container_dn); unset($tmp_virt_hosts); unset($mark);
  }
// }}}

  if(count($virt_hosts) > 0) {
	$got_virt_hosts = 1; // if we should give option to create virtual server directory or not
	$server_reference = $server_reference . " => " . $_REQUEST["virthost"];

	// {{{ Get virtual host directories
	$virt_host_dirs = pql_websrv_get_virtual_host_directories($container_dn, $_REQUEST["virthost"]);
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

	// {{{ Setup the virtual host buttons
	// If called from web server container:	virt_hosts     >= 1, ref=container
	// If called from virtual host:			virt_host_dirs == 1, ref=virtual
	if((count($virt_hosts) >= 1) and
	   (($_REQUEST["ref"] == 'container') or ($_REQUEST["ref"] == 'physical') or
		((count($virt_hosts) >= 1) and (count($virt_host_dirs) >= 1) and ($_REQUEST["ref"] == 'virtual')) or
		($_REQUEST["ref"] == 'global')) and
	   ($_REQUEST["ref"] != 'virtual'))
	{
	  // Show these buttons if:
	  //	1. Called from container with one or more virtual host(s)						TODO(?)
	  //	2. Called from container with one or more virtual host(s), where the default
	  //	   (or the one) virtual host have one or more virtual host location(s)
	  //	3. Called from virtual host, with one or more virtual host location(s)
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php if(pql_get_define("PQL_CONF_HOSTACL_USE") and $_SESSION["ALLOW_BRANCH_CREATE"] and ($_REQUEST["ref"] != 'virtual')) { ?>
    <a href="<?php echo $_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($container_dn)."&virthost=acl&ref=".$_REQUEST["ref"]?>"><img alt="// <?php echo $LANG->_('Access control')?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $LANG->_('Access control')?><?php if($_REQUEST["virthost"] == 'acl') { echo '&mark=yes'; } ?>"></a>
<?php }

	  foreach($virt_hosts as $virthost_dn => $virthost_name) {
		if($virthost_name == $_REQUEST["virthost"]) {
		  $VIRTHOST = $virthost_dn;

		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($container_dn)."&virthost=".urlencode($virthost_name)."&ref=".$_REQUEST["ref"]?><?php if($global) { echo "&global=1"; } ?>"><img alt="// <?php echo $virthost_name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $virthost_name?><?php echo $mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
	} else {
	  // Called with a specific virtual host (which don't have host dirs)
	  foreach($virt_hosts as $virthost_dn => $virthost_name) {
		if($virthost_name == $_REQUEST["virthost"]) {
		  $VIRTHOST = $virthost_dn;
		  break;
		}
	  }
	}

	$LINK_URL .= "&virthost=".urlencode($_REQUEST["virthost"]);
// }}}

	// {{{ Setup the virtual host directory buttons
	if(count($virt_host_dirs) >= 1) {
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($virt_host_dirs as $hostdir_name => $data) {
		if($hostdir_name == $_REQUEST["hostdir"]) {
		  $VIRTHOSTDIR = $hostdir_name;
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&server=".urlencode($container_dn)."&virthost=".urlencode($_REQUEST["virthost"])."&hostdir=$hostdir_name"."&ref=".$_REQUEST["ref"]?><?php if($global) { echo "&global=1"; } ?>"><img alt="// <?php echo $hostdir_name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $hostdir_name?><?php echo $mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
	  if($_REQUEST["hostdir"])
		$LINK_URL .= "&hostdir=".$_REQUEST["hostdir"];
	}
// }}}

	// {{{ Include tables/host_details-websrv_virt{dir,host}.inc
?>

  <table cellspacing="0" cellpadding="3" border="0">
<?php
if($_REQUEST["virthost"] == 'acl') {
  require($_SESSION["path"]."/tables/host_details-websrv_access.inc");
} elseif($_REQUEST["hostdir"]) {
  require($_SESSION["path"]."/tables/host_details-websrv_virtdir.inc");
} else {
  // Get the virtual host data
  $virthost = $_pql->search($VIRTHOST, 'objectClass=*', 'BASE');
  require($_SESSION["path"]."/tables/host_details-websrv_virthost.inc");
}
?>
  </table>

<?php
// }}}
  } else {
	// {{{ No virtual hosts defined
	$got_virt_hosts = 0;

	if(count($physical_data) == 1) {
	  // We got no virtual host and only ONE web server container.
	  // Output special message, so that isn't 'lost'.
	  echo pql_complete_constant($LANG->_('Got \ione\I web server (\b%container%\B), but no virtual hosts'), array('container' => $server_reference)).".<br>";
	} else
	  echo $LANG->_('No virtual hosts in this server').".<br>";
// }}}
  }
} else {
  // {{{ No web server objects
  echo $LANG->_('No web servers for this physical server defined.').".<br>";
// }}}
}

// {{{ Table footer (Inc. 'Add ... links')
if(($_REQUEST["virthost"] != 'acl') and
   ($_SESSION["ALLOW_BRANCH_CREATE"] or 
	pql_validate_administrator($_REQUEST["server"], pql_get_define("PQL_ATTR_UNIQUE_GROUP"), $_SESSION["USER_DN"])))
{
?>
  <p>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
<?php if($physical_data) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server'))); ?></a></td>
      </tr>
<?php	if($got_virt_hosts) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>&virthost=<?php echo urlencode($_REQUEST["virthost"])?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server directory'))); ?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_del.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>&virthost=<?php echo urlencode($_REQUEST["virthost"])?><?php echo $url_have_replicas?>"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => $LANG->_('this virtual server'))); ?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_copy.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>&virthost=<?php echo urlencode($_REQUEST["virthost"])?><?php echo $url_have_replicas?>&action=move"><?php echo pql_complete_constant($LANG->_('Move %what%'), array('what' => $LANG->_('this virtual server'))); ?></a></td>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_copy.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>&virthost=<?php echo urlencode($_REQUEST["virthost"])?><?php echo $url_have_replicas?>&action=clone"><?php echo pql_complete_constant($LANG->_('Clone %what%'), array('what' => $LANG->_('this virtual server'))); ?></a></td>
      </tr>

<?php		if($_REQUEST["hostdir"]) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_del.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>&virthost=<?php echo urlencode($_REQUEST["virthost"])?>&hostdir=<?php echo $_REQUEST["hostdir"]?>"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => $LANG->_('this virtual server directory'))); ?></a></td>
      </tr>
<?php		}
		}
?>
<?php	if(($_REQUEST["ref"] == 'physical') and $_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_del.php?view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST["host"])?>&server=<?php echo urlencode($container_dn)?>"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => $LANG->_('this web server'))); ?></a></td>
      </tr>
<?php	}
	  }
?>
    </th>
  </table>
<?php
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
