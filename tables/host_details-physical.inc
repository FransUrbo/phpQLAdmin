<?php
// View basic details for a physical host
// $Id: host_details-physical.inc,v 1.2 2007-09-13 18:23:07 turbo Exp $

// Basically only show the RDN and the IP. That's the only things of interest
// in a physical host...

// {{{ Get all available values for this physical host
$server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

$attribs = array("servicedn" => pql_get_define("PQL_ATTR_DHCP3_SERVICEDN"),
				 "ip"        => pql_get_define("PQL_ATTR_IPHOSTNUMBER"));
foreach($attribs as $key => $attrib) {
	$value = $_pql->get_attribute($_REQUEST["host"], $attrib);
	if(is_array($value))
	  $value = $value[0];
	$$key = $value;

	// Setup edit links. If it's a dcOrganizationNameForm attribute, then
	// we add a delete link as well.
	$link = $key . "_link";
	$alt1 = pql_complete_constant($LANG->_('Modify attribute %attribute% for %server%'),
								  array('attribute' => $attrib, 'server' => $server_reference));
	$alt2 = pql_complete_constant($LANG->_('Delete attribute %attribute% for %server%'),
								  array('attribute' => $attrib, 'server' => $server_reference));
	
	$$link = '<a href="host_edit_attribute.php?host='.urlencode($_REQUEST["host"]).'&action=modify'
	  . '&attrib='.$attrib.'&'.$attrib.'='.urlencode($value).'&view='.$_REQUEST["view"].'"><img '
	  . 'src="images/edit.png" width="12" height="12" border="0" alt="'.$alt1.'">'
	  . '</a>&nbsp;<a href="host_edit_attribute.php?host='.urlencode($_REQUEST["host"]).'action=delete'
	  . '&attrib='.$attrib.'&'.$attrib.'='.urlencode($value).'&view='.$_REQUEST["view"].'"><img '
	  . 'src="images/del.png" width="12" height="12" border="0" alt="'.$alt2.'"></a>';
}
// }}}
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Basic host details")."\n"?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">RDN</td>
          <td><?=$_REQUEST["host"]?></td>
          <td></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('IP Address')?></td>
          <td><?=$ip?></td>
          <td><?=$ip_link?></td>
        </tr>
<?php if(pql_get_define("PQL_CONF_DHCP3_USE")) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_("Service DN")?></td>
          <td><?php if(@$servicedn) { echo "$servicedn"; } else { echo "<i>".$LANG->_('Not set')."</i>"; } ?></td>
          <td align="right"><?=$servicedn_link?></td>
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
