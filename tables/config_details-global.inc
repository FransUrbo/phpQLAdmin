  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Global configuration')?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP host')?></td>
<?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><?=urldecode($_SESSION["USER_HOST"])?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Language')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_LANG")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_CONF_LANG")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_LANG")?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Who are we')?></td>
<?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_WHOAREWE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_WHOAREWE")?>"></a></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_WHOAREWE")?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Hostmaster')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_HOSTMASTER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_HOSTMASTER")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_HOSTMASTER")?>&nbsp;</td>
      </tr>

<?php if(pql_get_define("PQL_CONF_CONTROL_USE")) { ?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP control base dn')?></td>
<?php     $class=pql_format_table(0);
          if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_CONTROL_DN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set/Modify <?=pql_get_define("PQL_ATTR_CONTROL_DN")?>"></a></td>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php     } ?>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_CONTROL_DN")?></td>
      </tr>
<?php } ?>

      <tr></tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage Controls DB')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_USE")) {echo 'Yes';} else {echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage EZMLM mailinglists')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_EZMLM_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_EZMLM_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_EZMLM_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage DNS domains')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_BIND9_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_BIND9_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_BIND9_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage webserver config')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_USE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_WEBSRV_USE")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_WEBSRV_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Automatic reload of navigation bar')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_AUTO_RELOAD")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_AUTO_RELOAD")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_AUTO_RELOAD")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Allow change of LDAP server')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CHANGE_SERVER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_CONF_CHANGE_SERVER")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CHANGE_SERVER")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>
<?php if($show_kerberos_info) { ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('Kerberos realm')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_REALM")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_REALM")?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_KRB5_REALM"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin server')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_SERVER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_SERVER")?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin principal')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_PRINCIPAL")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_PRINCIPAL")?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin keytab')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_KEYTAB")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_KEYTAB")?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Path to Kerberos \ukadmin\U command')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=pql_get_define("PQL_ATTR_KRB5_ADMIN_COMMAND_PATH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_KRB5_ADMIN_COMMAND_PATH")?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH"); ?>&nbsp;</td>
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
              <?php echo pql_format_error_span($error["selected"]); ?>
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
