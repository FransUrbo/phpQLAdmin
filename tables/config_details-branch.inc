  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Branch configuration')?>
      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP base dn')?></td>
        <td class="<?=$class?>"><b><?=$_REQUEST["branch"]?>&nbsp;</b></td>
        <td class="<?=$class?>"></td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Show users')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_SHOW_USERS", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_SHOW_USERS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_SHOW_USERS", $_REQUEST["branch"])?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Allow absolute mailbox paths')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_ALLOW_ABSOLUTE_PATH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["branch"])?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Verify user/domain deletions \i\urecomended!\U\I')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_VERIFY_DELETE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["branch"])?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Reference %what% with'), array('what' => $LANG->_('users'))); ?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["branch"])?></td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_REFERENCE_USERS_WITH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_REFERENCE_USERS_WITH")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0);  ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Reference %what% with'), array('what' => $LANG->_('domains'))); ?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["branch"])?></td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_REFERENCE_DOMAINS_WITH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH")?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Minimum UID Number to be used for shell and system accounts')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $_REQUEST["branch"])?></td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_MINIMUM_UIDNUMBER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set <?=pql_get_define("PQL_ATTR_MINIMUM_UIDNUMBER")?>"></a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Minimum GID Number to be used for shell and system accounts')?></td>
        <td class="<?=$class?>"><?=$gidnr?></td>
<?php $gidnr = pql_get_define("PQL_CONF_MINIMUM_GIDNUMBER", $_REQUEST["branch"]);
	  if(empty($gidnr)) {
		  $gidnr = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $_REQUEST["branch"]);
	  }

      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_MINIMUM_GIDNUMBER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set <?=pql_get_define("PQL_ATTR_MINIMUM_GIDNUMBER")?>"></a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title">UID <?=$LANG->_('number to be used for forwarding accounts')?></td>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $_REQUEST["branch"])?></td>
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_FORWARDINGACCOUNT_UIDNUMBER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_ATTR_FORWARDINGACCOUNT_UIDNUMBER")?>"></a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
      </tr>

      <tr></tr>
<?php if($_SESSION["ADVANCED_MODE"]) { ?>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% creation script'), array('what' => $LANG->_('user'))); ?></td>
<?php	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["branch"])) { ?>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["branch"])?></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")?>"></a>
        </td>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% creation script'), array('what' => $LANG->_('domain')));?></td>
<?php	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["branch"])) { ?>
        <td class="<?=$class?>"><?=pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["branch"])?></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN")?>"></a>
        </td>
      </tr>

      <tr></tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Automatically add hostname to domain for replication')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", $_REQUEST["branch"])) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME")?>"></a></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically replicate domains to %what%'), array('what' => $LANG->_('locals'))); ?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["branch"])) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDLOCALS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDLOCALS")?>"></a></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically replicate domains to %what%'), array('what' => $LANG->_('rcpthosts'))); ?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["branch"])) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS")?>"></a></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <tr></tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0);
		$url_tmp = "config_edit_attribute.php?rootdn=".$_REQUEST["branch"]."&attrib=".pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")."&view=".$_REQUEST["branch"];

		$schemes = pql_split_oldvalues(pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["branch"]));
		$txt = array('tit' => $LANG->_('Allowed password encryption scheme'),
					 'add' => $LANG->_('Add encryption scheme'),
					 'del' => $LANG->_('Delete encryption scheme'));
		$url = array('add' => $url_tmp, 'del' => $url_tmp."&delval=1&submit=2");
//&delval=".$sc[$i][$_REQUEST["branch"]]
		pql_format_table_single($_pql->ldap_linkid, $txt, $url, $schemes, pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"), $class);
?>

      <tr></tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0);
		$url_tmp = "config_edit_attribute.php?rootdn=".$_REQUEST["branch"]."&attrib=".pql_get_define("PQL_ATTR_OBJECTCLASS_USER")."&view=".$_REQUEST["branch"];

		$objectclasses = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $_REQUEST["branch"]));
		$txt = array('tit' => $LANG->_('Allowed user objectclasses'),
					 'add' => $LANG->_('Add objectclass'),
					 'del' => $LANG->_('Delete objectclass'));
		$url = array('add' => $url_tmp, 'del' => $url_tmp."&delval=1&submit=2");
//&delval=".$oc[$i][$_REQUEST["branch"]]
		pql_format_table_single($_pql->ldap_linkid, $txt, $url, $objectclasses, pql_get_define("PQL_ATTR_OBJECTCLASS_USER"), $class);
?>
      <tr></tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0);
		$url_tmp = "config_edit_attribute.php?rootdn=".$_REQUEST["branch"]."&attrib=".pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN")."&view=".$_REQUEST["branch"];

		$objectclasses = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $_REQUEST["branch"]));
		$txt = array('tit' => $LANG->_('Allowed domain objectclasses'),
					 'add' => $LANG->_('Add objectclass'),
					 'del' => $LANG->_('Delete objectclass'));
		$url = array('add' => $url_tmp, 'del' => $url_tmp."&delval=1&submit=2");
//&delval=".$oc[$i][$_REQUEST["branch"]]

		// BUG: DLW: I cant add phpQLAdminBranch. I can't remove the last entry.
		pql_format_table_single($_pql->ldap_linkid, $txt, $url, $objectclasses, pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"), $class);
?>

      <tr></tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0);
		$url_tmp  = "domain_edit_attributes.php?rootdn=".$_REQUEST["branch"]."&domain=".$_REQUEST["branch"];
		$url_tmp .= "&branchdn=".$_REQUEST["branch"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR");
		$url_tmp .= "&view=config_detail/".$_REQUEST["branch"];

		$admins   = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["branch"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
		if($admins and !is_array($admins)) {
			$admins = array($admins);
		}

		$txt      = array('tit' => $LANG->_('Super administrator'),
						  'add' => $LANG->_('Add administrator'),
						  'del' => $LANG->_('Delete administrator'));
		$url      = array('add' => $url_tmp."&submit=3&action=add",
						  'del' => $url_tmp."&submit=4&action=delete");
		pql_format_table_single($_pql->ldap_linkid, $txt, $url, $admins, pql_get_define("PQL_ATTR_ADMINISTRATOR"), $class);
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
