<?php require("./include/pql_websrv.inc");
      $servers = pql_websrv_get_servers($_pql->ldap_linkid, "ou=Web,".urldecode($domain));
      if(is_array($servers)) {
		  if(!$server) {
			  // Use the first server as default (ie, if we're not called with a specific server to show)
			  foreach($servers as $server => $data) break;
		  }
?>
  <table cellspacing="0" border="0" width="100%" cellpadding="0" size="100%">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><?php $i=1; foreach($servers as $srv => $data) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&view=$view&server=$srv&view=$view"?>"><img alt="/ <?=$srv?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$srv?>"></a><?php if(!($i%2)) { ?><br><?php }; $i++; } ?></td>
    </tr>
  </table>

  <br>
<?php $url  = "websrv_edit_attributes.php?rootdn=$rootdn&domain=$domain&server=".urlencode($servers[$server]["dn"]);
      $url .= "&view=$view";
      $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
      $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Webserver administration')?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Server IP')?></td>
        <td><?=$servers[$server]["webserverip"]?></td>
        <td><a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_IP"]);?>&oldvalue=<?=$servers[$server]["webserverip"]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Server URL')?></td>
        <td><?=$servers[$server]["webserverurl"]?></td>
        <td><a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_URL"]);?>&oldvalue=<?=$servers[$server]["webserverurl"]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Server administrator')?></td>
        <td><?=$servers[$server]["webserveradmin"]?></td>
        <td><a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ADMIN"]);?>&oldvalue=<?=$servers[$server]["webserveradmin"]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Document root')?></td>
        <td><?=$servers[$server]["webdocumentroot"]?></td>
        <td><a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_DOCROOT"]);?>&oldvalue=<?=$servers[$server]["webdocumentroot"]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr></tr>

<?php if(is_array($servers[$server]["webserveralias"])) {
        $new_tr = 1;

        foreach($servers[$server]["webserveralias"] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
<?php      } else { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?=$srv?></td>
        <td>
          <a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ALIAS"]);?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
          <a href="<?=$url?>&action=del&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ALIAS"]);?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td><a href="<?=$url?>&action=add&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ALIAS"]);?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
        <td><a href="<?=$url?>&action=add&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SRV_ALIAS"]);?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Script alias URL')?></td>
        <td><?=$servers[$server]["webscriptaliasurl"]?></td>
        <td>
          <a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SCRIPT_URL"]);?>&oldvalue=<?=$servers[$server]["webscriptaliasurl"]?>"><?=$img["mod"]?></a>
          <a href="<?=$url?>&action=del&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SCRIPT_URL"]);?>&oldvalue=<?=$servers[$server]["webscriptaliasurl"]?>"><?=$img["del"]?></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Script alias path')?></td>
        <td><?=$servers[$server]["webscriptaliaspath"]?></td>
        <td>
          <a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SCRIPT_PATH"]);?>&oldvalue=<?=$servers[$server]["webscriptaliaspath"]?>"><?=$img["mod"]?></a>
          <a href="<?=$url?>&action=del&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_SCRIPT_PATH"]);?>&oldvalue=<?=$servers[$server]["webscriptaliaspath"]?>"><?=$img["del"]?></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Error logfile')?></td>
        <td><?=$servers[$server]["weblogerror"]?></td>
        <td>
          <a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_LOG_ERROR"]);?>&oldvalue=<?=$servers[$server]["weblogerror"]?>"><?=$img["mod"]?></a>
          <a href="<?=$url?>&action=del&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_LOG_ERROR"]);?>&oldvalue=<?=$servers[$server]["weblogerror"]?>"><?=$img["del"]?></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Transfer logfile')?></td>
        <td><?=$servers[$server]["weblogtransfer"]?></td>
        <td>
          <a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_LOG_TRANSFER"]);?>&oldvalue=<?=$servers[$server]["weblogtransfer"]?>"><?=$img["mod"]?></a>
          <a href="<?=$url?>&action=del&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_LOG_TRANSFER"]);?>&oldvalue=<?=$servers[$server]["weblogtransfer"]?>"><?=$img["del"]?></a>
        </td>
      </tr>

<?php if(is_array($servers[$server]["weboptions"])) {
        $new_tr = 1;

        foreach($servers[$server]["weboptions"] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
<?php      } else { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?=$srv?></td>
        <td>
          <a href="<?=$url?>&action=mod&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_OPTIONS"]);?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
          <a href="<?=$url?>&action=del&attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_WEBSRV_OPTIONS"]);?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$url?>&action=add&type=weboptions"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$url?>&action=add&type=weboptions"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } ?>
    </th>
  </table>

<?php } else { ?>
  <br>

  <?=$LANG->_('No webserver configurations in this branch')?>.
<?php } ?>

  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="websrv_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('webserver configuration'))); ?></a></td>
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
