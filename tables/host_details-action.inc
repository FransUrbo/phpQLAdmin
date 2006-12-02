  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_del.php?host=<?=urlencode($_REQUEST["host"])?>&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Remove physical server')?></a></td>
      </tr>
  </table>
