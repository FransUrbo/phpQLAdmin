<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mail.inc,v 2.10 2003-06-25 07:07:02 turbo Exp $

function attribute_check($type){
    global $mail, $domain, $oldvalue, $_pql, $error, $defaultdomain;
    
    // is typed in ?
    if($mail == ""){
	$error["mail"] = PQL_LANG_MISSING;
	return false;
    }
    
    // Is it REALLY a email address (ie, does it have an at in there?)
    if(!ereg("@", $mail)) {
	$mail = $mail . "@" . $defaultdomain;
    }
    
    // valid
    if(!check_email($mail)){
	$error["mail"] = PQL_LANG_INVALID;
	return false;
    }
    
    // if type is modify, check existance of old value, else the new one
    if($oldvalue != $mail and pql_email_exists($_pql, $mail)) {
	$error["mail"] = PQL_LANG_EXISTS;
	return false;
    }
    
    return true;
}

function attribute_init(){
    // dummy function
}

function attribute_print_form(){
    global $mail, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $defaultdomain, $view;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_MAIL_CHANGE; ?></th>
  <?php
    // We're changing the Main address. Give the old as reference
    if($oldvalue != "") {
	echo "<tr class=\"c2\">";
	echo "  <td class=\"title\">Old address</td>";
	echo "  <td>" . $mail . "</td>";
	echo "</tr>";
    }
  ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
          <td><?php echo format_error($error["mail"]); ?><input type="text" name="mail" value=""><b>@<?=$defaultdomain?></b></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_MAIL_HELP; ?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user ?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=PQL_LANG_SAVE?>">
  </form>
<?php
}

function attribute_save($type){
    global $domain, $user, $attrib, $oldvalue, $mail, $_pql, $view;

    // recontrol values
    if(!attribute_check($type)){
		return false;
    }

    switch($type){
      case "add":
		// no add operation possible with mail
		break;
      case "modify";
		if(pql_modify_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAIL"), $oldvalue, $mail)) {
			$msg = pql_complete_constant(PQL_LANG_MAIL_CHANGE_OK, array("old" => $oldvalue, "new" => $mail));
		} else {
			$msg = PQL_LANG_MAIL_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
		}
		
		// We use the mail attribute to reference users and it has just changed...
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", pql_get_rootdn($user)) == pql_get_define("PQL_GLOB_ATTR_MAIL")) {
			$user = $mail;
			// Reload the navigation bar (if allowed by config)
			attribute_forward($msg, true);
		} else {
			attribute_forward($msg);
		}
		
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
