  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Default domain values')?>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Branch DN')?></td>
        <td><?php echo $_REQUEST["domain"]?></td>
        <!-- TODO: Should we be able to rename the RDN? -->
        <td></td>
      </tr>
  
<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Default domain name')?></td>
        <td><?php if($defaultdomain){echo pql_maybe_idna_decode($defaultdomain);}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $defaultdomain_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Additional domain name')?></td>
<?php if($additionaldomainname) {
		if(!is_array($additionaldomainname)) {
		  $additionaldomainname = array($additionaldomainname);
		}

		$new_tr = 0;
		foreach($additionaldomainname as $additional) {
			$additional = pql_maybe_idna_decode($additional);

			if($new_tr) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php		} ?>
        <td><?php echo $additional?></td>
<?php		if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&<?php echo pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>=<?php echo $additional?>&submit=3&action=modify&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Modify %attribute% for %what%'), array('attribute' => pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"), 'what' => $domainname)); ?>"></a>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&delval=<?php echo $additional?>&submit=4&action=delete&view=<?php echo $_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Delete additional domain name')?>" border="0"></a>
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
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&submit=3&action=add&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
      </tr>
<?php } else { ?>
        <td colspan="4">
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")?>&rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&submit=3&action=add&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain name'))); ?></a>
        </td>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Base home directory for users')?></td>
        <td><?php if($basehomedir){echo $basehomedir;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $basehomedir_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Base mail directory for users')?></td>
        <td><?php if($basemaildir){echo $basemaildir;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $basemaildir_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Minimum UID Number to be used for shell and system accounts')?></td>
        <td><?php if($minimumuidnumber){echo $minimumuidnumber;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $minimumuidnumber_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Minimum GID Number to be used for shell and system accounts')?></td>
        <td><?php if($minimumgidnumber){echo $minimumgidnumber;}else{echo $LANG->_('Not set');} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $minimumgidnumber_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Maximum allowed users in branch')?></td>
        <td><?php if($maximumdomainusers){echo $maximumdomainusers;}else{echo $LANG->_('Unlimited');} ?></td>
        <td><?php echo $maximumdomainusers_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Maximum allowed mailinglists in branch')?></td>
        <td><?php if($maximummailinglists){echo $maximummailinglists;}else{echo $LANG->_('Unlimited');} ?></td>
        <td><?php echo $maximummailinglists_link?></td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Default quota')?></td>
        <td><?php if($basequota){echo $basequota;}else{echo "<i>".$LANG->_('Not set')."</i>";} ?></td>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
        <td><?php echo $basequota_link?></td>
<?php } else { ?>
        <td></td>
<?php } ?>
      </tr>

<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Default password scheme')?></td>
        <td><?php if($defaultpasswordscheme){echo $defaultpasswordscheme;}else{echo $LANG->_('No default');} ?></td>
        <td><?php echo $defaultpasswordscheme_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Default account template')?></td>
        <td><?php if($defaultaccounttype) {
		  // Retreive the short name of the account type (template name)
		  require($_SESSION["path"]."/include/pql_templates.inc");
		  $template = pql_get_template($defaultaccounttype);
		  echo $template["short"];}else{echo $LANG->_('No default');} ?></td>
        <td><?php echo $defaultaccounttype_link?></td>
      </tr>

<?php	if($defaultaccounttype) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Lock account type')?></td>
        <td><?php if($lockaccounttype){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $lockaccounttype_link?></td>
      </tr>

<?php	} ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('username'))); ?></td>
        <td><?php if($autocreateusername){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $autocreateusername_link?></td>
      </tr>

<?php	if($autocreateusername) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Lock username field')?></td>
        <td><?php if($lockusername){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $lockusername_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Username prefix')?></td>
        <td><?php if($usernameprefix){echo $usernameprefix;}else{echo "<i>".$LANG->_('Not set')."</i>";} ?></td>
        <td><?php echo $usernameprefix_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Length of user suffix')?></td>
        <td><?php if($usernameprefixlength){echo $usernameprefixlength;}else{echo "4 (<i>".$LANG->_('Default')."</i>)";} ?></td>
        <td><?php echo $usernameprefixlength_link?></td>
      </tr>

<?php	} ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('email address'))); ?></td>
        <td><?php if($autocreatemailaddress){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $autocreatemailaddress_link?></td>
      </tr>

<?php   if($autocreatemailaddress) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Lock email address field')?></td>
        <td><?php if($lockemailaddress){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $lockemailaddress_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Lock domain address')?></td>
        <td><?php if($lockdomainaddress){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $lockdomainaddress_link?></td>
      </tr>

<?php	} ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically generate %what%'), array('what' => $LANG->_('password'))); ?></td>
        <td><?php if($autocreatepassword){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $autocreatepassword_link?></td>
      </tr>

<?php   if($autocreatepassword) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">&nbsp;&nbsp;&nbsp;<?php echo $LANG->_('Lock password field')?></td>
        <td><?php if($lockpassword){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $lockpassword_link?></td>
      </tr>

<?php	} ?>
<?php }

	  if(pql_get_define("PQL_CONF_EZMLM_USE")) {
?>
      <!-- -------------------- --!>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('EZMLM Virtual User')?></td>
        <td><?php if($ezmlmvirtualuser) { echo $ezmlmvirtualuser; } else { echo "<i>Not set</i>"; }?>&nbsp;</td>
        <td><?php echo $ezmlmvirtualuser_link?></td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Start In My Account')?></td>
        <td><?php if($startinmyaccount){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?php echo $startinmyaccount_link?></td>
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
