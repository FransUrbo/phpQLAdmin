<?php
// attribute plugin for
// userpassword (password of a user)
// $Id: attrib.userpassword.inc,v 2.43 2008-03-18 22:18:43 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
	global $error, $LANG, $_pql;

	// is typed in ?
	if(empty($_REQUEST["userpassword"])) {
		$error["userpassword"] = $LANG->_('Missing');
		return false;
	}
	
	// valid - A KERBEROS scheme is ALWAYS valid! We only check
	// password validity if it ISN'T a kerberos scheme...
	if(!preg_match('/KERBEROS/i', $_REQUEST["pwscheme"])) {
		if(!pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["rootdn"]) and preg_match("/[^a-z0-9]/i", $_REQUEST["userpassword"])) {
			$error["userpassword"] = $LANG->_('Invalid');
			return false;
		}
		if(pql_get_define("PQL_CONF_STRONGPASSWD_USE")){
			if($_REQUEST['userpassword'] != $_REQUEST['userpassword2']){
				$error["userpassword"] = $LANG->_('Passwords do not match.');
				return false;
			}
			//Open CrackLib Dictionary
			$dictionary = crack_opendict('/usr/lib/cracklib_dict')
			     or die('Unable to open CrackLib dictionary');

			if(!crack_check($dictionary, $_REQUEST["userpassword"]) ){
				$error["userpassword"] = $LANG->_('Set password failed!\n\nThe input password ".
							 "is based off dictionary word. Please choose a different password.');
				// Close dictionary
				crack_closedict($dictionary);
				return false;
			}
			if(!pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["rootdn"]) and preg_match("/[^a-z0-9]/i", $_REQUEST["userpassword"])) {
				$error["userpassword"] = $LANG->_('Invalid');
				return false;
			}
			if(!preg_match("/[a-z]/i", $_REQUEST["userpassword"])){
				$error["userpassword"] = $LANG->_('Passwords must have atleast one letter');
				return false;
			}
			if(!preg_match("/[0-9]/i", $_REQUEST["userpassword"])){
				$error["userpassword"] = $LANG->_('Passwords must have atleast one number');
				return false;
			}
			if( 8 > strlen($_REQUEST["userpassword"]) ){
				$error["userpassword"] = $LANG->_('Passwords must be atleast 8 characters long');
				return false;
			}
			if(!preg_match("/[^a-zA-Z0-9]/i", $_REQUEST["userpassword"])){
				$error["userpassword"] = $LANG->_('Passwords must have atleast one symbol');
				return false;
			}
			$test_pql = new pql($_SESSION["USER_HOST"], '', '');
			if( $test_pql->bind($_REQUEST["user"], $_REQUEST["userpassword"]) ){
				$error["userpassword"] = $LANG->_('Error, Password must be different than previous password');
				return false;
			}
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
	$principal = pql_kadmin_get_principal($_REQUEST["user"]);
	if($principal)
	  unset($password);

	if($principal)
	  $defaultpasswordscheme = 'KERBEROS';
	else
	  // Get the default password scheme for branch
	  $defaultpasswordscheme = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"));

	$passwordschemes = pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]);

	$pw = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_PASSWD"));
	if($pw) {
	  // Extract the scheme from the password field
	  $tmp = explode('}', $pw);
	  $scheme = preg_replace('/{/', '', $tmp[0]);
	}
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Set password')?></th>
<?php if(!$defaultpasswordscheme or $_SESSION["ADVANCED_MODE"]) { ?>
        <!-- Password scheme -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Password encryption scheme')?></td>
          <td>
            <?php echo pql_format_error_span($error["pwscheme"]); ?>
<?php	if(preg_match('/,/', $passwordschemes)) {
			// We got more than one password scheme...
			$schemes = explode(",", $passwordschemes);
			foreach($schemes as $scheme) {
?>
            <input type="radio" name="pwscheme" value="<?php echo $scheme?>" <?php if($defaultpasswordscheme == $scheme) { echo "CHECKED"; } ?>><?=$scheme?>

<?php		}
		} elseif($passwordschemes) { ?>
            <?php echo $LANG->_('Scheme')?>: <b><?=$passwordschemes?></b>
            <input type="hidden" name="pwscheme" value="<?php echo $passwordschemes?>">
<?php	} else { ?>
            <input type="radio" name="pwscheme" value="<?php echo $scheme?>" CHECKED><?=$scheme?>
            <input type="radio" name="pwscheme" value="<?php echo $defaultpasswordscheme?>"><?=$defaultpasswordscheme?>
<?php	} ?>
          </td>
        </tr>
<?php } elseif($defaultpasswordscheme and !$principal) { ?>
        <input type="hidden" name="pwscheme" value="<?php echo $defaultpasswordscheme?>">
<?php } elseif($principal) { ?>
        <input type="hidden" name="pwscheme" value="KERBEROS">
<?php } ?>

