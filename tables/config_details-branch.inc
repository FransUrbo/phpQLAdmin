  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Branch configuration')?>
      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('LDAP base dn')?></td>
<?php $class=pql_format_table(0); ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><b><?=$_REQUEST["branch"]?>&nbsp;</b></td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('Show users')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?> <?=$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_SHOW_USERS", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('Allow absolute mailbox paths')?></td>
<?php $class=pql_format_table(0);
       if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?> <?=$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('Verify user/domain deletions \i\urecomended!\U\I')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?> <?=$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if(pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["branch"])) {echo $LANG->_('Yes');}else{echo $LANG->_('No');}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Reference %what% with'), array('what' => $LANG->_('users'))); ?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?> <?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["branch"]); ?></td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('Reference %what% with'), array('what' => $LANG->_('domains'))); ?></td>
<?php $class=pql_format_table(0); 
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?> <?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["branch"]); ?></td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('Minimum UID/GID Number to be used for shell and system accounts')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_MINIMUM_UIDNUMBER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Set <?=$PQL_ATTRIBUTE["PQL_CONF_MINIMUM_UIDNUMBER"]?>">
          </a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_MINIMUM_UIDNUMBER", $_REQUEST["branch"]); ?></td>
      </tr>

      <tr>
        <td class="title">UID <?=$LANG->_('number to be used for forwarding accounts')?></td>
<?php $class=pql_format_table(0);
      if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?> <?=$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"]?>">
          </a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER", $_REQUEST["branch"]); ?></td>
      </tr>

      <tr></tr>
<?php if($_SESSION["ADVANCED_MODE"]) { ?>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% creation script'), array('what' => $LANG->_('user'))); ?></td>
<?php	$class=pql_format_table(0);
		$new_tr = 0;
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"]?>">
          </a>
        </td>
<?php	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["branch"])) { ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_SCRIPT_CREATE_USER", $_REQUEST["branch"]); ?></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?php echo pql_complete_constant($LANG->_('External %what% creation script'), array('what' => $LANG->_('domain')));?></td>
<?php	$class=pql_format_table(0);
		$new_tr = 0;
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"]?>">
          </a>
        </td>
<?php	if(pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["branch"])) { ?>
        <td class="<?=$class?>"><?php echo pql_get_define("PQL_CONF_SCRIPT_CREATE_DOMAIN", $_REQUEST["branch"]); ?></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
		<!-- DLW: This doesnt work in the original either. -->
        <td class="title"><?=$LANG->_('Password encryption scheme')?></td>
<?php     $class=pql_format_table(0); $new_tr = 0;

          // Convert the array to a index
		  $schemes = pql_split_oldvalues(pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["branch"]));
	      
		  $i = 0;
		  foreach($schemes as $key) {
			  if($key) {
				  $sc[$i][$_REQUEST["branch"]] = $key;
				  $i++;
			  }
		  }
	      
		  if($max < $i-1)
			$max = $i-1;

          if(is_array($sc)) {
			  for($i=0; $i <= $max; $i++) {
				  if($new_tr) {
?>

      <tr>
        <td class="title"></td>
<?php             }
				  $new_tr = 1;
	      
				  if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"] and $sc[$i][$_REQUEST["branch"]]) {
?>

        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>&delval=<?=$sc[$i][$_REQUEST["branch"]]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete attribute')?> <?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>=<?=$sc[$i][$_REQUEST["branch"]]?>">
          </a>
        </td>
<?php			  } else { ?>
        <td class="<?=$class?>"></td>
<?php			  } ?>
        <td class="<?=$class?>"><?=$sc[$i][$_REQUEST["branch"]]?>&nbsp;</td>
<?php		  } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php		  if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('password encryption scheme'))); ?></a></td>
<?php		  } else { ?>
        <td class="<?=$class?>"></td>
<?php		  } ?>
      </tr>
<?php   } ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title"><?=$LANG->_('User objectclasses')?></td>
<?php     $new_tr = 0; $max = 0;

          // Convert the array to a index
		  $objectclasses = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $_REQUEST["branch"]));
			  
		  $i = 0;
		  foreach($objectclasses as $key) {
			  if($key) {
				  $oc[$i][$_REQUEST["branch"]] = $key;
				  $i++;
			  }
		  }
			  
		  if($max < $i-1)
			$max = $i-1;

		  if(is_array($oc)) {
			  for($i=0; $i <= $max; $i++) {
				  if($new_tr) {
?>
      <tr>
        <td class="title"></td>
<?php             }
				  $new_tr = 1;

				  if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"] and $oc[$i][$_REQUEST["branch"]]) {
?>

        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>&delval=<?=$oc[$i][$_REQUEST["branch"]]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>=<?=$oc[$i][$_REQUEST["branch"]]?>">
          </a>
        </td>
<?php             } else { ?>
        <td class="<?=$class?>"></td>
<?php             } ?>
        <td class="<?=$class?>"><?=$oc[$i][$_REQUEST["branch"]]?>&nbsp;</td>
      </tr>
<?php         } // for() ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('user objectClass'))); ?></a>
        </td>
<?php	  } ?>
      </tr>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php         if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) { ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('user objectClass'))); ?></a>
        </td>
<?php         } else { ?>
        <td class="<?=$class?>"></td>
<?php         } ?>
      </tr>
<?php	  } ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
		<!-- DLW: I cant add phpQLAdminBranch.  I cant remove the last entry. -->
        <td class="title"><?=$LANG->_('Domain objectclasses')?></td>
<?php     $new_tr = 0; $class=pql_format_table(0); unset($oc); $max = 0;

          // Convert the array to a index
		  unset($o); unset($objectclasses);
			  
		  $objectclasses = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $_REQUEST["branch"]));
			  
		  $i = 0;
		  foreach($objectclasses as $key) {
			  if($key) {
				  $oc[$i][$_REQUEST["branch"]] = $key;
				  $i++;
			  }
		  }
	      
		  if($max < $i-1)
			$max = $i-1;

          if(is_array($oc)) {
			  for($i=0; $i <= $max; $i++) {
				  if($new_tr) {
?>
      <tr>
        <td class="title"></td>
<?php             }
	          $new_tr = 1;

			  if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"] and $oc[$i][$_REQUEST["branch"]]) {
?>
        <td class="<?=$class?>">
           <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>&delval=<?=$oc[$i][$_REQUEST["branch"]]?>">
               <img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Delete attribute')?> <?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>=<?=$oc[$i][$_REQUEST["branch"]]?>">
           </a>
        </td>
<?php		  } else { ?>
        <td class="<?=$class?>"></td>
<?php         } ?>
        <td class="<?=$class?>"><?=$oc[$i][$_REQUEST["branch"]]?>&nbsp;</td>
<?php	      } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     	  if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$_REQUEST["branch"]?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain objectClass'))); ?></a>
        </td>
<?php	  	  }
		  }
	   }
?>
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
