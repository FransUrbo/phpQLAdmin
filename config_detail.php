<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");
global $config;

include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// print status message, if one is available
if(isset($msg)){
    print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and $config["PQL_GLOB_AUTO_RELOAD"]) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnav.location.reload();
  //--></script>
<?php
}

foreach($_pql->ldap_basedn as $dn) {
    if(eregi('KERBEROS', $config["PQL_CONF_PASSWORD_SCHEMES"][$dn]))
      $show_kerberos_info = 1;
}
?>
  <span class="title1">phpQLAdmin configuration</span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Global configuration
      <tr>
        <td class="title">LDAP host</td>
<?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><?=$USER_HOST?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Language</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE and 0) {
	  // TODO: Sorry, phpQLAdmin isn't translated propperly to anything else than English!
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_LANG"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_LANG"]?>">
          </a>
        </td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?=PQL_LANG_LANG?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Who are we</td>
<?php $class=table_bgcolor(0); ?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_WHOAREWE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_WHOAREWE"]?>"></a></td>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_WHOAREWE"]?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Hostmaster</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_HOSTMASTER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_HOSTMASTER"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_HOSTMASTER"]?>&nbsp;</td>
      </tr>

      <tr></tr>

      <tr>
        <td class="title">Manage Controls DB</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_USE"]?>"></a></td>
<?php } else { ?>
        <td class="<?=$class?>"></td>
<?php } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_CONTROL_USE"]) {echo 'Yes';} else {echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Manage EZMLM mailinglists</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_EZMLM_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_EZMLM_USE"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_EZMLM_USE"]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

<?php if($ADVANCED_MODE and 0) { // TODO: Currently disabled. Not even started. ?>
      <tr>
        <td class="title">Manage Bind 9 DNSSEC domains</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_BIND9_USE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_BIND9_USE"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_BIND9_USE"]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>
<?php } ?>

      <tr>
        <td class="title">Automatic reload of navigation bar</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_AUTO_RELOAD"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_AUTO_RELOAD"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_AUTO_RELOAD"]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Automatically replicate domains to locals</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDLOCALS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_AUTOADDLOCALS"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_CONTROL_AUTOADDLOCALS"]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Automatically replicate domains to rcpthosts</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_AUTOADDRCPTHOSTS"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS"]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Allow change of LDAP server</td>
<?php $class=table_bgcolor(0);
      if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_CHANGE_SERVER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_CHANGE_SERVER"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_GLOB_CHANGE_SERVER"]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
      </tr>
<?php	if($show_kerberos_info) { ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Kerberos realm</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_REALM"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_REALM"]?>"></a></td>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_KRB5_REALM"]?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Kerberos admin server</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_SERVER"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_SERVER"]?>"></a></td>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_KRB5_ADMIN_SERVER"]?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Kerberos admin principal</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_PRINCIPAL"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_PRINCIPAL"]?>"></a></td>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_KRB5_ADMIN_PRINCIPAL"]?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Kerberos admin keytab</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_KEYTAB"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_KEYTAB"]?>"></a></td>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_KRB5_ADMIN_KEYTAB"]?>&nbsp;</td>
      </tr>

      <tr>
        <td class="title">Path to Kerberos <u>kadmin</u> command</td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"]?>"></a></td>
        <td class="<?=$class?>"><?=$config["PQL_GLOB_KRB5_ADMIN_COMMAND_PATH"]?>&nbsp;</td>
      </tr>
<?php	} ?>
    </th>
  </table>

  <br><!-- ================================== --!>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Branch configuration
      <!-- -------------------- --!>
      <tr>
        <td class="title">LDAP base dn</td>
<?php $class=table_bgcolor(0);
      foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><b><?=$dn?>&nbsp;</b></td>
<?php } ?>
      </tr>
<?php if($config["PQL_GLOB_CONTROL_USE"]) { ?>

      <!-- -------------------- --!>
      <tr>
        <td class="title">LDAP control base dn</td>
<?php     $class=table_bgcolor(0);
          foreach($_pql->ldap_basedn as $dn) {
              if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_DN"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Set/Modify <?=$PQL_ATTRIBUTE["PQL_CONF_CONTROL_DN"]?>"></a></td>
<?php         } else { ?>
        <td class="<?=$class?>"></td>
<?php         }

              if($config["PQL_CONF_CONTROL_DN"][$dn]) {
?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_CONTROL_DN"][$dn]?>&nbsp;</td>
<?php         } else { ?>
        <td class="<?=$class?>"><i>undefined</i></td>
<?php         }
          }
?>
      </tr>
<?php } ?>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Show users</td>
<?php $class=table_bgcolor(0);
      foreach($_pql->ldap_basedn as $dn) {
         if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_SHOW_USERS"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_CONF_SHOW_USERS"][$dn]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php } ?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Allow absolute mailbox paths</td>
