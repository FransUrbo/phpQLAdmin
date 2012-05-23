<?php
$attribs = array("ppolicy_changed"		=> pql_get_define("PQL_ATTR_PPOLICY_CHANGED"),
				 "ppolicy_locked"		=> pql_get_define("PQL_ATTR_PPOLICY_LOCKED"),
				 "ppolicy_failure"		=> pql_get_define("PQL_ATTR_PPOLICY_HISTORY"),
				 "ppolicy_graceuse"		=> pql_get_define("PQL_ATTR_PPOLICY_GRACEUSE"),
				 "ppolicy_reset"		=> pql_get_define("PQL_ATTR_PPOLICY_RESET"));
foreach($attribs as $key => $attrib) {
  $value = $_pql->get_attribute($_GET["user"], $attrib), 1;
}

$table_columns = array( "ppolicy_changed"	=> $LANG->_('Last time password was changed'),
		        "ppolicy_locked"	=> $LANG->_('Account locked time'),
		        "ppolicy_failure"	=> $LANG->_('Failed authentications'),
		        "ppolicy_history"	=> $LANG->_('Password history'),
		        "ppolicy_graceuse"	=> $LANG->_('Password use after expiration'),
		        "ppolicy_reset"		=> $LANG->_('User must change password'));
?>
  <!-- Password Policy Details -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Password Policy Details')?></th>
<?php foreach($table_columns as $key => $text) {
	$attrib = $attribs[$key]; // Let's hope this isn't changed (in user_detail.php)!!
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $text?></td>
        <td><?php if(@$$key) { echo $$key; } else { echo "<i>Not set</i>"; }?></td>
        <td>
          <!-- NOTE: Don''t know if all of these can/should be changed/-able... -->
<?php if(@empty($_SESSION["SINGLE_USER"])) { ?>
          <a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=<?php echo $attrib?>&user=<?php echo $url["user"]?>&oldvalue=<?php echo $$key?>&view=<?php echo $_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change value')?>" border="0"></a>
<?php } ?>
        </td>
      </tr>
<?php } ?>
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
