<?php $res = getmxrr($defaultdomain, $rec, $weight); ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('DNS informations')?></th>
<?php if(count($rec) == 0) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="2"><?=$LANG->_('This domain isn\'t registred within the DNS system')?></td>
      </tr>
<?php } else { ?>
      <tr>
        <td class="title"><?=$LANG->_('Mailhost')?></td>
        <td class="title"><?=$LANG->_('Priority')?></td>
      </tr>
<?php	asort($weight);
		foreach($weight as $key => $prio) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$rec[$key]?></td>
        <td align="right"><?=$prio?></td>
      </tr>
<?php	} // end foreach
	} // end if (count($rec) == 0)
?>

      <tr class="subtitle">
        <td colspan="4"><a href="dnszonetemplate.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&defaultdomain=<?=$defaultdomain?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Create DNS template zone file')?></a></td>
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
