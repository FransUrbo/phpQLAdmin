  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Personal details</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Title</td>
        <td><?php if($personaltitle) { echo $personaltitle; } else { echo PQL_LANG_UNSET; }?></td>
        <td><?=$personaltitle_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Room number</td>
        <td><?php if($roomnumber) { echo $roomnumber; } else { echo PQL_LANG_UNSET; }?></td>
        <td><?=$roomnumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Work)</td>
        <td><?php if($telephonenumber) { echo $telephonenumber; } else { echo PQL_LANG_UNSET; }?></td>
        <td><?=$telephonenumber_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Home)</td>
        <td><?php if($homephone) { echo $homephone; } else { echo PQL_LANG_UNSET; }?></td>
        <td><?=$homephone_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Mobile)</td>
        <td><?php if($mobile) { echo $mobile; } else { echo PQL_LANG_UNSET; }?></td>
        <td><?=$mobile_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Telephone number (Pager)</td>
        <td><?php if($pager) { echo $pager; } else { echo PQL_LANG_UNSET; }?></td>
        <td><?=$pager_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Street address (Home)</td>
        <td><?php if($homepostaladdress) { echo $homepostaladdress; } else { echo PQL_LANG_UNSET; }?></td>
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
