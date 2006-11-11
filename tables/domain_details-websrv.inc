<?php
require($_SESSION["path"]."/include/pql_websrv.inc");

// {{{ ---------------- GET WEB SERVER OBJECTS
$servers = pql_websrv_find_servers($_pql->ldap_linkid, $_REQUEST["domain"]);
if(!$_REQUEST["server"] and count($servers)) {
  // Take the first server as default one.
  foreach($servers as $server_name => $_REQUEST["server"]) break;
  $server = $_REQUEST["server"];
} else {
  // We're called with a webserver. Make sure we use this in all following queries.
  if(eregi('^'.pql_get_define("PQL_ATTR_CN"), $_REQUEST["server"])) {
	// The server is a DN
	$server = $_REQUEST["server"];
	
    // Extract the name from the DN
	$tmp = split(',', $server);
	$tmp = split('=', $tmp[0]);
	$server_name = $tmp[1];
  } else {
	// The server isn't a DN
	$server_name = $_REQUEST["server"];

	// Generate a server DN
	$server = pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["server"].",".pql_get_define("PQL_CONF_SUBTREE_APACHE").",".$_REQUEST["domain"];
  }
}
$server_reference = $server_name;
// }}}

if(count($servers)) {
  // {{{ -------------- SETUP THE BASE LINK URL AND IMAGES HTML
  $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
  $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';

  $LINK_URL    = "websrv_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
  $LINK_URL   .= "&server=".urlencode($server)."&view=".$_REQUEST["view"];
// }}}

  // {{{ -------------- SETUP THE SERVER BUTTONS
  if(count($servers)) {
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($servers as $srv => $dn) {
		if($dn == $_REQUEST["server"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?=$_SERVER["PHP_SELF"]."?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"]."&server=".urlencode($dn)?>"><img alt="/ <?=$srv?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?><?=$mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
  }
// }}}


  // {{{ -------------- GET VIRTUAL HOST OBJECTS
  $virt_hosts = pql_websrv_get_virtual_host_objects($_pql->ldap_linkid, $server);
  if(count($virt_hosts) > 1) {
    if(empty($_REQUEST["virthost"])) {
	  // Use the first virtual host as default (ie, if we're not called with a specific virtual host to show)
	  foreach($virt_hosts as $_REQUEST["virthost"] => $data) break;
    }

    // Shortcut to simplify referencing the virtual host
    $virt_host = $_REQUEST["virthost"];
	$server_reference = $server_name." => ".$virt_host;

    if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
      echo "DEBUG (virtual host): $virt_host";
      printr($virt_hosts[$virt_host]);
    }
// }}}

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
    <a href="<?=$_SERVER["PHP_SELF"]."?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"]."&server=".urlencode($server)."&virthost=$srv"?>"><img alt="/ <?=$srv?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?><?=$mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
		$LINK_URL .= "&virthost=".$virt_host;
	}
// }}}


	// {{{ -------------- GET VIRTUAL HOST DIRECTORIES
	$virt_host_dirs = pql_websrv_get_virtual_host_directories($_pql->ldap_linkid, $server, $virt_host);
	if(count($virt_host_dirs) > 1) {
	  // Shortcut to simplify referencing the virtual host
	  if(empty($_REQUEST["hostdir"])) {
		// Use the first virtual host directory as default (ie, if we're not called with a specific virtual host to show)
		foreach($virt_host_dirs as $_REQUEST["hostdir"] => $data) break;
	  } else {
		$virt_host_dir = $_REQUEST["hostdir"];
		$server_reference = $server_name." => ".$virt_host." => ".$virt_host_dir;
	  }
	  
	  if(file_exists($_SESSION["path"]."/.DEBUG_WEBSRV")) {
		echo "DEBUG (virtual host directory/-ies): $virt_host_dirs";
		printr($virt_host_dirs[$virt_host_dir]);
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
    <a href="<?=$_SERVER["PHP_SELF"]."?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"]."&server=".urlencode($server)."&virthost=$virt_host&hostdir=$srv"?>"><img alt="/ <?=$srv?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?><?=$mark?>"></a>
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
?>

  <table cellspacing="0" cellpadding="3" border="0">
<?php
if($_REQUEST["hostdir"]) {
  require($_SESSION["path"]."/tables/domain_details-websrv_virtdir.inc");
} else {
  require($_SESSION["path"]."/tables/domain_details-websrv_virthost.inc");
}
?>
  </table>

<?php	} else { ?>
  <?=$LANG->_('No virtual hosts in this server')?>.
<?php	} ?>
<?php } else { ?>
  <br>

  <?=$LANG->_('No web servers in this branch')?>.
<?php } ?>

  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('webserver'))); ?></a></td>
      </tr>
<?php if($servers) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>&server=<?=urlencode($server)?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server'))); ?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>&server=<?=urlencode($server)?>&virthost=<?=$virt_host?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual server directory'))); ?></a></td>
      </tr>
<?php } ?>
    </th>
  </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
