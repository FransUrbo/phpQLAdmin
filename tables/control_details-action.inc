  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="subtitle">
        <td colspan="4"><a href="installmailserver.php?domain=<?=$domain?>&mxhost=<?=$_REQUEST["mxhost"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Create mailserver install script')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="control_del.php?mxhost=<?=$_REQUEST["mxhost"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Delete mailserver control object')?></a></td>
      </tr>
    </th>
  </table>
