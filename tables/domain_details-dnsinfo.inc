<?php $res = getmxrr($defaultdomain, $rec, $weight); ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('DNS informations')?></th>
<?php if(count($rec) == 0) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="2"><?php echo $LANG->_('This domain isn\'t registred within the DNS system')?></td>
      </tr>
<?php } else { ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Mailhost')?></td>
        <td class="title"><?php echo $LANG->_('Priority')?></td>
      </tr>
<?php	asort($weight);
		foreach($weight as $key => $prio) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo pql_maybe_idna_decode($rec[$key])?></td>
        <td align="right"><?php echo $prio?></td>
      </tr>
<?php	} // end foreach
	} // end if (count($rec) == 0)
?>
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
