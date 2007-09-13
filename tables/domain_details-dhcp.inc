<?php
// View and manage DHCP server configuration accessible from this
// domain/branch.
// $Id: domain_details-dhcp.inc,v 1.1 2007-09-13 18:25:15 turbo Exp $

// {{{ Include the DHCP API and retreive DHCP configuration
require($_SESSION["path"]."/include/pql_dhcp3.inc");
$dhcp_config = pql_dhcp3_get_configs($_REQUEST["domain"]);
// }}}

if(is_array($dhcp_config)) {
  if($dhcp_config[0]) {
	// {{{ Get selected config, or if not supplied, use first as default
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php
	// Save all configurations, so we can put selected one back into the dhcp_config array
	$tmp = array();

	for($i=0; $dhcp_config[$i]; $i++) {
	  $dhcp_config_dn = $dhcp_config[$i]["dn"];
	  unset($dhcp_config[$i]["dn"]);

	  $tmp[$dhcp_config_dn] = $dhcp_config[$i];
	  
	  // Get the 'name' of the configuration
	  $confname = $_pql->get_attribute($dhcp_config_dn, pql_get_define("PQL_ATTR_CN"));

	  foreach($dhcp_config[$i] as $attrib => $data) {
		// {{{ Use the first configuration as default
		// ie, if we're not called with a specific configuration to show
		if(!@$_REQUEST["config"] and !@$config) {
		  $config = $dhcp_config_dn;
		  $_REQUEST["config"] = $config;
		} elseif(@$_REQUEST["config"]) {
		  $config = $_REQUEST["config"];
		}
		
		if($dhcp_config_dn == $_REQUEST["config"])
		  $mark_config = "&mark=yes";
		else
		  unset($mark_config);
// }}}
	  }
?>
    <a href="<?=$_SERVER["PHP_SELF"]?>?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>&config=<?=urlencode($dhcp_config_dn)?>"><img alt="/ <?=$confname?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($confname)?><?=$mark_config?>"></a>
<?php
	// "
	}
?>
    </th>
  </table>
<?php
	$dhcp_config = $tmp[$config];
// }}}

	$RDN  = $config;
  } else {
	$dhcp_config_dn = $dhcp_config["dn"];
	unset($dhcp_config["dn"]);

	if(!@$_REQUEST["config"]) {
	  $_REQUEST["config"] = $dhcp_config_dn;
	} else {
	  if($setup_dn == $_REQUEST["config"])
		$mark_config = "&mark=yes";
	  else
		unset($mark_config);
	}

	$RDN  = $dhcp_config_dn;
  }
  $DATA = $dhcp_config;

  // Get (possible) network/host config setup etc for this DHCP configuration
  $setup_values = pql_dhcp3_get_setup($_REQUEST["config"]);
  if(is_array($setup_values)) {
	if($setup_values[0]) {
	  // {{{ Get selected config, or if not supplied, use first as default
?>

  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php
	  // Save all configurations, so we can put selected one back into the dhcp_config array
	  $tmp = array();

	  for($i=0; $setup_values[$i]; $i++) {
		$setup_dn = $setup_values[$i]["dn"];
		unset($setup_values[$i]["dn"]);

		$tmp[$setup_dn] = $setup_values[$i];

		// Get the 'name' of the configuration
		$setupname = $_pql->get_attribute($setup_dn, pql_get_define("PQL_ATTR_CN"));

		if(@$_REQUEST["setup"]) {
		  if($setup_dn == $_REQUEST["setup"]) {
			$mark_setup = "&mark=yes";

			$DATA = $setup_values[$i];
			$RDN  = $setup_dn;
			$NAME = $setupname;
		  } else
			unset($mark_setup);
		} else
		  unset($mark_setup);
?>
      <a href="<?=$_SERVER["PHP_SELF"]?>?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>&config=<?=urlencode($_REQUEST["config"])?>&setup=<?=urlencode($setup_dn)?>"><img alt="/ <?=$setupname?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($setupname)?><?=$mark_setup?>"></a>
<?php
	// "
	}
?>
    </th>
  </table>
<?php
	$setup = $tmp[$setup];
// }}}
	} else {
	  $setup_dn = $setup_values["dn"];
	  unset($setup_values["dn"]);

	  if(@$_REQUEST["setup"]) {
		if($setup_dn == $_REQUEST["setup"]) {
		  $mark_setup = "&mark=yes";

		  $DATA = $setup_values;
		  $RDN  = $setup_dn;
		  $NAME = $setupname;
		} else
		  unset($mark_setup);
	  } else
		unset($mark_setup);

	  // Get the 'name' of the configuration
	  $setupname = $_pql->get_attribute($setup_dn, pql_get_define("PQL_ATTR_CN"));

	  // {{{ Setup the network/host config setup buttons
?>

  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
      <a href="<?=$_SERVER["PHP_SELF"]?>?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>&config=<?=urlencode($_REQUEST["config"])?>&setup=<?=urlencode($setup_dn)?>"><img alt="/ <?=$setupname?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($setupname)?><?=$mark_setup?>"></a>
    </th>
  </table>
<?php
	// "
// }}}
	}
  }
?>

  <br>

  <table cellspacing="0" border="0" width="70%" cellpadding="0">
    <th colspan="3" align="left"><?=$LANG->_('DHCP Configuration')?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">RDN</td>
        <td>&nbsp;<?=$RDN?></td>
        <td></td>
      </tr>
<?php
	foreach($DATA as $attrib => $data) {
	  if(is_array($data)) {
		for($i=0; $data[$i]; $i++) {
		  $info = split(' ', $data[$i]);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$info[0]?>&nbsp;&nbsp;</td>
        <td>&nbsp;<?php
		  for($j=1; $info[$j]; $j++) {
			$info[$j] = preg_replace('/"/', '', $info[$j]);
			echo $info[$j];
			if($info[$j+1]) {
			  echo " ";
			}
		  }?></td>
        <td valign="right">
<?php if($attrib != pql_get_define("PQL_ATTR_CN")) { ?>
          <a href="dhcp3_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($_REQUEST["config"])?>&view=<?=$_REQUEST["view"]?>&attrib=<?=$attrib?>&data=<?=urlencode($data[$i])?>&action=modify"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change DHCP3 data')?>" border="0"></a>&nbsp;&nbsp;<a href="dhcp3_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($_REQUEST["config"])?>&view=<?=$_REQUEST["view"]?>&attrib=<?=$attrib?>&data=<?=urlencode($data[$i])?>&action=delete"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete value')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>
<?php	}
	  } else {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$attrib?>&nbsp;&nbsp;</td>
        <td>&nbsp;<?=$data?></td>
        <td valign="right">
<?php if($attrib != pql_get_define("PQL_ATTR_CN")) { ?>
          <a href="dhcp3_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($_REQUEST["config"])?>&view=<?=$_REQUEST["view"]?>&attrib=<?=$attrib?>&data=<?=urlencode($data[$i])?>&action=modify"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change DHCP3 data')?>" border="0"></a>&nbsp;&nbsp;<a href="dhcp3_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($_REQUEST["config"])?>&view=<?=$_REQUEST["view"]?>&attrib=<?=$attrib?>&data=<?=urlencode($data[$i])?>&action=delete"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete value')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>
<?php }
	}

	if($NAME) {
	  if(pql_check_ipaddress($NAME))
		// IP address, hence a subnet.
		$action = 'subnet';
	  else
		// Not an IP address, hence a subhost.
		$action = 'subhost';
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td colspan="2">&nbsp;
          <a href="dhcp3_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($_REQUEST["config"])?>&setup=<?=urlencode($setup_dn)?>&action=<?=$action?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Add DHCP option')?>">
            <?=$LANG->_('Add DHCP option')?>
          </a>
        </td>
      </tr>
<?php } ?>

      <tr><th><td></td></th></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3">
          <a href="dhcp3_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($RDN)?>&action=subnet&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Add DHCP subnetwork')?>">
            <?=$LANG->_('Add DHCP subnetwork')?>
          </a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3">
          <a href="dhcp3_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&config=<?=urlencode($RDN)?>&action=host&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Add DHCP host statement')?>">
            <?=$LANG->_('Add DHCP host statement')?>
          </a>
        </td>
      </tr>
    </th>
  </table>
<?php
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
