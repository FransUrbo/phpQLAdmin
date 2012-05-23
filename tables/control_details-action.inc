  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
      <tr class="subtitle">
        <td colspan="4"><a href="tools/installmailserver.php?domain=<?php echo $domain?>&mxhost=<?php echo $_REQUEST["mxhost"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?php echo $LANG->_('Create mailserver install script')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="control_del.php?mxhost=<?php echo $_REQUEST["mxhost"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?php echo $LANG->_('Delete mailserver control object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=base&dn=<?php echo urlencode($_REQUEST["mxhost"])?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?php echo $LANG->_('LDIF Dump object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=sub&dn=<?php echo urlencode($_REQUEST["mxhost"])?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?php echo $LANG->_('LDIF Dump branch')?></a></td>
      </tr>
    </th>
  </table>
