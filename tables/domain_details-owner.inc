  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Branch owner')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Organization name')?></td>
        <td><?php if($o) {echo urldecode($o);}else{echo $LANG->_('Not set');}?></td>
        <td><?=$o_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Postal code')?></td>
        <td><?php if($postalcode) {echo $postalcode;}else{echo $LANG->_('Not set');}?></td>
        <td><?=$postalcode_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Post box')?></td>
        <td><?php if($postofficebox) {echo $postofficebox;}else{echo $LANG->_('Not set');}?></td>
        <td><?=$postofficebox_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Postal address')?></td>
        <td><?php if($postaladdress) {echo $postaladdress;}else{echo $LANG->_('Not set');}?></td>
        <td><?=$postaladdress_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Street address')?></td>
        <td><?php if($street) {echo urldecode($street);}else{echo $LANG->_('Not set');}?></td>
        <td><?=$street_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('City')?></td>
        <td><?php if($l) {echo urldecode($l);}else{echo $LANG->_('Not set');}?></td>
        <td><?=$l_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('State')?></td>
        <td><?php if($st) {echo $st;}else{echo $LANG->_('Not set');}?></td>
        <td><?=$st_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?></td>
        <td><?php if($telephonenumber) {echo $telephonenumber;}else{echo $LANG->_('Not set');}?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Fax number')?></td>
        <td><?php if($facsimiletelephonenumber) {echo $facsimiletelephonenumber;}else{echo $LANG->_('Not set');}?></td>
        <td><?=$facsimiletelephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Contact person')?></td>
<?php if(is_array($seealso)) {
		$new_tr = 0;
		foreach($seealso as $sa) {
			$sa = pql_maybe_encode($sa); // Re-encode value that we got earlier from pql_domain_value().

			$username = pql_get_attribute($_pql->ldap_linkid, $sa, pql_get_define("PQL_GLOB_ATTR_CN"));
			if(!$username[0])
			  $username = $sa;
			else
			  $username = $username[0];

			if($new_tr) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$sa?>&view=<?=$view?>"><?=urldecode($username)?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_SEEALSO");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?php echo pql_get_define("PQL_GLOB_ATTR_SEEALSO");?>=<?=$sa?>&submit=3&action=modify&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify contact persons for %domain%'), array('domain' => $o)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_SEEALSO");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?php echo pql_get_define("PQL_GLOB_ATTR_SEEALSO");?>=<?=$sa?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Remove contact person from')?> <?=$o?>" border="0"></a>
        </td>
      </tr>

<?php		$new_tr = 1;
		}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_SEEALSO");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('contact person for domain'))); ?></a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_SEEALSO");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('contact person for domain'))); ?></a>
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
