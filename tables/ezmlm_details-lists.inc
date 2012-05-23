<?php
// Just to simplify and shorten the href below
$link = "ezmlm_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&domainname=".$_REQUEST["domainname"];
?>
  <span class="title1">Domain: <?php echo $_REQUEST["domain"]?> (domainname: <?php echo $_REQUEST["domainname"]?>)</span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Existing lists</th>
      <tr>
        <td class="title">Address</td>
        <td class="title">Subscribers</td>
        <td class="title">Owner</td>
        <td class="title">Options</td>
      </tr>

<?php if(!empty($ezmlm->mailing_lists_hostsindex)) {
		foreach($ezmlm->mailing_lists_hostsindex as $listdomain => $listarray) {
			if(is_array($listarray)) {
				// Go through each list in the domain
				foreach($listarray as $listname => $listno) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $listname."@".$listdomain?></td>
        <td align="right"><?php echo $ezmlm->mailing_lists[$listno]["subscribers"]?></td>
        <td><?php echo $ezmlm->mailing_lists[$listno]["owner"]?></td>
        <td><a href="ezmlm_detail.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&domainname=<?php echo $_REQUEST["domainname"]?>&listno=<?php echo $listno?>"><img src="images/edit.png" width="12" height="12" alt="edit list" border="0"></a>&nbsp;&nbsp;<a href="<?php echo $link?>&listno=<?php echo $listno?>&type=subscriber">add subscriber</a></td>
      </tr>
<?php			}
	  		}
		}
	  }
?>

      <tr class="subtitle">
        <td colspan="4"><a href="ezmlm_add.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&domainname=<?php echo $_REQUEST["domainname"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0">Register new list</a></td>
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
