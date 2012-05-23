  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Branch owner')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Organization name')?></td>
        <td><?php if($o) { echo urldecode($o); } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $o_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('VAT number')?></td>
        <td><?php if($vatnumber) { echo $vatnumber; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $vatnumber_link?></td>
      </tr>

      <tr><td></td><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Postal code')?></td>
        <td><?php if($postalcode) {echo $postalcode;}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $postalcode_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Post box')?></td>
        <td><?php if($postofficebox) {echo $postofficebox;}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $postofficebox_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Postal address')?></td>
        <td><?php if($postaladdress) {echo $postaladdress;}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $postaladdress_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Street address')?></td>
        <td><?php if($street) {echo urldecode($street);}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $street_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('City')?></td>
        <td><?php if($l) {echo urldecode($l);}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $l_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('State')?></td>
        <td><?php if($st) {echo $st;}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $st_link?></td>
      </tr>

      <tr><td></td><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Telephone number')?></td>
        <td><?php if($telephonenumber) {echo $telephonenumber;}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $telephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Fax number')?></td>
        <td><?php if($facsimiletelephonenumber) {echo $facsimiletelephonenumber;}else{echo $LANG->_('Not set');}?></td>
        <td><?php echo $facsimiletelephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Telephone number')?> (<?php echo $LANG->_('Mobile')?>)</td>
        <td><?php if($mobile) { echo $mobile; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $mobile_link?></td>
      </tr>

      <tr><td></td><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Additional information')?></td>
        <td><?php if($info) { echo $info; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $info_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Contact person')?></td>
<?php if(is_array($seealso)) {
		$new_tr = 0;
		foreach($seealso as $sa) {
			$sa = pql_maybe_encode($sa);

			$username = $_pql->get_attribute($sa, pql_get_define("PQL_ATTR_CN"));
			if(!$username)
			  $username = $sa;
			elseif(is_array($username))
			  $username = $username[0];

			if($new_tr) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="user_detail.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&user=<?php echo $sa?>&view=<?php echo $_REQUEST["view"]?>"><?php echo urldecode($username)?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_SEEALSO")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&<?php echo pql_get_define("PQL_ATTR_SEEALSO")?>=<?php echo $sa?>&submit=3&action=modify&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify contact persons for %domain%'), array('domain' => $o)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_SEEALSO")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&<?php echo pql_get_define("PQL_ATTR_SEEALSO")?>=<?php echo $sa?>&submit=4&action=delete&view=<?php echo $_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Remove contact person from')?> <?php echo $o?>" border="0"></a>
        </td>
      </tr>

<?php		$new_tr = 1;
		}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_SEEALSO")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&submit=3&action=add&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('contact person for domain'))); ?></a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_SEEALSO")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&submit=3&action=add&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('contact person for domain'))); ?></a>
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
