  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Branch configuration
      <!-- -------------------- --!>
      <tr>
        <td class="title">LDAP base dn</td>
<?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><b><?=$branch?>&nbsp;</b></td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Show users</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$branch?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if($config["PQL_CONF_SHOW_USERS"][$branch]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Allow absolute mailbox paths</td>
<?php $class=table_bgcolor(0);
       if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$branch?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if($config["PQL_CONF_ALLOW_ABSOLUTE_PATH"][$branch]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Verify user/domain deletions etc <b>[recomended!]</b></td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$branch?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if($config["PQL_CONF_VERIFY_DELETE"][$branch]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Reference users with</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_REFERENCE_USERS_WITH"][$branch]?></td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Reference domains with</td>
<?php $class=table_bgcolor(0); 
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_REFERENCE_DOMAINS_WITH"][$branch]?></td>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Minimum UID/GID Number to be used for shell and system accounts</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_MINIMUM_UIDNUMBER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Set <?=$PQL_ATTRIBUTE["PQL_CONF_MINIMUM_UIDNUMBER"]?>">
          </a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_MINIMUM_UIDNUMBER"][$branch]?></td>
      </tr>

      <tr>
        <td class="title">UID Number to be used for forwarding accounts</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"]?>">
          </a>
        </td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"][$branch]?></td>
      </tr>

      <tr></tr>
<?php if($ADVANCED_MODE) { ?>

      <!-- -------------------- --!>
      <tr>
        <td class="title">External user creation script</td>
<?php	$class=table_bgcolor(0);
		$new_tr = 0;
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"]?>">
          </a>
        </td>
<?php	if($config["PQL_CONF_SCRIPT_CREATE_USER"][$branch]) { ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_SCRIPT_CREATE_USER"][$branch]?></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">External domain creation script</td>
<?php	$class=table_bgcolor(0);
		$new_tr = 0;
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"]?>">
          </a>
        </td>
<?php	if($config["PQL_CONF_SCRIPT_CREATE_DOMAIN"][$branch]) { ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_SCRIPT_CREATE_DOMAIN"][$branch]?></td>
<?php	} else { ?>
        <td class="<?=$class?>"></td>
<?php	} ?>
      </tr>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Password encryption schemes</td>
<?php     $class=table_bgcolor(0); $new_tr = 0;

          // Convert the array to a index
		  $schemes = pql_split_oldvalues($config["PQL_CONF_PASSWORD_SCHEMES"][$branch]);
	      
		  $i = 0;
		  foreach($schemes as $key) {
			  if($key) {
				  $sc[$i][$branch] = $key;
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
	      
				  if($ALLOW_GLOBAL_CONFIG_SAVE and $sc[$i][$branch]) {
?>

        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>&delval=<?=$sc[$i][$branch]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>=<?=$sc[$i][$branch]?>">
          </a>
        </td>
<?php			  } else { ?>
        <td class="<?=$class?>"></td>
<?php			  } ?>
        <td class="<?=$class?>"><?=$sc[$i][$branch]?>&nbsp;</td>
<?php		  } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php		  if($ALLOW_GLOBAL_CONFIG_SAVE) { ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>">Add password enc scheme</a></td>
<?php		  } else { ?>
        <td class="<?=$class?>"></td>
<?php		  } ?>
      </tr>
<?php   } ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">User objectclasses</td>
<?php     $new_tr = 0; $max = 0;

          // Convert the array to a index
		  $objectclasses = pql_split_oldvalues($config["PQL_CONF_OBJECTCLASS_USER"][$branch]);
			  
		  $i = 0;
		  foreach($objectclasses as $key) {
			  if($key) {
				  $oc[$i][$branch] = $key;
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

				  if($ALLOW_GLOBAL_CONFIG_SAVE and $oc[$i][$branch]) {
?>

        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>&delval=<?=$oc[$i][$branch]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>=<?=$oc[$i][$branch]?>">
          </a>
        </td>
<?php             } else { ?>
        <td class="<?=$class?>"></td>
<?php             } ?>
        <td class="<?=$class?>"><?=$oc[$i][$branch]?>&nbsp;</td>
      </tr>
<?php         } // for() ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     if($ALLOW_GLOBAL_CONFIG_SAVE) { ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>">Add user objectClass
        </td>
<?php	  } ?>
      </tr>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php         if($ALLOW_GLOBAL_CONFIG_SAVE) { ?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>">Add user objectClass</a>
        </td>
<?php         } else { ?>
        <td class="<?=$class?>"></td>
<?php         } ?>
      </tr>
<?php	  } ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Domain objectclasses</td>
<?php     $new_tr = 0; $class=table_bgcolor(0); unset($oc); $max = 0;

          // Convert the array to a index
		  unset($o); unset($objectclasses);
			  
		  $objectclasses = pql_split_oldvalues($config["PQL_CONF_OBJECTCLASS_DOMAIN"][$branch]);
			  
		  $i = 0;
		  foreach($objectclasses as $key) {
			  if($key) {
				  $oc[$i][$branch] = $key;
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

			  if($ALLOW_GLOBAL_CONFIG_SAVE and $oc[$i][$branch]) {
?>
        <td class="<?=$class?>">
           <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>&delval=<?=$oc[$i][$branch]?>">
               <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>=<?=$oc[$i][$branch]?>">
           </a>
        </td>
<?php		  } else { ?>
        <td class="<?=$class?>"></td>
<?php         } ?>
        <td class="<?=$class?>"><?=$oc[$i][$branch]?>&nbsp;</td>
<?php	      } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     	  if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$branch?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>">Add domain objectClass</a>
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
