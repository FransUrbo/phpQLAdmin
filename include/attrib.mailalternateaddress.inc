<?php
// attribute plugin for
// mailalternateaddress (emailaddress alias)
// $Id: attrib.mailalternateaddress.inc,v 2.6 2003-04-04 16:37:50 turbo Exp $

// Construct the emailaddress if it don't contain an at character. Needed later...
if(($mailalternateaddress != "") && !ereg("@", $mailalternateaddress)) {
    $mailalternateaddress = $mailalternateaddress . "@" . $defaultdomain;
}

function attribute_check($type){
    global $mailalternateaddress, $oldvalue, $domain, $_pql, $error, $defaultdomain;
    
    // is typed in ?
    if($mailalternateaddress == ""){
	$error["mailalternateaddress"] = PQL_LANG_MISSING;
	return false;
    }
    
    // valid
    if(!check_email($mailalternateaddress)){
	$error["mailalternateaddress"] = PQL_LANG_INVALID;
	return false;
    }
    
    // if type is modify, check existance of old value, else the new one
    if($type == "add"){
	// add
	if(pql_email_exists($_pql, $mailalternateaddress)) {
	    $error["mailalternateaddress"] = PQL_LANG_EXISTS;
	    return false;
	}
    } else {
	// modify
	if($oldvalue != $mailalternateaddress and pql_email_exists($_pql, $mailalternateaddress)) {
	    $error["mailalternateaddress"] = PQL_LANG_EXISTS;
	    return false;
	}
    }
    
    return true;
}

function attribute_init(){
    // dummy function
}

function attribute_print_form(){
    global $mailalternateaddress, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $defaultdomain;

  ?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php if($oldvalue == ""){echo PQL_LANG_MAILALTERNATEADDRESS_NEW;}else{echo PQL_LANG_MAILALTERNATEADDRESS_CHANGE;} ?></th>
  <?php
    // We're changing a MailAlternateAddress. Give the old as reference
    if($oldvalue != "") {
	echo "<tr class=\"c2\">";
	echo "  <td class=\"title\">Old address</td>";
	echo "  <td>" . $mailalternateaddress . "</td>";
	echo "</tr>";
    }
  ?>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_EMAIL; ?></td>
		<td><?php echo format_error($error["mailalternateaddress"]); ?><input type="text" name="mailalternateaddress" value=""><b>@<?php echo $defaultdomain; ?></b></td>
	</tr>
	<tr class="subtitle">
		<td colspan="2">
		<img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo PQL_LANG_MAILALTERNATEADDRESS_HELP; ?></td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="1">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	<input type="hidden" name="user" value="<?php echo $user ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
	</form>
  <?php
}

function attribute_save($type){
    global $domain, $user, $attrib, $oldvalue, $mailalternateaddress;
    global $_pql, $defaultdomain, $config;
    
    // recontrol values
    if(!attribute_check($type)){
	return false;
    }
    
    switch($type){
      case "add":
	$email = $mailalternateaddress;
	
	if(pql_add_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MAILALTERNATE"], $email)){
	    $msg = pql_complete_constant(PQL_LANG_MAILALTERNATEADDRESS_ADD_OK, array("mail" => $email));
	} else {
	    $msg = PQL_LANG_MAILALTERNATEADDRESS_ADD_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
	}
	
	if (PQL_CONF_TESTMAIL_AUTOSEND) {
	    $url = "user_sendmail.php?email=" . urlencode($email) . "&domain=$domain&user=" . urlencode($user) . "&msg=" . urlencode($msg);
	    header("Location: " . $config["PQL_GLOB_URI"] . $url);
	} else {
	    attribute_forward($msg);
	}
	
	break;
      case "modify";
	$email = $mailalternateaddress;
	
	if($email == $oldvalue){
	    // don't change if new value equal to old one
	    $msg = PQL_LANG_MAILALTERNATEADDRESS_UNCHANGED;
	    attribute_forward($msg);
	    break;
	}
	
	if(pql_modify_userattribute($_pql->ldap_linkid, $user, $config["PQL_GLOB_ATTR_MAILALTERNATE"], $oldvalue, $email)){
	    $msg = pql_complete_constant(PQL_LANG_MAILALTERNATEADDRESS_CHANGE_OK, array("old" => $oldvalue, "new" => $email));
	} else {
	    $msg = PQL_LANG_MAILALTERNATEADDRESS_CHANGE_FAILED . ":&nbsp;" . pql_ldap_error($_pql->ldap_linkid);
	}
	
	if (PQL_CONF_TESTMAIL_AUTOSEND) {
	    $url = "user_sendmail.php?email=" . urlencode($email) . "&domain=$domain&user=" . urlencode($user) . "&msg=" . urlencode($msg);
	    header("Location: " . $config["PQL_GLOB_URI"] . $url);
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
