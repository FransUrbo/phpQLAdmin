<?php
// attribute plugin for
// loginshell
// $Id: attrib.loginshell.inc,v 2.1 2002-12-17 12:24:35 turbo Exp $

function attribute_check($type){
    global $loginshell, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($loginshell == ""){
	$error["loginshell"] = PQL_MISSING;
	return false;
    }
    
    return true;
}

function attribute_init(){
    global $loginshell, $oldvalue;
    
    $loginshell = $oldvalue;
}

function attribute_print_form(){
    global $loginshell, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error;
      
    ?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_USER_LOGINSHELL; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_USER_LOGINSHELL; ?></td>
		<td><?php echo format_error($error["loginshell"]); ?>
		  <select name="loginshell">
		    <option value="/bin/bash" SELECTED>/bin/bash</option>
		    <option value="/bin/sh">/bin/sh</option>
		    <?php 
	  // TODO: Extract valid shells from /etc/shells
		    ?>
		  </select>
		</td>
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
    global $domain, $user, $attrib, $oldvalue, $loginshell, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
	return false;
    }
    
    switch($type){
      case "add":
	// no add operation possible with loginshell
	break;
      case "modify";
	if($loginshell == $oldvalue){
	    // don't change if new value equal to old one
	    $msg = PQL_LDAP_LOGINSHELL_UNCHANGED;
	    attribute_forward($msg);
	    break;
    	}
	
	if(pql_replace_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, $attrib, $loginshell)){
	    $msg = pql_complete_constant(PQL_LDAP_LOGINSHELL_CHANGE_OK, array("old" => $oldvalue, "new" => $loginshell));
	} else {
	    $msg = PQL_LDAP_LOGINSHELL_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	attribute_forward($msg);
	
	break;

      default:
	die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}
