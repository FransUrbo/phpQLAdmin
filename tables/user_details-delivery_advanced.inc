  <!-- advanced delivery options -->
<?php $qmaildotmode = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_DOTMODE"));
      $qmaildotmode = $qmaildotmode[0];

      $deliveryprogrampath = pql_get_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_PROGRAM"));
      $deliveryprogrampath = $deliveryprogrampath[0];

      if($qmaildotmode == "") {
		  $qmaildotmode = $LANG->_('Default');
      }

      if($deliveryprogrampath == "") {
		  $deliveryprogrampath = $LANG->_('None');
      }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Advanced delivery properties')?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Dot mode')?></td>
        <td><?=$qmaildotmode?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=qmaildotmode&user=<?=$userdn?>&oldvalue=<?=$qmaildotmode?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Delivery program')?></td>
        <td><?=$deliveryprogrampath?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&attrib=deliveryprogrampath&user=<?=$userdn?>&view=<?=$view?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
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