<?php if($principal) { ?>
        <!-- Kerberos V principal -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Kerberos V principal')?></td>
          <td><?php echo $principal?></td>
        </tr>
        <input type="hidden" name="principal" value="<?php echo $principal?>">

<?php } ?>
        <!-- Password -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">
            <?php echo $LANG->_('Password')?>
<?php if(pql_get_define("PQL_CONF_STRONGPASSWD_USE")){ ?> 
            <br><?php echo $LANG->_('Re-Type Password')?><br>
<?php } ?>
            <?php echo pql_format_error_span($error["userpassword"]) . "\n"; ?>
          </td>

          <td>
            <!-- Ugly hack. Should really have type=password here, but what''s the use. Really!? -->
<?php if(pql_get_define("PQL_CONF_STRONGPASSWD_USE")){ ?> 
		<input type="password" name="userpassword" size="40"><br>
		<input type="password" name="userpassword2" size="40">
<?php } else {?>
		<input type="input" name="userpassword" size="40">
<?php } ?>
          </td>
        </tr>
<?php if(!pql_get_define("PQL_CONF_ALLOW_ALL_CHARS", $_REQUEST["rootdn"])) { ?>

        <tr class="subtitle">
          <td></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?php echo $LANG->_('A password can only contain numbers and letters, special characters are not allowed')?>
            </table>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="domain" value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn" value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"   value="<?php echo $_REQUEST["user"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
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
		$objectclasses = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_OBJECTCLASS"));
		if($objectclasses and !is_array($objectclasses))
		  // Make it an array for the following foreach() to work..
		  $objectclasses = array($objectclasses);

		foreach($objectclasses as $oc) {
		  if(preg_match('/'.pql_get_define("PQL_ATTR_SAMBAOBJECTCLASS").'/i', $oc))
			$USER_IS_SAMBA = 1;
		}
		
		if($USER_IS_SAMBA) {
		  $nt = pql_password_hash($_REQUEST["userpassword"], '{ntpasswd}');
		  pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_SAMBANTPASSWORD"), 1, $nt);
		}

		if($_REQUEST["principal"] and extension_loaded('kadm5') and function_exists("kadm5_init_with_password") and
		   ($_REQUEST["pwscheme"] == '{KERBEROS}'))
		{
		  if(kadmin_cpw($_REQUEST["principal"], $_REQUEST["userpassword"]))
			$success = 1;
		} else {
		  // Change an 'ordinary' LDAP password
		  $pw = pql_password_hash($_REQUEST["userpassword"], $_REQUEST["pwscheme"]);
	
		  if(pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_PASSWD"), 1, $pw)) {
			$expiredays = $_pql->get_attribute($_REQUEST['rootn'], pql_get_define("PQL_ATTR_DAYSUNTILEXPIRE"));

			// Since Password is changeing, I better update the shadowstuff also
			if(isset($expiredays)) {
			  $today = floor(time() * (1/60) * (1/60) * (1/24) );
			  $next = $today + $expiredays;

			  pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_EXPIRATION"), 1, $next);
			  pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_PASSWORD_LASTCHANGED"), 1, $today);
			}
			
			$success = 1;
		  }
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
