<?php
// attribute plugin for administrator and seealso modification
// Also manages the qmailGroup attributes dnModerator, dnMember and dnSender
// $Id: attrib.administrator.inc,v 1.43 2004-10-14 08:50:32 turbo Exp $

require("./left-head.html");

// {{{ attribute_setup_aci(old_acis, userdn)
function attribute_setup_aci($old_acis, $userdn) {
	if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci")) {
		// Insert the NEW aci in the right place of the OLD.
		// The user entries should be inserted if
		//	$old_acis[?][RIGHTS][TARGET][?] == [children]
		// Then the user we're giving admin access should be inserted in
		//	$old_acis[?][SUBJECT]
		for($i=0; $old_acis[$i]; $i++) {
			if($old_acis[$i+1]["TYPE"] == "access-id") {
				if(in_array('[entry]', $old_acis[$i+1]["RIGHTS"]["TARGET"]) and !$added_child) {
					// Next ACI entry is an 'access-id/entry'. Put the 'access-id/children' here...

					$new_acis[] = $old_acis[$i]["LINE"];
					$new_acis[] = "1.2.3#entry#grant;w,r,s,c;[children]#access-id#".$userdn;
					$added_child = 1;
				} elseif(in_array('[all]', $old_acis[$i+1]["RIGHTS"]["TARGET"]) and !$added_entry) {
					// Next ACI entry is an 'access-id/all'. Put the 'access-id/entry' here...

					$new_acis[] = $old_acis[$i]["LINE"];
					$new_acis[] = "1.2.3#entry#grant;w,r,s,c,x;[entry]#access-id#".$userdn;
					$added_entry = 1;
				} else
				  $new_acis[] = $old_acis[$i]["LINE"];
			} else
			  $new_acis[] = $old_acis[$i]["LINE"];
		}
		$new_acis[] = "1.2.3#entry#grant;w,r,s,c,x;[all]#access-id#".$userdn;

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
    global $orgname; 			// Set by domain_edit_attributes.php

	$attrib = $_REQUEST["attrib"];
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php // ------------ S H O W  A  H E A D E R ------------ ?>
<?php if($attrib == pql_get_define("PQL_ATTR_SEEALSO")) { ?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('contact person'), 'domain' => $orgname)); ?></th>
<?php } elseif($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")) { ?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('mailinglist administrator'), 'domain' => $orgname)); ?></th>
<?php } elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR")) { ?>
      <th colspan="3" align="left"><?=$LANG->_('Add group moderator')?></th>
<?php } elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_MEMBER")) { ?>
      <th colspan="3" align="left"><?=$LANG->_('Add group member')?></th>
<?php } elseif($attrib == pql_get_define("PQL_ATTR_GROUP_DN_SENDER")) { ?>
      <th colspan="3" align="left"><?=$LANG->_('Add group allowed sender')?></th>
<?php } else {
		if($_REQUEST["user"]) {
			$_REQUEST[$attrib] = $_REQUEST["user"];
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %user% as administrator for domain'), array('user' => pql_maybe_decode($_REQUEST["user"]))); ?></th>
<?php	} else {
?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('domain administrator'), 'domain' => $orgname)); ?></th>
<?php	}
	  }
?>
        <tr class="<?php pql_format_table(); ?>">
