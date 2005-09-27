  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="domain_del.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Remove domain')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=base&dn=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('LDIF Dump object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=sub&dn=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('LDIF Dump branch')?></a></td>
      </tr>
    </th>
  </table>
