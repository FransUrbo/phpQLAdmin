  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Global configuration')?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP host')?></td>
<?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><?=$USER_HOST?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Language')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_LANG"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_LANG"]?>">
          </a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_LANG")?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Who are we')?></td>
<?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_WHOAREWE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_WHOAREWE"]?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_WHOAREWE"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Hostmaster')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_HOSTMASTER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_HOSTMASTER"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_HOSTMASTER"); ?>&nbsp;</td>
      </tr>

<?php if(pql_get_define("PQL_GLOB_CONTROL_USE")) { ?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP control base dn')?></td>
<?php     $class=table_bgcolor(0);
          if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_DN"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set/Modify <?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_DN"]?>"></a></td>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php     } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_CONTROL_DN"); ?></td>
      </tr>
<?php } ?>

      <tr></tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage Controls DB')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_USE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_CONTROL_USE")) {echo 'Yes';} else {echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage EZMLM mailinglists')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_EZMLM_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_EZMLM_USE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_EZMLM_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage DNS domains')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_BIND9_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_BIND9_USE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_BIND9_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Manage webserver config')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_WEBSRV_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_WEBSRV_USE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_WEBSRV_USE")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Automatic reload of navigation bar')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_AUTO_RELOAD"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_AUTO_RELOAD"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_AUTO_RELOAD")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically replicate domains to %what%'), array('what' => $LANG->_('locals'))); ?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDLOCALS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_AUTOADDLOCALS"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_CONTROL_AUTOADDLOCALS")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically replicate domains to %what%'), array('what' => $LANG->_('rcpthosts'))); ?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_AUTOADDRCPTHOSTS"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Automatically add hostname to domain for replication')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDHOSTNAME"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_AUTOADDHOSTNAME"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_CONTROL_AUTOADDHOSTNAME")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Allow change of LDAP server')?></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CHANGE_SERVER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CHANGE_SERVER"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_GLOB_CHANGE_SERVER")) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>
<?php if($show_kerberos_info) { ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('Kerberos realm')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_REALM"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_REALM"]?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_KRB5_REALM"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin server')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_SERVER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_SERVER"]?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_KRB5_ADMIN_SERVER"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin principal')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_PRINCIPAL"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_PRINCIPAL"]?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_KRB5_ADMIN_PRINCIPAL"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Kerberos admin keytab')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_KEYTAB"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_KEYTAB"]?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_KRB5_ADMIN_KEYTAB"); ?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?=$LANG->_('Path to Kerberos \ukadmin\U command')?></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"]?>"></a></td>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"); ?>&nbsp;</td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