<?php if(($_REQUEST["attrib"] == 'dnmember') or
		 ($_REQUEST["attrib"] == 'dnmoderator') or
		 ($_REQUEST["attrib"] != 'dnsender'))
	  {
		  // We're adding a user dn as subscriber,allowed sender or moderator to a qmailGroup object
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
			$i = 0; $j = 0;

			// We're should give a user access to THIS branch	
			foreach($_pql->ldap_basedn as $dn) {
				$dn = urldecode($dn);
				unset($dom); unset($domains);

				$dom = pql_domain_get_value($_pql, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"]);
				if($dom) {
					foreach($dom as $d) {
						$domains[] = $d;
					}
				}

				if(isset($domains)) {
					asort($domains);
					foreach($domains as $key => $branch) {
						// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
						$d = split(',', $branch); $d = split('=', $d[0]); $d = $d[1];
						$d = pql_maybe_decode($d);
?>

<?php					if($_SESSION["opera"]) { ?>
            <div id="el2Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
              <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              <font color="black" class="heada"><?=$d?></font>
            </div>
<?php					} else { ?>
            <div id="el<?=$j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              </a>
        
              <a class="item">
                <font color="black" class="heada"><?=$d?></font>
              </a>
            </div>
<?php					} ?>
        
<?php					if($_SESSION["opera"]) { ?>
            <span id="el<?=$j?>Spn" style="display:'none'">
<?php					} else { ?>
            <div id="el<?=$j?>Child" class="child">
<?php					}

						// Get all users (their DN) in this domain
						$users = pql_user_get($_pql->ldap_linkid, $branch);
						if(is_array($users)) {
							// Zero out the variables, othervise we won't get users in
							// specified domain, but also in the PREVIOUS domain shown!
							unset($cns);
							foreach($users as $username) {
								$cn = pql_get_attribute($_pql->ldap_linkid, $username, pql_get_define("PQL_ATTR_CN"));
								$cns[$username] = $cn[0];
							}
								
							foreach($cns as $userdn => $username) {
?>
              <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="<?=$attrib?>_<?=$i?>" value="<?=$userdn?>"><?=$username?></nobr><br>
<?php              				$i++;
               				}
						}

						$j++;

						if($_SESSION["opera"]) {
?>
            </span>
<?php					} else { ?>
            </div>
<?php					}
					}
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
				$domains = pql_domain_get($_pql);
			} else {
				// We can only give user access to a branch we (the logged in user)
				// have access to. Otherwise, we could 'steal' access to any domain
				// in the system. Only a branch owner/admin can give someone access
				// to the current branch. Am I making myself clear?
				foreach($_pql->ldap_basedn as $dn)  {
					$dn = urldecode($dn);

					$dom = pql_domain_get_value($_pql, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"),
											$_SESSION["USER_DN"], 1);
					if($dom)
					  foreach($dom as $d)
						$domains[] = $d;
				}
			}

			if(is_array($domains)) {
				$domains = pql_uniq($domains);
				asort($domains);
				foreach($domains as $branch) {
					if($branch == $_pql->ldap_basedn[0]) {
						$shown_first_rootdn = 1;
?>
              <option value="<?=$branch?>">*&nbsp;<?=pql_maybe_decode($branch)?></option>
<?php				} else { ?>
              <option value="<?=$branch?>">&nbsp;&nbsp;&nbsp;<?=pql_maybe_decode($branch)?></option>
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

	// There is serious problems with this. I fix for one usage and break another
	// one! Let's try to remember (and document) how it's called. It's called from
	// the following places:
	// * left.php->[user]->User access->Allow user to create domains below domain
	// * left.php->[user]->User access->Disallow user to create domains below domain
	// * left.php->[user]->Group stuff->Member DN(s)->Add member
	// * left.php->[user]->Group stuff->Sender DN(s)->Add sender
	// * left.php->[user]->Group stuff->Moderator DN(s)->Add moderator
	// [anything else!?]

	$attrib = $_REQUEST["attrib"];
	$value = $_REQUEST[$attrib];

	// Which DN should we use?
	if($_REQUEST["branchdn"])
	  $DN = $_REQUEST["branchdn"];
	elseif(!$_REQUEST["user"])
	  $DN = $_REQUEST["domain"];
	elseif($_REQUEST["view"] == 'group')
	  // We're adding a subscriber,allowed sender or moderator to a qmailGroup object.
	  $DN = $_REQUEST["user"];
	else
	  $DN = $_REQUEST["rootdn"];
	
	if($value and $_REQUEST["oldvalue"] and ($value == $_REQUEST["oldvalue"])) {
		// don't change if new value equal to old one
		$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Domain administrator')));
		attribute_forward($msg);
		break;
	}
	
	// Get the old values, so we can remove the one we're modifying
	// (delete the one we want to delete, keep the others etc)
	$admins = pql_domain_get_value($_pql, $DN, $attrib);
	
	// Add the old values to the resulting array.
	$ADMs[$attrib] = array();
	if(is_array($admins)) {
		foreach($admins as $admin)
		  $ADMs[$attrib][] = $admin;
	} elseif($admins)
	  $ADMs[$attrib][] = $admins;
	
	if($type == 'delete') {
		// Delete user DN from array of administrators
		foreach($ADMs[$attrib] as $adm) {
			if(!eregi($value, $adm))
			  $adms[] = $adm;
		}
		
		$ADMs[$attrib] = $adms;
		
		if(!$_REQUEST["user"] and !$_REQUEST["branchdn"])
		  unset($_REQUEST[$attrib]);
	} else {
		// Add user 
		// NOTE: No ACI additions/modifications if we're modifying a qmailGroup object!
		if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci") and ($_REQUEST["view"] != 'group'))
		  $old_acis = pql_aci_get($_pql->ldap_linkid, $_REQUEST["domain"]);
		
		if($_REQUEST["user"] and ($_REQUEST["view"] != 'group')) {
			$ADMs[$attrib][] = $_REQUEST["user"];
			
			// Add the OpenLDAPaci attribute (maybe)
			if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci"))
			  $ACIs = attribute_setup_aci($old_acis, $_REQUEST["user"]);
		} elseif($value) {
			$amount = $value;
			for($i=0; $i < $amount; $i++) {
				$var_name = $attrib . "_$i";
				if($_POST[$var_name])  {
					$ADMs[$attrib][] = $_POST[$var_name];
					
					// Add the OpenLDAPaci attribute (maybe)
					if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci") and
					   ($_REQUEST["view"] != 'group'))
					  {
						  if(!is_array($ACIs))
							$ACIs = array();
						  
						  $new  = attribute_setup_aci($old_acis, $_POST[$var_name]);
						  $ACIs = $ACIs + $new;
					  }
				}
			}
			
			unset($_REQUEST[$attrib]);
		}
		
		if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci") and is_array($ACIs))
		  $ADMs[pql_get_define("PQL_ATTR_LDAPACI")] = $ACIs;
	}
	
	$success = 0;
	if(pql_domain_set_value($_pql->ldap_linkid, $DN, $attrib, $ADMs))
	  $success = 1;
	
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
