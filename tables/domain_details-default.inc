  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Default domain values')?></th>
<?php if($ALLOW_BRANCH_CREATE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Branch DN')?></td>
        <td><?php echo maybe_decode(urldecode($domain)); ?></td>
        <td><?=// TODO: Should we be able to rename the RDN? ?></td>
      </tr>
  
<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Default domain name')?></td>
        <td><?php if($defaultdomain){echo $defaultdomain;}else{echo $LANG->_('Not set');} ?></td>
<?php if($ALLOW_BRANCH_CREATE) { ?>
        <td><?=$defaultdomain_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Additional domain name')?></td>
<?php if(is_array($additionaldomainname)) {
		$new_tr = 0;
		foreach($additionaldomainname as $additional) {
			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><?=$additional?></td>
<?php		if($ALLOW_BRANCH_CREATE) { ?>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>=<?=$additional?>&submit=3&action=modify&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify %attribute% for %what%'), array('attribute' => lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"]), 'what' => $domainname)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&delval=<?=$additional?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete additional domain name')?>" border="0"></a>
        </td>
<?php		} else { ?>
        <td></td>
<?php		}

			$new_tr = 1;
		} // if new_tr
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
<?php } ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Default homedirectory for users')?></td>
        <td><?php if($basehomedir){echo $basehomedir;}else{echo $LANG->_('Not set');} ?></td>
<?php if($ALLOW_BRANCH_CREATE) { ?>
        <td><?=$basehomedir_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Default maildirectory for users')?></td>
        <td><?php if($basemaildir){echo $basemaildir;}else{echo $LANG->_('Not set');} ?></td>
<?php if($ALLOW_BRANCH_CREATE) { ?>
        <td><?=$basemaildir_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Default quota')?></td>
        <td><?php if($basequota){echo $basequota;}else{echo $LANG->_('Not set');} ?></td>
<?php if($ALLOW_BRANCH_CREATE) { ?>
        <td><?=$basequota_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

<?php if($ALLOW_BRANCH_CREATE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Maximum allowed users in branch')?></td>
        <td><?php if($maximumdomainusers){echo $maximumdomainusers;}else{echo $LANG->_('Unlimited');} ?></td>
        <td><?=$maximumdomainusers_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Maximum allowed mailinglists in branch')?></td>
        <td><?php if($maximummailinglists){echo $maximummailinglists;}else{echo $LANG->_('Unlimited');} ?></td>
        <td><?=$maximummailinglists_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Default password scheme')?></td>
        <td><?php if($defaultpasswordscheme){echo $defaultpasswordscheme;}else{echo $LANG->_('No default');} ?></td>
        <td><?=$defaultpasswordscheme_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('username'))); ?></td>
        <td><?php if($autocreateusername){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$autocreateusername_link?></td>
      </tr>

<?php if($autocreateusername) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('username prefix'))); ?></td>
        <td><?php if($usernameprefix){echo $usernameprefix;}else{echo "<i>".$LANG->_('Not set')."</i>";} ?></td>
        <td><?=$usernameprefix_link?></td>
      </tr>

<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('email address'))); ?></td>
        <td><?php if($autocreatemailaddress){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$autocreatemailaddress_link?></td>
      </tr>

<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Domain administrator')?></td>
<?php if(is_array($domain_admins)) {
		$new_tr = 0;
		foreach($domain_admins as $admin) {
			$admin = maybe_encode($admin); // Re-encode value that we got earlier from pql_get_domain_value().

			$username = pql_get_userattribute($_pql->ldap_linkid, $admin, pql_get_define("PQL_GLOB_ATTR_CN"));
			if(!$username[0])
			  $username = $admin;
			else
			  $username = $username[0];

			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$admin?>&view=<?=$view?>"><?=urldecode($username)?></a></td>
<?php		if(pql_validate_administrator($_pql->ldap_linkid, $_pql->ldap_basedn[0], $admin) and
			   !$ALLOW_GLOBAL_CONFIG_SAVE) {
				// This is a super-admin, and we (user logged in) isn't super-admin.
				// Don't allow to remove this admin
?>
        <td></td>
<?php		} else { ?>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>=<?=$admin?>&submit=3&action=modify&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify administrators for %domainname%'), array('domainname' => $domainname)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>=<?=$admin?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Remove domain administrator')?>" border="0"></a>
        </td>
<?php		} ?>
      </tr>

<?php
			$new_tr = 1;
		}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain administrator for domain'))); ?></a>
        </td>
      </tr>
<?php
	} else {
?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain administrator for domain'))); ?></a>
        </td>
<?php
	}
?>

<?php if(pql_get_define("maximummailinglists") >= 0) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Mailinglist administrators')?></td>
<?php	if(is_array($mailinglist_admins)) {
			$new_tr = 0;
			foreach($mailinglist_admins as $admin) {
				$username = pql_get_userattribute($_pql->ldap_linkid, $admin, pql_get_define("PQL_GLOB_ATTR_CN"));
				if(!$username[0])
				  $username = $admin;
				else
				  $username = $username[0];
				
				if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php			} ?>
        <td><a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$admin?>&view=<?=$view?>"><?=urldecode($username)?></a></td>
<?php			if(pql_validate_administrator($_pql->ldap_linkid, $_pql->ldap_basedn[0], $admin) and
				   !$ALLOW_GLOBAL_CONFIG_SAVE) {
					// This is a super-admin, and we (user logged in) isn't super-admin.
					// Don't allow to remove this admin
?>
        <td></td>
<?php			} else { ?>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>=<?=$admin?>&submit=3&action=modify&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify administrators for %domainname%'), array('domainname' => $domainname)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>=<?=$admin?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Remove domain administrator')?>" border="0"></a>
        </td>
<?php			} ?>
      </tr>

<?php
				$new_tr = 1;
			}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('mailinglist administrator for domain'))); ?></a>
        </td>
      </tr>
<?php
		} else {
?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('mailinglist administrator for domain'))); ?></a>
        </td>
<?php	}
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
