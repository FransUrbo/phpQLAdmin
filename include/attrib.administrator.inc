<?php
// attribute plugin for administrator and seealso modification
// Also manages the qmailGroup attributes dnModerator, dnMember and dnSender
// $Id: attrib.administrator.inc,v 1.63 2007-01-09 16:41:23 turbo Exp $

// There is serious problems with this. I fix for one usage and break another
// one! Let's try to remember (and document) how it's called. It's called from
// the following places:
// * Via domain_edit_attributes.php
//   + Execute attribute_print_form():
//     - left.php->[user]->User access->Administrator in branch
//     - left.php->[user]->User access->Mailinglist administrator in branch
//     - left.php->[user]->User access->Webserver administrator in branch
//     - left.php->[user]->User access->DNS administrator in branch
//   + Go directly to attribute_save():
//     - left.php->[user]->User access->Super administrator->[Yes/No]
//     - left.php->[user]->User access->Administrate global mailservers->[Yes/No]
//     - left.php->[user]->User access->Administrate global webservers->[Yes/No]
//
// * Via user_edit_attributes.php
//   - left.php->[user]->Group stuff->Member DN(s)->Add member
//   - left.php->[user]->Group stuff->Sender DN(s)->Add sender
//   - left.php->[user]->Group stuff->Moderator DN(s)->Add moderator
//
// * Via domain_edit_attributes.php
//   - left.php->[domain]->Branch access->Domain administrators
//   - left.php->[domain]->Branch Access->Mailinglists administrator
//   - left.php->[domain]->Branch Access->Webserver administrator
//   - left.php->[domain]->Branch Access->DNS administrator
//   - left.php->[domain]->Branch Owner Details->Contact person
//
// * Via domain_edit_attributes.php
//   - left.php->phpQLAdmin Configuration->[rootdn]->Super administrator->Add administrator

// Need some java stuff since I'm showing foldable branches etc.
require("./left-head.html");

