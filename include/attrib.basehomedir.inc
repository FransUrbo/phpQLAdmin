<?php
// attribute plugin for
// base homedirectory for domain
// $Id: attrib.basehomedir.inc,v 2.2 2003-04-04 06:54:29 turbo Exp $

function attribute_check(){
    global $basehomedir, $domain, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($basehomedir == ""){
	$error["basehomedir"] = PQL_LANG_MISSING;
	return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $basehomedir)){
	$error["basehomedir"] = PQL_LANG_INVALID;
	return false;
    }
    
    return true;
}

function attribute_init(){
    global $basehomedir, $oldvalue;
    
    $basehomedir = $oldvalue;
}

function attribute_print_form(){
    global $basehomedir, $domain, $PHP_SELF, $attrib, $oldvalue, $error;

?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LANG_DOMAIN_DEFAULT_CHANGE_HOMEDIR . $domain; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_USER_HOMEDIR . " base"; ?></td>
		<td><?php echo format_error($error["basehomedir"]); ?><input type="text" name="basehomedir" value="<?php echo $basehomedir; ?>" size="50"></td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="2">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
	</form>
<?php
}

function attribute_save($type){
    global $domain, $attrib, $oldvalue, $basehomedir, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
	return false;
    }
    
    switch($type){
      case "add":
      case "modify":
	if($basehomedir == $oldvalue){
	    // don't change if new value equal to old one
	    $msg = PQL_LANG_MAILMESSAGESTORE_UNCHANGED;
	    attribute_forward($msg);
	    break;
    	}
	
	if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $basehomedir)){
	    $msg = pql_complete_constant(PQL_LANG_DELIVERYPROGRAMPATH_CHANGE_OK, array("path" => $basehomedir));
	} else {
	    $msg = PQL_LANG_DELIVERYPROGRAMPATH_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	attribute_forward($msg);
	break;
	
      default:
	die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}
