  <!-- Deliverymode -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LANG_DELIVERYMODE_TITLE; ?></th></th>
      <tr>
	<td class="title"><?php echo PQL_LANG_DELIVERYMODE_MODE; ?></td>
      </tr>

<?php	$deliverymode = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MODE"]);
	if(empty($deliverymode)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?php echo PQL_LANG_DELIVERYMODE_NULL; ?></td>
      </tr>
<?php	} else {
		foreach($deliverymode as $mode){
		    $mode_text = pql_ldap_deliverymode($mode);
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?=$mode_text?></td>
      </tr>
<?php	} // end of foreach
      } // end of if-else
?>

<?php if(!$SINGLE_USER) { ?>
      <tr class="subtitle">
	<td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=deliverymode&user=<?=$user?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?php echo PQL_LANG_DELIVERYMODE_CHANGE; ?></a>
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
