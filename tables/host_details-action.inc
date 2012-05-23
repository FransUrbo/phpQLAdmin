  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
<?php if($_REQUEST["host"] != 'Global') { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="host_del.php?host=<?php echo urlencode($_REQUEST["host"])?>&view=<?=$_REQUEST["view"]?>"><?=$LANG->_('Remove physical server')?></a></td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_edit_attributes.php?host=<?php echo urlencode($_REQUEST["host"])?>&type=a&action=modify"><?=$LANG->_('Replace IP address globally')?></a></td>
      </tr>
    </th>
  </table>
