<?php
$forwarders = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_FORWARDS"));
if(!is_array($forwarders) and $forwarders) {
  $forwarders = array($forwarders);
}

// Get the forwarding options
$options = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_MODE"));
if(is_array($options)) {
  // Flatten it...
  unset($opt); // Just in case...
  for($i=0; $options[$i]; $i++) {
	$opt .= $options[$i];
	if($options[$i+1])
	  $opt .= '<br>';
  }

  $options = $opt;
}
?>
  <!-- Forwarders (mailalternateaddress) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo $LANG->_('Forward mail to')?></th>
      <tr>
        <td class="title"><?php echo $LANG->_('Email')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
        <td class="title"></td>
      </tr>

<?php if(empty($forwarders)) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3"><?php echo $LANG->_('None')?></td>
      </tr>
<?php } else { 
	foreach($forwarders as $forwarder) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo pql_maybe_idna_decode($forwarder)?></td>
        <td><?php echo $options?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mailforwardingaddress&user=<?php echo $url["user"]?>&mailforwardingaddress=<?php echo $forwarder?>&oldvalue=<?php echo $forwarder?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?attrib=mailforwardingaddress&user=<?php echo $url["user"]?>&oldvalue=<?php echo $forwarder?>&view=<?php echo $_GET["view"]?>"><img src="images/del.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>
<?php	} // end of foreach
      } // end of if-else
?>

      <tr class="subtitle">
        <td colspan="3">
          <a href="user_add_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=mailforwardingaddress&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"><?php echo $LANG->_('Create new forwarding address')?></a>
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
