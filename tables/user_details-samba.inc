<?php //Edited By Aaron Collins 7-8-2005 to allow the addition of o=organization ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Samba Settings')?></th>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Network Drive Letter')?></td>
        <td><?php if($sambahomedrive) { echo $sambahomedrive; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambahomedrive_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Roaming profile path')?></td>
        <td><?php if($sambaprofilepath) { echo $sambaprofilepath; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambaprofilepath_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Home directory UNC path')?></td>
        <td><?php if($sambahomepath) { echo $sambahomepath; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambahomepath_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Logon Script')?></td>
        <td><?php if($sambalogonscript) { echo $sambalogonscript; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambalogonscript_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Windows NT Domain Name')?></td>
        <td><?php if($sambadomainname) { echo $sambadomainname; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambadomainname_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('List of user workstations the user is allowed to logon to')?></td>
        <td><?php if($sambauserworkstations) { echo $sambauserworkstations; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambauserworkstations_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('User SID')?></td>
        <td><?php if($sambasid) { echo $sambasid; } else { echo $LANG->_('Not set'); }?></td>
        <td><?=$sambasid_link?></td>
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
