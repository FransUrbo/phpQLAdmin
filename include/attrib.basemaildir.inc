<?php
// attribute plugin for
// base homedirectory for domain
// $Id: attrib.basemaildir.inc,v 2.0 2002-12-13 14:34:57 turbo Exp $

function attribute_check(){
    global $basemaildir, $domain, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($basemaildir == ""){
	$error["basemaildir"] = PQL_MISSING;
	return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $basemaildir)){
	$error["basemaildir"] = PQL_INVALID;
	return false;
    }
    
    return true;
}

function attribute_init(){
    global $basemaildir, $oldvalue;
    
    $basemaildir = $oldvalue;
}

function attribute_print_form(){
    global $basemaildir, $domain, $PHP_SELF, $attrib, $oldvalue, $error;

    ?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_DOMAIN_DEFAULT_CHANGE_MAILDIR . $domain; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_USER_MAILDIR; ?></td>
		<td><?php echo format_error($error["basemaildir"]); ?><input type="text" name="basemaildir" value="<?php echo $basemaildir; ?>" size="50"></td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="2">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
	</form>
    <?php
}

function attribute_save($type){
    global $domain, $attrib, $oldvalue, $basemaildir, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
	return false;
    }
    
    switch($type){
      case "add":
      case "modify":
	if($basemaildir == $oldvalue){
	    // don't change if new value equal to old one
	    $msg = PQL_LDAP_MAILMESSAGESTORE_UNCHANGED;
	    attribute_forward($msg);
	    break;
    	}
	
	if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $basemaildir)){
	    $msg = pql_complete_constant(PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_OK, array("path" => $basemaildir));
	} else {
	    $msg = PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	attribute_forward($msg);
	break;
	
      default:
	die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}
