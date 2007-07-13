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
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_SHOW_USERS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_SHOW_USERS", $_REQUEST["branch"])?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Allow absolute mailbox paths')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_ALLOW_ABSOLUTE_PATH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["branch"])?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Allow \uall\U characters in passwords')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_ALLOW_ALL_CHARS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["branch"])?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Verify deletions \i\urecomended!\U\I')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_VERIFY_DELETE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["branch"])?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Reference %what% with'), array('what' => $LANG->_('users'))); ?></td>
        <td class="<?=$class?>">
<?php if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["branch"])?>
<?php } ?>
        </td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_REFERENCE_USERS_WITH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_REFERENCE_USERS_WITH")?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('LDAP filter to be used to find users')?></td>
        <td class="<?=$class?>">
<?php if(pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["branch"])?>
<?php } ?>
        </td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_USER_FILTER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_ATTR_USER_FILTER")?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0);  ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Reference %what% with'), array('what' => $LANG->_('domains'))); ?></td>
        <td class="<?=$class?>">
<?php if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["branch"])?>
<?php } ?>
        </td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_REFERENCE_DOMAINS_WITH")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH")?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Minimum UID Number to be used for shell and system accounts')?></td>
        <td class="<?=$class?>">
<?php if(pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $_REQUEST["branch"])?>
<?php } ?>
        </td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_MINIMUM_UIDNUMBER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set <?=pql_get_define("PQL_ATTR_MINIMUM_UIDNUMBER")?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Minimum GID Number to be used for shell and system accounts')?></td>
<?php $gidnr = pql_get_define("PQL_CONF_MINIMUM_GIDNUMBER", $_REQUEST["branch"]);
	  if(empty($gidnr)) {
		  $gidnr = pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $_REQUEST["branch"]);
	  }
?>
        <td class="<?=$class?>"><?=$gidnr?></td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_MINIMUM_GIDNUMBER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set <?=pql_get_define("PQL_ATTR_MINIMUM_GIDNUMBER")?>"></a>
<?php } ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title">UID <?=$LANG->_('number to be used for forwarding accounts')?></td>
        <td class="<?=$class?>">
<?php if(pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $_REQUEST["branch"])?>
<?php } ?>
        </td>
        <td class="<?=$class?>">
<?php if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_FORWARDINGACCOUNT_UIDNUMBER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_ATTR_FORWARDINGACCOUNT_UIDNUMBER")?>"></a>
<?php } ?>
        </td>
      </tr>
<?php if(pql_get_define("PQL_CONF_RADIUS_USE")) { ?>

      <!-- -------------------- -->
      <tr>
        <td class="title"><?=$LANG->_('RADIUS configuration identity (DN)')?></td>
        <td class="<?=$class?>">
<?php if(pql_get_define("PQL_CONF_RADIUS_CONFIG_IDENTITY", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_RADIUS_CONFIG_IDENTITY", $_REQUEST["branch"])?>
<?php } ?>
        </td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")?>"></a></td>
      </tr>
<?php }

	  pql_format_table_empty(2);

	  if($_SESSION["ADVANCED_MODE"]) {
	    if(pql_get_define("PQL_CONF_SAMBA_USE")) {
?>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('SMB Security ID')?></td>
        <td class="<?=$class?>">
<?php	  if(pql_get_define("PQL_CONF_SAMBASID", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_SAMBASID", $_REQUEST["branch"])?>
<?php	  } ?>
        </td>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SAMBASID")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SAMBASID")?>"></a>
        </td>
      </tr>

<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Days until expiring/requesting password change')?></td>
        <td class="<?=$class?>">
<?php	  if(pql_get_define("PQL_CONF_DAYSUNTILEXPIRE", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_DAYSUNTILEXPIRE", $_REQUEST["branch"])?>
<?php	  } ?>
        </td>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_DAYSUNTILEXPIRE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_DAYSUNTILEXPIRE")?>"></a>
        </td>
      </tr>

<?php	  pql_format_table_empty(2);
		}
?>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% creation script'), array('what' => $LANG->_('user'))); ?></td>
        <td class="<?=$class?>">
<?php	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["branch"])?>
<?php	} ?>
        </td>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")?>"></a>
        </td>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% creation script'), array('what' => $LANG->_('domain')));?></td>
        <td class="<?=$class?>">
<?php	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["branch"])?>
<?php	} ?>
        </td>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN")?>"></a>
        </td>
      </tr>


      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% removal script'), array('what' => $LANG->_('user'))); ?></td>
        <td class="<?=$class?>">
