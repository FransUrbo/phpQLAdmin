  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="domain_del.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('Remove domain')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=base&dn=<?php echo $url["domain"]?>&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('LDIF Dump object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=sub&dn=<?php echo $url["domain"]?>&view=<?php echo $_REQUEST["view"]?>"><?php echo $LANG->_('LDIF Dump branch')?></a></td>
      </tr>
    </th>
  </table>
