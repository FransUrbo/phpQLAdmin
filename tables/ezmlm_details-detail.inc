  <span class="title1"><?=$LANG->_('List')?>: <?=$ezmlm->mailing_lists[$listno]["name"]."@".$domainname?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('List information')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Configuration')?></td>
        <td class="title"><?=$LANG->_('Value')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>
<?php foreach($ezmlm->mailing_lists[$listno] as $key => $value) {
		// Defined value
		if(!is_array($value)) {
			// Not an array (ie, not subscriber/text value)
			
			if($value == 1)
			  $value = $LANG->_('Yes');
			elseif(!$value)
			  $value = $LANG->_('No');
			
			// if this is number of subscribers, don't output it
			if(($key != 'subscribers') and ($key != 'directory') and ($key != 'dotpath')) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td><?=$key?></td>
        <td><?=$value?></td>
<?php		}
		} else {
			$j = 0;
			
			// Only output the key value once (the first entry)
			foreach($value as $x) {
?>
      <tr class="<?php pql_format_table(); ?>">
<?php			if($j == 0) { ?>
        <td><?=$key?></td>
<?php			} else { ?>
        <td></td>
<?php			} ?>
        <td><?=$x?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>&value=<?=$x?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete attribute')?>" border="0"></a></td>
<?php			$j++;
			}
		}

		if(($key != 'subscriber') and ($key != 'directory') and ($key != 'dotpath')) {
			if(($key == 'name') or ($key == 'local') or ($key == 'subscribers') or ($key == 'host')) {
				// Special circumstances - not editable/deletable
?>
        <td></td>
<?php		} else { // Editable/Deletable values ?>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Toggle')?>" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>"></a></td>
<?php		}
		}
?>
      </tr>
<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td></td>
        <td><a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=subscriber"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('subscriber'))); ?></a></td>
        <td></td>
      </tr>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="ezmlm_del.php?domain=<?=$domain?>&listno=<?=$listno?>"><?=$LANG->_('Delete list')?></a></td>
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
