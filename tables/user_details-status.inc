<?php	$status = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_ISACTIVE"));
	$status = $status[0];

	$status = pql_ldap_accountstatus($status);
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('Account status')?></th>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?=$status?></td>
      </tr>

<?php if(!$SINGLE_USER) { ?>
      <tr class="subtitle">
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=accountstatus&user=<?=$userdn?>&set=active&view=<?=$view?>"><?=$LANG->_('Activate')?></a>
	| <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=accountstatus&user=<?=$userdn?>&set=nopop&view=<?=$view?>"><?=$LANG->_('Lock POP account')?></a>
	| <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=accountstatus&user=<?=$userdn?>&set=disabled&view=<?=$view?>"><?=$LANG->_('Disable account')?></a>
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
