  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
<?php if($_REQUEST["user"] != $_SESSION["USER_DN"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="user_del.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><?=$LANG->_('Delete user')?></a></td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=base&dn=<?php echo $url["user"]?>&view=<?=$_GET["view"]?>"><?=$LANG->_('LDIF Dump object')?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="dump_object.php?scope=sub&dn=<?php echo $url["user"]?>&view=<?=$_GET["view"]?>"><?=$LANG->_('LDIF Dump branch')?></a></td>
      </tr>
    </th>
  </table>
