<?php //Edited By Aaron Collins 7-8-2005 to allow the addition of o=organization ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Samba Settings')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('User SID')?></td>
        <td><?php if($sambasid) { echo $sambasid; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambasid_link?></td>
      </tr>

<?php if(!$sambasid) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3">
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp
          <?php echo $LANG->_('No SID defined for user. Please set one before continuing.')?>
        </td>
      </tr>
<?php } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Network Drive Letter')?></td>
        <td><?php if($sambahomedrive) { echo $sambahomedrive; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambahomedrive_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Roaming profile path')?></td>
        <td><?php if($sambaprofilepath) { echo $sambaprofilepath; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambaprofilepath_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Home directory UNC path')?></td>
        <td><?php if($sambahomepath) { echo $sambahomepath; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambahomepath_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Logon Script')?></td>
        <td><?php if($sambalogonscript) { echo $sambalogonscript; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambalogonscript_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Windows NT Domain Name')?></td>
        <td><?php if($sambadomainname) { echo $sambadomainname; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambadomainname_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('List of user workstations the user is allowed to logon to')?></td>
        <td><?php if($sambauserworkstations) { echo $sambauserworkstations; } else { echo $LANG->_('Not set'); }?></td>
        <td><?php echo $sambauserworkstations_link?></td>
      </tr>
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
