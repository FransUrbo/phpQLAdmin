<?php
// Add DHCP3 subnetwork
// $Id: dhcp3_add.php,v 2.1 2007-07-19 10:27:57 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(isset($_REQUEST['action'])) {
  if($_REQUEST['action'] == 'subnet') {
    if($_REQUEST["Submit"]) {
      // {{{ Add the subnet
      $dn = pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["network"].','.$_REQUEST["servicedn"];

      $entry = array();
      $entry[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["network"];
      $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_get_define("PQL_ATTR_DHCP3_SUBNET");
      $entry[pql_get_define("PQL_ATTR_DHCP3_NETMASK")] = $_REQUEST["netmask"];
      $entry[pql_get_define("PQL_ATTR_DHCP3_RANGE")] = $_REQUEST["range"];

      if($_pql->add($dn, $entry, 'dhcp3', 'dhcp3_add.php/subnet'))
	$msg  = pql_complete_constant($LANG->_("DHCP subhost %subnet% added successfully."),
				      array('subnet' => $_REQUEST['subnet']));
      else
	$msg  = pql_complete_constant($LANG->_("Failed to add DHCP subhost %subnet%."),
				      array('subnet' => $_REQUEST['subnet']));

      $link  = "host_detail.php?host=".urlencode($_REQUEST["host"])."&view=".$_REQUEST["view"];
      $link .= "&subnet=".urlencode($dn);
      $link .= "&msg=".urlencode($msg);
      pql_header($link);
// }}}
    } else {
      // {{{ Output a form to add subnet
      require($_SESSION["path"]."/header.html");
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_("Add New Computer")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("Network")?></td>
            <td>
              <input type="text" name="network" size="15" value="<?=$_REQUEST['network']?>">
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("Network mask")?></td>
            <td>
              <input type="text" name="netmask" size="5" value="<?=$_REQUEST['netmask']?>" size="2">
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Network range')?></td>
            <td><input type="text" name="range" value="<?=$_REQUEST["range"]?>" size="31">
          </tr>
        </th>
      </table>

      <input type="hidden" name="view"      value="dhcp3">
      <input type="hidden" name="host"      value="<?=$_REQUEST["host"]?>">
      <input type="hidden" name="servicedn" value="<?=$_REQUEST["servicedn"]?>">
      <input type="hidden" name="action"    value="<?=$_REQUEST["action"]?>">
      <input type="submit" name="Submit"    value="<?=$LANG->_('Add New Host')?>">
    </form>
<?php
// }}}
    }
  } elseif($_REQUEST['action'] == 'host') {
    if($_REQUEST["Submit"]) {
      // {{{ Add the host
      $dn = pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["hostname"].','.$_REQUEST["servicedn"];

      $entry = array();
      $entry[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["hostname"];
      $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_get_define("PQL_ATTR_DHCP3_HOST");

      if($_pql->add($dn, $entry, 'dhcp3', 'dhcp3_add.php/host'))
	$msg  = pql_complete_constant($LANG->_("DHCP subhost %subnet% added successfully."),
				      array('subnet' => $_REQUEST['subnet']));
      else
	$msg  = pql_complete_constant($LANG->_("Failed to add DHCP subhost %subnet%."),
				      array('subnet' => $_REQUEST['subnet']));

      $link  = "host_detail.php?host=".urlencode($_REQUEST["host"])."&view=".$_REQUEST["view"];
      $link .= "&subhost=".urlencode($dn);
      $link .= "&msg=".urlencode($msg);
      pql_header($link);
// }}}
    } else {
      // {{{ Output a form to add subnet
      require($_SESSION["path"]."/header.html");
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_("Add New DHCP host")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("Hostname")?></td>
            <td>
              <input type="text" name="hostname" size="15" value="<?=$_REQUEST['hostname']?>">
            </td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="view"      value="dhcp3">
      <input type="hidden" name="host"      value="<?=$_REQUEST["host"]?>">
      <input type="hidden" name="servicedn" value="<?=$_REQUEST["servicedn"]?>">
      <input type="hidden" name="action"    value="<?=$_REQUEST["action"]?>">
      <input type="submit" name="Submit"    value="<?=$LANG->_('Add New Host')?>">
    </form>
<?php
// }}}
    }
  }
}

/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
