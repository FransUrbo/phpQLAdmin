  <!-- Deliverymode -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Delivery mode')?></th></th>
<?php	$deliverymode = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_MODE"));
	    $class = pql_format_table(0);

		if(empty($deliverymode)) {
?>
      <tr>
        <td class="title"><?=$LANG->_('Delivery mode')?></td>
        <td class="<?=$class?>"><?=$LANG->_('Default properties')?></td>
      </tr>
<?php	} else {
			$new_tr = 0;
			foreach($deliverymode as $mode) {
			    $mode_text = pql_ldap_deliverymode($mode);
				
				if($new_tr) {
?>
      <tr>
        <td class="title"></td>
        <td class="<?=$class?>"><?=$mode_text?></td>
      </tr>
<?php			} else { ?>
      <tr>
        <td class="title"><?=$LANG->_('Delivery mode')?></td>
        <td class="<?=$class?>"><?=$mode_text?></td>
      </tr>
<?php			}  // end of new_tr

				$new_tr = 1;
			} // end of foreach
		} // end of if-else
?>

      <tr class="subtitle">
        <td align="right"><img src="images/edit.png" width="12" height="12" alt="" border="0"></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=deliverymode&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>">
            <?=$LANG->_('Change properties')?>
          </a>
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
