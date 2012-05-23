<?php
if(is_array($sudo_results)) {
  // {{{ Show 'Current Sudo Roles' output view
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_("Current Sudo Roles")."\n"?>
        <tr class="c2">
          <td class="title"><?php echo $LANG->_("Role")?></td>
          <td class="title"><?php echo $LANG->_("Users")?></td>
          <td class="title"><?php echo $LANG->_("Commands")?></td>
          <td class="title"><?php echo $LANG->_("Run As")?></td>
          <td class="title"><?php echo $LANG->_("Host")?></td>
          <td class="title"></td>
        </tr>

<?php $row = 'c1';
	  for($i=0; $i < count($sudo_results); $i++) {
		$sudodn   = $sudo_results[$i]['dn'];
		$role     = $sudo_results[$i][pql_get_define("PQL_ATTR_CN")];
		
		$users    = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDOUSER")];    if(!is_array($users))    $users    = array($users);
		$commands = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDOCOMMAND")]; if(!is_array($commands)) $commands = array($commands);
		$runas    = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDORUNAS")];   if(!is_array($runas))    $runas    = array($runas);
		$host     = $sudo_results[$i][pql_get_define("PQL_ATTR_SUDOHOST")];    if(!is_array($host))     $host     = array($host);
?>
        <tr class="<?php echo $row?>">
          <td class="title"><?php echo $role?></td>
          <td>
<?php	for($j=0; $users[$j]; $j++) {
		  echo $users[$j];
		  if($users[$j+1]) {
			echo "<br>";
		  }
		}
		//pql_sudo_list_expand($role, $users, pql_get_define("PQL_ATTR_SUDOUSER"), "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
?>&nbsp;&nbsp;&nbsp;
          </td>

          <td>
<?php	for($j=0; $commands[$j]; $j++) {
		  echo $commands[$j];
		  if($commands[$j+1]) {
			echo "<br>";
		  }
		}
		//pql_sudo_list_expand($role, $commands, pql_get_define("PQL_ATTR_SUDOCOMMAND"), "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
?>&nbsp;&nbsp;&nbsp;
          </td>

          <td>
<?php	for($j=0; $runas[$j]; $j++) {
		  echo $runas[$j];
		  if($runas[$j+1]) {
			echo "<br>";
		  }
		}
		//pql_sudo_list_expand($role, $runas, pql_get_define("PQL_ATTR_SUDORUNAS"), "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
?>&nbsp;&nbsp;&nbsp;
          </td>

          <td>
<?php	for($j=0; $host[$j]; $j++) {
		  echo $host[$j];
		  if($host[$j+1]) {
			echo "<br>";
		  }
		}
		//pql_sudo_list_expand($role, $host, pql_get_define("PQL_ATTR_SUDOHOST"), "&action=remove_attribute_from_sudoRole&sudodn=" . $sudodn);
?>&nbsp;
          </td>

          <td>
            <a href="sudo_modify.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&sudodn=<?=$sudodn?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt=""></a>&nbsp;<a href="sudo_modify.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&sudodn=<?=$sudodn?>&action=del&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt=""></a>
          </td>
        </tr>
<?php	if($row == 'c1')
		  $row = 'c2';
		else
		  $row = 'c1';
	  }

	  if(!@$_REQUEST["domain"]) {
?>
        <tr class="c1">
          <td align="right" class="title"><img src="images/edit.png" width="12" height="12" alt="" border="0"></td>
          <td colspan="5">
            <?php echo $LANG->_('Note that you will have to go into a branch to \uadd\U users and sudo roles.')?>
          </td>
        </tr>
<?php } else { ?>
        <tr class="subtitle">
          <td align="right" class="title"><img src="images/edit.png" width="12" height="12" alt="" border="0"></td>
          <td colspan="5">
            <a href="sudo_add.php?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&view=<?=$_REQUEST["view"]?>&action=add"><?=$LANG->_('Register new sudo role')?></a>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>
<?php
// }}}
} else {
  // {{{ No sudo rules!
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_("Current Sudo Roles")."\n"?>
        <tr class="c1">
          <td align="right" class="title"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>

          <td>
            <table>
              <th align="left">
                <tr>
<?php if(@$_REQUEST["domain"]) { ?>
                  <?php echo $LANG->_('No sudo rules in this branch.');?>
<?php } else { ?>
                  <?php echo $LANG->_('No sudo rules for this host.');?>
<?php } ?>
                </tr>
              </th>
            </table>
          </td>
        </tr>

<?php if(!@$_REQUEST["domain"]) { ?>
        <tr class="c2">
          <td align="right" class="title"><img src="images/edit.png" width="12" height="12" alt="" border="0"></td>
          <td colspan="5">
            <?php echo $LANG->_('Note that you will have to go into a branch to \uadd\U users and sudo roles.')?>
          </td>
        </tr>
<?php } else { ?>
        <tr class="c2">
          <td align="right" class="title"><img src="images/edit.png" width="12" height="12" alt="" border="0"></td>
          <td colspan="5">
            <a href="sudo_add.php?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&view=<?=$_REQUEST["view"]?>&action=add"><?=$LANG->_('Register new sudo role')?></a>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>
<?php
// }}}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
