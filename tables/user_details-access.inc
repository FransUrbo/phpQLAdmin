  <!-- Access list -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('User access')?></th>
<?php
    foreach($_pql->ldap_basedn as $branch) {
		// Check the top-most (namingContexts) DN's if user is admin of that...
		if(pql_validate_administrator($_pql->ldap_linkid, $branch, $user))
		  $domains[$branch][] = $branch;

		// Find all branches the user is admin of
		$dom = pql_get_domain_value($_pql, $branch, 'administrator', $user);
		if($dom)
		  foreach($dom as $d) {
			  if($d != $branch)
				$domains[$branch][] = $d;
		  }
    }
    
    $class=table_bgcolor(0);
    if(isset($domains)) {
		asort($domains);
		$new_tr = 0;
		foreach($domains as $branch => $doms) {
			foreach($doms as $branch) {
				if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php			} else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Access to DN')?>:</td>
<?php			} $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?php echo urldecode($branch); ?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&domain=<?=$branch?>&user=<?=$userdn?>&submit=4&action=delete&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php		}
		}
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&user=<?=$userdn?>&submit=3&action=add"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php   if($ALLOW_BRANCH_CREATE) {
	    $class=table_bgcolor(0);
?>

      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Create branches')?></td>
<?php       if(pql_validate_administrator($_pql->ldap_linkid, $rootdn, $user)) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&user=<?=$userdn?>&submit=4&action=delete&view=<?=$view?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Disallow user to create domains below %domain%'), array('domain', $rootdn)); ?>">
          </a>
        </td>
<?php       } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&rootdn=<?=$rootdn?>&user=<?=$userdn?>&submit=4&action=add&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Allow user to create domains below %domain%'), array('domain', $rootdn)); ?>">
          </a>
        </td>
<?php      } ?>
      </tr>

<?php   }
    } else {
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Access to DN')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=administrator&user=<?=$userdn?>&submit=3&action=add&view=<?=$view?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
    }
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrate QmailLDAP/Controls')?></td>
<?php       if($controlsadministrator) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="config_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=controlsadministrator&delval=<?=$userdn?>&submit=2&view=<?=$view?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Disallow user to administrate Controls information')?>">
          </a>
        </td>
<?php       } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="config_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=controlsadministrator&controlsadministrator=<?=$userdn?>&submit=2&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Allow user to administrate Controls information')?>">
          </a>
        </td>
<?php      } ?>
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
