  <!-- Deliverymode -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Delivery mode')?></th></th>
      <tr>
        <td class="title"><?=$LANG->_('Mode')?></td>
      </tr>

<?php	$deliverymode = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_GLOB_ATTR_MODE"));
	if(empty($deliverymode)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Default properties')?></td>
      </tr>
<?php	} else {
		foreach($deliverymode as $mode) {
		    $mode_text = pql_ldap_deliverymode($mode);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$mode_text?></td>
      </tr>
<?php	} // end of foreach
      } // end of if-else
?>

      <tr class="subtitle">
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=deliverymode&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?=$LANG->_('Change properties')?></a>
        </td>
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
