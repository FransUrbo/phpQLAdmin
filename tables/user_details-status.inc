<?php
// Retreive 'days from Jan 1, 1970'
$tmp  = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_EXPIRATION"));

// Convert to UNIX timestamp
$seconds = $tmp[0] * (60 * 60 * 24);

// Convert to human readable
$time_expiration = date("D M j h:i Y", $seconds);

// Retreive the accountStatus attribute
$tmp = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_ISACTIVE"));
$status = $tmp[0];

$status = pql_ldap_accountstatus($status);
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Account status')?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td>Account expiration date</td>
        <td><?=$time_expiration?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=<?=pql_get_define("PQL_GLOB_ATTR_EXPIRATION")?>&user=<?=$userdn?>&set=expiration&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set expiration date"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td>Status</td>
<?php if(eregi('Active', $status)) { ?>
        <td><b><?=$LANG->_('Active')?></b></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=<?=pql_get_define("PQL_GLOB_ATTR_ISACTIVE")?>&user=<?=$userdn?>&set=disabled&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="Disable account"></a></td>
<?php } elseif(eregi('Locked', $status)) { ?>
        <td><b><?=$LANG->_('Disabled')?></b></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=<?=pql_get_define("PQL_GLOB_ATTR_ISACTIVE")?>&user=<?=$userdn?>&set=active&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Enable account"></a></td>
<?php } ?>
      </tr>
    </th>
  </table>

<?php if(!$SINGLE_USER) { ?>
  <p>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('POP status')?></th>
      <tr class="subtitle">
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=<?=pql_get_define("PQL_GLOB_ATTR_ISACTIVE")?>&user=<?=$userdn?>&set=nopop&view=<?=$view?>"><?=$LANG->_('Lock POP account')?></a>
        </td>
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
