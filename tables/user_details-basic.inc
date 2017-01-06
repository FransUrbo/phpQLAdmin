  <!-- Basic user details - Full name (sn), Login shell etc -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('User data')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Username')?></td>
<?php $ref = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_GET["rootdn"]); ?>
        <td><?php if(!empty($$ref)) { echo $$ref; } ?></td>
        <td>
<?php if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=uid&user=<?php echo $url["user"]?>&oldvalue=<?php echo $uid?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change username')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>

<?php if(!@empty($_SESSION["ADVANCED_MODE"])) {
		// Part of the DN might be URL/UTF8 encoded...
		$dnparts    = explode(",", $_GET["user"]);						// Split up the DN to get the very first part
		$userparts  = explode("=", $dnparts[0]);							// Split the DN part into two parts - attribute and value.
		$userdnpart = pql_maybe_decode($userparts[1], $userparts[0]);	// Maybe URL/UTF8 decode the value
		$_GET["user"] = $userparts[0]."=".$userdnpart.",";				// Create a new DN part
		for($i=1; $i < count($dnparts); $i++) {							// Add the rest of the DN
			$_GET["user"] .= $dnparts[$i];
			if(isset($dnparts[$i+1]))
			  $_GET["user"] .= ",";
		}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('User DN')?></td>
        <td><?php echo $_GET["user"]?></td>
        <td>
<?php	if(@empty($_SESSION["SINGLE_USER"])) {
			@ldap_get_option($_pql->ldap_linkid, LDAP_OPT_PROTOCOL_VERSION, $version);
			if($version >= 3) {
			  // Only allow an admin to rename object (if we're using LDAP v3)
?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=modrdn&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Rename DN')?>" border="0"></a>
<?php		}
	  	}
?>
        </td>
      </tr>

<?php }

	  if(empty($USER_IS_GROUP)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Password')?></td>
        <td><?php echo $userpassword?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=userpassword&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Set password')?>" border="0"></a>
        </td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Password policy')?></td>
        <td><?php if($ppolicy_entry){echo $ppolicy_entry;}else{echo "<i>".$LANG->_('Not set')."</i>";} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $ppolicy_entry_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Firstname')?></td>
        <td><?php if(!empty($givenname)) { echo $givenname; }?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=givenname&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>&oldvalue=<?php echo $givenname?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change First name')?>" border="0"></a></td>
      </tr>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Lastname')?></td>
        <td><?php if(!empty($sn)) { echo $sn; }?></td>
        <td><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=sn&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>&oldvalue=<?php echo $sn?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change last name')?>" border="0"></a></td>
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

	  pql_format_table_single($txt, $link, $cn,
							  pql_get_define("PQL_ATTR_CN"), $class);
// }}}

	  if(!@empty($_SESSION["ADVANCED_MODE"]) and empty($USER_IS_GROUP)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Login shell')?></td>
        <td><?php if(!empty($loginshell)){echo $loginshell;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php	if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=loginshell&user=<?php echo $url["user"]?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change login shell')?>" border="0"></a>
<?php	} ?>
        </td>
      </tr>

      <!-- DLW: Why do only some of these have "oldvalue" entries? -->
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Home directory')?></td>
        <td><?php if(!empty($homedirectory)){echo $homedirectory;}else{echo $LANG->_('None');} ?></td>
        <td>
<?php	if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=homedirectory&user=<?php echo $url["user"]?>&oldvalue=<?php echo urlencode($homedirectory);?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change home directory')?>" border="0"></a>
<?php	} ?>
        </td>
      </tr>

<?php	if(!empty($uidnumber) && !empty($gidnumber)) {
			// Get the group name for $gidnumber
			$group = $_pql->get_dn($_REQUEST["rootdn"], "(&(objectClass=posixGroup)(gidNumber=$gidnumber))");
			if($group[0])
			  $group_name = $_pql->get_attribute($group[0], pql_get_define("PQL_ATTR_CN"));
			if(!$group_name)
			  $group_name = "<i>unknown</i>";
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('UID;GID')?></td>
        <td><?php echo $uidnumber . ";" . $gidnumber?> (<?php echo $group_name?>)</td>
        <td><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=uidnumber&user=<?php echo $url["user"]?>&oldvalue=<?php echo urlencode($uidnumber . ";" . $gidnumber);?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change UID/GID')?>" border="0"></a></td>
      </tr>

<?php			$class = pql_format_table(0);
			
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
				
				pql_format_table_single($txt, $link, $memberuid,
										pql_get_define("PQL_ATTR_ADDITIONAL_GROUP"),
										$class);
	  	}
	  }

	  // {{{ Disable or not to start in advanced mode (also affects the 'Advanced mode' selector in left-base.php).
?>
      <tr class="<?php echo $class?>">
        <td class="title"><?php echo $LANG->_('Disable advanced mode selector')?></td>
<?php if($disableadvancedmode) { ?>
        <td><?php echo $LANG->_('Yes')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_DISABLE_ADVANCED_MODE")?>&oldvalue=1&view=<?php echo $_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Disallow user from select Advanced mode')?>"></a>
        </td>
<?php } else { ?>
        <td><?php echo $LANG->_('No')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_DISABLE_ADVANCED_MODE")?>&oldvalue=0&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Allow user to select Advanced mode')?>"></a>
        </td>
<?php } ?>
      </tr>
<?php
// }}}

	  // {{{ When user do a login, should the advanced mode be loaded?
	  if(!pql_get_define("PQL_CONF_DISABLE_ADVANCED_MODE")) {
?>
      <tr class="<?php echo $class?>">
        <td class="title"><?php echo $LANG->_('Start in advanced mode')?></td>
<?php	if($startwithadvancedmode) { ?>
        <td><?php echo $LANG->_('Yes')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_START_ADVANCED")?>&oldvalue=1&view=<?php echo $_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Do not start in advanced mode')?>"></a>
        </td>
<?php	} else { ?>
        <td><?php echo $LANG->_('No')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&user=<?php echo $url["user"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_START_ADVANCED")?>&oldvalue=0&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Make sure we start in advanced mode')?>"></a>
        </td>
<?php	} ?>
      </tr>

      <tr class="<?php echo $class?>">
        <td class="title"><?php echo $LANG->_('Last authenticated connection')?></td>
        <td><?php echo $authtimestamp ?></td>
        <td></td>
      </tr>
<?php }
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
