<?php
// attribute plugin for administrator and seealso modification
// $Id: attrib.administrator.inc,v 1.16 2003-05-27 19:00:55 turbo Exp $

require("./left-head.html");

function attribute_check() {
    global $$attrib, $domain, $oldvalue, $_pql, $error, $$attrib;
	$value = $$attrib;
    
    // is typed in ?
    if($value == "") {
		$error[$attrib] = PQL_LANG_MISSING;
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $value)) {
		$error[$attrib] = PQL_LANG_INVALID;
		return false;
    }
    
    return true;
}

function attribute_print_form($action) {
    global $$attrib, $domain, $rootdn, $PHP_SELF, $attrib, $oldvalue;
	global $error, $_pql, $config, $user;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($attrib == 'seealso') { ?>
      <th colspan="3" align="left">Add contact person for domain <?=$domain?></th>
<?php } else { ?>
      <th colspan="3" align="left"><?php echo PQL_LANG_DOMAIN_ADMIN_ADD . $domain; ?></th>
<?php } ?>
        <tr class="<?php table_bgcolor(); ?>">
<?php
	if(($attrib == 'administrator') and $rootdn and $domain) {
		// We're should give a user access to THIS branch
?>
          <td class="title"><?=PQL_LANG_DOMAIN_ADMIN_CHANGE?></td>
<?php
	} elseif(($attrib == 'administrator') and !$rootdn and !$domain and $user) {
		// We're giving a user access to ('random') branch
?>
          <td class="title">Branch DN</td>
<?php
	} else {
?>
          <td class="title">See also DN</td>
<?php
	}
?>
          <td>
            <?php echo format_error($error[$$attrib]) . "\n"; ?>
<?php if($action != 'add') { ?>
            <input type="text" name="<?=$$attrib?>" value="<?=$$attrib?>" size="50"></td>
<?php } else {
		// TODO: This is a REALLY bad idea. If we have a 'huge' database
		// (hundred, thousands or more users), this will take AGES!
		if($rootdn and $domain) {
			// We're giving access to a specific domain
			$i = 0; $j = 0;

			// We're should give a user access to THIS branch	
			foreach($_pql->ldap_basedn as $dn) {
				unset($dom); unset($domains);

				$dom = pql_get_domain_value($_pql, $dn, 'administrator', $GLOBALS["USER_DN"]);
				if($dom) {
					foreach($dom as $d) {
						$domains[] = $d;
					}
				}

				if(isset($domains)) {
					asort($domains);
					foreach($domains as $key => $domain) {
						// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
						$d = split(',', $domain); $d = split('=', $d[0]); $d = $d[1];
?>

            <div id="el<?=$j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              </a>
        
              <a class="item">
                <font color="black" class="heada"><?=$d?></font>
              </a>
            </div>
        
            <div id="el<?=$j?>Child" class="child">
<?php					// Get all users (their DN) in this domain
						$users = pql_get_user($_pql->ldap_linkid, $domain);
						if(is_array($users)) {
							// Zero out the variables, othervise we won't get users in
							// specified domain, but also in the PREVIOUS domain shown!
							unset($cns);
							foreach($users as $username) {
								$cn = pql_get_userattribute($_pql->ldap_linkid, $username, $config["PQL_GLOB_ATTR_CN"]);
								$cns[$username] = $cn[0];
							}
								
							foreach($cns as $userdn => $username) {
?>
              <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="<?=$attrib?>_<?=$i?>" value="<?=$userdn?>"><?=$username?></nobr><br>
<?php              				$i++;
               				}
						}

						$j++;
?>
            </div>
<?php				}
				}
			}
?>

            <input type="hidden" name="<?=$attrib?>_<?=$i?>" value="on">
            <input type="hidden" name="<?=$attrib?>" value="<?=$i?>">
<?php	} elseif($user) { // We're giving a specific user access to a domain ?>
            <select name="domain">
<?php		if($GLOBALS["ALLOW_GLOBAL_CONFIG_SAVE"]) {
				// We're a super-admin, we can give/take
				// adminstration on ANY branch/domain
				// that exists in the system
				$domains = pql_get_domains($_pql);
			} else {
				// We can only give user access to a branch we (the logged in user)
				// have access to. Otherwise, we could 'steal' access to any domain
				// in the system. Only a branch owner/admin can give someone access
				// to the current branch. Am I making myself clear?
				foreach($_pql->ldap_basedn as $dn)  {
					$dom = pql_get_domain_value($_pql, $dn, 'administrator', $GLOBALS["USER_DN"], 1);
					if($dom)
					  foreach($dom as $d)
						$domains[] = $d;
				}
			}

			if(is_array($domains)) {
				$domains = array_values(array_unique($domains));
				asort($domains);
				foreach($domains as $branch) {
					if($branch == $_pql->ldap_basedn[0]) {
						$shown_first_rootdn = 1;
?>
              <option value="<?=$branch?>">*&nbsp;<?=$branch?></option>
<?php				} else { ?>
              <option value="<?=$branch?>">&nbsp;&nbsp;&nbsp;<?=$branch?></option>
<?php				}
				}
			}
?>
            </select>
<?php	}
	}
?>
<?php if($shown_first_rootdn) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td>
            The DN with the asterisk (*) is the DN where phpQLAdmin configurations are stored.
            If you add a user to that DN, the user will get 'Super Administration Rights'!
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

<?php if(!$user) { ?>
    <input type="hidden" name="domain" value="<?=$domain?>">
<?php } else { ?>
    <input type="hidden" name="administrator" value="<?=$user?>">
    <input type="hidden" name="user" value="<?=$user?>">
<?php } ?>
    <input type="hidden" name="submit" value="4">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="action" value="<?=$action?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="oldvalue" value="<?=$$attrib?>">
    <input type="submit" value="<?=PQL_LANG_SAVE?>">
  </form>
<?php
}

