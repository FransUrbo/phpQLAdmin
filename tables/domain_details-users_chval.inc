  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE_TITLE?></th>
      <tr>
        <td class="title"><?=PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE?></td>
        <td class="title"><?=PQL_LANG_OPTIONS?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_ACCOUNTSTATUS_STATUS?></td>
        <td><a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=active"><?=PQL_LANG_ACCOUNTSTATUS_CHANGE_ACTIVE?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=nopop"><?=PQL_LANG_ACCOUNTSTATUS_CHANGE_NOPOP?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=disabled"><?=PQL_LANG_ACCOUNTSTATUS_CHANGE_DISABLE?></a>
        </td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_MAILQUOTA_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailquota&rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_MAILHOST_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailhost&rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=PQL_LANG_DELIVERYMODE_TITLE?></td>
        <td><a href="domain_edit_attributes.php?attrib=deliverymode&rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
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
