<?php
// attribute plugin for
// default domain name (the default domainname for a domain)
// $Id: attrib.defaultdomain.inc,v 2.7 2003-01-14 18:54:39 turbo Exp $

function attribute_check(){
    global $defaultdomain, $domain, $oldvalue, $_pql, $error;
    
    // is typed in ?
    if($defaultdomain == ""){
		$error["defaultdomain"] = PQL_MISSING;
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $defaultdomain)){
		$error["defaultdomain"] = PQL_INVALID;
		return false;
    }
    
    return true;
}

function attribute_init(){
    global $defaultdomain, $oldvalue;
    
    $defaultdomain = $oldvalue;
}

function attribute_print_form(){
    global $defaultdomain, $domain, $PHP_SELF, $attrib, $oldvalue, $error;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_DOMAIN_DEFAULT_CHANGE_NAME . $domain; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_DOMAIN_DEFAULT_NAME; ?></td>
          <td><?php echo format_error($error["defaultdomain"]); ?><input type="text" name="defaultdomain" value="<?php echo $defaultdomain; ?>" size="20"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="oldvalue" value="<?php echo $oldvalue; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>
<?php
}

function attribute_save($type){
    global $domain, $attrib, $oldvalue, $defaultdomain, $_pql;
    
    // recontrol values
    if(!attribute_check($type)){
		return false;
    }
    
    switch($type){
      case "add":
      case "modify":
		if($defaultdomain == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_DOMAIN_DEFAULT_CHANGE_UNCHANGED;
			attribute_forward($msg);
			break;
    	}
		
		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $defaultdomain)){
			$msg = pql_complete_constant(PQL_DOMAIN_DEFAULT_CHANGE_OK, array("name" => $defaultdomain));
		} else {
			$msg = PQL_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		}
		
		if(PQL_LDAP_ATTR_DOMAIN == "dc") {
			require("./include/pql_control.inc");
			$_pql_control = new pql_control($GLOBALS["USER_HOST"], $GLOBALS["USER_DN"], $GLOBALS["USER_PASS"]);
			
			// update locals if control patch is enabled
			if(pql_control_update_domains($_pql->ldap_linkid,
										  $GLOBALS["USER_SEARCH_DN_USR"],
										  $_pql_control->ldap_linkid,
										  $GLOBALS["USER_SEARCH_DN_CTR"])) {
				// message ??
			}
		}
		
		attribute_forward($msg);
		break;
		
      default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
