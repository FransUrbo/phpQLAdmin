<?php
// attribute plugin for administrator and seealso modification
// Also manages the qmailGroup attributes dnModerator, dnMember and dnSender
// $Id: attrib.administrator.inc,v 1.48.4.1 2004-11-19 08:43:57 turbo Exp $

// There is serious problems with this. I fix for one usage and break another
// one! Let's try to remember (and document) how it's called. It's called from
// the following places:
// * left.php->[user]->User access->Allow user to create domains below domain
// * left.php->[user]->User access->Disallow user to create domains below domain
// * left.php->[user]->Group stuff->Member DN(s)->Add member
// * left.php->[user]->Group stuff->Sender DN(s)->Add sender
// * left.php->[user]->Group stuff->Moderator DN(s)->Add moderator
// * left.php->[domain]->Branch Defaults->Add domain administrator domain
// * left.php->[domain]->Branch Defaults->Add mailinglist administrator for domain
// * left.php->[domain]->Branch Owner Details->Add contact person for domain
// * left.php->phpQLAdmin Configuration->[rootdn]->Super administrator->Add administrator
// [anything else!?]

// Need some java stuff since I'm showing foldable branches etc.
require("./left-head.html");

// {{{ attribute_setup_aci(linkid, dn, userdn, action)
// Options: dn		-> what object to modify ACI's for
//			user	-> array with user(s) to add/delete from ACI list
//			action	-> 'add' or 'delete' (only!). Changing an RDN for a
//					   user isn't done with this function, but in
//					   the file 'attrib.modrdn.inc'.
function attribute_setup_aci($linkid, $dn, $userdn, $action) {
	$old_acis = pql_aci_get($linkid, $dn);

	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci")) {
		// Insert the NEW aci in the right place of the OLD.
		// The user entries should be inserted if
		//	$old_acis[?][RIGHTS][TARGET][?] == [children]
		// Then the user we're giving admin access should be inserted in
		//	$old_acis[?][SUBJECT]
		for($i=0, $oid=0; $old_acis[$i]; $i++, $oid++) {
			// {{{ Generate a new ACI line (with updated OID number)
			$line  = "$oid#";
			$line .= $old_acis[$i]["SCOPE"]."#";
			$line .= $old_acis[$i]["RIGHTS"]["ACTION"].";";
			for($j=0; $old_acis[$i]["RIGHTS"]["PERMISSIONS"][$j]; $j++) {
				$line .= $old_acis[$i]["RIGHTS"]["PERMISSIONS"][$j];
				if($old_acis[$i]["RIGHTS"]["PERMISSIONS"][$j+1])
				  $line .= ",";
				else
				  $line .= ";";
			}
			for($j=0; $old_acis[$i]["RIGHTS"]["TARGET"][$j]; $j++) {
				$line .= $old_acis[$i]["RIGHTS"]["TARGET"][$j];
				if($old_acis[$i]["RIGHTS"]["TARGET"][$j+1])
				  $line .= ",";
				else
				  $line .= "#";
			}
			$line .= $old_acis[$i]["TYPE"]."#";
			$line .= $old_acis[$i]["SUBJECT"];
			// }}}

			if($action == 'add') {
				// {{{ We're adding an administrator - add the user DN(s) to the ACI list as well.
				// (Re)create new ACI's.
				if($old_acis[$i+1]["TYPE"] == "access-id") {
					if(in_array('[entry]', $old_acis[$i+1]["RIGHTS"]["TARGET"]) and !$added_child) {
						// Next ACI entry is an 'access-id/entry'. Put the 'access-id/children' here...
						
						$new_acis[]  = $line; $oid++;
						for($j=0; $userdn[$j]; $j++) {
							$new_acis[]  = "$oid#entry#grant;w,r,s,c;[children]#access-id#".$userdn[$j];
							if($userdn[$j+1])
							  $oid++;
						}
						$added_child = 1;
					} else
					  $new_acis[] = $line;
				} else
				  $new_acis[] = $line;
				// }}}
			} else {
				// BUG: We're removing an administrator - find the user(s) and remove them from the ACI list.
				if($old_acis[$i]['SUBJECT']) {
					for($j=0; $userdn[$j]; $j++) {
						if(lc($old_acis[$i]['SUBJECT']) != lc($userdn[$j]))
						  $new_acis[] = $line;
					}
				} else
				  $new_acis[] = $line;
			}

// BUG: This won't work correctly. Not all user might exists, but it's a flat value...
//			for($j=0; $userdn[$j]; $j++) {
//				// Just make sure the user don't already exists in the ACI list...
//				if(lc($old_acis[$i]["SUBJECT"]) == lc($userdn[$j]))
//				  $added_user_full = 1;
//			}
		}

		if(!$added_user_full and ($action == 'add')) {
			for($j=0; $userdn[$j]; $j++) {
				$new_acis[] = "$oid#entry#grant;w,r,s,c,x;[all]#access-id#".$userdn[$j];
				$oid++;
			}
		}

		return($new_acis);
	}
}

