<?php
// attribute plugin for
// additional domain name
// $Id: attrib.additionaldomainname.inc,v 2.2 2003-05-06 11:34:08 turbo Exp $

function attribute_check() {
    global $additionaldomainname, $domain, $oldvalue, $_pql, $error, $rootdn;
    
    // is typed in ?
    if($additionaldomainname == "") {
		$error["additionaldomainname"] = PQL_LANG_MISSING;
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $additionaldomainname)) {
		$error["additionaldomainname"] = PQL_LANG_INVALID;
		return false;
    }
    
    return true;
}

function attribute_init() {
    global $additionaldomainname, $oldvalue, $rootdn;
    
    $additionaldomainname = $oldvalue;
}

function attribute_print_form() {
    global $additionaldomainname, $domain, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn;
	$oldvalue = $additionaldomainname;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Add an additional domain name for branch <?=$domain?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Additional domain name</td>
          <td><?php echo format_error($error["additionaldomainname"]); ?><input type="text" name="additionaldomainname" value="<?php echo $additionaldomainname; ?>" size="20"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function attribute_save($type) {
    global $domain, $attrib, $oldvalue, $additionaldomainname, $_pql, $config, $rootdn, $delval;
    
    // recontrol values
    if(!attribute_check($type) and !delval)
	  return false;

	if(($additionaldomainname == $oldvalue) and !delval) {
		// don't change if new value equal to old one
		$msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_UNCHANGED;
		attribute_forward($msg);
		break;
	}
		
    switch($type) {
      case "add":
      case "modify":
      case "delete":
		// Fetch old values from DB
		$oldvalues = pql_get_domain_value($_pql, $rootdn, $attrib);
		if(is_array($oldvalues)) {
			foreach($oldvalues as $val) {
				if(!@eregi($delval, $val)) {
					// Remove the one we don't want
					$entry[$attrib][] = $val;
				}
			}
		}

		if(!$delval)
		  // Add the new value
		  $entry[$attrib][] = $additionaldomainname;
		elseif(!is_array($entry)) 
		  // We're deleting the last entry...
		  $entry[$attrib] = array();
		
		if(pql_set_domain_value($_pql->ldap_linkid, $domain, '', $entry)) {
			$msg = pql_complete_constant(PQL_LANG_DOMAIN_DEFAULT_CHANGE_OK, array("name" => $additionaldomainname));

			require("./include/pql_control.inc");
			$_pql_control = new pql_control($GLOBALS["USER_HOST"], $GLOBALS["USER_DN"], $GLOBALS["USER_PASS"]);
			
			// update locals if control patch is enabled
			pql_control_update_domains($_pql_control, $GLOBALS["USER_SEARCH_DN_CTR"],
									   '*', array($oldvalue, $additionaldomainname));
		} else
		  $msg = PQL_LANG_DOMAIN_DEFAULT_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		
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
