      <table cellspacing="0" cellpadding="3" border="0">
        <th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title">rcpthosts</td>
<?php $i = 0;
	  if(is_array($rcpthosts)) {
		foreach($rcpthosts as $rcpthost) {
			if(!$i) {
?>
            <td><?=pql_maybe_idna_decode($rcpthost)?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=add&set=<?=$rcpthost?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=del&set=<?=$rcpthost?>&submit=1&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php		} else { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"></td>
            <td><?=pql_maybe_idna_decode($rcpthost)?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=add&set=<?=$rcpthost?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=del&set=<?=$rcpthost?>&submit=1&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php		}
			$i++;
		}
	  } else {
?>
          <td><i>not defined</i></td>
          <td></td>
        </tr>
<?php } ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&view=<?=$view?>">Add value</a></td>
            <td></td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?attrib=rcpthosts&type=replicate&mxhost=<?=$mxhost?>&submit=1&view=<?=$view?>">Replicate</a></td>
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
