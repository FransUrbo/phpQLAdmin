<?php $forwarders = pql_search_forwarders($_pql, $user); ?>
  <!-- forwarders in other accounts to this user  -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LANG_MAILFORWARDINGADDRESS_OTHER; ?></th>
      <tr>
	<td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
	<td class="title"><?php echo PQL_LANG_USER; ?></td>
      </tr>

<?php
if(empty($forwarders)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td colspan="3"><?php echo PQL_LANG_MAILFORWARDINGADDRESS_NONE; ?></td>
      </tr>
<?php
} else {
	foreach($forwarders as $forwarder){
?>
      <tr class="<?php table_bgcolor(); ?>">
	<td><a href="mailto:<?=$forwarder["email"]?>"><?=$forwarder["email"]?></a></td>
	<td>
          <a href="user_detail.php?user=<?php echo urlencode($forwarder["reference"]); ?>"><?=$forwarder[PQL_CONF_REFERENCE_USERS_WITH]?></a>
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
