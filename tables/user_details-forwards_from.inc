<?php $forwarders = pql_search_forwarders($_pql, $_GET["user"]); ?>
  <!-- forwarders in other accounts to this user  -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Forwarders from other accounts to this user')?></th>
      <tr>
        <td class="title"><?=$LANG->_('Email')?></td>
        <td class="title"><?=$LANG->_('User')?></td>
      </tr>

<?php
if(empty($forwarders)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3"><?=$LANG->_('None')?></td>
      </tr>
<?php
} else {
	foreach($forwarders as $forwarder){
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="mailto:<?=$forwarder["email"]?>"><?=pql_maybe_idna_decode($forwarder["email"])?></a></td>
        <td>
          <a href="user_detail.php?rootdn=<?=$_GET["rootdn"]?>&domain=<?=$_GET["domain"]?>&user=<?php echo urlencode($forwarder["reference"]); ?>"><?=$forwarder[pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn)]?></a>
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
