  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Global configuration')?>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP host')?></td>
        <td class="<?=$class?>"><?=urldecode($_SESSION["USER_HOST"])?>&nbsp;</td>
        <td class="<?=$class?>"></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Language')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_LANG")?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_LANG")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_CONF_LANG")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Who are we')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_WHOAREWE")?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_WHOAREWE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_WHOAREWE")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Hostmaster')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_HOSTMASTER")?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_HOSTMASTER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_HOSTMASTER")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php if(pql_get_define("PQL_CONF_CONTROL_USE")) { ?>
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP control base dn')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_CONTROL_DN")?></td>
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_CONTROL_DN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set/Modify <?=pql_get_define("PQL_ATTR_CONTROL_DN")?>"></a></td>
<?php   } else { ?>
        <td class="<?=$class?>"></td>
<?php   } ?>
      </tr>
<?php } ?>

      <tr><td></td></tr>

<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('GUI Template')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_GUI_TEMPLATE")?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_GUI_TEMPLATE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_GUI_TEMPLATE")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Manage Access Control Information')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_ACI_USE")) {echo 'Yes';} else {echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_ACI_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_ACI_USE")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Manage Controls DB')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_USE")) {echo 'Yes';} else {echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_USE")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Manage EZMLM mailinglists')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_EZMLM_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_EZMLM_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_EZMLM_USE")?>"></a></td>
      </tr>
<?php } ?>
<?php if(pql_get_define("PQL_CONF_EZMLM_USE")) { $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Path to EZMLM commands')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_EZMLM_PATH"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_EZMLM_PATH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_EZMLM_PATH")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('EZMLM server is remote')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_EZMLM_REMOTE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_EZMLM_REMOTE")?>"></a></td>
      </tr>
<?php	if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) { $class=pql_format_table(0); ?>

      <tr>
        <td class="title"><?=$LANG->_('EZMLM remote host')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_EZMLM_REMOTE_HOST"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_EZMLM_REMOTE_HOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_EZMLM_REMOTE_HOST")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('EZMLM remote/rsh user')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_EZMLM_REMOTE_USER"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_EZMLM_REMOTE_USER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_EZMLM_REMOTE_USER")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Remote EZMLM mailinglist\ninformation retreival script')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_EZMLM_REMOTE_RETREIVE_SCRIPT"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_EZMLM_REMOTE_RETREIVE_SCRIPT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_EZMLM_REMOTE_RETREIVE_SCRIPT")?>"></a></td>
      </tr>
<?php	} ?>
<?php } ?>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Manage DNS domains')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_BIND9_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_BIND9_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_BIND9_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Manage webserver config')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_WEBSRV_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_WEBSRV_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Manage SimScan config')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_SIMSCAN_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_SIMSCAN_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_SIMSCAN_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Automatic reload of navigation bar')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_AUTO_RELOAD")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_AUTO_RELOAD")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_AUTO_RELOAD")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Allow change of LDAP server')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CHANGE_SERVER")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CHANGE_SERVER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_CONF_CHANGE_SERVER")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Use MBox format instead of Maildir for mail store')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CREATE_MBOX")) {echo 'Yes';} else {echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CREATE_MBOX")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CREATE_MBOX")?>"></a></td>
      </tr>
<?php if($show_kerberos_info) { ?>

      <tr><td></td></tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Kerberos realm')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_KRB5_REALM"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_REALM")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_REALM")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin server')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_SERVER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_SERVER")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin principal')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_PRINCIPAL")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_PRINCIPAL")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin keytab')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_KEYTAB")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_KEYTAB")?>"></a></td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Path to Kerberos \ukadmin\U command')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH"); ?>&nbsp;</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_COMMAND_PATH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_COMMAND_PATH")?>"></a></td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="config_detail.php?view=<?=$_GET["view"]?>&action=clear_session"><?=$LANG->_('Clear session array')?></a></td>
      </tr>

      <tr class="subtitle">
        <table>
          <tr>
            <td>
              <?=@pql_format_error_span($error["selected"])?>
              <img src="images/info.png" width="16" height="16" alt="" border="0">
            </td>
            <td>
              &nbsp;<?=$LANG->_('Reloading of the session array should/can be done after a schema change on the LDAP server have been done. This so that you don\'t have to logout and then login again for the update to be known to phpQLAdmin/the browser.')?>
            </td>
          </tr>
        </table>
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
