<?php $res = getmxrr($defaultdomain, $rec, $weight); ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_LANG_DNS_TITLE?></th>
<?php if(count($rec) == 0) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="2"><?=PQL_LANG_DNS_NONE?></td>
      </tr>
<?php } else { ?>
      <tr>
        <td class="title"><?=PQL_LANG_DNS_MAILHOST?></td>
        <td class="title"><?=PQL_LANG_DNS_PRIORITY?></td>
      </tr>
<?php	asort($weight);
		foreach($weight as $key => $prio) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$rec[$key]?></td>
        <td align="right"><?=$prio?></td>
      </tr>
<?php	} // end foreach
	} // end if (count($rec) == 0)
?>

      <tr class="subtitle">
        <td colspan="4"><a href="dnszonetemplate.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&defaultdomain=<?=$defaultdomain?>"><img src="images/edit.png" width="12" height="12" border="0">Create DNS template zone file</a></td>
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
