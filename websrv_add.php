<?php
// add a webserver configuration to the LDAP db
// $Id: websrv_add.php,v 2.2 2003-11-20 08:01:29 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_websrv.inc");

include("./header.html");

if($submit) {
	$error = false;
	$error_text = array();
	
	if(!$serverip) {
		$error = true;
		$error_text["serverip"] = $LANG->_('Missing');
	}
	
	if(!$serverurl) {
		$error = true;
		$error_text["serverurl"] = $LANG->_('Missing');
	} elseif(!ereg('\/$', $serverurl))
	  $serverurl .= '/';
	
	if(!$serveradmin) {
		$error = true;
		$error_text["serveradmin"] = $LANG->_('Missing');
	} elseif(!ereg('@', $serveradmin)) {
		$error = true;
		$error_text["serveradmin"] = $LANG->_('Invalid');
	}
	
	if(!$documentroot) {
		$error = true;
		$error_text["documentroot"] = $LANG->_('Missing');
	} elseif(!ereg('^\/', $documentroot)) {
		$error = true;
		$error_text["documentroot"] = $LANG->_('Invalid');
	} elseif(!ereg('\/$',  $documentroot))
	  $documentroot .= '/';
}

if(($error == 'true') or !$serverip or !serverurl or !$serveradmin or !$documentroot) {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a webserver configuration in branch %domain%'), array('domain' => $domain)); ?></span>

  <br><br>

  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('webserver configuration'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server IP[:PORT]')?></td>
          <td><?php echo pql_format_error_span($error_text["serverip"]); ?><input type="text" name="serverip" size="40" value="<?=$serverip?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server URL')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?=$serverurl?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server Administrator')?></td>
          <td><?php echo pql_format_error_span($error_text["serveradmin"]); ?><input type="text" name="serveradmin" size="40" value="<?=$serveradmin?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Document root')?></td>
          <td><?php echo pql_format_error_span($error_text["documentroot"]); ?><input type="text" name="documentroot" size="40" value="<?=$documentroot?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="submit">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="view"   value="<?=$view?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="submit" value="Create">
  </form>
<?php
} else {
	$entry["webserverip"]		= $serverip;
	$entry["webserverurl"]		= $serverurl;
	$entry["webserveradmin"]	= $serveradmin;
	$entry["webdocumentroot"]	= $documentroot;

	if(pql_websrv_add_server($_pql->ldap_linkid, $domain, $entry))
	  $msg = "Successfully added webserver configuration $serverurl";
	else
	  $msg = "Failed to add webserver configuration $serverurl";
	
	$url = "domain_detail.php?rootdn=$rootdn&domain=$domain&view=$view&msg=".urlencode($msg);
	header("Location: ".pql_get_define("PQL_GLOB_URI") . "$url");
}
?>
  </body>
</html>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
