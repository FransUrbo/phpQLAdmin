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
			if(($key != 'subscribers') and ($key != 'directory') and
			   ($key != 'dotpath') and ($key != 'killlisted') and
			   ($key != 'killlist') and ($key != 'kill'))
			{
?>

      <tr class="<?php pql_format_table(); ?>">
        <td><b><?=$key?></b></td>
        <td><?=$value?></td>
<?php		}
		} elseif($key == 'subscriber') {
			$j = 0;
			
			// Only output the key value once (the first entry)
			foreach($value as $x) {
				// $x => subscriber address!
?>
      <tr class="<?php pql_format_table(); ?>">
<?php			if($j == 0) { ?>
        <td><b><?=$key?></b></td>
<?php			} else { ?>
        <td></td>
<?php			} ?>
        <td><?=$x?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>&value=<?=$x?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete attribute')?>" border="0"></a></td>
<?php			$j++;
			}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td></td>
        <td><a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=subscriber"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('subscriber'))); ?></a></td>
        <td></td>
      </tr>

<?php	} elseif($key == 'killlist') {
?>
      <!-- Kill list -->
	  <tr><td></td><td></td><td></td></tr>

<?php		$j = 0;
			
			// Only output the key value once (the first entry)
			foreach($value as $x) {
				// $x => subscriber address!
?>
      <tr class="<?php pql_format_table(); ?>">
<?php			if($j == 0) { ?>
        <td><b><?=$key?></b></td>
<?php			} else { ?>
        <td></td>
<?php			} ?>
        <td><?=$x?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>&value=<?=$x?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete attribute')?>" border="0"></a></td>
<?php			$j++;
			}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td></td>
        <td><a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=killaddress"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('rejected address'))); ?></a></td>
        <td></td>
      </tr>
<?php	}

		if(($key != 'killlist') and ($key != 'killlisted') and ($key != 'subscriber') and
		   ($key != 'directory') and ($key != 'dotpath') and ($key != 'kill'))
		{
			if(($key == 'name') or ($key == 'local') or ($key == 'subscribers') or ($key == 'host')) {
				// Special circumstances - not editable/deletable
?>
        <td></td>
<?php		} else { // Editable/Deletable values ?>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Toggle')?>" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=<?=$key?>"></a></td>
<?php		}
		}
?>
      </tr>
<?php }

	  if(!$ezmlm->mailing_lists[$listno]["subscriber"][0]) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td><b>subscriber</b></td>
        <td><a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=subscriber"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('subscriber'))); ?></a></td>
        <td></td>
      </tr>
<?php }

	  if(!$ezmlm->mailing_lists[$listno]["killlist"][0]) {
?>

	  <tr><td></td><td></td><td></td></tr>
      <tr class="<?php pql_format_table(); ?>">
        <td><b>killlist</b></td>
        <td><a href="ezmlm_edit_attribute.php?rootdn=<?=$_REQUEST["rootdn"]?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=killaddress"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('rejected address'))); ?></a></td>
        <td></td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="ezmlm_del.php?rootdn=<?=$_REQUEST["rootdn"]?>&listno=<?=$listno?>"><?=$LANG->_('Delete list')?></a></td>
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
