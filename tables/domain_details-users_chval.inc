  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Change values of all users in this domain')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Attribute')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Status')?></td>
        <td><a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&set=active&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Activate')?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&set=nopop&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Lock POP account')?></a>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&set=disabled&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Disable account')?></a>
        </td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Default quota')?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailquota&rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Mailserver')?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailhost&rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Deliverymode')?></td>
        <td><a href="domain_edit_attributes.php?attrib=deliverymode&rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
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
