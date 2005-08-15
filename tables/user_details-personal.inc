  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Personal details')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Company/Organization')?></td>
        <td><?php if($o) { echo $o; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$o_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Title')?></td>
        <td><?php if($personaltitle) { echo $personaltitle; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$personaltitle_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Room number')?></td>
        <td><?php if($roomnumber) { echo $roomnumber; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$roomnumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?> (<?=$LANG->_('Work')?>)</td>
        <td><?php if($telephonenumber) { echo $telephonenumber; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?> (<?=$LANG->_('Home')?>)</td>
        <td><?php if($homephone) { echo $homephone; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$homephone_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?> (<?=$LANG->_('Mobile')?>)</td>
        <td><?php if($mobile) { echo $mobile; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$mobile_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Telephone number')?> (<?=$LANG->_('Pager')?>)</td>
        <td><?php if($pager) { echo $pager; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$pager_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Street address')?> (<?=$LANG->_('Home')?>)</td>
        <td><?php if($homepostaladdress) { echo $homepostaladdress; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$homepostaladdress_link?></td>
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
