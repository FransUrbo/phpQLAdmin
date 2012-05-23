<?php $forwarders = pql_search_forwarders($_GET["user"]); ?>
  <!-- forwarders in other accounts to this user  -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo $LANG->_('Forwarders from other accounts to this user')?></th>
      <tr>
        <td class="title"><?php echo $LANG->_('Email')?></td>
        <td class="title"><?php echo $LANG->_('User')?></td>
      </tr>

<?php
if(empty($forwarders)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3"><?php echo $LANG->_('None')?></td>
      </tr>
<?php
} else {
	foreach($forwarders as $forwarder) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="mailto:<?php echo $forwarder["email"]?>"><?php echo pql_maybe_idna_decode($forwarder["email"])?></a></td>
        <td>
          <a href="user_detail.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&user=<?php echo urlencode($forwarder["reference"]); ?>"><?php echo $forwarder[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])]?></a>
        </td>
      </tr>
<?php
	} // end of foreach
} // end of if-else
?>
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
