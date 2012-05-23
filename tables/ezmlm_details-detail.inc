<?php
// Just to simplify and shorten the href below
$link = "ezmlm_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&domainname=".$_REQUEST["domainname"];
?>
  <span class="title1"><?php echo $LANG->_('List')?>: <?=$ezmlm->mailing_lists[$listno]["name"]."@".$_REQUEST["domainname"]?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('List information')?></th>
      <tr>
        <td class="title"><?php echo $LANG->_('Configuration')?></td>
        <td class="title"><?php echo $LANG->_('Value')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
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
        <td><b><?php echo $key?></b></td>
        <td><?php echo $value?></td>
<?php		}
		} elseif($key == 'subscriber') {
			$j = 0;
			
			// Only output the key value once (the first entry)
			foreach($value as $x) {
				// $x => subscriber address!
?>
      <tr class="<?php pql_format_table(); ?>">
<?php			if($j == 0) { ?>
        <td><b><?php echo $key?></b></td>
<?php			} else { ?>
        <td></td>
<?php			} ?>
        <td><?php echo $x?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=<?=$key?>&<?=$key?>=<?=$x?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete attribute')?>" border="0"></a></td>
<?php			$j++;
			}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td></td>
        <td><a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=subscriber"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('subscriber'))); ?></a></td>
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
        <td><b><?php echo $key?></b></td>
<?php			} else { ?>
        <td></td>
<?php			} ?>
        <td><?php echo $x?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=<?=$key?>&<?=$key?>=<?=$x?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete attribute')?>" border="0"></a></td>
<?php			$j++;
			}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td></td>
        <td><a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=killaddress"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('rejected address'))); ?></a></td>
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
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=<?=$key?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Toggle')?>" border="0"></a>&nbsp;&nbsp;<a href="<?=$list?>&listno=<?=$_REQUEST["listno"]?>&type=<?=$key?>"></a></td>
<?php		}
		}
?>
      </tr>
<?php }

	  if(!@$ezmlm->mailing_lists[$listno]["subscriber"][0]) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td><b>subscriber</b></td>
        <td><a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=subscriber"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('subscriber'))); ?></a></td>
        <td></td>
      </tr>
<?php }

	  if(!@$ezmlm->mailing_lists[$listno]["killlist"][0]) {
?>

	  <tr><td></td><td></td><td></td></tr>
      <tr class="<?php pql_format_table(); ?>">
        <td><b>killlist</b></td>
        <td><a href="<?php echo $link?>&listno=<?=$_REQUEST["listno"]?>&type=killaddress"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('rejected address'))); ?></a></td>
        <td></td>
      </tr>
<?php } ?>
    </th>
  </table>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="ezmlm_del.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?=$url["domain"]?>&listno=<?=$_REQUEST["listno"]?>"><?=$LANG->_('Delete list')?></a></td>
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
