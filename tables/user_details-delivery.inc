  <!-- Deliverymode -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo $LANG->_('Delivery mode')?></th></th>
<?php	$deliverymode = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_MODE"));
		if(!is_array($deliverymode))
			$deliverymode = array($deliverymode);
	    $class = pql_format_table(0);

		if(empty($deliverymode)) {
?>
      <tr>
        <td class="title"><?php echo $LANG->_('Delivery mode')?></td>
        <td class="<?php echo $class?>"><?php echo $LANG->_('Default properties')?></td>
      </tr>
<?php	} else {
			$new_tr = 0;
			foreach($deliverymode as $mode) {
			    $mode_text = pql_ldap_deliverymode($mode);
				
				if($new_tr) {
?>
      <tr>
        <td class="title"></td>
        <td class="<?php echo $class?>"><?php echo $mode_text?></td>
      </tr>
<?php			} else { ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Delivery mode')?></td>
        <td class="<?php echo $class?>"><?php echo $mode_text?></td>
      </tr>
<?php			}  // end of new_tr

				$new_tr = 1;
			} // end of foreach

			$deliveryprog = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_PROGRAM"));
			if($deliveryprog and in_array('nolocal', $deliverymode)) {
?>
      <tr>
        <td class="title"><?php echo $LANG->_('Mail delivery program')?></td>
        <td class="<?php echo pql_format_table()?>"><?php echo $deliveryprog?></td>
      </tr>
<?php		}
		} // end of if-else
?>

      <tr class="subtitle">
        <td align="right"><img src="images/edit.png" width="12" height="12" alt="" border="0"></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=deliverymode&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>">
            <?php echo $LANG->_('Change properties')?>
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
