  <!-- advanced delivery options -->
<?php $qmaildotmode = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"));
      $qmaildotmode = $qmaildotmode[0];

      $deliveryprogrampath = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_PROGRAM"));
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
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Dot mode')?></td>
        <td><?=$qmaildotmode?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=qmaildotmode&user=<?=$url["user"]?>&oldvalue=<?=$qmaildotmode?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Delivery program')?></td>
        <td><?=$deliveryprogrampath?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=deliveryprogrampath&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>
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
