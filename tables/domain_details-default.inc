  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=PQL_LANG_DOMAIN_DEFAULT_VALUES?></th>
<?php if($ALLOW_BRANCH_CREATE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Domain/Branch DN</td>
        <td><?=$domain?></td>
        <td><?=// TODO: Should we be able to rename the RDN? ?></td>
      </tr>
  
<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_DEFAULT_NAME?></td>
        <td><?php if($defaultdomain){echo $defaultdomain;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$defaultdomain_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Additional domain name(s)</td>
<?php if(is_array($additionaldomainname)) {
		$new_tr = 0;
		foreach($additionaldomainname as $additional) {
			if($new_tr) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><?=$additional?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>=<?=$additional?>&submit=3&action=modify&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify <?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?> for <?=$domain?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&delval=<?=$additional?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="Delete additional domain name" border="0"></a>
        </td>
<?php		$new_tr = 1;
		} // if new_tr
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>">Add additional domain name</a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?=lc($PQL_ATTRIBUTE["PQL_CONF_ADDITIONAL_DOMAIN_NAME"])?>&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>">Add additional domain name</a>
        </td>
<?php } ?>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_DEFAULT_HOMEDIR?></td>
        <td><?php if($basehomedir){echo $basehomedir;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$basehomedir_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_DEFAULT_MAILDIR?></td>
        <td><?php if($basemaildir){echo $basemaildir;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$basemaildir_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_MAILQUOTA_TITLE?></td>
        <td><?php if($basequota){echo $basequota;}else{echo PQL_LANG_UNSET;} ?></td>
        <td><?=$basequota_link?></td>
      </tr>

<?php if($ALLOW_BRANCH_CREATE and $ADVANCED_MODE) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Maximum allowed users in branch</td>
        <td><?php if($maximumdomainusers){echo $maximumdomainusers;}else{echo "unlimited";} ?></td>
        <td><?=$maximumdomainusers_link?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default password scheme</td>
        <td><?php if($defaultpasswordscheme){echo $defaultpasswordscheme;}else{echo "no default";} ?></td>
        <td><?=$defaultpasswordscheme_link?></td>
      </tr>

<?php } ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=PQL_LANG_DOMAIN_ADMIN_TITLE?></td>
<?php if(is_array($admins)) {
		$new_tr = 0;
		foreach($admins as $admin) {
			$username = pql_get_userattribute($_pql->ldap_linkid, $admin, 'cn');
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
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&administrator=<?=$admin?>&submit=3&action=modify&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Modify administrators for <?=$domain?>"></a>
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&administrator=<?=$admin?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" alt="<?=PQL_LANG_DOMAIN_ADMIN_DELETE?>" border="0"></a>
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
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?=PQL_LANG_DOMAIN_ADMIN_ADD?></a>
        </td>
      </tr>
<?php
	} else {
?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&domain=<?=$domain?>&submit=3&action=add&view=<?=$view?>"><?=PQL_LANG_DOMAIN_ADMIN_ADD?></a>
        </td>
<?php
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