<?php $class=table_bgcolor(0);
      foreach($_pql->ldap_basedn as $dn) {
         if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_ALLOW_ABSOLUTE_PATH"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_CONF_ALLOW_ABSOLUTE_PATH"][$dn]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php } ?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Verify user/domain deletions etc <b>[recomended!]</b></td>
<?php $class=table_bgcolor(0);
      foreach($_pql->ldap_basedn as $dn) {
         if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&toggle=1&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_VERIFY_DELETE"]?>"></a></td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?php if($config["PQL_CONF_VERIFY_DELETE"][$dn]) {echo 'Yes';}else{echo 'No';}?>&nbsp;</td>
<?php } ?>
      </tr>

      <tr>
        <td class="title">Reference users with</td>
<?php    $class=table_bgcolor(0);
         foreach($_pql->ldap_basedn as $dn) {
            if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_USERS_WITH"]?>"></a></td>
<?php       } else { ?>
        <td class="<?=$class?>"></td>
<?php       } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_REFERENCE_USERS_WITH"][$dn]?></td>
<?php    } ?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Reference domains with</td>
<?php    $class=table_bgcolor(0); 
         foreach($_pql->ldap_basedn as $dn) {
            if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_REFERENCE_DOMAINS_WITH"]?>"></a></td>
<?php       } else { ?>
        <td class="<?=$class?>"></td>
<?php       } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_REFERENCE_DOMAINS_WITH"][$dn]?></td>
<?php    } ?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Minimum UID/GID Number to be used for shell and system accounts</td>
<?php $class=table_bgcolor(0);
      foreach($_pql->ldap_basedn as $dn) {
         if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_GLOB_MINIMUM_UIDNUMBER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_GLOB_MINIMUM_UIDNUMBER"]?>">
          </a>
        </td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"][$dn]?></td>
<?php } ?>
      </tr>

      <tr>
        <td class="title">UID Number to be used for forwarding accounts</td>
<?php $class=table_bgcolor(0);
      foreach($_pql->ldap_basedn as $dn) {
         if($ALLOW_GLOBAL_CONFIG_SAVE) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Toggle <?=$PQL_ATTRIBUTE["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"]?>">
          </a>
        </td>
<?php    } else { ?>
        <td class="<?=$class?>"></td>
<?php    } ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_FORWARDINGACCOUNT_UIDNUMBER"][$dn]?></td>
<?php } ?>
      </tr>

      <tr></tr>
<?php if($ADVANCED_MODE) { ?>

      <!-- -------------------- --!>
      <tr>
        <td class="title">External user creation script</td>
<?php	$class=table_bgcolor(0);
		$new_tr = 0;
		foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_USER"]?>">
          </a>
        </td>
<?php		if($config["PQL_CONF_SCRIPT_CREATE_USER"][$dn]) { ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_SCRIPT_CREATE_USER"][$dn]?></td>
<?php		} else { ?>
        <td class="<?=$class?>"></td>
<?php		}
	  }
?>
      </tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">External domain creation script</td>
<?php	$class=table_bgcolor(0);
		$new_tr = 0;
		foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"]?>">
            <img src="images/edit.png" width="12" height="12" border="0" alt="Edit attrib <?=$PQL_ATTRIBUTE["PQL_CONF_SCRIPT_CREATE_DOMAIN"]?>">
          </a>
        </td>
<?php		if($config["PQL_CONF_SCRIPT_CREATE_DOMAIN"][$dn]) { ?>
        <td class="<?=$class?>"><?=$config["PQL_CONF_SCRIPT_CREATE_DOMAIN"][$dn]?></td>
<?php		} else { ?>
        <td class="<?=$class?>"></td>
<?php		}
	  }
?>
      </tr>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">Password encryption schemes</td>
<?php     $class=table_bgcolor(0); $new_tr = 0;

          // Convert the array to a index
          foreach($_pql->ldap_basedn as $dn) {
	      unset($s);
	      
	      $schemes = pql_split_oldvalues($config["PQL_CONF_PASSWORD_SCHEMES"][$dn]);
	      
	      $i = 0;
	      foreach($schemes as $key) {
		  if($key) {
		      $sc[$i][$dn] = $key;
		      $i++;
		  }
	      }
	      
	      if($max < $i-1)
		$max = $i-1;
	  }

          if(is_array($sc)) {
	      for($i=0; $i <= $max; $i++) {
		  if($new_tr) {
?>

      <tr>
        <td class="title"></td>
<?php             }
	          $new_tr = 1;
	      
		  foreach($_pql->ldap_basedn as $dn) {
		      if($ALLOW_GLOBAL_CONFIG_SAVE and $sc[$i][$dn]) {
?>

        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>&delval=<?=$sc[$i][$dn]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>=<?=$sc[$i][$dn]?>">
          </a>
        </td>
<?php                 } else { ?>
        <td class="<?=$class?>"></td>
<?php                 } ?>
        <td class="<?=$class?>"><?=$sc[$i][$dn]?>&nbsp;</td>
<?php             } ?>
      </tr>
<?php         } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     if($ALLOW_GLOBAL_CONFIG_SAVE) {
              foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>">Add password enc scheme</a></td>
<?php         }
          }
