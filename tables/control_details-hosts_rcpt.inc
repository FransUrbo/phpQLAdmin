      <table cellspacing="0" cellpadding="3" border="0">
        <th>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title">rcpthosts</td>
<?php $i = 0;
	  if(is_array($rcpthosts)) {
		foreach($rcpthosts as $rcpthost) {
			if(!$i) {
?>
            <td><?=$rcpthost?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=add&set=<?=$rcpthost?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=del&set=<?=$rcpthost?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php		} else { ?>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title"></td>
            <td><?=$rcpthost?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=add&set=<?=$rcpthost?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=del&set=<?=$rcpthost?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
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
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts">Add value</a></td>
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
