  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Registred users')?></th>
      <tr>
        <td class="title"><?=$LANG->_('User')?></td>
        <td class="title"><?=$LANG->_('Username')?></td>
        <td class="title"><?=$LANG->_('Status')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>

<?php 	asort($users);
		foreach($users as $user) {
			$userdn = urlencode($user);

			$uid    = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_UID")); $uid = $uid[0];
			$cn     = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_CN")); $cn = $cn[0];
			$uidnr  = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_QMAILUID")); $uidnr = $uidnr[0];
			
			$status = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_ISACTIVE"));
			$status = pql_ldap_accountstatus($status[0]);

			if(($uid != 'root') or ($uidnr != '0')) {
				// Do NOT show root user(s) here! This should (for safty's sake)
				// not be availible to administrate through phpQLAdmin!
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$userdn?>"><?=$cn?></a></td>
        <td><?=$uid?></td>
        <td><?=$status?></td>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$userdn?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change user data')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$userdn?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete user')?>" border="0"></a></td>
      </tr>

<?php		}
		}
?>
      <tr class="subtitle">
        <td colspan="4"><a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=$LANG->_('Register new user')?></a></td>
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
