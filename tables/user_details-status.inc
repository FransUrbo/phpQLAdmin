<?php
// Retreive 'days from Jan 1, 1970'
$tmp  = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_EXPIRATION"));

// Convert to UNIX timestamp
$seconds = $tmp[0] * (60 * 60 * 24);

// Convert to human readable
$time_expiration = date("D M j h:i Y", $seconds);

// Retreive the accountStatus attribute
$tmp = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_ISACTIVE"));
$status = $tmp[0];

$status = pql_ldap_accountstatus($status);
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Account status')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td>Account expiration date</td>
        <td><?=$time_expiration?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_EXPIRATION")?>&user=<?=$url["user"]?>&set=expiration&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set expiration date"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td>Status</td>
<?php if(eregi('Active', $status)) { ?>
        <td><b><?=$LANG->_('Active')?></b></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=disabled&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="Disable account"></a></td>
<?php } elseif(eregi('Locked', $status)) { ?>
        <td><b><?=$LANG->_('Disabled')?></b></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=active&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Enable account"></a></td>
<?php } ?>
      </tr>
    </th>
  </table>

<?php if(!$_SESSION["SINGLE_USER"]) { ?>
  <p>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('POP status')?></th>
      <tr class="subtitle">
        <td>
<?php if(!$_SESSION["NEW_STYLE_QUOTA"])  { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=nopop&view=<?=$_GET["view"]?>"><?=$LANG->_('Lock POP account')?></a>
<?php } else { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=noaccess&view=<?=$_GET["view"]?>"><?=$LANG->_('Lock account')?></a>
<?php } ?>
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
