<?php
if($nonprimaryrcpthosts and !is_array($nonprimaryrcpthosts)) {
  $nonprimaryrcpthosts = array($nonprimaryrcpthosts);
}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Non-primary MX values')?>
<?php if(is_array($nonprimaryrcpthosts)) {
        foreach($nonprimaryrcpthosts as $i => $value) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td align="right"><?=$i+1?></td>
        <td align="right"><?=$value?></td>
        <td align="right">
          <a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=nonprimaryrcpthosts&nonprimaryrcpthosts=<?=$value?>&type=add&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Non-primary MX Host domain')?>" border="0"></a>
          <a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=nonprimaryrcpthosts&nonprimaryrcpthosts=<?=$value?>&type=del&view=<?=$_REQUEST["view"]?>&submit=1"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Remove domain name')?>" border="0"></a>
        </td>
      </tr>
<?php   }
      } 
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="<?php pql_format_table(); ?>"></td>
        <td colspan="2">
          <a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=nonprimaryrcpthosts&action=add&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
      </tr>
    </th>
  </table>

  <p>

<?php include("./include/attrib.control.nonprimmx.inc") ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">
      <tr class="<?php pql_format_table(); ?>">
        <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td><?php nonprimaryrcpthosts_help() ?>
      </tr>
    </th>
  </table>
