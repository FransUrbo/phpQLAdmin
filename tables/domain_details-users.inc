  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_LANG_USER_REGISTRED?></th>
<?php if(is_array($users)) { ?>
      <tr>
        <td class="title"><?=PQL_LANG_USER?></td>
        <td class="title"><?=PQL_LANG_USER_ID?></td>
        <td class="title"><?=PQL_LANG_ACCOUNTSTATUS_STATUS?></td>
        <td class="title"><?=PQL_LANG_OPTIONS?></td>
      </tr>

<?php	asort($users);
		foreach($users as $user) {
			$userdn = urlencode($user);

			$uid    = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_UID"]); $uid = $uid[0];
			$cn     = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_CN"]); $cn = $cn[0];
			$uidnr  = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_QMAILUID"]); $uidnr = $uidnr[0];
			
			$status = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_ISACTIVE"]);
			$status = pql_ldap_accountstatus($status[0]);

			if(($uid != 'root') or ($uidnr != '0')) {
				// Do NOT show root user(s) here! This should (for safty's sake)
				// not be availible to administrate through phpQLAdmin!
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$userdn?>"><?=$cn?></a></td>
        <td><?=$uid?></td>
        <td><?=$status?></td>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$userdn?>"><img src="images/edit.png" width="12" height="12" alt="<?=PQL_LANG_USER_EDIT?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$userdn?>"><img src="images/del.png" width="12" height="12" alt="<?=PQL_LANG_USER_DELETE?>" border="0"></a></td>
      </tr>

<?php		}
		}
	  } else {
		  // no users registred
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="4"><?=PQL_LANG_USER_NONE?></td>
      </tr>
<?php } ?>

      <tr class="subtitle">
        <td colspan="4"><a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=PQL_LANG_USER_NEW?></a></td>
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
