  <!-- advanced delivery options -->
<?php $qmaildotmode = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_DOTMODE"]);
      $qmaildotmode = $qmaildotmode[0];

      $deliveryprogrampath = pql_get_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_PROGRAM"]);
      $deliveryprogrampath = $deliveryprogrampath[0];

      if($qmaildotmode == "") {
	  $qmaildotmode = PQL_LANG_QMAILDOTMODE_NONE;
      }

      if($deliveryprogrampath == "") {
	  $deliveryprogrampath = PQL_LANG_DELIVERYPROGRAMPATH_NONE;
      }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo PQL_LANG_USER_DELIVERYPROPERTIES; ?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_QMAILDOTMODE_TITLE; ?></td>
        <td><?=$qmaildotmode?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=qmaildotmode&user=<?=$user?>&oldvalue=<?=$qmaildotmode?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LANG_DELIVERYPROGRAMPATH_TITLE; ?></td>
        <td><?=$deliveryprogrampath?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=deliveryprogrampath&user=<?=$user?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
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