?>
      </tr>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php         if($ALLOW_GLOBAL_CONFIG_SAVE) {
	          foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>"><a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"]?>">Add password enc scheme</a></td>
<?php             }
              } else {
?>
        <td class="<?=$class?>"></td>
<?php         } ?>
      </tr>
<?php   } ?>

      <tr></tr>

      <!-- -------------------- --!>
      <tr>
        <td class="title">User objectclasses</td>
<?php     $new_tr = 0; $max = 0;

          // Convert the array to a index
          foreach($_pql->ldap_basedn as $dn) {
	      unset($o);
	      
	      $objectclasses = pql_split_oldvalues($config["PQL_CONF_OBJECTCLASS_USER"][$dn]);
	      
	      $i = 0;
	      foreach($objectclasses as $key) {
		  if($key) {
		      $oc[$i][$dn] = $key;
		      $i++;
		  }
	      }
	      
	      if($max < $i-1)
		$max = $i-1;
	  }

          if(is_array($oc)) {
	      for($i=0; $i <= $max; $i++) {
		  if($new_tr) {
?>
      <tr>
        <td class="title"></td>
<?php             }
	          $new_tr = 1;

	          foreach($_pql->ldap_basedn as $dn) {
		      if($ALLOW_GLOBAL_CONFIG_SAVE and $oc[$i][$dn]) {
?>

        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>&delval=<?=$oc[$i][$dn]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>=<?=$oc[$i][$dn]?>">
          </a>
        </td>
<?php                 } else { ?>
        <td class="<?=$class?>"></td>
<?php                 } ?>
        <td class="<?=$class?>"><?=$oc[$i][$dn]?>&nbsp;</td>
<?php             } ?>
      </tr>
<?php         } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     if($ALLOW_GLOBAL_CONFIG_SAVE) {
              foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>">Add user objectClass
        </td>
<?php         }
          }
?>
      </tr>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php         if($ALLOW_GLOBAL_CONFIG_SAVE) {
	          foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"]?>">Add user objectClass</a>
        </td>
<?php             }
              } else {
?>
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
          foreach($_pql->ldap_basedn as $dn) {
	      unset($o); unset($objectclasses);
	      
	      $objectclasses = pql_split_oldvalues($config["PQL_CONF_OBJECTCLASS_DOMAIN"][$dn]);
	      
	      $i = 0;
	      foreach($objectclasses as $key) {
		  if($key) {
		      $oc[$i][$dn] = $key;
		      $i++;
		  }
	      }
	      
	      if($max < $i-1)
		$max = $i-1;
	  }

          if(is_array($oc)) {
	      for($i=0; $i <= $max; $i++) {
		  if($new_tr) {
?>
      <tr>
        <td class="title"></td>
<?php             }
	          $new_tr = 1;

	          foreach($_pql->ldap_basedn as $dn) {
		      if($ALLOW_GLOBAL_CONFIG_SAVE and $oc[$i][$dn]) {
?>
        <td class="<?=$class?>">
           <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>&delval=<?=$oc[$i][$dn]?>">
               <img src="images/del.png" width="12" height="12" border="0" alt="Delete attrib <?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>=<?=$oc[$i][$dn]?>">
           </a>
        </td>
<?php                 } else { ?>
        <td class="<?=$class?>"></td>
<?php                 } ?>
        <td class="<?=$class?>"><?=$oc[$i][$dn]?>&nbsp;</td>
<?php              } ?>
      </tr>
<?php	      } ?>
      </tr>

      <tr>
        <td class="title"></td>
<?php     if($ALLOW_GLOBAL_CONFIG_SAVE) {
              foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>"></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>">Add domain objectClass</a>
        </td>
<?php         }
          }
?>
      </tr>
<?php     } else { ?>
        <td class="<?=$class?>"></td>
<?php         if($ALLOW_GLOBAL_CONFIG_SAVE) {
	          foreach($_pql->ldap_basedn as $dn) {
?>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=$PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]?>">Add domain objectClass</a>
        </td>
<?php             }
	      } else {
?>
        <td class="<?=$class?>"></td>
<?php         } ?>
      </tr>
<?php 	  }
      }
?>
    </th>

    <th colspan="3" align="left">
      <tr class="subtitle">
        <table>
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
          <td>The phpQLAdmin configuration values are stored in config.inc<?php
if($ALLOW_GLOBAL_CONFIG_SAVE) {
    foreach($_pql->ldap_basedn as $dn)
      $dns[] = $dn;

    if($dns[0])
      echo " and in DN ";

    for($i=0; $dns[$i]; $i++) {
      if($dns[$i+1]) {
	  echo " <u>".$dns[$i]."</u>";
	  if($dns[$i+2])
	     echo ", ";
      } else {
	  echo " and ";
	  echo " <u>".$dns[$i]."</u>";
      }
    }
}
?></td>
        </table>
      </tr>
    </th>
  </table>
</body>
</html>
