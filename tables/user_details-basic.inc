  <!-- Basic user details - Full name (sn), Login shell etc -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('User data')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Username')?></td>
<?php $ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_GET["rootdn"]); ?>
        <td><?=$$ref?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=uid&user=<?=$url["user"]?>&oldvalue=<?=$uid?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change username')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php if($_SESSION["ADVANCED_MODE"]) {
		// Part of the DN might be URL/UTF8 encoded...
		$dnparts    = split(",", $_GET["user"]);							// Split up the DN to get the very first part
		$userparts  = split("=", $dnparts[0]);						// Split the DN part into two parts - attribute and value.
		$userdnpart = pql_maybe_decode($userparts[1], $userparts[0]);	// Maybe URL/UTF8 decode the value
		$_GET["user"] = $userparts[0]."=".$userdnpart.",";					// Create a new DN part
		for($i=1; isset($dnparts[$i]); $i++) {								// Add the rest of the DN
			$_GET["user"] .= $dnparts[$i];
			if(isset($dnparts[$i+1]))
			  $_GET["user"] .= ",";
		}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('User DN')?></td>
        <td><?=pql_maybe_decode($_GET["user"])?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) {
		@ldap_get_option($_pql->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, $version);
		if($version >= 3) {
			// Only allow an admin to rename object (if we're using LDAP v3)
?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=modrdn&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Rename DN')?>" border="0"></a>
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
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=userpassword&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Set password')?>" border="0"></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Surname').", ".$LANG->_('Lastname')?></td>
        <td><?=$cn?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=cn&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change common name')?>" border="0"></a></td>
      </tr>

<?php if($_SESSION["ADVANCED_MODE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Login shell')?></td>
        <td><?php if($loginshell){echo $loginshell;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=loginshell&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change login shell')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

      <!-- DLW: Why do only some of these have "oldvalue" entries? -->
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Home directory')?></td>
        <td><?php if($homedirectory){echo $homedirectory;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=homedirectory&user=<?=$url["user"]?>&oldvalue=<?=urlencode($homedirectory);?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change home directory')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php
	if(($uidnumber != '') && ($gidnumber != '')) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('UID;GID')?></td>
        <td><?=$uidnumber . ";" . $gidnumber?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=uidnumber&user=<?=$url["user"]?>&oldvalue=<?=urlencode($uidnumber . ";" . $gidnumber);?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change UID/GID')?>" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional group(s)')?></td>
<?php if(isset($memberuid[1])) {
		$new_tr = 0;
		foreach($memberuid as $muid) {
			$muid = pql_maybe_idna_decode($muid);

			if($new_tr) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><?=$muid?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_GROUP")?>&<?=pql_get_define("PQL_ATTR_ADDITIONAL_GROUP")?>=<?=$muid?>&submit=4&action=delete&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Remove user from group')?>" border="0"></a>
        </td>
<?php		$new_tr = 1;
		} // if new_tr
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_GROUP")?>&submit=3&action=add&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional group'))); ?></a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_GROUP")?>&submit=3&action=add&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional group'))); ?></a>
        </td>
<?php }
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
