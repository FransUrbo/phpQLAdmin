  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo $LANG->_('Change values of all users in this domain')?></th>
      <tr>
        <td class="title"><?php echo $LANG->_('Attribute')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $LANG->_('Status')?></td>
        <td><a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&set=active&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('Activate accounts')?></a>
<?php if(!$_SESSION["NEW_STYLE_QUOTA"])  { ?>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&set=nopop&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('Lock POP accounts')?></a>
<?php } else { ?>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&set=noaccess&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('Lock accounts')?></a>
<?php } ?>
  	| <a href="domain_edit_attributes.php?attrib=accountstatus&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&set=disabled&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('Disable accounts')?></a>
        </td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $LANG->_('Default quota')?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailquota&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $LANG->_('Mailserver')?></td>
        <td><a href="domain_edit_attributes.php?attrib=mailhost&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $LANG->_('Deliverymode')?></td>
        <td><a href="domain_edit_attributes.php?attrib=deliverymode&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
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
