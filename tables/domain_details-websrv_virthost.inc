    <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Webserver administration: \b%server%\B'), array('server' => $server_reference))?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Server name')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?></td>
        <td><a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?>"><?php echo $img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Server administrator')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?></td>
        <td><a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?>"><?php echo $img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Document root')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?></td>
        <td><a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?>"><?php echo $img["mod"]?></a></td>
      </tr>

      <?php echo pql_format_table_empty(2)?>

<?php if(@is_array($virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")])) {
        $new_tr = 1;

        foreach($virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Server alias')?></td>
<?php      } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?php echo $srv?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?php echo $srv?>"><?php echo $img["mod"]?></a>
<?php	   if($srv) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?php echo $srv?>"><?php echo $img["del"]?></a>
<?php	   } ?>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td><a href="<?php echo $LINK_URL?>&action=add&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?php echo  pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Server alias')?></td>
        <td><a href="<?php echo $LINK_URL?>&action=add&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?php echo  pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Script alias URL')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Script alias path')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Error logfile')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Transfer logfile')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

<?php if(@is_array($virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")])) {
        $new_tr = 1;

        foreach($virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Additional option')?></td>
<?php      } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?php echo $srv?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>&oldvalue=<?php echo $srv?>"><?php echo $img["mod"]?></a>
<?php	   if($srv) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>&oldvalue=<?php echo $srv?>"><?php echo $img["del"]?></a>
<?php 	   } ?>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Additional option')?></td>
        <td><a href="<?php echo $LINK_URL?>&action=add&type=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>"><?php echo  pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Additional option')?></td>
        <td><a href="<?php echo $LINK_URL?>&action=add&type=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>"><?php echo  pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

<?php if(0) { ?>
      <?php echo pql_format_table_empty(2)?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('LDAP Server')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('LDAP Login DN')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('LDAP Login Password')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")?>&oldvalue=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>
<?php } ?>

      <?php echo pql_format_table_empty(2)?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Rewrite engine')?></td>
        <td><?php echo @$virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")]?></td>
        <td><a href="<?php echo $LINK_URL?>&toggle=1&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>&<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>=<?php echo $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Toggle')?><?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>"></a>
</td>
      </tr>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE");

      if($virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE")]) {
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
      pql_format_table_single($txt, $link_url,
							  $virt_host_object[pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE")],
							  pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE"), $class);
?>

    </th>

<?php

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
