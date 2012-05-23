  <!-- advanced delivery options -->
<?php $qmaildotmode = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"));
      $deliveryprogrampath = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_PROGRAM"));

      if($qmaildotmode == "") {
		  $qmaildotmode = $LANG->_('Default');
      }

      if($deliveryprogrampath == "") {
		  $deliveryprogrampath = $LANG->_('None');
      }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Advanced delivery properties')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Dot mode')?></td>
        <td><?php echo $qmaildotmode?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=qmaildotmode&user=<?=$url["user"]?>&oldvalue=<?=$qmaildotmode?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Delivery program')?></td>
        <td><?php echo $deliveryprogrampath?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=deliveryprogrampath&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
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
