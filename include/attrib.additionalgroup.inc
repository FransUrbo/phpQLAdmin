<?php
// Attribute plugin for additional group (memberUID)
// $Id: attrib.additionalgroup.inc,v 2.8 2004-10-19 10:40:40 turbo Exp $

// {{{ attribute_init(void)...

function attribute_init() {
    global $_pql, $LANG;

	// Get the UID value for this user
	$uid = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], lc(pql_get_define("PQL_ATTR_UID")));
	$_REQUEST["uid"] = $uid[0];
}

// }}}

// {{{ attribute_check(void)...
function attribute_check() {
    global $_pql, $LANG;

	return true;
}
// }}}

// {{{ attribute_print_form(void)...

function attribute_print_form() {
    global $_pql, $LANG;

	// Initialize variables etc
	attribute_init();
	$groups = array();

	// Setup the LDAP search filter to get groups
	$filter  = "(&";
	$filter .=   "(".pql_get_define("PQL_ATTR_CN")."=*)";
	$filter .=   "(objectClass=posixGroup)";
	$filter .=   "(".pql_get_define("PQL_ATTR_QMAILGID")."=*)";
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
												pql_get_define("PQL_ATTR_ADDITIONAL_GROUP"));
				
				// Make sure the user isn't part of this group
				for($j=0; $j < count($additional); $j++) {
					if($additional[$j] == $_REQUEST["uid"])
					  $is_member = 1;
				}
				
				if(!$is_member) {
					// Get the group name for this group
					$groupname = pql_get_attribute($_pql->ldap_linkid, $grps[$i], pql_get_define("PQL_ATTR_CN"));
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
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'),
																	array('what'   => $LANG->_('additional group'),
																		  'domain' => $_REQUEST["domain"])); ?></th>
<?php foreach($groups as $name => $data) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td>
<?php
		$links = array();
		for($i=0; $i< count($data["MEMBERS"]); $i++) {
			$new = array("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Current member: ".$data["MEMBERS"][$i] => '');
			$links = $links + $new;
		}

		$text = "<input type='checkbox' name='" . $_REQUEST["attrib"] . "_$j' value='".$data["DN"]."'>$name";

		pql_format_tree($text, 0, $links, 1);

		$j++;
?>
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="<?=$_REQUEST["attrib"]?>" value="<?=$j?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
    <input type="hidden" name="submit"   value="2">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

// }}}

// {{{ attribute_save(action)...

function attribute_save($action) {
    global $_pql, $LANG;

	/* Sometimes attribute_save gets called directly, bypassing attribute_print_form.
	 * That's why attribute_init has to be called here, even if we cached the results of
	 * the previous call in the form in attribute_print_form. */
	attribute_init();
	$attrib_name = $_REQUEST["attrib"];

	switch($action) {
	  // {{{ case: delete

	  case "delete":
		// Setup the search filter to get the exact DN of the group
		$filter  = "(&($attrib_name=" . $_REQUEST["uid"] . ")(".pql_get_define("PQL_ATTR_CN")
		  ."=".$_REQUEST[$attrib_name]."))";

		// Get the DN of the group we're removing the user from
		foreach($_pql->ldap_basedn as $base) {
			$base = urldecode($base);

			if(!isset($group)) {
				$group = pql_search($_pql->ldap_linkid, $base, $filter);
				$group = $group[0];
			}
		}

		if(pql_modify_attribute($_pql->ldap_linkid, $group, $attrib_name, $_REQUEST["uid"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %user% from %what% %group%'),
									   array('what'  => $LANG->_('additional group'),
											 'user'  => $_REQUEST["uid"],
											 'group' => $_REQUEST[$attrib_name]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %user% from %what%'),
									   array('what' => $LANG->_('additional group'),
											 'user'  => $_REQUEST["uid"])) . ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

		// }}}

	  // {{{ case: modify
	  case "modify":
		// Get the group DNs we should add the user to
		$amount = $_REQUEST[$attrib_name];

		for($i=0; $i < $amount; $i++) {
			$var_name = $attrib_name . "_$i";
			if($_POST[$var_name]) 
			  $groups[] = $_POST[$var_name];
		}

		$msg = '';
		for($i=0; $i< count($groups); $i++) {
			if(pql_modify_attribute($_pql->ldap_linkid, $groups[$i], $attrib_name, '', $_REQUEST["uid"])) {
				$msg .= pql_complete_constant($LANG->_('Successfully added user %user% to group %group%<br>'),
											  array('user' => $_REQUEST["uid"], 'group' => $groups[$i]));
			} else {
				$msg .= pql_complete_constant($LANG->_('Failed to add user %user% to group %group%<br>'),
											  array('user' => $_REQUEST["uid"], 'group' => $groups[$i]));
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
