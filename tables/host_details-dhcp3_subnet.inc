<?php
// View DHCP3 subnet details
// $Id: host_details-dhcp3_subnet.inc,v 1.1 2007-07-19 10:28:08 turbo Exp $

// Retreive options and statements etc from the subnet object
$options = pql_dhcp3_get_subnet_options($_REQUEST["subnet"]);

// Setup subnetwork buttons etc
?>
    <table cellspacing="0" border="0" width="50%" cellpadding="0">
      <th colspan="3" align="left">
<?php for($i=0; $subentries[$i]; $i++) {
		$dn   = $subentries[$i]["dn"];
		$mask = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_DHCP3_NETMASK"));
		if(!$mask) {
		  // No dhcpNetMask attribute (required by the dhcpSubnet object class) - this is a host!
		  $mask = '';
		  $type = 'subhost';
		} else {
		  $mask = "/$mask";
		  $type = 'subnet';
		}
		$name = $subentries[$i]["cn"]."$mask";
		
		if($dn == $_REQUEST["subnet"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
      <a href="<?php echo $_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&$type=".urlencode($dn)?>"><img alt="// <?=$name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$name?><?=$mark?>"></a>
<?php } ?>
      </th>
    </table>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_("DHCP3 subnet")."\n"?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">RDN</td>
          <td><?php echo $_REQUEST["subnet"]?></td>
          <td></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Netmask')?></td>
          <td><?php echo $options["netmask"]?></td>
          <td></td>
        </tr>

<?php
// {{{ Network range
$class=pql_format_table(0);

$tmp  = "host_edit_attribute.php?host=".urlencode($_REQUEST["host"])."&action=modify";
$tmp .= "&attrib=".pql_get_define("PQL_ATTR_DHCP3_RANGE")."&view=".$_REQUEST["view"];
$tmp .= "&subnet=".$_REQUEST["subnet"];
$txt  = array('tit' => $LANG->_('DHCP subnet range'),
			  'add' => $LANG->_('Add range'),
			  'del' => $LANG->_('Delete range'),
			  'mod' => $LANG->_('Modify range'));

$link = array('add' => $tmp."&type=add",
			  'del' => $tmp."&submit=1&type=del",
			  'mod' => $tmp); unset($tmp);

pql_format_table_single($txt, $link, $options["range"], pql_get_define("PQL_ATTR_DHCP3_RANGE"), $class);
// }}}

// {{{ Statemens
$class=pql_format_table(0);

$tmp  = "host_edit_attribute.php?host=".urlencode($_REQUEST["host"])."&action=modify";
$tmp .= "&attrib=".pql_get_define("PQL_ATTR_DHCP3_STATEMENTS")."&view=".$_REQUEST["view"];
$tmp .= "&servicedn=$servicedn";
$txt  = array('tit' => $LANG->_('DHCP Statement'),
			  'add' => $LANG->_('Add statement'),
			  'del' => $LANG->_('Delete statement'),
			  'mod' => $LANG->_('Modify statement'));

$link = array('add' => $tmp."&type=add",
			  'del' => $tmp."&submit=1&type=del",
			  'mod' => $tmp); unset($tmp);

pql_format_table_single($txt, $link, $options["statements"], pql_get_define("PQL_ATTR_DHCP3_STATEMENTS"), $class);
// }}}

// {{{ Options
$class=pql_format_table(0);

$tmp  = "host_edit_attribute.php?host=".urlencode($_REQUEST["host"])."&action=modify";
$tmp .= "&attrib=".pql_get_define("PQL_ATTR_DHCP3_OPTION")."&view=".$_REQUEST["view"];
$tmp .= "&subnet=".$_REQUEST["subnet"];
$txt  = array('tit' => $LANG->_('DHCP subnet options'),
			  'add' => $LANG->_('Add option'),
			  'del' => $LANG->_('Delete option'),
			  'mod' => $LANG->_('Modify option'));

$link = array('add' => $tmp."&type=add",
			  'del' => $tmp."&submit=1&type=del",
			  'mod' => $tmp); unset($tmp);

pql_format_table_single($txt, $link, $options["options"], pql_get_define("PQL_ATTR_DHCP3_OPTION"), $class);
// }}}
?>

        <tr><th><td></td></th></tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="3">
            <a href="dhcp3_add.php?host=<?php echo urlencode($_REQUEST["host"])?>&servicedn=<?=urlencode($servicedn)?>&action=subnet&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Add DHCP subnetwork')?>">
              <?php echo $LANG->_('Add DHCP subnetwork')?>
            </a>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="3">
            <a href="dhcp3_add.php?host=<?php echo urlencode($_REQUEST["host"])?>&servicedn=<?=urlencode($servicedn)?>&action=host&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Add DHCP host statement')?>">
              <?php echo $LANG->_('Add DHCP host statement')?>
            </a>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="3">
            <a href="dhcp3_del.php?host=<?php echo urlencode($_REQUEST["host"])?>&servicedn=<?=urlencode($servicedn)?>&subnet=<?=urlencode($_REQUEST["subnet"])?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete DHCP subnetwork')?>">
              <?php echo $LANG->_('Delete DHCP subnetwork')?>
            </a>
          </td>
        </tr>
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
