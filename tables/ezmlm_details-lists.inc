  <span class="title1">Domain: <?=$domain?> (domainname: <?=$domainname?>)</span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Existing lists</th>
      <tr>
        <td class="title">Address</td>
        <td class="title">Subscribers</td>
        <td class="title">Owner</td>
        <td class="title">Options</td>
      </tr>

<?php foreach($ezmlm->mailing_lists_hostsindex as $listdomain => $listarray) {
		if(is_array($listarray)) {
			// Go through each list in the domain
			foreach($listarray as $listname => $listno) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$listname."@".$listdomain?></td>
        <td align="right"><?=$ezmlm->mailing_lists[$listno]["subscribers"]?></td>
        <td><?=$ezmlm->mailing_lists[$listno]["owner"]?></td>
        <td><a href="ezmlm_detail.php?domain=<?=$domain?>&domainname=<?=$domainname?>&listno=<?=$listno?>"><img src="images/edit.png" width="12" height="12" alt="edit list" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&domainname=<?=$domainname?>&listno=<?=$listno?>&attrib=subscriber">add subscriber</a></td>
      </tr>
<?php		}
	  	}
	}
?>

      <tr class="subtitle">
        <td colspan="4"><a href="ezmlm_add.php?domain=<?=$domain?>&domainname=<?=$domainname?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> Register new list</a></td>
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
