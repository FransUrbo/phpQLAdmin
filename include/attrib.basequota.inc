<?php
// attribute plugin for
// base quota for domain
// $Id: $

function attribute_check(){
    global $basequota, $domain, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($basequota == ""){
	$error["basequota"] = PQL_MISSING;
	return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $basequota)){
	$error["basequota"] = PQL_INVALID;
	return false;
    }
    
    return true;
}

function attribute_init(){
    global $basequota, $oldvalue;
    
    $basequota = $oldvalue;
}

function attribute_print_form(){
    global $basequota, $domain, $PHP_SELF, $attrib, $oldvalue, $error;

    ?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LDAP_MAILQUOTA_DEFAULT_CHANGE . $domain; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LDAP_MAILQUOTA_TITLE; ?></td>
		<td><?php echo format_error($error["basequota"]); ?><input type="text" name="basequota" value="<?php echo $basequota; ?>" size="50"></td>
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
    global $domain, $attrib, $oldvalue, $basequota, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
	return false;
    }
    
    switch($type){
      case "add":
      case "modify":
	if($basequota == $oldvalue){
	    // don't change if new value equal to old one
	    $msg = PQL_LDAP_MAILQUOTA_UNCHANGED;
	    attribute_forward($msg);
	    break;
    	}
	
	if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $basequota)){
	    $msg = pql_complete_constant(PQL_LDAP_MAILQUOTA_CHANGE_OK, array("value" => $basequota));
	} else {
	    $msg = PQL_LDAP_MAILQUOTA_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	attribute_forward($msg);
	break;
	
      default:
	die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}
