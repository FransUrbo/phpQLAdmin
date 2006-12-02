<?php
// attribute plugin for
// userpassword (password of a user)
// $Id: attrib.userpassword.inc,v 2.37 2006-12-16 12:02:14 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
	global $error, $LANG;

	// is typed in ?
	if(empty($_REQUEST["userpassword"])) {
		$error["userpassword"] = $LANG->_('Missing');
		return false;
  	}
	
	// valid - A KERBEROS scheme is ALWAYS valid! We only check
	// password validity if it ISN'T a kerberos scheme...
	if(!eregi('KERBEROS', $_REQUEST["pwscheme"])) {
		if(!pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["rootdn"]) and preg_match("/[^a-z0-9]/i", $_REQUEST["userpassword"])) {
			$error["userpassword"] = $LANG->_('Invalid');
			return false;
		}
	}
	
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
  // dummy funciton
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	global $error, $_pql, $LANG;

	// Get current password
	$principal = kadmin_get_principal($_pql->ldap_linkid, $_REQUEST["user"]);
	if($principal)
	  unset($password);

	if($principal)
	  $defaultpasswordscheme = 'KERBEROS';
	else
	  // Get the default password scheme for branch
	  $defaultpasswordscheme = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"));
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Set password')?></th>
<?php if(!$defaultpasswordscheme or $_SESSION["ADVANCED_MODE"]) { ?>
        <!-- Password scheme -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password encryption scheme')?></td>
          <td>
            <?php echo pql_format_error_span($error["pwscheme"]); ?>
<?php	if(eregi(',', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]))) {
			// We got more than one password scheme...
			$schemes = split(",", pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]));
			foreach($schemes as $scheme) {
?>
            <input type="radio" name="pwscheme" value="{<?=$scheme?>}" <?php if($defaultpasswordscheme == $scheme) { echo "CHECKED"; } ?>><?=$scheme?>

<?php		}
		} else { ?>
            <?=$LANG->_('Scheme')?>: <b><?=pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"])?></b>
            <input type="hidden" name="pwscheme" value="{<?=pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"])?>}">
<?php	} ?>
          </td>
        </tr>
<?php } elseif($defaultpasswordscheme and !$principal) { ?>
        <input type="hidden" name="pwscheme" value="{<?=$defaultpasswordscheme?>}">
<?php } elseif($principal) { ?>
        <input type="hidden" name="pwscheme" value="{KERBEROS}">
<?php } ?>

<?php if($principal) { ?>
        <!-- Kerberos V principal -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Kerberos V principal')?></td>
          <td><?=$principal?></td>
        </tr>
        <input type="hidden" name="principal" value="<?=$principal?>">

<?php } ?>
        <!-- Password -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <td>
            <?php echo pql_format_error_span($error["userpassword"]) . "\n"; ?>
            <!-- Ugly hack. Should really have type=password here, but what''s the use. Really!? -->
            <input type="input" name="userpassword" size="40">
          </td>
        </tr>
<?php if(!pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["rootdn"])) { ?>

        <tr class="subtitle">
          <td></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?=$LANG->_('A password can only contain numbers and letters, special characters are not allowed')?>
            </table>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="domain" value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"   value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
	global $_pql, $LANG;
	$success = 0;

	switch($type) {
	  case "add":
		// no add function for userpassword
		break;

	  case "modify":
		if($_REQUEST["user"] == $_SESSION["USER_DN"]) {
		  // Make the new password a session variable as well, so that
		  // we can update the used session password value in pql_modify_attribute()
		  $_SESSION["NEW_PASS"] = $_REQUEST["userpassword"];
		}

		// Check if samba is in use so we can keep the passwords in sync.
		// TODO: Perhaps we should give the admin the choice of weather or
		//       not we should use lanman since it's so insecure.
		//       -> Thoughts anyone?
		$objectclasses = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"],
										   pql_get_define("PQL_ATTR_OBJECTCLASS"));
		if($objectclasses and !is_array($objectclasses))
		  // Make it an array for the following foreach() to work..
		  $objectclasses = array($objectclasses);

		foreach($objectclasses as $oc) {
		  if(eregi(pql_get_define("PQL_ATTR_SAMBAOBJECTCLASS"), $oc))
			$USER_IS_SAMBA = 1;
		}
		
		if($USER_IS_SAMBA){
		  $nt = pql_password_hash($_REQUEST["userpassword"], '{ntpasswd}');
		  $lm = pql_password_hash($_REQUEST["userpassword"], '{lmpasswd}');
		  pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"],
							   pql_get_define("PQL_ATTR_SAMBANTPASSWORD"), 1, $nt);
		  pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"],
							   pql_get_define("PQL_ATTR_SAMBALMPASSWORD"), 1, $lm);
		}

		if($_REQUEST["principal"] and extension_loaded('kadm5') and function_exists("kadm5_init_with_password") and ($_REQUEST["pwscheme"] == '{KERBEROS}')) {
		  if(kadmin_cpw($_pql->ldap_linkid, $_REQUEST["principal"], $_REQUEST["userpassword"]))
			$success = 1;
		} else {
		  // Change an 'ordinary' LDAP password
		  $pw = pql_password_hash($_REQUEST["userpassword"], $_REQUEST["pwscheme"]);
	
		  if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_PASSWD"), 1, $pw))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('password')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('password'))) . ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
		
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
