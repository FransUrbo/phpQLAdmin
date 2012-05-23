<?php
// Copy or move a virtual (webserver) host
// $Id: host_copy.php,v 1.1 2007-03-14 12:16:36 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_websrv.inc");
include($_SESSION["path"]."/header.html");
// }}}
echo "_REQUEST:"; printr($_REQUEST);

if(@$_REQUEST["save"] and $_REQUEST["server"] and $_REQUEST["virthost"]) {
  // {{{ Do the copy/move
  // {{{ Get the virtual host object
  $virthost = pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME").'='.$_REQUEST["virthost"].','.$_REQUEST["server"];
  $object = $_pql->search($virthost, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', 'BASE');
  unset($object["dn"]); // This is/will be in the way so remove it from the array
  if(is_array($object) and @$_SESSION["ACI_SUPPORT_ENABLED"]) {
    // Get the ACI's of the object
    $acis = $_pql->get_attribute($virthost, pql_get_define("PQL_ATTR_LDAPACI"), 1);
    if(is_array($acis))
      $object[pql_get_define("PQL_ATTR_LDAPACI")] = $acis;
  }
// }}}

  $msg = '';
  if($_REQUEST["action"] == 'clone') {
    // {{{ Copy a webserver to one or more webserver containers
    for($i=0; $i < $_REQUEST["containers"]; $i++) {
      $var_name = "container_$i";
      if($_REQUEST[$var_name]) {
	// A destination...
	$dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME").'='.$_REQUEST["virthost"].','.$_REQUEST[$var_name];
	if($_pql->add($dn, $object, 'websrv', 'host_copy.php:_add'))
	  $msg .= pql_complete_constant($LANG->_('Successfully cloned virtual host to %container%'),
					array('container' => $_REQUEST[$var_name]))."<br>";
	else
	  $msg .= pql_complete_constant($LANG->_('Failed to clone virtual host to %container%'),
					array('container' => $_REQUEST[$var_name]))."<br>";
      }
    }
// }}}
  } elseif($_REQUEST["action"] == 'move') {
    // {{{ Move object to another webserver container
    // First add the object to the new container
    $dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME").'='.$_REQUEST["virthost"].','.$_REQUEST["container"];
    if($_pql->add($dn, $object, 'websrv', 'host_copy.php:_add')) {
      $msg = $LANG->_('Successfully moved the virtual host');
      // Then delete the original object
      if(!$_pql->delete($virthost))
	$msg = $LANG->_('Failed to remove the original virtual host');
    } else
      $msg = $LANG->_('Failed to move the virtual host');
// }}}
  }

  $link  = 'host_detail.php?host='.urlencode($_REQUEST["host"]).'&server='.urlencode($_REQUEST["server"]);
  $link .= '&view=websrv&ref=virtual&msg='.urlencode($msg);
  pql_header($link);
// }}}
} else { 
  // {{{ Show a input form to figure out what to do
  // {{{ Retreive all physical hosts
  $hosts = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
			 '(&('.pql_get_define("PQL_ATTR_CN").'=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))',
			 'ONELEVEL');
// }}}
  
  // {{{ For each physical host, find web containers and their virtual hosts
  $DATA = array();
  foreach($hosts as $host_dn) {
    $tmp = pql_websrv_get_data($host_dn);
    if(is_array($tmp)) {
      foreach($tmp as $host => $data)
	$DATA[$host] = $data;
    }
  }
// }}}

  // {{{ Find all round-robin servers for this virtual host
  if($_REQUEST["replicas"]) {
    $filter  = '(&('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ApacheSectionObj)(';
    $filter .= pql_get_define("PQL_ATTR_OBJECTCLASS").'=ApacheVirtualHostObj)(';
    $filter .= pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["virthost"].'))';
    $roundrobin = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"], $filter);
  }
// }}}

  // {{{ Extract only the web containers
  // Go through each container, adding to an array
  foreach($DATA as $physical_host => $web_container_data) {
    foreach($web_container_data as $container => $virtual_hosts) {
      // Get the FQDN and PORT for this webserver container
      // container = cn=aurora.bayour.com:80,cn=aurora.bayour.com,ou=computers,c=se
      $tmp = split(',', $container);

      // tmp[0]    = cn=aurora.bayour.com:80
      $tmp = split('=', $tmp[0]);

      // tmp[1]    = aurora.bayour.com:80
      $CONTAINERS[$tmp[1]] = $container;
    }
  }

  // ... go through this array and remove all the ones this
  // vhost already exist in
  foreach($CONTAINERS as $container => $container_dn) {
    if(is_array($roundrobin)) {
      for($i=0; $roundrobin[$i]; $i++) {
	// Extract the container from the full DN to the virtual host
	$vhost_tmp = split(',', $roundrobin[$i]);
	$vhost_container = '';
	for($j=1; $vhost_tmp[$j]; $j++) {
	  $vhost_container .= $vhost_tmp[$j];
	  if($vhost_tmp[$j+1])
	    $vhost_container .= ',';
	}
	
	if(($container_dn == $vhost_container) or ($container == $_REQUEST["server"]))
	  unset($CONTAINERS[$container]);
      }
    } else {
      // No replicas. Filter out the server this virtual host was in..
      if($container_dn == $_REQUEST["server"])
	unset($CONTAINERS[$container]);
    }
  }
// }}}

  if(is_array($CONTAINERS)) {
    if($_REQUEST["action"] == 'clone') {
?>
    <span class="title1"><?php echo pql_complete_constant($LANG->_('Clone virtual host %vhost%'), array('vhost' => $_REQUEST["virthost"])); ?></span>
<?php } elseif($_REQUEST["action"] == 'move') { ?>
    <span class="title1"><?php echo pql_complete_constant($LANG->_('Move virtual host %vhost%'), array('vhost' => $_REQUEST["virthost"])); ?></span>
<?php } ?>

    <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="POST">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left">
          <tr class="<?php pql_format_table(); ?>">
            <td class="title">
<?php if($_REQUEST["action"] == 'clone') { ?>
              <?php echo $LANG->_('Clone virtual host to one or more\nof the following webserver containers')?>
<?php } elseif($_REQUEST["action"] == 'move') { ?>
              <?php echo $LANG->_('Move virtual host to one of the\nfollowing webserver containers')?>
<?php } ?>
            </td>

            <td>
<?php $i=0; foreach($CONTAINERS as $container => $container_dn) { ?>
<?php	if($_REQUEST["action"] == 'clone') { ?>
              <input type="checkbox" name="container_<?php echo $i?>" value="<?php echo $container_dn?>"><?php echo $container?><br>
<?php	} elseif($_REQUEST["action"] == 'move') { ?>
              <input type="radio" name="container" value="<?php echo $container_dn?>"><?php echo $container?><br>
<?php	} ?>
<?php $i++; } ?>
            </td>
          </tr>
        </th>
      </table>

<?php if($_REQUEST["action"] == 'clone') { ?>
      <input type="hidden" name="containers" value="<?php echo $i?>">
<?php } ?>
      <input type="hidden" name="host"       value="<?php echo $_REQUEST["host"]?>">
      <input type="hidden" name="server"     value="<?php echo $_REQUEST["server"]?>">
      <input type="hidden" name="virthost"   value="<?php echo $_REQUEST["virthost"]?>">
      <input type="hidden" name="action"     value="<?php echo $_REQUEST["action"]?>">
      <br>
      <input type="submit" name="save"       value="<?php echo $LANG->_('Yes')?>">
    </form>
<?php
  }
// }}}
}

/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
