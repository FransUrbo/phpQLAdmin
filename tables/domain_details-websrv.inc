<?php require("./include/pql_websrv.inc");
      $servers = pql_websrv_get_servers($_pql->ldap_linkid, "ou=Web,".$_REQUEST["domain"]);
      if(is_array($servers)) {
		  if(!$server) {
			  // Use the first server as default (ie, if we're not called with a specific server to show)
			  foreach($servers as $server => $data) break;
		  } elseif(eregi('=', $server)) {
			  // Server is a RDN				   -> server   = 'cn=testing.somwehere.tld,ou=WEB,dc=somewhere,dc=tld'
			  // This will happen after changing a value with websrv_edit_attributes.php.
			  $server = split('=', $server);    // -> server[1]= 'testing.somwehere.tld,ou'
			  $server = split(',', $server[1]); // -> server[0]= 'testing.somwehere.tld'a
			  $server = $server[0];
		  }
?>
  <table cellspacing="0" border="0" width="100%" cellpadding="0" size="100%">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><?php $i=1; foreach($servers as $srv => $data) { ?><a href="<?=$_SERVER["PHP_SELF"]."?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"]."&server=$srv"?>"><img alt="/ <?=$srv?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$srv?>"></a><?php if(!($i%2)) { ?><br><?php }; $i++; } ?></td>
    </tr>
  </table>

  <br>
<?php $link  = "websrv_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&server=".urlencode($servers[$server]["dn"]);
      $link .= "&view=".$_REQUEST["view"];
      $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
      $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Webserver administration')?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server DN')?></td>
        <td><?=$servers[$server]["dn"]?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server IP')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]?></td>
        <td><a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server URL')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?></td>
        <td><a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server administrator')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?></td>
        <td><a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Document root')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?></td>
        <td><a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr></tr>

<?php if(is_array($servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")])) {
        $new_tr = 1;

        foreach($servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
<?php      } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?=$srv?></td>
        <td>
          <a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
          <a href="<?=$link?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td><a href="<?=$link?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
        <td><a href="<?=$link?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Script alias URL')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?></td>
        <td>
          <a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?=$img["mod"]?></a>
          <a href="<?=$link?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?=$img["del"]?></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Script alias path')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?></td>
        <td>
          <a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?=$img["mod"]?></a>
          <a href="<?=$link?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?=$img["del"]?></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Error logfile')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?></td>
        <td>
          <a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?=$img["mod"]?></a>
          <a href="<?=$link?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?=$img["del"]?></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Transfer logfile')?></td>
        <td><?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?></td>
        <td>
          <a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?=$img["mod"]?></a>
          <a href="<?=$link?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?=$servers[$server][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?=$img["del"]?></a>
        </td>
      </tr>

<?php if(is_array($servers[$server][pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")])) {
        $new_tr = 1;

        foreach($servers[$server][pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
<?php      } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?=$srv?></td>
        <td>
          <a href="<?=$link?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
          <a href="<?=$link?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$link?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$link?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_OPTIONS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
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
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('webserver configuration'))); ?></a></td>
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
