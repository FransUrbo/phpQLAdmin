<?php
// View basic details for a physical host
// $Id: host_details-physical.inc,v 1.1 2007-07-19 10:28:08 turbo Exp $

// Basically only show the RDN and the IP. That's the only things of interest
// in a physical host...

$_REQUEST["host"] = urldecode($_REQUEST["host"]);
$ip = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_IPHOSTNUMBER"));

$attrib  = pql_get_define("PQL_ATTR_IPHOSTNUMBER");
$alt     = pql_complete_constant($LANG->_('Modify attribute %attribute% for %server%'),
								 array('attribute' => $attrib, 'server' => $_REQUEST["host"]));
$ip_link = '<a href="host_edit_attribute.php?host='.urlencode($_REQUEST["host"]).'&action=modify'
. '&attrib='.$attrib.'&'.$attrib.'='.urlencode($ip).'&view='.$_REQUEST["view"].'"><img '
. 'src="images/edit.png" width="12" height="12" border="0" alt="'.$alt.'"></a>';
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
