  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Private domain values')?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('VAT number')?></td>
        <td><?php if($vatnumber) { echo $vatnumber; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$vatnumber_link?></td>
      </tr>

      <tr><td></td><td></td></tr>

      <!-- telephone numbers -->
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?></td>
        <td><?php if($telephonenumber) { echo $telephonenumber; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telefax number')?></td>
        <td><?php if($facsimiletelephonenumber) { echo $facsimiletelephonenumber; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$facsimiletelephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?> (<?=$LANG->_('Mobile')?>)</td>
        <td><?php if($mobile) { echo $mobile; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$mobile_link?></td>
      </tr>

      <tr><td></td><td></td></tr>

      <!-- addresses -->
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Street address')?></td>
        <td><?php if($registeredaddress) { echo $registeredaddress; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$registeredaddress_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Postal address')?></td>
        <td><?php if($postaladdress) { echo $postaladdress; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$postaladdress_link?></td>
      </tr>

      <tr><td></td><td></td></tr>

      <!-- misc -->
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Miscellaneous')?></td>
        <td><?php if($info) { echo $info; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$info_link?></td>
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