<?php	if(pql_get_define("PQL_CONF_SCRIPT_DELETE_USER", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_SCRIPT_DELETE_USER", $_REQUEST["branch"])?>
<?php	} ?>
        </td>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SCRIPT_DELETE_USER")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SCRIPT_DELETE_USER")?>"></a>
        </td>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% removal script'), array('what' => $LANG->_('domain')));?></td>
        <td class="<?=$class?>">
<?php	if(pql_get_define("PQL_CONF_SCRIPT_DELETE_DOMAIN", $_REQUEST["branch"])) { ?>
          <?=pql_get_define("PQL_CONF_SCRIPT_DELETE_DOMAIN", $_REQUEST["branch"])?>
<?php	} ?>
        </td>
<?php	$new_tr = 0; ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=pql_get_define("PQL_ATTR_SCRIPT_DELETE_DOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=pql_get_define("PQL_ATTR_SCRIPT_DELETE_DOMAIN")?>"></a>
        </td>
      </tr>

<?=pql_format_table_empty(2)?>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Automatically add local hostname to domain for replication')?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_AUTOADDHOSTNAME", $_REQUEST["branch"])) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>">
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDHOSTNAME")?>"></a>
<?php	} ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically replicate domains to %what%'), array('what' => $LANG->_('locals'))); ?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["branch"])) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>">
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDLOCALS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDLOCALS")?>"></a>
<?php	} ?>
        </td>
      </tr>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Automatically replicate domains to %what%'), array('what' => $LANG->_('rcpthosts'))); ?></td>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_CONTROL_AUTOADDRCPTHOSTS", $_REQUEST["branch"])) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
        <td class="<?=$class?>">
<?php	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=pql_get_define("PQL_ATTR_CONTROL_AUTOADDRCPTHOSTS")?>"></a>
<?php	} ?>
        </td>
      </tr>

<?=pql_format_table_empty(2)?>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0);
		$url_tmp = "config_edit_attribute.php?rootdn=".$_REQUEST["branch"]."&attrib=".pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN")."&view=".$_REQUEST["branch"];

		$objectclasses = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $_REQUEST["branch"]));
		$txt = array('tit' => $LANG->_('Allowed domain objectclasses'),
					 'add' => $LANG->_('Add objectclass'),
					 'del' => $LANG->_('Delete objectclass'));
		$url = array('add' => $url_tmp, 'del' => $url_tmp."&action=del");
//&delval=".$oc[$i][$_REQUEST["branch"]]

		pql_format_table_single($txt, $url, $objectclasses, pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"), $class);
?>

      <!-- -------------------- -->
<?php	$class=pql_format_table(0);
		$url_tmp  = "domain_edit_attributes.php?rootdn=".$_REQUEST["branch"]."&domain=".$_REQUEST["branch"];
		$url_tmp .= "&branchdn=".$_REQUEST["branch"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR");
		$url_tmp .= "&view=config_detail/".$_REQUEST["branch"];

		$admins   = $_pql->get_attribute($_REQUEST["branch"], pql_get_define("PQL_ATTR_ADMINISTRATOR"));
		if($admins and !is_array($admins)) {
			$admins = array($admins);
		}

		$txt      = array('tit' => $LANG->_('Super administrator'),
						  'add' => $LANG->_('Add administrator'),
						  'del' => $LANG->_('Delete administrator'));
		$url      = array('add' => $url_tmp."&submit=3&action=add",
						  'del' => $url_tmp."&submit=4&action=delete");
		pql_format_table_single($txt, $url, $admins, pql_get_define("PQL_ATTR_ADMINISTRATOR"), $class);
      }
?>

      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3">
          <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
          <table>
            <tr>
              <td>
                <?=$LANG->_('You can either define an attribute to use to find users or specify your own search filter.')?><br>
                <?=$LANG->_('If using an attribute, the search filter will be generates as \b\iattribute\I=*\B to find all users.')?><br>
                <?=$LANG->_('If setting a search filter, this will take precedence, but you will still need to set a user reference attribute. This will be used else were in phpQLAdmin.')?>
              </td>
            </tr>
          </table>
        </td>
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
