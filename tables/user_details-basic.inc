  <!-- Basic user details - Full name (sn), Login shell etc -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('User data')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Username')?></td>
<?php $ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_GET["rootdn"]); ?>
        <td><?php if(!empty($$ref)) { echo $$ref; } ?></td>
        <td>
<?php if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=uid&user=<?=$url["user"]?>&oldvalue=<?=$uid?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change username')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php if(!@empty($_SESSION["ADVANCED_MODE"])) {
		// Part of the DN might be URL/UTF8 encoded...
		$dnparts    = split(",", $_GET["user"]);						// Split up the DN to get the very first part
		$userparts  = split("=", $dnparts[0]);							// Split the DN part into two parts - attribute and value.
		$userdnpart = pql_maybe_decode($userparts[1], $userparts[0]);	// Maybe URL/UTF8 decode the value
		$_GET["user"] = $userparts[0]."=".$userdnpart.",";				// Create a new DN part
		for($i=1; $i < count($dnparts); $i++) {							// Add the rest of the DN
			$_GET["user"] .= $dnparts[$i];
			if(isset($dnparts[$i+1]))
			  $_GET["user"] .= ",";
		}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('User DN')?></td>
        <td><?=$_GET["user"]?></td>
        <td>
<?php	if(@empty($_SESSION["SINGLE_USER"])) {
			@ldap_get_option($_pql->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, $version);
			if($version >= 3) {
			  // Only allow an admin to rename object (if we're using LDAP v3)
?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=modrdn&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Rename DN')?>" border="0"></a>
<?php		}
	  	}
?>
        </td>
      </tr>

<?php }

	  if(empty($USER_IS_GROUP)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Password')?></td>
        <td><?=$userpassword?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=userpassword&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Set password')?>" border="0"></a>
        </td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Firstname')?></td>
        <td><?php if(!empty($givenname)) { echo $givenname; }?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=givenname&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>&oldvalue=<?=$givenname?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change First name')?>" border="0"></a></td>
      </tr>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Lastname')?></td>
        <td><?php if(!empty($sn)) { echo $sn; }?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=sn&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>&oldvalue=<?=$sn?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change last name')?>" border="0"></a></td>
      </tr>
<?php
	  // {{{ Surname and Lastname
	  // - Can/'should' be multi-valued, hence pql_format_table_single()
	  $class=pql_format_table(0);

	  $tmp  = "user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&attrib=cn";
	  $tmp .= "&user=".$url["user"]."&view=".$_GET["view"];

	  $txt  = array('tit' => $LANG->_('Surname').", ".$LANG->_('Lastname'),
					'add' => $LANG->_('Add name'),
					'del' => $LANG->_('Delete name'),
					'mod' => $LANG->_('Modify name'));

	  $link = array('add' => $tmp."&type=add",
					'del' => $tmp."&submit=1&type=del",
					'mod' => $tmp); unset($tmp);

	  pql_format_table_single($_pql->ldap_linkid, $txt, $link, $cn,
							  pql_get_define("PQL_ATTR_CN"), $class);
// }}}

	  if(!@empty($_SESSION["ADVANCED_MODE"]) and empty($USER_IS_GROUP)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Login shell')?></td>
        <td><?php if(!empty($loginshell)){echo $loginshell;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php	if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=loginshell&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change login shell')?>" border="0"></a>
<?php	} ?>
        </td>
      </tr>

      <!-- DLW: Why do only some of these have "oldvalue" entries? -->
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Home directory')?></td>
        <td><?php if(!empty($homedirectory)){echo $homedirectory;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php	if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=homedirectory&user=<?=$url["user"]?>&oldvalue=<?=urlencode($homedirectory);?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change home directory')?>" border="0"></a>
<?php	} ?>
        </td>
      </tr>

<?php	if(!empty($uidnumber) && !empty($gidnumber)) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('UID;GID')?></td>
        <td><?=$uidnumber . ";" . $gidnumber?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=uidnumber&user=<?=$url["user"]?>&oldvalue=<?=urlencode($uidnumber . ";" . $gidnumber);?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change UID/GID')?>" border="0"></a></td>
      </tr>

<?php 		if(isset($memberuid)) {
				$class = pql_format_table(0);
			
				$tmp  = "user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
				$tmp .= "&user=".$url["user"]."&attrib=".pql_get_define("PQL_ATTR_ADDITIONAL_GROUP");
				$tmp .= "&uid=".$uid;
				$tmp .= "&view=".$_GET["view"];
				
				$txt  = array('tit' => $LANG->_('Additional group(s)'),
							  'add' => $LANG->_('Add group'),
							  'del' => $LANG->_('Delete group'));
				
				$link = array('add' => $tmp."&submit=3&action=add",
							  'del' => $tmp."&submit=4&action=delete");
				unset($tmp);
				
				pql_format_table_single($_pql->ldap_linkid, $txt, $link, $memberuid,
										pql_get_define("PQL_ATTR_ADDITIONAL_GROUP"),
										$class);
			}
	  	}
	  }

// {{{ When user do a login, should the advanced mode be loaded?
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Start in advanced mode')?></td>
<?php if($startwithadvancedmode) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_START_ADVANCED")?>&oldvalue=1&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Do not start in advanced mode')?>"></a>
        </td>
<?php } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_START_ADVANCED")?>&oldvalue=0&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Make sure we start in advanced mode')?>"></a>
        </td>
<?php } ?>
      </tr>
<?php
// }}}
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
