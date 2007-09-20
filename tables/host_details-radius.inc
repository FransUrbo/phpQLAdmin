<?php
// View RADIUS details about/for a physical host
// $Id: host_details-radius.inc,v 1.5 2007-09-20 15:18:51 turbo Exp $

// {{{ Include RADIUS API etc
//require($_SESSION["path"]."/include/pql_radius.inc");

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);
}
// }}}

// {{{ Get all needed RADIUS values for this physical host
$server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

$attribs = array("radiusfilterid"	=> pql_get_define("PQL_ATTR_RADIUS_FILTER_ID"));
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
      <th colspan="3" align="left"><?=$LANG->_("RADIUS Controls")."\n"?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">RDN</td>
          <td><?=$_REQUEST["host"]?></td>
          <td></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_("Filter ID")?></td>
          <td><?php if(@$radiusfilterid) { echo "$radiusfilterid"; } else { echo "<i>".$LANG->_('Not set')."</i>"; } ?></td>
          <td align="right"><?=$radiusfilterid_link?></td>
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
