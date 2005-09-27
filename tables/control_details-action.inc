  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="subtitle">
        <td colspan="4"><a href="tools/installmailserver.php?domain=<?=$domain?>&mxhost=<?=$_REQUEST["mxhost"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Create mailserver install script')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="control_del.php?mxhost=<?=$_REQUEST["mxhost"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Delete mailserver control object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=base&dn=<?=urlencode(pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["mxhost"].",".$_SESSION["USER_SEARCH_DN_CTR"])?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('LDIF Dump object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=sub&dn=<?=urlencode(pql_get_define("PQL_ATTR_CN")."=".$_REQUEST["mxhost"].",".$_SESSION["USER_SEARCH_DN_CTR"])?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('LDIF Dump branch')?></a></td>
      </tr>
    </th>
  </table>