// {{{ attribute_setup_aci(linkid, dn, userdn, action)
// Options: dn		-> what object to modify ACI's for
//			userdn	-> array with user(s) to add/delete from ACI list
//			action	-> 'add' or 'delete' (only!). Changing an RDN for a
//					   user isn't done with this function, but in
//					   the file 'attrib.modrdn.inc'.
//
// NOTE: user template objects are treated slightly different.
//       User ($userdn) will only get READ access to the [all]
//       target. The [children] will not be added...
function attribute_setup_aci($linkid, $dn, $userdn, $action) {
	$old_acis = pql_aci_get($linkid, $dn);

	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci")) {
		// Insert the NEW aci in the right place of the OLD.
		// The user entries should be inserted if
		//	$old_acis[?][RIGHTS][TARGET][?] == [children]
		// Then the user we're giving admin access should be inserted in
		//	$old_acis[?][SUBJECT]
		for($i=0, $oid=0; $i < $old_acis["count"]; $i++, $oid++) {
			// {{{ Generate a new ACI line (with updated OID number)
			$line  = "$oid#";
			$line .= $old_acis[$i]["SCOPE"]."#";
			$line .= $old_acis[$i]["RIGHTS"]["ACTION"].";";
			for($j=0; $j < count($old_acis[$i]["RIGHTS"]["PERMISSIONS"]); $j++) {
				$line .= $old_acis[$i]["RIGHTS"]["PERMISSIONS"][$j];
				if($old_acis[$i]["RIGHTS"]["PERMISSIONS"][$j+1])
				  $line .= ",";
				else
				  $line .= ";";
			}
			for($j=0; $j < count($old_acis[$i]["RIGHTS"]["TARGET"]); $j++) {
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
			  // {{{ We're adding an administrator - add the user DN(s) to the '[entry]' ACI list as well.
			  if(!eregi('usertemplatename', $dn)) {
				// (Re)create new ACI's.
				if($old_acis[$i+1]["TYPE"] == "access-id") {
					if((in_array('[entry]', $old_acis[$i+1]["RIGHTS"]["TARGET"]) or in_array('[all]', $old_acis[$i+1]["RIGHTS"]["TARGET"])) and !$added_child) {
						// Next ACI entry is an 'access-id/entry'. Put the 'access-id/children' here...
						
						$new_acis[]  = $line; $oid++;
						for($j=0; $j < count($userdn); $j++) {
						  $new_acis[]  = "$oid#entry#grant;w,r,s,c;[children]#access-id#".$userdn[$j];
						  
						  if($userdn[$j+1])
							$oid++;
						}
						$added_child = 1;
					} else
					  $new_acis[] = $line;
				} else
				  $new_acis[] = $line;
			  }
			  // }}}
			} else {
			  // {{{ We're removing an administrator - find the user(s) and remove them from the ACI list.
			  if($old_acis[$i]['SUBJECT']) {
				for($j=0; $j < count($userdn); $j++) {
				  if(lc($old_acis[$i]['SUBJECT']) != lc($userdn[$j]))
					$new_acis[] = $line;
				}
			  } else
				$new_acis[] = $line;
			  // }}}
			}
		}

		if(!$added_user_full and ($action == 'add') and
		   (($_REQUEST["attrib"] == pql_get_define("PQL_ATTR_ADMINISTRATOR")) or
			($dn != $_REQUEST["domain"])))
		{
		  // Only do this if it's a branch admin OR it's not the branch DN
		  // I.e. do it for {web,DNS,mailinglist} admins etc IF it's not
		  //      the branch DN. Always if it's the 'administrator' attribute
		  //      though.
		  for($j=0; $j < count($userdn); $j++) {
			// Just make sure the user don't already exists in the ACI list...
			$add_user = 1;

			for($i=0; $i < count($old_acis); $i++) {
			  if(lc($old_acis[$i]["SUBJECT"]) == lc($userdn[$j]))
				$add_user = 0;
			}
			
			if($add_user) {
			  if(eregi('usertemplatename', $dn))
				$new_acis[] = "$oid#entry#grant;r,s,c;[all]#access-id#".$userdn[$j];
			  else
				$new_acis[] = "$oid#entry#grant;w,r,s,c;[all]#access-id#".$userdn[$j];
			  $oid++;
			}
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
<?php
	// {{{ Show a header
	if($attrib == pql_get_define("PQL_ATTR_SEEALSO"))
	  $what = $LANG->_('contact person');
	elseif($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"))
	  $what = $LANG->_('mailinglist administrator');
	elseif($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"))
	  $what = $LANG->_('webserver administrator');
	elseif($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"))
	  $what = $LANG->_('DNS administrator');
	elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR"))
	  $what = $LANG->_('group moderator');
	elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER"))
	  $what = $LANG->_('group member');
	elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_SENDER"))
	  $what - $LANG->_('group allowed sender');

	 if($_REQUEST["user"]) {
	   $_REQUEST[$attrib] = $_REQUEST["user"];
	   
	   if(!$what)
		 $what = pql_maybe_decode($_REQUEST["user"])." ".$LANG->_('as administrator');
	   else
		 $what = $_REQUEST["user"]." ".$what;
	 } elseif(!$what)
	   $what = $LANG->_('domain administrator');
	 // }}}
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $what, 'domain' => $_REQUEST["orgname"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
<?php if(($_REQUEST["attrib"] == 'dnmember') or
		 ($_REQUEST["attrib"] == 'dnmoderator') or
		 ($_REQUEST["attrib"] == 'dnsender'))
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
			// {{{ Show branches and users
			// We're giving user access to a known domain/branch
			$i = 0; $div_counter = 0;

			// Get all domains in the database.
			$domains = pql_get_domains($_pql);
			if(isset($domains)) {
				foreach($domains as $key => $domain) {
					// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
					$d = split(',', $domain); $d = split('=', $d[0]); $d = $d[1];

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
<?php
			// }}}
		} else {
			// {{{ Show a menu with branches
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
				foreach($_SESSION["BASE_DN"] as $dn)  {
					$dom = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"),
											$_SESSION["USER_DN"], 1);
					if($dom)
					  foreach($dom as $d)
						$domains[] = $d;
				}
			}

			if(is_array($domains)) {
				$domains = pql_uniq($domains);
				foreach($domains as $domain) {
					if($domain == $_SESSION["BASE_DN"][0]) {
						$shown_first_rootdn = 1;
?>
              <option value="<?=$domain?>">*&nbsp;<?=$domain?></option>
<?php				} else { ?>
              <option value="<?=$domain?>">&nbsp;&nbsp;&nbsp;<?=$domain?></option>
<?php				}
				}
			}
?>
            </select>
<?php
			// }}}
		}
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
	$value = $_REQUEST[$attrib];

	// {{{ Which DN should we use?
	if($_REQUEST["branchdn"]) {
	  if($_REQUEST["branchdn"] == 'superadmin')
		$DN = $_pql->ldap_basedn;
	  else
		$DN = array($_REQUEST["branchdn"]);
	} elseif(!$_REQUEST["user"])
	  $DN = array($_REQUEST["domain"]);
	elseif($_REQUEST["view"] == 'group')
	  // We're adding a subscriber,allowed sender or moderator to a qmailGroup object.
	  $DN = array($_REQUEST["user"]);
	else
	  $DN = array($_REQUEST["rootdn"]);
	// }}}

	// NOTE: No ACI additions/modifications if we're modifying a qmailGroup object,
	//       and only if it's an administrator we're adding!
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci") and
	   ($_REQUEST["view"] != 'group') and
	   (($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR")) or
		($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")) or
		($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) or
		($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"))))
	  $use_aci = 1;
	
	if($value and $_REQUEST["oldvalue"] and ($value == $_REQUEST["oldvalue"])) {
		// don't change if new value equal to old one
		$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Domain administrator')));
		attribute_forward($msg);
		break;
	}
	
	// Get the old values, so we can remove the one we're modifying
	// (delete the one we want to delete, keep the others etc)
	for($i=0; $DN[$i]; $i++) {
	  $dn = $DN[$i];
	  $tmp = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
	  if($tmp) {
		if(!is_array($tmp))
		  $admins[$dn] = array($tmp);
		else
		  $admins[$dn] = $tmp;
	  } else
		$admins[$dn] = array();
	}

	if($type == 'delete') {
	  // {{{ Delete user DN from array of administrators
	  if(!$_REQUEST["user"] and !$_REQUEST["branchdn"])
		unset($_REQUEST[$attrib]);

	  foreach($admins as $dn => $adm) {
		if(is_array($adm)) {
		  for($i=0; $adm[$i]; $i++) {
			if(!eregi($value, $adm[$i]))
			  $adms[$dn][] = $adm[$i];
		  }
		} else {
		  if(!eregi($value, $adm))
			$adms[$dn][] = $adm;
		}
	  }

	  for($i=0; $DN[$i]; $i++) {
		$dn = $DN[$i];

		if(is_array($adms[$dn]))
		  $ADMs[$dn][$attrib] = $adms[$dn];
		else
		  $ADMs[$dn][$attrib] = array();
		
		if($use_aci and
		   ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")) and
		   ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) and
		   ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9")))
		  // Modify the OpenLDAPaci attribute
		  $ACIs[$dn] = attribute_setup_aci($_pql->ldap_linkid, $_REQUEST["domain"], array($value), $type);
	  }
	  // }}}
	} else {
		// {{{ Add user 
		if($_REQUEST["user"] and ($_REQUEST["view"] != 'group')) {
		  for($i=0; $DN[$i]; $i++) {
			$dn = $DN[$i];

			$ADMs[$dn][$attrib][] = $_REQUEST["user"];
			
			if($use_aci and
			   ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")) and
			   ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) and
			   ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9")))
			  // Add the OpenLDAPaci attribute
			  $ACIs[$dn] = attribute_setup_aci($_pql->ldap_linkid, $_REQUEST["domain"], array($_REQUEST["user"]), $type);
		  }
		} elseif($value) {
			if(is_numeric($value)) {
			  // {{{ Numeric value
			  // Add the previous value(s)
			  $ADMs[$dn][$attrib] = $admins[$dn];

			  // Add the new value(s) from value '<attrib>_<number(s)>'
			  $amount = $value;
			  for($i=0; $i < $amount; $i++) {
				$var_name = $attrib . "_$i";
				if($_POST[$var_name])  {
				  for($j=0; $DN[$j]; $j++) {
					$dn = $DN[$j];

					$admins2add[$dn][] = $_POST[$var_name]; // Remember this for later...
					$ADMs[$dn][$attrib][] = $_POST[$var_name];
				  }
				}
			  }
			  // }}}
			} else {
			  // {{{ Specified user DN to add
			  for($i=0; $DN[$i]; $i++) {
				$dn = $DN[$i];

				// First add all the OLD values
				$adms = $admins[$dn];
				foreach($adms as $root_dn => $user_dn)
				  $ADMs[$dn][$attrib][] = $user_dn;

				// Add the NEW value
				$admins2add[$dn][] = $value;
				$ADMs[$dn][$attrib][] = $value;
			  }
			  // }}}
			}

			// Add the OpenLDAPaci attribute (maybe)
			if($use_aci) {
			  for($i=0; $DN[$i]; $i++) {
				$dn = $DN[$i];
				
				if(is_array($admins2add[$dn])) 
				  $ACIs[$dn] = attribute_setup_aci($_pql->ldap_linkid, $_REQUEST["domain"], $admins2add[$dn], $type);
			  }
			}
			
			unset($_REQUEST[$attrib]);
		}
		// }}}
	}

	// {{{ Transfer the ACI's from the '$ACIs' array to the '$ADMs' array
	if($use_aci) {
	  for($i=0; $DN[$i]; $i++) {
		$dn = $DN[$i];

		if(is_array($ACIs[$dn]))
		  $ADMs[$dn][pql_get_define("PQL_ATTR_LDAPACI")] = $ACIs[$dn];
	  }
	}

	unset($ACIs); // We're done with this. Any reference to ACIs is NEW values!
	// }}}

	// {{{ Do the modification(s)
	$success = 0;
	for($i=0; $DN[$i]; $i++) {
	  $dn = $DN[$i];

	  if(pql_modify_attribute($_pql->ldap_linkid, $dn, '', '', $ADMs[$dn]))
		$success = 1;
	}

	if($success) {
		// {{{ Using ACI's and got either <attribute=XXX> or user=XXX?
		if($use_aci and ($value or @$_REQUEST["user"])) {
		  // {{{ Adding/removing an administrator?
		  if($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR")) {
			// Get all the objects below this branch.
			for($i=0; $DN[$i]; $i++) {
			  $dn = $DN[$i];

			  $tmp = pql_get_dn($_pql->ldap_linkid, $dn, 'objectClass=*');

			  // Unset the first array entry - it's the root dn, and we've already
			  // fixed this!
			  unset($tmp[0]);

			  if($tmp)
				pql_add2array($branches, $tmp);
			}

			// A branch admin must have read access to all the templates!
			$filter = '(&(objectClass=phpQLAdminUserTemplate)('.pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME").'=*))';
			for($i=0; $_SESSION["BASE_DN"][$i]; $i++) {
			  $tmp = pql_get_dn($_pql->ldap_linkid, 'ou=Templates,'.$_SESSION["BASE_DN"][$i], $filter, 'ONELEVEL');
			  if($tmp)
				pql_add2array($branches, $tmp);
			}

			for($i=0; $i < count($branches); $i++) {
			  $skip = 0;

			  // ----------------
			  // Skip any DNS branches if:
			  //	* Bind9 management is enabled
			  //	* The current branch is a DNS branch
			  //	* The user is DNS admin for this domain/branch
			  if(pql_get_define("PQL_CONF_BIND9_USE") and
				 eregi(pql_get_define("PQL_CONF_SUBTREE_BIND9").',', $branches[$i]) and
				 pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["branchdn"],
											pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9"), $value))
				$skip = 1;

			  // ----------------
			  // Skip any web branches if:
			  //	* We haven't already desided to skip this branch
			  //	* Web server management is enabled
			  //	* The current branch is a webserver branch	Q: Now do I know that?!
			  //	* The user is web server admin for this domain/branch
			  //if(!$skip and pql_get_define("PQL_CONF_WEBSRV_USE") and
			  // eregi(pql_get_define("PQL_CONF_SUBTREE_BIND9").',', $branches[$i]) and
			  // pql_validate_administrator($_pql->ldap_linkid, $_REQUEST["branchdn"],
			  //							pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"), $value))
			  //$skip = 1;

			  // ----------------
			  if(!$skip) {
				for($j=0; $DN[$j]; $j++) {
				  $dn = $DN[$j];
				  
				  if($branches[$i] != $dn) {
					// Create an ACI entry for this branch.
					if($type == 'delete') {
					  $ACIs = attribute_setup_aci($_pql->ldap_linkid, $branches[$i], array($value), $type);
					} else {
					  $ACIs = attribute_setup_aci($_pql->ldap_linkid, $branches[$i], $admins2add[$dn], $type);
					}
					
					// Modify this object. BUG: Should we care about failure/successes?!
					pql_modify_attribute($_pql->ldap_linkid, $branches[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);
				  }
				}
			  }
			}
		  }
		  // }}}

		  // {{{ Adding/removing an DNS administrator?
		  if(pql_get_define("PQL_CONF_BIND9_USE") and
			 ($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9")) and
			 pql_get_define("PQL_CONF_SUBTREE_BIND9"))
		  {
			// Find all ou=DNS objects in all root DN's
			$filter   = '(&('.pql_get_define("PQL_CONF_SUBTREE_BIND9").')';
			$filter  .= '('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=*))';
			$branches = pql_get_dn($_pql->ldap_linkid, $_REQUEST["branchdn"], $filter);

			// Get all the objects below this branch.
			$objects = array();
			for($i=0; $branches[$i]; $i++) {
			  $tmp = pql_get_dn($_pql->ldap_linkid, $branches[$i], 'objectClass=*');
			  pql_add2array($objects, $tmp);
			}

			for($i=0; $i < count($objects); $i++) {
			  for($j=0; $DN[$j]; $j++) {
				$dn = $DN[$j];

				if($objects[$i] != $dn) {
				  // Create an ACI entry for this branch.
				  if($type == 'delete') {
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], array($value), $type);
				  } else {
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], $admins2add[$dn], $type);
				  }
				  
				  // Modify this object. BUG: Should we care about failure/successes?!
				  pql_modify_attribute($_pql->ldap_linkid, $objects[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);
				}
			  }
			}
		  }
		  // }}}
		
		  // {{{ Adding/removing an controlsAdministrator?
		  if(($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) and $_SESSION["USER_SEARCH_DN_CTR"]) {
			$objects = array();

			// If we're adding a super admin, he/she needs access to the
			// ou=Computers,<rootdn> branch as well!
			if($_REQUEST["branchdn"] == "superadmin")
			  $objects[] = $_SESSION["USER_SEARCH_DN_CTR"];

			// Get all physical hosts
			$filter = '(&('.pql_get_define("PQL_ATTR_CN").'=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))';
			$hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter, 'ONELEVEL');

			// For each physical host, try to find the mailservers
			for($i=0; $hosts[$i]; $i++) {
			  // Make sure this physical hosts get ACI changes
			  $objects[] = $hosts[$i];

			  $tmp = pql_get_dn($_pql->ldap_linkid, $hosts[$i], '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
			  pql_add2array($objects, $tmp);
			}

			for($i=0; $i < count($objects); $i++) {
			  for($j=0; $DN[$j]; $j++) {
				$dn = $DN[$j];
				
				if($objects[$i] != $dn) {
				  // Create an ACI entry for this branch.
				  if($type == 'delete')
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], array($value), $type);
				  else
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], $admins2add[$dn], $type);
				  
				  // Modify this object. BUG: Should we care about failure/successes?!
				  pql_modify_attribute($_pql->ldap_linkid, $objects[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);

				  if(($_REQUEST["branchdn"] == "superadmin") and
					 eregi('^'.pql_get_define("PQL_ATTR_OU").'=Computers,', $objects[$i]))
				  {
					// This so that we don't add new ACI's to the 'ou=Computers,<rootdn>' object
					// for each and every entry in $DN!
					$j = count($DN);
				  }
				}
			  }
			}
		  }
		  // }}}

		  // {{{ Adding/removing an webSrvAdministrator?
		  if(($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")) and $_SESSION["USER_SEARCH_DN_CTR"]) {
			// Include Webserver API etc
			require($_SESSION["path"]."/include/pql_websrv.inc");
			$objects = array();

			// If we're adding a super admin, he/she needs access to the
			// ou=Computers,<rootdn> branch as well!
			if($_REQUEST["branchdn"] == "superadmin")
			  $objects[] = $_SESSION["USER_SEARCH_DN_CTR"];
		  
			// Get all physical hosts
			$filter = '(&('.pql_get_define("PQL_ATTR_CN").'=*)(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=ipHost)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=device)))';
			$hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"], $filter, 'ONELEVEL');

			// For each physical host, try to find the webserver containers
			$servers = array();
			foreach($hosts as $host) {
			  // Make sure this physical hosts get ACI changes
			  $servers[] = $host;

			  $tmp = pql_websrv_find_servers($_pql->ldap_linkid, $host);
			  if(is_array($tmp))
				$servers = $servers + $tmp;
			}

			// For each webserver container, retreive ALL objects
			foreach($servers as $srvname => $srvdn) {
			  // Make sure this container get ACI changes
			  $servers[] = $srvdn;

			  $tmp = pql_get_dn($_pql->ldap_linkid, $srvdn, 'objectClass=*');
			  pql_add2array($objects, $tmp);
			}

			for($i=0; $i < count($objects); $i++) {
			  for($j=0; $DN[$j]; $j++) {
				$dn = $DN[$j];

				if($objects[$i] != $dn) {
				  // Create an ACI entry for this branch.
				  if($type == 'delete')
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], array($value), $type);
				  else
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], $admins2add[$dn], $type);
				  
				  // Modify this object. BUG: Should we care about failure/successes?!
				  pql_modify_attribute($_pql->ldap_linkid, $objects[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);

				  if(($_REQUEST["branchdn"] == "superadmin") and
					 eregi('^'.pql_get_define("PQL_ATTR_OU").'=Computers,', $objects[$i]))
				  {
					// This so that we don't add new ACI's to the 'ou=Computers,<rootdn>' object
					// for each and every entry in $DN!
					$j = count($DN);
				  }
				}
			  }
			}
		  }
		  // }}}
		}
		// }}}

		// {{{ Setup message and forward to correct details page
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
		// }}}
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
