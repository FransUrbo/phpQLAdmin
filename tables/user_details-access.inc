  <!-- Access list -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('User access')?></th>
<?php
    foreach($_pql->ldap_basedn as $branch) {
		$branch = urldecode($branch);

		// Check the top-most (namingContexts) DN's if user is admin of that...
		if(pql_validate_administrator($_pql->ldap_linkid, $branch, $_GET["user"]))
		  $domains_admins[$branch][] = $branch;

		// Find all branches the user is admin of
		$dom = pql_domain_get_value($_pql, $branch, pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"), $_GET["user"]);
		if($dom) {
			foreach($dom as $d) {
				// Filter out the RootDN
				if($d != $branch)
				  $domains_admins[$branch][] = $d;
			}
		}

		// Find all branches the user is mailinglist admin of
		$dom = pql_domain_get_value($_pql, $branch, pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR"), $_GET["user"]);
		if($dom) {
			foreach($dom as $d)
			  $domains_ezmlm[$branch][] = $d;
		}
    }


	// ---------------------------------------
	// Go through and show all the domains that the user is administrator of
    $class=pql_format_table(0);
    if(isset($domains_admins)) {
		asort($domains_admins);

		$new_tr = 0;
		foreach($domains_admins as $branch => $doms) {
			foreach($doms as $branch) {
				$branch = urlencode($branch);

				if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php			} else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrator in branch')?>:</td>
<?php			} $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?=pql_maybe_decode(urldecode($branch))?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$branch?>&domain=<?=$branch?>&branchdn=<?=$branch?>&administrator=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php		}
		}
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>

<?php
	} else {
?>
      <tr class="<?=$class?>">
		 <!-- DLW: This isnt working, but its also broke in the CVS so Ill fix it later. -->
        <td class="title"><?=$LANG->_('Administrator in branch')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
    }
?>

<?php
	// ---------------------------------------
	// Go through and show all the domains that the user is MAILINGLIST administrator of
	$class=pql_format_table(0);
	if(isset($domains_ezmlm)) {
		asort($domains_ezmlm);

		$new_tr = 0;
		foreach($domains_ezmlm as $branch => $doms) {
			foreach($doms as $branch) {
				$branch = urlencode($branch);

				if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php			} else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Mailinglist administrator in branch')?>:</td>
<?php			} $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?=pql_maybe_decode(urldecode($branch))?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$branch?>&domain=<?=$branch?>&branchdn=<?=$branch?>&administrator=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php		}
		}
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
	} else {
?>
      <tr class="<?=$class?>">
		 <!-- DLW: This isnt working, but its also broke in the CVS so Ill fix it later. -->
        <td class="title"><?=$LANG->_('Mailinglist administrator in branch')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR");?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
	}

	// ---------------------------------------
	// Are the user allowed to create branches (i.e. is the user listed in the Root DN)?
	if($_SESSION["ALLOW_BRANCH_CREATE"]) {
		$class=pql_format_table(0);
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Create branches')?></td>
<?php	if(pql_validate_administrator($_pql->ldap_linkid, $_GET["rootdn"], $_GET["user"])) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$url["rootdn"]?>&user=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Disallow user to create domains below %domain%'), array('domain', $_GET["rootdn"])); ?>">
          </a>
        </td>
<?php   } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>&rootdn=<?=$url["rootdn"]?>&user=<?=$url["user"]?>&submit=4&action=add&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Allow user to create domains below %domain%'), array('domain', $_GET["rootdn"])); ?>">
          </a>
        </td>
<?php   } ?>
      </tr>

<?php
	}
?>

<?php
	// ---------------------------------------
	// Are the user allowed to administrate QmailLDAP/Controls object(s)?
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrate QmailLDAP/Controls')?></td>
<?php       if($controlsadministrator) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="config_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR");?>&delval=<?=$url["user"]?>&submit=2&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Disallow user to administrate Controls information')?>">
          </a>
        </td>
<?php       } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="config_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?php echo pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR");?>&<?php echo pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR");?>=<?=$url["user"]?>&submit=2&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Allow user to administrate Controls information')?>">
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