// }}}

// {{{ attribute_init(void)
function attribute_init() {
	// dummy function
}
// }}}

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;
	$attrib = $_REQUEST["attrib"];

    // is typed in ?
    if(empty($_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_print_form(action)
function attribute_print_form($action) {
    global $_pql, $error, $LANG;

	$attrib = $_REQUEST["attrib"];
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php // ------------ S H O W  A  H E A D E R ------------
	  if($attrib == pql_get_define("PQL_ATTR_SEEALSO")) {
		  $what = $LANG->_('contact person');
	  } elseif($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")) {
		  $what = $LANG->_('mailinglist administrator');
	  } elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR")) {
		  $what = $LANG->_('group moderator');
	  } elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER")) {
		  $what = $LANG->_('group member');
	  } elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_SENDER")) { 
		  $what - $LANG->_('group allowed sender');
	  } else {
		if($_REQUEST["user"]) {
			$_REQUEST[$attrib] = $_REQUEST["user"];
			$what = pql_maybe_decode($_REQUEST["user"]).$LANG->_('as administrator');
		} else {
			$what = $LANG->_('domain administrator');
		}
	}
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $what, 'domain' => $_REQUEST["orgname"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
<?php if(($_REQUEST["attrib"] == 'dnmember') or
		 ($_REQUEST["attrib"] == 'dnmoderator') or
		 ($_REQUEST["attrib"] != 'dnsender'))
	  {
		  // We're adding a user dn as subscriber, allowed sender or administrator to a qmailGroup object.
		  // or:
		  // moderator or contact person to a domain/branch.
?>
          <td class="title"><?=$LANG->_('User DN')?></td>
<?php } elseif($_REQUEST["user"]) {
		// We're giving a user access to ('random') branch
?>
          <td class="title"><?=$LANG->_('Branch DN')?></td>
<?php } else { // We're should give a user access to THIS branch ?>
          <td class="title"><?=$LANG->_('Administrator DN')?></td>
<?php } ?>
          <td>
            <?php echo pql_format_error_span($error[$_REQUEST[$attrib]]) . "\n"; ?>
<?php if($action != 'add') { ?>
            <input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="50"></td>
<?php } else {
		// TODO: This is a REALLY bad idea. If we have a 'huge' database
		// (hundred, thousands or more users), this will take AGES!
		if(!$_REQUEST["user"] or ($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR")) or
								 ($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER")) or
								 ($attrib == pql_get_define("PQL_ATTR_GROUP_DN_SENDER")))
		{
			// ------------ S H O W  B R A N C H E S  A N D  U S E R S ------------

			// We're giving user access to a known domain/branch
			$i = 0; $div_counter = 0;

			// Get all domains in the database.
			$domains = pql_get_domains($_pql);
			if(isset($domains)) {
				asort($domains);
				foreach($domains as $key => $domain) {
					// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
					$d = split(',', $domain); $d = split('=', $d[0]); $d = $d[1];
					$d = pql_maybe_decode($d);

					// Create a user search filter (only look for mail users - !?!?).
					$filter  = "(&(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*)(";
					$filter .= pql_get_define("PQL_ATTR_MAIL")."=*))";
						
					// Retreive all users in this branch/domain.
					$users   = pql_get_dn($_pql->ldap_linkid, $domain, $filter);
					if(is_array($users)) {
						// Zero out the variables, othervise we won't get users in
						// specified domain, but also in the PREVIOUS domain shown!
						unset($cns); $links = array();
						foreach($users as $dn) {
							$tmp = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_CN"));
							if(is_array($tmp))
							  $cns[$dn] = $tmp[0];
							else
							  $cns[$dn] = $tmp;
						}

						if(is_array($cns)) {
							asort($cns);
							foreach($cns as $dn => $cn) {
								$new   = array($cn => array($_REQUEST["attrib"], $i, $dn));
								$links = $links + $new;

								$i++;
							}
						}

						pql_format_tree($d, 0, $links, 0);
					}

					// This an ending for the domain tree
					pql_format_tree_end();
				}
			}
?>

            <input type="hidden" name="<?=$attrib?>_<?=$i?>" value="on">
            <input type="hidden" name="<?=$attrib?>" value="<?=$i?>">
<?php	} else {
			// ------------ S H O W  A  M E N U  W I T H  B R A N C H E S ------------
			// We're giving a specific user access to a unknown domain
?>
            <select name="branchdn">
<?php		if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
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
					$dn = urldecode($dn);

					$dom = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"),
											$_SESSION["USER_DN"], 1);
					if($dom)
					  foreach($dom as $d)
						$domains[] = $d;
				}
			}

			if(is_array($domains)) {
				$domains = pql_uniq($domains);
				asort($domains);
				foreach($domains as $domain) {
					if($domain == $_pql->ldap_basedn[0]) {
						$shown_first_rootdn = 1;
?>
              <option value="<?=$domain?>">*&nbsp;<?=pql_maybe_decode($domain)?></option>
<?php				} else { ?>
              <option value="<?=$domain?>">&nbsp;&nbsp;&nbsp;<?=pql_maybe_decode($domain)?></option>
<?php				}
				}
			}
?>
            </select>
<?php	}
	  }
