<?php
// attribute plugin for
// homedirectory (path to homedirectory)
// $Id: attrib.homedirectory.inc,v 2.2 2002-12-18 16:28:46 turbo Exp $

function attribute_check($type){
    global $homedirectory, $domain, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($homedirectory == ""){
	$error["homedirectory"] = PQL_MISSING;
	return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $homedirectory)){
	$error["homedirectory"] = PQL_INVALID;
	return false;
    }
    
    return true;
}

function attribute_init(){
    global $homedirectory, $oldvalue;
    
    $homedirectory = $oldvalue;
}

function attribute_print_form(){
    global $homedirectory, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error

    ?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_HOMEDIRECTORY_CHANGE; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_HOMEDIRECTORY_CHANGE; ?></td>
		<td><?php echo format_error($error["homedirectory"]); ?><input type="text" name="homedirectory" value="<?php echo $homedirectory; ?>" size="50"></td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="1">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	<input type="hidden" name="user" value="<?php echo $user ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
	</form>

  
    <?php
}

function attribute_save($type){
    global $domain, $user, $attrib, $oldvalue, $homedirectory, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
	return false;
    }
    
    switch($type){
      case "add":
	// no add operation possible with homedirectory
	break;
      case "modify";
	if($homedirectory == $oldvalue){
	    // don't change if new value equal to old one
	    $msg = PQL_LDAP_HOMEDIRECTORY_UNCHANGED;
	    attribute_forward($msg);
	    break;
    	}
	
	if(pql_replace_userattribute($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user, $attrib,$homedirectory)){
	    $msg = pql_complete_constant(PQL_LDAP_HOMEDIRECTORY_CHANGE_OK, array("path" => $homedirectory));
	} else {
	    $msg = PQL_LDAP_HOMEDIRECTORY_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	attribute_forward($msg);
	break;
	
      default:
	die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}
