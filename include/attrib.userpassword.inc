<?php
// attribute plugin for
// userpassword (password of a user)
// $Id: attrib.userpassword.inc,v 2.18.2.2 2003-12-15 20:33:07 dlw Exp $

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
		if(preg_match("/[^a-z0-9]/i", $_REQUEST["userpassword"])){
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
	global $error, $_pql, $LANG;

	// Get the default password scheme for branch
	$defaultpasswordscheme = pql_domain_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_GLOB_ATTR_DEFAULTPASSWORDSCHEME"));
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Set password')?></th>
<?php if(!$defaultpasswordscheme or $_SESSION["ADVANCED_MODE"]) { ?>
        <!-- Password schema -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password encryption scheme')?></td>
          <td>
            <?php echo pql_format_error_span($error["pwscheme"]); ?>
<?php	if(eregi(',', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]))) {
			// We got more than one password scheme...
			$schemes = split(",", pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]));
			foreach($schemes as $scheme) {
				if(!eregi('KERBEROS', $scheme)) {
?>
            <input type="radio" name="pwscheme" value="<?=$scheme?>" <?php if($defaultpasswordscheme == $scheme) { echo "CHECKED"; } ?>><?=$scheme?>

<?php			}
			}
	  	} else { ?>
            <?=$LANG->_('Scheme')?>: <b>{<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]); ?>}</b>
            <input type="hidden" name="pwscheme" value="<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"])?>">
<?php 	} ?>
          </td>
        </tr>
<?php } elseif($defaultpasswordscheme) { ?>
        <input type="hidden" name="pwscheme" value="<?=$defaultpasswordscheme?>">
<?php } ?>

        <!-- Password -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <td>
            <?php echo pql_format_error_span($error["userpassword"]) . "\n"; ?>
            <!-- Ugly hack. Should really have type=password here, but what''s the use. Really!? -->
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
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="domain" value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"   value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
	global $_pql, $LANG;

	switch($type) {
	  case "add":
		// no add function for userpassword
		break;

	  case "modify";
		$pw = pql_password_hash($_REQUEST["userpassword"], $_REQUEST["pwscheme"]);
		
		if(pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"],
								 pql_get_define("PQL_GLOB_ATTR_PASSWD"),
								 $pw))
		{
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $LANG->_('password')));

			// DLW: If I'm changing my own password, save it in the session too?
			if($_REQUEST["user"] == $_SESSION["USER_DN"])
			  $_SESSION["USER_PASS"] = $userpassword;
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
