<?php
// View DHCP3 details for a physical host
// $Id: host_details-dhcp3.inc,v 1.2 2007-09-13 18:20:17 turbo Exp $

$attrib    = pql_get_define("PQL_ATTR_DHCP3_SERVICEDN");
$alt       = pql_complete_constant($LANG->_('Modify attribute %attribute% for %server%'),
								   array('attribute' => $attrib, 'server' => $_REQUEST["host"]));
$servicedn_link = '<a href="host_edit_attribute.php?host='.urlencode($_REQUEST["host"]).'&action=modify'
. '&attrib='.$attrib.'&'.$attrib.'='.urlencode($servicedn).'&view='.$_REQUEST["view"].'"><img '
. 'src="images/edit.png" width="12" height="12" border="0" alt="'.$alt.'"></a>';

// Retreive options and statements etc from the service object
$options = pql_dhcp3_get_service_options($servicedn);

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
      <a href="<?php echo $_SERVER["PHP_SELF"]."?view=".$_REQUEST["view"]."&host=".urlencode($_REQUEST["host"])."&$type=".urlencode($dn)?>"><img alt="// <?php echo $name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $name?><?php echo $mark?>"></a>
<?php } ?>
      </th>
    </table>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_("DHCP3 details")."\n"?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">RDN</td>
          <td><?php echo $_REQUEST["host"]?></td>
          <td></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Service DN')?></td>
          <td><?php echo $servicedn?></td>
          <td>
<?php if(!$servicedn) { ?>
            <?php echo $servicedn_link?>
<?php } ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Primary server')?></td>
          <td><?php echo $options["primarydn"]?></td>
          <td></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Secondary server')?></td>
          <td><?php echo $options["secondarydn"]?></td>
          <td></td>
        </tr>

        <tr><th><td></td></th></tr>

<?php
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
$tmp .= "&servicedn=$servicedn";
$txt  = array('tit' => $LANG->_('DHCP Options'),
			  'add' => $LANG->_('Add option'),
			  'del' => $LANG->_('Delete option'),
			  'mod' => $LANG->_('Modify option'));

$link = array('add' => $tmp."&type=add",
			  'del' => $tmp."&submit=1&type=del",
			  'mod' => $tmp); unset($tmp);

pql_format_table_single($txt, $link, $options["options"], pql_get_define("PQL_ATTR_DHCP3_OPTION"), $class);
// }}}
?>
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
