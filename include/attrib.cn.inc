<?php
// attribute plugin for
// cn (Full name)
// $Id: attrib.cn.inc,v 2.10 2003-06-04 08:04:33 turbo Exp $

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
    global $cn, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $view;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_LANG_SEARCH_CN; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LANG_SEARCH_CN; ?></td>
          <td><?php echo format_error($error["cn"]); ?><input type="text" name="cn"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="hidden" name="user" value="<?php echo $user ?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

<?php
}

function attribute_save($type){
    global $domain, $user, $attrib, $oldvalue, $cn, $_pql, $view;
    
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
		
		// Well. Gecos is the same thing as CN, so we should
		// try to change that as well... But only if it
		// already exist in the object.
		$attribs = array($attrib, 'gecos');
		foreach($attribs as $attrib) {
			$old = pql_get_userattribute($_pql->ldap_linkid, $user, $attrib);
			if(is_array($old)) {
				// We have an old value of this array, try to change it...
				if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib, $cn))
				  $msg = pql_complete_constant(PQL_LANG_CN_CHANGE_OK, array("old" => $oldvalue, "new" => $cn));
				else
				  $msg = PQL_LANG_CN_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
			}
		}

		// Get first and last name
		$name = split(" ", $cn);

		// Change givenName which is the first name
		$old = pql_get_userattribute($_pql->ldap_linkid, $user, 'givenName');
		if(is_array($old)) {
			// We have an old value of this array, try to change it...
			pql_replace_userattribute($_pql->ldap_linkid, $user, 'givenName', $name[0]);
		}

		// Change SN which is the last name
		$old = pql_get_userattribute($_pql->ldap_linkid, $user, 'sn');
		if(is_array($old)) {
			// We have an old value of this array, try to change it...
			pql_replace_userattribute($_pql->ldap_linkid, $user, 'sn', $name[1]);
		}

		attribute_forward($msg, true);
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
