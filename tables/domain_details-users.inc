  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Registred users')?></th>
      <tr>
        <td class="title"><?=$LANG->_('User')?></td>
<?php if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_ATTR_UID")) { ?>
        <td class="title"><?=$LANG->_('Username')?></td>
<?php } ?>
        <td class="title"><?=$LANG->_('Status')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>

<?php 	if(is_array($users)) {
			asort($users);
			foreach($users as $user) {
				$userdn = urlencode($user);

				$uid    = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_UID"));
				$uid    = $uid[0];

				// DLW: This should be sorted on DisplayName, not cn.
				$cn     = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_CN"));
				$cn     = $cn[0];

				$uidnr  = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_QMAILUID"));
				$uidnr  = $uidnr[0];
			
				$status = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_ISACTIVE"));
				$status = pql_ldap_accountstatus($status[0]);

				if(($uid != 'root') or ($uidnr != '0')) {
					// Do NOT show root user(s) here! This should (for safty's sake)
					// not be availible to administrate through phpQLAdmin!
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="user_detail.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$userdn?>"><?=$cn?></a></td>
<?php if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn) == pql_get_define("PQL_ATTR_UID")) { ?>
        <td><?=$uid?></td>
<?php } ?>
        <td><?=$status?></td>
        <td><a href="user_detail.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$userdn?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change user data')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$userdn?>&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete user')?>" border="0"></a></td>
      </tr>

<?php			}
			}
		}
?>
      <tr class="subtitle">
        <td colspan="4"><a href="user_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=$LANG->_('Register new user')?></a></td>
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
