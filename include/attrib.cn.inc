<?php
// attribute plugin for
// cn (Full name)
// $Id: attrib.cn.inc,v 2.5 2003-04-04 06:54:30 turbo Exp $

function attribute_check($type){
    global $cn, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($cn == ""){
		$error["cn"] = PQL_LANG_MISSING;
		return false;
    }
    
    return true;
}

function attribute_init(){
    global $cn, $oldvalue;
    
    $cn = $oldvalue;
}

function attribute_print_form(){
    global $cn, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error;

?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left"><?php echo PQL_LANG_SEARCH_CN; ?></th>
	<tr class="<?php table_bgcolor(); ?>">
		<td class="title"><?php echo PQL_LANG_SEARCH_CN; ?></td>
		<td><?php echo format_error($error["cn"]); ?><input type="text" name="cn"></td>
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
    global $domain, $user, $attrib, $oldvalue, $cn, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
		return false;
    }
    
    switch($type){
      case "add":
		// no add operation possible with cn
		break;

      case "modify";
		if($cn == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LANG_CN_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		// TODO: Should this be configurable? What if we don't
		//       use gecos and/or given/Name!?!?
		$attribs = array($attrib, 'gecos', 'givenName');
		foreach($attribs as $attrib) {
			if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib, $cn)) {
				$msg = pql_complete_constant(PQL_LANG_CN_CHANGE_OK, array("old" => $oldvalue, "new" => $cn)) . "&rlnb=1";
			} else {
				$msg = PQL_LANG_CN_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
				attribute_forward($msg);
			}
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
