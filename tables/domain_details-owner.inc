  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Branch owner</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Organization name</td>
        <td><?php if($o) {echo urldecode($o);}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$o_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Postal code</td>
        <td><?php if($postalcode) {echo $postalcode;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$postalcode_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Post box</td>
        <td><?php if($postofficebox) {echo $postofficebox;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$postofficebox_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Postal address</td>
        <td><?php if($postaladdress) {echo $postaladdress;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$postaladdress_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Street address</td>
        <td><?php if($street) {echo urldecode($street);}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$street_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">City</td>
        <td><?php if($l) {echo urldecode($l);}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$l_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">State</td>
        <td><?php if($st) {echo $st;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$st_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number</td>
        <td><?php if($telephonenumber) {echo $telephonenumber;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Fax number</td>
        <td><?php if($facsimiletelephonenumber) {echo $facsimiletelephonenumber;}else{echo PQL_LANG_UNSET;}?></td>
        <td><?=$facsimiletelephonenumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Contact person</td>
<?php if(is_array($seealso)) {
		$new_tr = 0;
		foreach($seealso as $sa) {
			$username = pql_get_userattribute($_pql->ldap_linkid, $sa, 'cn');
			if(!$username[0])
			  $username = $sa;
			else
			  $username = $username[0];

			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$sa?>"><?=urldecode($username)?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&seealso=<?=$sa?>&submit=3&action=modify"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify contact persons for <?=$o?>"></a>
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&seealso=<?=$sa?>&submit=4&action=delete"><img src="images/del.png" width="12" height="12" alt="Remove contact person from <?=$o?>" border="0"></a>
        </td>
      </tr>

<?php		$new_tr = 1;
		}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add">Add contact person for domain</a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=seealso&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add">Add contact person for domain</a>
        </td>
<?php } ?>
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
