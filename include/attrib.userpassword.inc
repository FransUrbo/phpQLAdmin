<?php
// attribute plugin for
// userpassword (password of a user)
// $Id: attrib.userpassword.inc,v 2.16 2003-11-18 13:16:38 turbo Exp $

function attribute_check($type) {
	global $userpassword, $userpassword_check, $error, $pwscheme, $LANG;

	// is typed in ?
	if($userpassword == "") {
		$error["userpassword"] = $LANG->_('Missing');
		return false;
  	}
	
	// valid - A KERBEROS scheme is ALWAYS valid! We only check
	// password validity if it ISN'T a kerberos scheme...
	if(!eregi('KERBEROS', $pwscheme)) {
		if(preg_match("/[^a-z0-9]/i", $userpassword)){
			$error["userpassword"] = $LANG->_('Invalid');
			return false;
		}
	}
	
	return true;
}

function attribute_init() {
	// dummy function
}

function attribute_print_form() {
	global $domain, $user, $PHP_SELF, $attrib, $error, $rootdn, $pwscheme, $view, $_pql, $LANG;

	// Get the default password scheme for branch
	$defaultpasswordscheme = pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTPASSWORDSCHEME"));
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Set password')?></th>
<?php if(!$defaultpasswordscheme or $GLOBALS["ADVANCED_MODE"]) { ?>
        <!-- Password schema -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Password encryption scheme')?></td>
          <td>
            <?php echo format_error($error["pwscheme"]); ?>
<?php	if(eregi(',', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn))) {
			// We got more than one password scheme...
			$schemes = split(",", pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn));
			foreach($schemes as $scheme) {
				if(!eregi('KERBEROS', $scheme)) {
?>
            <input type="radio" name="pwscheme" value="<?=$scheme?>" <?php if($defaultpasswordscheme == $scheme) { echo "CHECKED"; } ?>><?=$scheme?>

<?php			}
			}
	  	} else { ?>
            <?=$LANG->_('Scheme')?>: <b>{<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn); ?>}</b>
            <input type="hidden" name="pwscheme" value="<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn)?>">
<?php 	} ?>
          </td>
        </tr>
<?php } elseif($defaultpasswordscheme) { ?>
        <input type="hidden" name="pwscheme" value="<?=$defaultpasswordscheme?>">
<?php } ?>

        <!-- Password -->
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <td>
            <?php echo format_error($error["userpassword"]) . "\n"; ?>
            <!-- Ugly hack. Should realy have type=password here, but what's the use. Really!? -->
            <input type="input" name="userpassword">
          </td>
        </tr>

        <tr class="subtitle">
          <td></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?=$LANG->_('A password can only contain numbers and letters, special characters are not allowed')?>
            </table>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="user"   value="<?=$user?>">
    <input type="hidden" name="view"   value="<?=$view?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// '
function attribute_save($type) {
	global $domain, $user, $attrib, $userpassword, $_pql, $pwscheme, $view, $LANG;

	switch($type) {
	  case "add":
		// no add function for userpassword
		break;
		
	  case "modify";
		$pw = pql_password_hash($userpassword, $pwscheme);
		
		if(pql_replace_attribute($_pql->ldap_linkid, $user,
									 pql_get_define("PQL_GLOB_ATTR_PASSWD"),
									 $pw))
		{
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $LANG->_('password')));

			if($user == $GLOBALS["USER_DN"])
			  $GLOBALS["USER_PASS"] = $userpassword;
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('password'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
		
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
