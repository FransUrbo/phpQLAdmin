<?php
// Retreive 'days from Jan 1, 1970'
$tmp  = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_EXPIRATION"));
if($tmp[0]) {
	// Convert to UNIX timestamp
	$seconds = $tmp[0] * (60 * 60 * 24);
	
	// Convert to human readable
	$time_expiration = date("D M j h:i Y", $seconds);
} else {
	$time_expiration = '<i>Not set</i>';
}
// Retreive the accountStatus attribute
$status = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_ISACTIVE"));
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Account status')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Account expiration date</td>
        <td><?=$time_expiration?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_EXPIRATION")?>&user=<?=$url["user"]?>&set=expiration&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set expiration date"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Status</td>
<?php if(($status == "disabled") or ($status == "deleted")) { ?>
<?php   if($status == "disabled") { ?>
        <td><b><?=$LANG->_('Disabled')?></b></td>
<?php   } elseif($status == "deleted") { ?>
        <td><b><?=$LANG->_('Deleted')?></b></td>
<?php   } ?>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=active&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Enable account"></a></td>
<?php } elseif($status == "noaccess") { ?>
        <!-- --------------------------------------------------- --> 
        <!-- This looks weird! Two trashcans beside each other!! -->
        <!-- --------------------------------------------------- --> 
        <td><b><?=$LANG->_('POP Locked')?></b></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=active&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="Unlock account"></a></td>
<?php } else { ?>
        <td><b><?=$LANG->_('Active')?></b></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=disabled&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="Disable account"></a></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?=$url["user"]?>&set=noaccess&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="POP Lock"></a></td>
<?php } ?>
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
