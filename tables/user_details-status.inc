<?php
// Retreive 'days from Jan 1, 1970'
$tmp  = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_EXPIRATION"));
if($tmp) {
	$tmp++;
	// Convert to UNIX timestamp
	$seconds = $tmp * (60 * 60 * 24);
	
	// Convert to human readable
	$time_expiration = date("D M j h:i Y", $seconds);
} else {
	$time_expiration = '<i>Not set</i>';
}

// Retreive the accountStatus attribute
$status = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_ISACTIVE"));
if($status == "deleted") {
  $date = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_ACCOUNT_PURGE"));
  if($date)
	$date = date("M j, Y", $date);
}
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo $LANG->_('Account status')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Account expiration date</td>
        <td><?php echo $time_expiration?></td>
        <td align="center"><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_EXPIRATION")?>&user=<?php echo $url["user"]?>&set=expiration&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set expiration date"></a></td>
      </tr>

      <form action="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_ISACTIVE")?>&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>" method="post" accept-charset="UTF-8">
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Status</td>
          <td>
            <select name="set">
              <option value="active"   <?php if($status == "active")   { echo "SELECTED"; } ?>><?php echo $LANG->_('Active')?></option>
              <option value="disabled" <?php if($status == "disabled") { echo "SELECTED"; } ?>><?php echo $LANG->_('Disabled')?></option>
              <option value="deleted"  <?php if($status == "deleted")  { echo "SELECTED"; } ?>><?php echo $LANG->_('Deleted')?></option>
              <option value="noaccess" <?php if($status == "noaccess") { echo "SELECTED"; } ?>><?php echo $LANG->_('No access')?></option>
<?php if ($_SESSION["OLD_STYLE_QUOTA"]) { ?>
              <option value="nopop"    <?php if($status == "nopop")    { echo "SELECTED"; } ?>><?php echo $LANG->_('POP Locked')?></option>
<?php } ?>
            </select>
          </td>
          <td>
            <input type="submit" value="<?php echo $LANG->_('Change')?>">
          </td>
        </form>
      </tr>
<?php if(($status == "deleted") and $date) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Account deletion date</td>
        <td><?php echo $date?></td>
        <td></td>
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
