<?php	$status = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_ISACTIVE"]);
	$status = $status[0];

	$status = pql_ldap_accountstatus($status);
?>
  <!-- accountstatus -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?php echo PQL_LANG_ACCOUNTSTATUS_TITLE; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
	<td><?=$status?></td>
      </tr>

<?php if(!$SINGLE_USER) { ?>
      <tr class="subtitle">
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=accountstatus&user=<?=$user?>&set=active"><?php echo PQL_LANG_ACCOUNTSTATUS_CHANGE_ACTIVE; ?></a>
	| <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=accountstatus&user=<?=$user?>&set=nopop"><?php echo PQL_LANG_ACCOUNTSTATUS_CHANGE_NOPOP; ?></a>
	| <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=accountstatus&user=<?=$user?>&set=disabled"><?php echo PQL_LANG_ACCOUNTSTATUS_CHANGE_DISABLE; ?></a>
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
