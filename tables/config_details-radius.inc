<?php
// Plugin for RADIUS Policies
// $Id: config_details-radius.inc,v 1.1 2007-09-13 18:25:14 turbo Exp $
require($_SESSION["path"]."/include/pql_radius.inc");
$radprofiles = pql_get_radprofiles();

// {{{ Simplicity - with this we can have ONE single foreach() loop
// to output the whole view!
// Idea taken from tables/config_details-ppolicy.inc.
$table_columns = array(pql_get_define("PQL_ATTR_RADIUS_ARAP_FEATURES")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_ARAP_SECURITY")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_ARAP_ZONE_ACCESS")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_AUTH_TYPE")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CALLBACK_ID")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CALLBACK_NR")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CALLED_STATION_ID")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CALLING_STATION_ID")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CLASS")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CLIENT_IP")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FILTER_ID")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_LINK")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_NETW")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_APPLETALK_ZONE")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_COMPRESSION")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IP_ADDR")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IP_NET")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_IPX_NET")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_MTU")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_PROTO")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_ROUTE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_FRAMED_ROUTING")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_GROUP_NAME")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_HINT")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_HUNTGROUP_NAME")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_IDLE_TIMEOUT")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_IP_HOST")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_GROUP")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_NODE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_PORT")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_LAT_SERVICE")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_SERVICE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_TCP_PORT")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_LOGIN_TIME")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PASSWORD_RETRY")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PORT_LIMIT")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PROFILE_DN")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PROMPT")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_PROXY2REALM")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_REPLICATE2REALM")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_REALM")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_SERVICE_TYPE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_SESSION_TIMEOUT")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TERMINATE_ACTION")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_ASSIGN_ID")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_MEDIUM_TYPE")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PASSWORD")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PREFS")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_PRIVATE_GROUP_ID")	=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_SERVER_ENDPOINT")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_CLIENT_ENDPOINT")		=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_TUNNEL_TYPE")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_VSA")						=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_SIMULT_USE")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_USER_CATEGORY")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_STRIP_USER_NAME")			=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_DIALUP_ACCESS")				=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_EXPIRATION")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_CHECK_ITEM")					=> $LANG->_(''),
					   pql_get_define("PQL_ATTR_RADIUS_REPLY_ITEM")					=> $LANG->_(''));
// }}}
?>
  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Global RADIUS Policies')?>
<?php for($i=0; $radprofiles[$i]; $i++) { ?>
<?php	foreach($table_columns as $attrib => $text) {
	  $link = 'config_edit_attribute.php?attrib='.$attrib;
	  $class=pql_format_table(0);
?>
      <tr>
        <td class="title"><?=$text?></td>
<?php if($attrib == pql_get_define("PQL_ATTR_CN")) { ?>
        <td class="title">
<?php } else { ?>
        <td class="<?=$class?>">
<?php } ?>
          <?=$radprofiles[$i][$attrib]?>
        </td>

        <td class="<?=$class?>" align="right">
          <a href="<?=$link?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change profile value')?>"></a>
        </td>
      </tr>
<?php	} ?>

      <tr><td colspan="3"></td></tr>

<?php } ?>
      <tr>
<?php if(!$radprofiles) { ?>
        <td class="title"><?=$LANG->_('No profiles defined')?></td>
<?php } else { ?>
        <td class="title"></td>
<?php }

      $class=pql_format_table(0);
?>
        <td class="<?=$class?>" colspan="2">
          <a href="config_radius_add.php?rootdn=<?=urlencode($_SESSION["BASE_DN"][0])?>"><?=$LANG->_('Add profile')?></a>
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
