      <table cellspacing="0" cellpadding="3" border="0">
        <th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title">locals</td>
<?php $i = 0;
	  if(is_array($locals)) {
		foreach($locals as $local) {
			if(!$i) {
?>
            <td><?php echo pql_maybe_idna_decode($local)?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=add&set=<?php echo $local?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=del&set=<?php echo $local?>&submit=1&view=<?php echo $_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php		} else { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"></td>
            <td><?php echo pql_maybe_idna_decode($local)?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=add&set=<?php echo $local?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=del&set=<?php echo $local?>&submit=1&view=<?php echo $_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php		}
			$i++;
		}
	  } else {
?>
          <td><i>not defined</i></td>
          <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=add&set=<?php echo $local?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=del&set=<?php echo $local?>&submit=1&view=<?php echo $_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
        </tr>
<?php } ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=locals&type=add&view=<?php echo $_REQUEST["view"]?>">Add value</a></td>
            <td></td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?attrib=locals&type=replicate&mxhost=<?php echo $_REQUEST["mxhost"]?>&submit=1&view=<?php echo $_REQUEST["view"]?>">Replicate</a></td>
            <td></td>
          </tr>
        </th>
      </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
