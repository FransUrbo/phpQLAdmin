  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Change values of all users in this domain')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Attribute')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Status')?></td>
        <td><a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=active&view=<?=$view?>"><?=$LANG->_('Activate')?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=nopop&view=<?=$view?>"><?=$LANG->_('Lock POP account')?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$rootdn?>&domain=<?=$domain?>&set=disabled&view=<?=$view?>"><?=$LANG->_('Disable account')?></a>
        </td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Default quota')?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailquota&rootdn=<?=$rootdn?>&domain=<?=$domain?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Mailserver')?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailhost&rootdn=<?=$rootdn?>&domain=<?=$domain?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Deliverymode')?></td>
        <td><a href="domain_edit_attributes.php?attrib=deliverymode&rootdn=<?=$rootdn?>&domain=<?=$domain?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
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
