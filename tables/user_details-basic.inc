  <!-- Basic user details - Full name (sn), Login shell etc -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_LANG_USER_DATA; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_USER_ID ?></td>
<?php $ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn); ?>
        <td><?=$$ref?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=uid&user=<?=$userdn?>&oldvalue=<?=$uid?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_UID_CHANGE; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php if($ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">User DN</td>
        <td><?=$user?></td>
        <td></td>
      </tr>

<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_USERPASSWORD_TITLE; ?></td>
        <td><?=$userpassword?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=userpassword&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_USERPASSWORD_NEW; ?>" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_USER_DATA_SURNAME . ", " . PQL_LANG_USER_DATA_LASTNAME; ?></td>
        <td><?=$cn?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=cn&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_CN_CHANGE; ?>" border="0"></a></td>
      </tr>

<?php if($ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_USER_LOGINSHELL; ?></td>
        <td><?php if($loginshell){echo $loginshell;}else{echo "none";} ?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=loginshell&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_LOGINSHELL_CHANGE; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_USER_HOMEDIR; ?></td>
        <td><?php if($homedirectory){echo $homedirectory;}else{echo "none";} ?></td>
        <td>
<?php if(!$SINGLE_USER) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=homedirectory&user=<?=$userdn?>&oldvalue=<?=urlencode($homedirectory);?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo PQL_LANG_HOMEDIRECTORY_CHANGE; ?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php
	if(($uidnumber != '') && ($gidnumber != '')) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo "UID;GID"; ?></td>
        <td><?=$uidnumber . ";" . $gidnumber?></td>
        <td></td>
      </tr>
<?php
	}
}
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