function attribute_save($type) {
    global $domain, $rootdn, $attrib, $oldvalue, $$attrib, $_pql, $user;
	$value = $$attrib;

    switch($type) {
      case "add":
      case "modify":
      case "delete":
		if($value and $oldvalue and ($value == $oldvalue)) {
			// don't change if new value equal to old one
			$msg = PQL_LANG_DOMAIN_ADMIN_UNCHANGED;
			attribute_forward($msg);
			break;
    	}

		// Get the old values, so we can remove the one we're modifying
		// (delete the one we want to delete, keep the others etc)
		if(($rootdn and $domain) or ($user and $domain))
		  $admins = pql_get_domain_value($_pql, $domain, $attrib);
		elseif($user and $rootdn)
		  $admins = pql_get_domain_value($_pql, $rootdn, $attrib);
		elseif($user and $$attrib and !$value)
		  $admins = pql_get_domain_value($_pql, $$attrib, $attrib);

		if(is_array($admins)) {
			foreach($admins as $admin)
			  $ADMs[] = $admin;
		}

		if($type == 'delete') {
			// Delete attribute
			if(!$user and !$$attrib) {
				// Add the new value of administrator
				$ADMs[] = $value;
			} elseif($$attrib) {
				// Delete user DN from array of administrators
				foreach($ADMs as $adm) {
					if(!eregi($$attrib, $adm))
					  $adms[] = $adm;
				}
				
				$ADMs = $adms;
			} else {
				// Delete user DN from array of administrators
				foreach($ADMs as $adm) {
					if(!eregi($user, $adm))
					  $adms[] = $adm;
				}
				
				$ADMs = $adms;
			}
		} else {
			// Add user 
			if($user)
			  $ADMs[] = $user;
			elseif($$attrib) {
				$amount = $_POST[$attrib];
				for($i=0; $i < $amount; $i++) {
					$var_name = $attrib . "_$i";
					if($_POST[$var_name]) 
					  $ADMs[] = $_POST[$var_name];
				}
			}
		}

		$success = 0;
		if($domain and (($$attrib and !$user) or ($$attrib and $user) or (!$$attrib and $user))) {
			echo "1: pql_set_domain_value(linkid, $domain, $attrib, $ADMs)<br>";

			if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $ADMs))
			  $success = 1;
		} elseif($rootdn and $user) {
			echo "2: pql_set_domain_value(linkid, $rootdn, $attrib, $ADMs)<br>";

			if(pql_set_domain_value($_pql->ldap_linkid, $rootdn, $attrib, $ADMs))
			  $success = 1;
		}

		if($success) {
			if($type == 'add')
			  $msg = pql_complete_constant(PQL_LANG_DOMAIN_ADMIN_ADD_OK, array("value" => $value));
			elseif($type == 'delete')
			  $msg = pql_complete_constant(PQL_LANG_DOMAIN_ADMIN_DELETE_OK, array("value" => $value));
			else
			  $msg = pql_complete_constant(PQL_LANG_DOMAIN_ADMIN_CHANGE_OK, array("value" => $value));
		} else {
			if($type == 'add')
			  $msg = PQL_LANG_DOMAIN_ADMIN_ADD_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
			elseif($type == 'delete')
			  $msg = PQL_LANG_DOMAIN_ADMIN_DELETE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
			else
			  $msg = PQL_LANG_DOMAIN_ADMIN_CHANGE_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		}

		attribute_forward($msg);
		break;

      default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}

if(($attrib == 'administrator') or ($attrib == 'seealso')) {
	require("./left-trailer.html");
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
