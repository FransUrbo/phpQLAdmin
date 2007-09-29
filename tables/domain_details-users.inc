<?php require("./left-head.html"); ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Registred users')?></th>
      <tr>
        <td class="title"></td>
        <td class="title"><?=$LANG->_('Status')?></td>
        <td class="title"><?=$LANG->_('Address')?></td>
<?php if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == sprintf("%s", pql_get_define("PQL_ATTR_UID"))) { ?>
        <td class="title"><?=$LANG->_('Username')?></td>
<?php } else { ?>
        <td class="title"><?php echo pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]);?></td>
<?php } ?>
      </tr>

<?php 	if(is_array($users)) {
			asort($users);
			foreach($users as $user) {
				$userdn = urlencode($user);

				$urldet = "user_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&user=$userdn";
				$urledit= "user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&user=$userdn";

				$uid    = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_UID"));

				// DLW: This should be sorted on DisplayName, not cn.
				$cn     = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_CN"));
				if(is_array($cn)) $cn = $cn[0];

				$uidnr  = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_QMAILUID"));
				$status = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_ISACTIVE"));
				$status = pql_ldap_accountstatus($status);

				if(($uid != 'root') or ($uidnr != '0')) {
					// Do NOT show root user(s) here! This should (for safty's sake)
					// not be availible to administrate through phpQLAdmin!

					// Get registered addresses for this user
					$email   = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAIL"));
					$tmp     = split('@', $email);
					$mail    = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);

					if($mail)
					  $links = array("<b>$mail</b>" => $urledit."&attrib=mail&mail=$email&oldvalue=$email&view=email");
					else
					  $links = array();

					// Get any mail aliases for this user
					$aliases = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAILALTERNATE"));
					if($aliases and !is_array($aliases))
					  $aliases = array($aliases);

					if($aliases) {
						foreach($aliases as $alias) {
							$tmp = split('@', $alias);
							$a   = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);

							// user_edit_attribute.php
							// rootdn=o%3DTurbo+Fredriksson
							// domain=o%3DTurbo+Fredriksson
							// user=cn%3DUser+1+Test%2Co%3DSesam+L%C3%A5s+%26+Larm%2Co%3DTurbo+Fredriksson
							//
							// attrib=mail
							// mail=tst_001@xn--sesamls-jxa.se
							// oldvalue=tst_001@xn--sesamls-jxa.se
							// view=email

							$new = array($a => $urledit."&attrib=".pql_get_define("PQL_ATTR_MAILALTERNATE")."&".pql_get_define("PQL_ATTR_MAILALTERNATE")."=$alias&oldvalue=$alias&view=email");
							$links = $links + $new;
						}
					}

					// See if the attribute mailForwardingAddress is there
					$forwards = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_FORWARDS"));
					if($forwards) {
						if(!is_array($forwards))
						  $forwards = array($forwards);

						foreach($forwards as $forward) {
							$new = array("=> $forward" =>
										 $urledit."&attrib=".pql_get_define("PQL_ATTR_FORWARDS")."&".pql_get_define("PQL_ATTR_FORWARDS")."=$forward&oldvalue=$forward&view=email");
							$links = $links + $new;
						}
					}
?>
      <tr class="<?php pql_format_table(); ?>">
        <!-- Options -->
        <td><a href="<?=$urldet?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change user data')?>" border="0"></a>&nbsp;&nbsp;<a href="user_del.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$userdn?>&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete user')?>" border="0"></a></td>
        <!-- Status -->
        <td><?=$status?></td>
        <!-- User -->
        <td nowrap="1">
<?php
					pql_format_tree($cn, 0, $links, 0);
					pql_format_tree_end();
?>
        </td>
<?php				if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == sprintf("%s", pql_get_define("PQL_ATTR_UID"))) { ?>
        <!-- Username -->
        <td><?=$uid?></td>
<?php				} ?>
      </tr>

<?php			}
			}
		}
?>
      <tr class="subtitle">
        <td colspan="4"><a href="user_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=$LANG->_('Register new user')?></a></td>
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
