  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Default domain values')?>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Branch DN')?></td>
        <td><?php echo pql_maybe_decode(urldecode($url["domain"])); ?></td>
        <td><?=// TODO: Should we be able to rename the RDN? ?></td>
      </tr>
  
<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Default domain name')?></td>
        <td><?php if($defaultdomain){echo pql_maybe_idna_decode($defaultdomain);}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?=$defaultdomain_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional domain name')?></td>
<?php if(is_array($additionaldomainname)) {
		$new_tr = 0;
		foreach($additionaldomainname as $additional) {
			$additional = pql_maybe_idna_decode($additional);

			if($new_tr) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><?=$additional?></td>
<?php		if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&<?=pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>=<?=$additional?>&submit=3&action=modify&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify %attribute% for %what%'), array('attribute' => pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"), 'what' => $domainname)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&delval=<?=$additional?>&submit=4&action=delete&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete additional domain name')?>" border="0"></a>
        </td>
<?php		} else { ?>
        <td></td>
<?php		}

			$new_tr = 1;
		} // if new_tr
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&submit=3&action=add&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&submit=3&action=add&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Base home directory for users')?></td>
        <td><?php if($basehomedir){echo $basehomedir;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?=$basehomedir_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Base mail directory for users')?></td>
        <td><?php if($basemaildir){echo $basemaildir;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?=$basemaildir_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Default quota')?></td>
        <td><?php if($basequota){echo $basequota;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?=$basequota_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Maximum allowed users in branch')?></td>
        <td><?php if($maximumdomainusers){echo $maximumdomainusers;}else{echo $LANG->_('Unlimited');} ?></td>
        <td><?=$maximumdomainusers_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Maximum allowed mailinglists in branch')?></td>
        <td><?php if($maximummailinglists){echo $maximummailinglists;}else{echo $LANG->_('Unlimited');} ?></td>
        <td><?=$maximummailinglists_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Default password scheme')?></td>
        <td><?php if($defaultpasswordscheme){echo $defaultpasswordscheme;}else{echo $LANG->_('No default');} ?></td>
        <td><?=$defaultpasswordscheme_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('username'))); ?></td>
        <td><?php if($autocreateusername){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$autocreateusername_link?></td>
      </tr>

<?php	if($autocreateusername) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Username prefix')?></td>
        <td><?php if($usernameprefix){echo $usernameprefix;}else{echo "<i>".$LANG->_('Not set')."</i>";} ?></td>
        <td><?=$usernameprefix_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Length of user suffix')?></td>
        <td><?php if($usernameprefixlength){echo $usernameprefixlength;}else{echo "4 (<i>".$LANG->_('Default')."</i>)";} ?></td>
        <td><?=$usernameprefixlength_link?></td>
      </tr>

<?php	} ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('email address'))); ?></td>
        <td><?php if($autocreatemailaddress){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$autocreatemailaddress_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('password'))); ?></td>
        <td><?php if($autocreatepassword){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$autocreatepassword_link?></td>
      </tr>

<?php }

	  if(pql_get_define("PQL_CONF_EZMLM_USE")) {
?>
      <!-- -------------------- --!>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('EZMLM Virtual User')?></td>
        <td><?php if($ezmlmvirtualuser) { echo $ezmlmvirtualuser; } else { echo "<i>Not set</i>"; }?>&nbsp;</td>
        <td><?=$ezmlmvirtualuser_link?></td>
      </tr>

<?php }

	  // Domain administrators
	  $class = pql_format_table(0);
	  $url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"];
	  $url_tmp .= "&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR");
	  
	  $tbl_txt = array('tit' => $LANG->_('Domain administrators'),
					   'add' => $LANG->_('Add domain administrator for domain '),
					   'del' => $LANG->_('Remove domain administrator'));
	  $tbl_url = array('add' => $url_tmp."&submit=3&action=add",
					   'del' => $url_tmp."&submit=4&action=delete");
	  pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $domain_admins,
							  pql_get_define("PQL_ATTR_ADMINISTRATOR"), $class);

	  // Mailinglist administrators
	  $class = pql_format_table(0);
	  $url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"];
	  $url_tmp .= "&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM");
	  
	  $tbl_txt = array('tit' => $LANG->_('Mailinglists administrator'),
					   'add' => $LANG->_('Add list administrator for domain '),
					   'del' => $LANG->_('Remove list administrator'));
	  $tbl_url = array('add' => $url_tmp."&submit=3&action=add",
					   'del' => $url_tmp."&submit=4&action=delete");
	  pql_format_table_single($_pql->ldap_linkid, $tbl_txt, $tbl_url, $mailinglist_admins,
							  pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $class);
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
