<?php
// attribute plugin for administrator and seealso modification
// $Id: attrib.administrator.inc,v 1.4 2003-01-13 07:15:53 turbo Exp $

function attribute_check() {
    global $$attrib, $domain, $oldvalue, $_pql, $error, $$attrib;
	$value = $$attrib;
    
    // is typed in ?
    if($value == "") {
		$error[$attrib] = PQL_MISSING;
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $value)) {
		$error[$attrib] = PQL_INVALID;
		return false;
    }
    
    return true;
}

function attribute_print_form($action) {
    global $$attrib, $domain, $PHP_SELF, $attrib, $oldvalue, $error, $_pql;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo PQL_DOMAIN_ADMIN_ADD . $domain; ?></th>
        <tr class="<?php table_bgcolor(); ?>">
<?php
	if($attrib == 'administrator') {
?>
          <td class="title"><?=PQL_DOMAIN_ADMIN_CHANGE?></td>
<?php
	} else {
?>
          <td class="title">See also DN</td>
<?php
	}
?>
          <td>
          <?php echo format_error($error[$$attrib]); ?>

<?php
	if($action != 'add') {
?>
            <input type="text" name="<?=$$attrib?>" value="<?=$$attrib?>" size="50"></td>
<?php
	} else {
		// TODO: This is a REALLY bad idea. If we have 'huge'
		// (hundred, thousands or more users, this will take AGES!)

		// Get ALL domains
		$domains = pql_get_domains($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"]);
		if(is_array($domains)){
			asort($domains);
?>
            <select name="<?=$attrib?>">
<?php
			foreach($domains as $dom) {
				echo "<!-- Domain: $dom -->
";
				// Get all users in all the domains
				$users = pql_get_user($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $dom);
				if($users) {
					foreach($users as $user) {
						$cn = pql_get_userattribute($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], $domain, $user, PQL_LDAP_ATTR_CN);
						$cns[$user] = $cn[0];
					}
					asort($cns);
					
					foreach($cns as $user => $cn){
						$dn = pql_search_attribute($_pql->ldap_linkid, $GLOBALS["USER_SEARCH_DN_USR"], 'cn', $cn);
?>
              <option value="<?=$dn?>"><?php echo $dom . ": " . $cn?></option>
<?php
					}
				}
			}
		}
?>
            </select>
<?php
	}
?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="4">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="action" value="<?=$action?>">
    <input type="hidden" name="oldvalue" value="<?=$$attrib?>">
    <input type="submit" value="<?=PQL_SAVE?>">
  </form>
<?php
}

function attribute_save($type) {
    global $domain, $attrib, $oldvalue, $$attrib, $_pql;
	$value = $$attrib;
    
    switch($type) {
      case "add":
      case "modify":
      case "delete":
		if($value == $oldvalue) {
			// don't change if new value equal to old one
			$msg = PQL_DOMAIN_ADMIN_UNCHANGED;
			attribute_forward($msg);
			break;
    	}

		// Get the old values, so we can remove the one we're modifying
		// (delete the one we want to delete, keep the others etc)
		$admins	= pql_get_domain_value($_pql->ldap_linkid, $domain, $attrib);
		if(is_array($admins)) {
			foreach($admins as $admin) {
				if(($type == 'modify' and $admin != $oldvalue) or
				   ($type == 'delete' and $admin != $value) or
				   ($type == 'add'))
				  {
					  $ADMs[] = $admin;
				  }
			}
		}

		if($type != 'delete') {
			// Add the new value of administrator
			$ADMs[] = $value;
		}

		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $ADMs)) {
			if($type == 'add') {
				$msg = pql_complete_constant(PQL_DOMAIN_ADMIN_ADD_OK, array("value" => $value));
			} elseif($type == 'delete') {
				$msg = pql_complete_constant(PQL_DOMAIN_ADMIN_DELETE_OK, array("value" => $value));
			} else {
				$msg = pql_complete_constant(PQL_DOMAIN_ADMIN_CHANGE_OK, array("value" => $value));
			}
		} else {
			if($type == 'add') {
				$msg = PQL_DOMAIN_ADMIN_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
			} elseif($type == 'delete') {
				$msg = PQL_DOMAIN_ADMIN_DELETE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
			} else {
				$msg = PQL_DOMAIN_ADMIN_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
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
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
