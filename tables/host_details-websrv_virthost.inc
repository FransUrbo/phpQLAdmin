    <th colspan="3" align="left"><?=pql_complete_constant($LANG->_('Webserver administration: \b%server%\B'), array('server' => $server_reference))?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server DN')?></td>
        <td><?=@$virt_hosts[$virt_host]["dn"]?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server IP:PORT')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server name')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server administrator')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Document root')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr><td></td></tr>

<?php if(@is_array($virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")])) {
        $new_tr = 1;

        foreach($virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")] as $srv) {
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
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
<?php	   if($srv) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
<?php	   } ?>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td><a href="<?=$LINK_URL?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
        <td><a href="<?=$LINK_URL?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Script alias URL')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Script alias path')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Error logfile')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Transfer logfile')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

<?php if(@is_array($virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")])) {
        $new_tr = 1;

        foreach($virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")] as $srv) {
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
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
<?php	   if($srv) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
<?php 	   } ?>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$LINK_URL?>&action=add&type=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$LINK_URL?>&action=add&type=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

<?php if(0) { ?>
      <tr><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('LDAP Server')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('LDAP Login DN')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('LDAP Login Password')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?>"><?=$img["mod"]?></a>
<?php if(@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")?>&oldvalue=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>
<?php } ?>

      <tr><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Rewrite engine')?></td>
        <td><?=@$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")]?></td>
        <td><a href="<?=$LINK_URL?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>&<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>=<?=$virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>"></a>
</td>
      </tr>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE");

      if($virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE")]) {
		$txt = array('tit' => $LANG->_('Rewrite rule'),
					 'add' => $LANG->_('Add rule'),
					 'mod' => $LANG->_('Modify rule'),
					 'del' => $LANG->_('Delete rule'));
		$link_url = array('add' => $url_tmp."&action=add",
						  'del' => $url_tmp."&action=del",
						  'mod' => $url_tmp."&action=mod");
      } else {
		$txt = array('tit' => $LANG->_('Rewrite rule'),
					 'add' => $LANG->_('Add rule'));
		$link_url = array('add' => $url_tmp."&action=add");
      }
      pql_format_table_single($_pql->ldap_linkid, $txt, $link_url,
							  $virt_hosts[$virt_host][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE")],
							  pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE"), $class);
?>

    </th>
