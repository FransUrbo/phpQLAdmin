<?php
// attribute plugin for
// userpassword (password of a user)
// $Id: attrib.userpassword.inc,v 2.6 2003-04-04 16:37:50 turbo Exp $

function attribute_check($type){
	global $userpassword, $userpassword_check, $error, $pwscheme;
	
	// is typed in ?
	if($userpassword == ""){
		$error["userpassword"] = PQL_LANG_MISSING;
		return false;
  	}
	
	// valid - A KERBEROS scheme is ALWAYS valid! We only check
	// password validity if it ISN'T a kerberos scheme...
	if($pwscheme != "{KERBEROS}") {
		if(preg_match("/[^a-z0-9]/i", $userpassword)){
			$error["userpassword"] = PQL_LANG_INVALID;
			return false;
		}
		
		// exactly retyped ?
		if($userpassword != $userpassword_check){
			$error["userpassword_check"] = PQL_LANG_FAILED_RETYPE;
			return false;
		}
	}
	
	return true;
}

function attribute_init(){
	// dummy function
}

function attribute_print_form(){
	global $domain, $user, $PHP_SELF, $attrib, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_USERPASSWORD_NEW; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_USERPASSWORD_TITLE; ?></td>

          <!-- Ugly hack. Should realy have type=password here, but what's the use. Really!? -->
          <td>
          <?php echo format_error($error["userpassword"]); ?>

            <input type="input" name="userpassword">
          </td>

          <td>
          <?php echo format_error($error["pwscheme"]); ?>

            <select name="pwscheme">
              <option value="none">Choose password scheme to use
<?php
	$schemes = split(",", PQL_CONF_PASSWORD_SCHEMES);
	foreach($schemes as $scheme) {
?>
              <option value="{<?=$scheme?>}"><?=$scheme?>

<?php
	}
?>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td></td>
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_USERPASSWORD_HELP; ?></td>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="hidden" name="user" value="<?php echo $user ?>">
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function attribute_save($type) {
	global $domain, $user, $attrib, $userpassword, $_pql, $pwscheme, $config;

	// recontrol values
	if(!attribute_check($type)) {
 		return false;
	}

	switch($type) {
		case "add":
			// no add function for userpassword
			break;

  		case "modify";
				$pw = pql_password_hash($userpassword, $pwscheme);

				if(pql_replace_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_PASSWD"], $pw)){
					$msg = PQL_LANG_USERPASSWORD_CHANGE_OK;
				} else {
					$msg = PQL_LANG_USERPASSWORD_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
				}

				attribute_forward($msg);
  			break;

			default:
			  die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