?>
<?php if($shown_first_rootdn) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td>
            <?=$LANG->_('The DN with the asterisk (*) is the DN where phpQLAdmin configurations are stored. If you add a user to that DN, the user will get \'Super Administration Rights\'')?>!
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="rootdn"        value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain"        value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="attrib"        value="<?=$_REQUEST["attrib"]?>">
<?php if($_REQUEST["user"] and ($attrib != pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR")) and
		 ($attrib != pql_get_define("PQL_ATTR_GROUP_DN_MEMBER")) and
		 ($attrib != pql_get_define("PQL_ATTR_GROUP_DN_SENDER")))
      {
?>
    <input type="hidden" name="<?=$attrib?>"  value="<?=$_REQUEST[$attrib]?>">
<?php } ?>
    <input type="hidden" name="user"          value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="action"        value="<?=$action?>">
    <input type="hidden" name="view"          value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="submit"        value="4">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	$value  = $_REQUEST[$attrib];

	// {{{ Which DN should we use?
	if($_REQUEST["branchdn"])
	  $DN = $_REQUEST["branchdn"];
	elseif(!$_REQUEST["user"])
	  $DN = $_REQUEST["domain"];
	elseif($_REQUEST["view"] == 'group')
	  // We're adding a subscriber,allowed sender or moderator to a qmailGroup object.
	  $DN = $_REQUEST["user"];
	else
	  $DN = $_REQUEST["rootdn"];

	// }}}

	// NOTE: No ACI additions/modifications if we're modifying a qmailGroup object,
	//       and only if it's an administrator we're adding!
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci") and
	   ($_REQUEST["view"] != 'group') and ($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR")))
	  $use_aci = 1;
	
	if($value and $_REQUEST["oldvalue"] and ($value == $_REQUEST["oldvalue"])) {
		// don't change if new value equal to old one
		$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Domain administrator')));
		attribute_forward($msg);
		break;
	}
	
	// Get the old values, so we can remove the one we're modifying
	// (delete the one we want to delete, keep the others etc)
	$admins = pql_get_attribute($_pql->ldap_linkid, $DN, $attrib);

	// {{{ Add the old values to the resulting array.
	$ADMs[$attrib] = array();
	if(is_array($admins)) {
		foreach($admins as $admin)
		  $ADMs[$attrib][] = $admin;
	} elseif($admins)
	  $ADMs[$attrib][] = $admins;
	// }}}
	
	if($type == 'delete') {
		// {{{ Delete user DN from array of administrators
		foreach($ADMs[$attrib] as $adm) {
			if(!eregi($value, $adm))
			  $adms[] = $adm;
		}
		
		if(is_array($adms))
		  $ADMs[$attrib] = $adms;
		else
		  $ADMs[$attrib] = array();

		if(!$_REQUEST["user"] and !$_REQUEST["branchdn"])
		  unset($_REQUEST[$attrib]);

		if($use_aci)
		  // Modify the OpenLDAPaci attribute
		  $ACIs = attribute_setup_aci($_pql->ldap_linkid, $_REQUEST["domain"], array($value), $type);
		// }}}
	} else {
		// {{{ Add user 
		if($_REQUEST["user"] and ($_REQUEST["view"] != 'group')) {
			$ADMs[$attrib][] = $_REQUEST["user"];
			
			if($use_aci)
			  // Add the OpenLDAPaci attribute
			  $ACIs = attribute_setup_aci($_pql->ldap_linkid, $_REQUEST["domain"], array($_REQUEST["user"]), $type);
		} elseif($value) {
			$amount = $value;
			for($i=0; $i < $amount; $i++) {
				$var_name = $attrib . "_$i";
				if($_POST[$var_name])  {
					$admins2add[] = $_POST[$var_name]; // Remember this for later...
					$ADMs[$attrib][] = $_POST[$var_name];
				}
			}

			// Add the OpenLDAPaci attribute (maybe)
			if($use_aci and is_array($admins2add)) 
			  $ACIs  = attribute_setup_aci($_pql->ldap_linkid, $_REQUEST["domain"], $admins2add, $type);
			
			unset($_REQUEST[$attrib]);
		}
		// }}}
	}

	// {{{ Do the modification(s)
	if($use_aci and is_array($ACIs))
	  $ADMs[pql_get_define("PQL_ATTR_LDAPACI")] = $ACIs;

	$success = 0;
	if(pql_modify_attribute($_pql->ldap_linkid, $DN, '', '', $ADMs)) {
		$success = 1;

		if($use_aci and $value and ($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR"))) {
			// We're using ACI's and we're adding/removing an administrator.

			// Get all the objects below this branch.
			$branches = pql_get_dn($_pql->ldap_linkid, $DN, 'objectClass=*');

			for($i=0; $branches[$i]; $i++) {
				if($branches[$i] != $DN) {
					// Create an ACI entry for this branch.
					if($type == 'delete') {
						$ACIs = attribute_setup_aci($_pql->ldap_linkid, $branches[$i], array($value), $type);
					} else {
						$ACIs = attribute_setup_aci($_pql->ldap_linkid, $branches[$i], $admins2add, $type);
					}

					// Modify this object. BUG: Should we care about failure/successes?!
					pql_modify_attribute($_pql->ldap_linkid, $branches[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);
				}
			}
		}
		
		if($attrib == pql_get_define("PQL_ATTR_SEEALSO"))
		  $what = $LANG->_('contact person');
		elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR"))
		  $what = $LANG->_('group moderator');
		elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER"))
		  $what = $LANG->_('group member');
		elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_SENDER"))
		  $what = $LANG->_('group allowed sender');
		else
		  $what = $LANG->_('domain administrator');
		
		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'), array('what' => $what));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $what))
			. ": " . ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
	}
	// }}}
}
// }}}

if(($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_ADMINISTRATOR")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_SEEALSO")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_GROUP_DN_SENDER")))
{
	require("./left-trailer.html");
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
