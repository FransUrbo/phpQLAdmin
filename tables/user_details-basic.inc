  <!-- Basic user details - Full name (sn), Login shell etc -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('User data')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Username')?></td>
<?php $ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $rootdn); ?>
        <td><?=$$ref?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=uid&user=<?=$userdn?>&oldvalue=<?=$uid?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change username')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php if($_SESSION["ADVANCED_MODE"]) {
		// Part of the DN might be URL/UTF8 encoded...
		$dnparts    = split(",", $user);							// Split up the DN to get the very first part
		$userparts  = split("=", $dnparts[0]);						// Split the DN part into two parts - attribute and value.
		$userdnpart = pql_maybe_decode($userparts[1], $userparts[0]);	// Maybe URL/UTF8 decode the value
		$user = $userparts[0]."=".$userdnpart.",";					// Create a new DN part
		for($i=1; $dnparts[$i]; $i++) {								// Add the rest of the DN
			$user .= $dnparts[$i];
			if($dnparts[$i+1])
			  $user .= ",";
		}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('User DN')?></td>
        <td><?=$user?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) {
		@ldap_get_option($_pql->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, $version);
		if($version >= 3) {
			// Only allow an admin to rename object (if we're using LDAP v3)
?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=modrdn&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Rename DN')?>" border="0"></a>
<?php	}
	  }
?>
        </td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Password')?></td>
        <td><?=$userpassword?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=userpassword&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Set password')?>" border="0"></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Surname').", ".$LANG->_('Lastname')?></td>
        <td><?=$cn?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=cn&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change common name')?>" border="0"></a></td>
      </tr>

<?php if($_SESSION["ADVANCED_MODE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Login shell')?></td>
        <td><?php if($loginshell){echo $loginshell;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=loginshell&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change login shell')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Home directory')?></td>
        <td><?php if($homedirectory){echo $homedirectory;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=homedirectory&user=<?=$userdn?>&oldvalue=<?=urlencode($homedirectory);?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change home directory')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php
	if(($uidnumber != '') && ($gidnumber != '')) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?="UID;GID"?></td>
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
