<?php
// Attribute plugin for additional group (memberUID)
// $Id: attrib.additionalgroup.inc,v 2.1 2003-11-26 16:42:07 turbo Exp $

// {{{ attribute_init(void)...
function attribute_init() {
    global $rootdn, $domain, $user, $attrib, $$attrib, $_pql, $LANG;
	global $group, $uid;

	// Get the UID value for this user
	$uid = pql_get_attribute($_pql->ldap_linkid, $user, lc(pql_get_define("PQL_GLOB_ATTR_UID"))); $uid = $uid[0];
}
// }}}

// {{{ attribute_check(void)...
function attribute_check() {
    global $rootdn, $domain, $user, $attrib, $$attrib, $_pql, $LANG;
	return true;
}
// }}}

// {{{ attribute_print_form(void)...
function attribute_print_form() {
    global $rootdn, $domain, $user, $attrib, $uid, $action, $view, $_pql, $LANG;

	// Initialize variables etc
	attribute_init();
	$groups = array();

	// Setup the LDAP search filter to get groups
	$filter  = "(&";
	$filter .=   "(".pql_get_define("PQL_GLOB_ATTR_CN")."=*)";
	$filter .=   "(objectClass=posixGroup)";
	$filter .=   "(".pql_get_define("PQL_GLOB_ATTR_QMAILGID")."=*)";
	$filter .= ")";

	foreach($_pql->ldap_basedn as $base) {
		$base = urldecode($base);

		// Get all the groups in this root DN.
		$grps = pql_search($_pql->ldap_linkid, $base, $filter);
		if(is_array($grps)) {
			for($i=0; $i < count($grps); $i++) {
				$is_member = 0;
				
				// Get additional members in this group
				$additional = pql_get_attribute($_pql->ldap_linkid, $grps[$i],
												lc(pql_get_define("PQL_GLOB_ATTR_ADDITIONAL_GROUP")));
				
				// Make sure the user isn't part of this group
				for($j=0; $j < count($additional); $j++) {
					if($additional[$j] == $uid)
					  $is_member = 1;
				}
				
				if(!$is_member) {
					// Get the group name for this group
					$groupname = pql_get_attribute($_pql->ldap_linkid, $grps[$i], lc(pql_get_define("PQL_GLOB_ATTR_CN")));
					$groups[$groupname[0]]["DN"] = $grps[$i];
					
					$groups[$groupname[0]]["MEMBERS"] = $additional;
				}
			}
		}
	}

	asort($groups, SORT_STRING); // Case insensitive sort

	// Used for the checkbox counter below
	$j = 0;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'),
																	array('what'   => $LANG->_('additional group'),
																		  'domain' => $domain)); ?></th>
<?php foreach($groups as $name => $data) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td>
<?php
		$links = array();
		for($i=0; $i< count($data["MEMBERS"]); $i++) {
			$new = array("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Current member: ".$data["MEMBERS"][$i] => '');
			$links = $links + $new;
		}

		$text = "<input type='checkbox' name='$attrib"."_$j' value='".$data["DN"]."'>$name";

		pql_format_tree($text, 0, $links, 1);

		$j++;
?>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="<?=$attrib?>" value="<?=$j?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <input type="hidden" name="action"   value="<?=$action?>">
    <input type="hidden" name="submit"   value="2">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(action)...
function attribute_save($action) {
    global $rootdn, $domain, $user, $attrib, $$attrib, $_pql, $LANG;
	global $group, $uid;

	attribute_init();

	switch($action) {
	  // {{{ case: delete
	  case "delete":
		// Setup the search filter to get the exact DN of the group
		$filter  = "(&($attrib=".$uid.")(".pql_get_define("PQL_GLOB_ATTR_CN")."=".$$attrib."))";
		
		// Get the DN of the group we're removing the user from
		foreach($_pql->ldap_basedn as $base) {
			$base = urldecode($base);
			
			if(!$group) {
				$group = pql_search($_pql->ldap_linkid, $base, $filter);
				$group = $group[0];
			}
		}
		
		if(pql_modify_userattribute($_pql->ldap_linkid, $group, $attrib, $uid, ''))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %user% from %what% %group%'),
									   array('what'  => $LANG->_('additional group'),
											 'user'  => $uid,
											 'group' => $$attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %user% from %what%'),
									   array('what' => $LANG->_('additional group'))) . ": " . pql_ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;
		// }}}

	  // {{{ case: modify
	  case "modify":
		// Get the group DNs we should add the user to
		$amount = $_POST[$attrib];
		for($i=0; $i < $amount; $i++) {
			$var_name = $attrib . "_$i";
			if($_POST[$var_name]) 
			  $groups[] = $_POST[$var_name];
		}

		$msg = '';
		for($i=0; $i< count($groups); $i++) {
			if(pql_modify_userattribute($_pql->ldap_linkid, $groups[$i], $attrib, '', $uid)) {
				$msg .= pql_complete_constant($LANG->_('Successfully added user %user% to group %group%<br>'),
											  array('user' => $uid, 'group' => $groups[$i]));
			} else {
				$msg .= pql_complete_constant($LANG->_('Failed to add user %user% to group %group%<br>'),
											  array('user' => $uid, 'group' => $groups[$i]));
			}
		}

		attribute_forward($msg);
		break;
		// }}}

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
