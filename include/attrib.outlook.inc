<?php
// attribute plugin for pilotPerson objects etc
// $Id: attrib.outlook.inc,v 1.13 2003-07-02 07:59:01 turbo Exp $

/*
	Attributes managed here:
	------------------------
	c
	facsimiletelephonenumber
	homePhone
	info
	l
	mobile
	o
	pager
	physicalDeliveryOfficeName
	postalAddress
	postalCode
	postOfficebox
	seealso
	st
	telephoneNumber
	title
	street

	Not implemented (see no point!)
	-------------------------------
	registeredAddress
	preferredDeliveryMethod
	physicalDeliveryOfficeName
*/

function attribute_check() {
    global $user, $domain, $oldvalue, $_pql, $error, $attrib, $$attrib, $LANG;

    // is typed in ?
    if($$attrib == "") {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}

function attribute_init() {
    global $$attrib, $oldvalue;
    $$attrib = $oldvalue;
}

function attribute_print_form() {
    global $user, $domain, $rootdn, $PHP_SELF, $attrib, $$attrib, $oldvalue, $error, $_pql, $view;
	global $orgname, $LANG;
?>

  <form action="<?=$PHP_SELF?>" method="post" name="outlook">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Set/Modify attribute %attribute%'), array('attribute' => $attrib)); ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$attrib?></td>
          <td><?php echo format_error($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?=$$attrib?>" size="30"></td>
        </tr>
      </th>
    </table>

	<input type="hidden" name="submit"   value="2">
	<input type="hidden" name="attrib"   value="<?=$attrib?>">
	<input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
	<input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
	<input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
	<input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

  <script language="JavaScript">
    <!--
    document.outlook.<?=$attrib?>.focus();
    // -->
  </script>
<?php
}

function attribute_save($type) {
    global $user, $domain, $rootdn, $attrib, $$attrib, $oldvalue, $_pql, $view, $LANG;

    switch($type) {
      case "add":
      case "modify":
		if($$attrib == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $attrib));
			attribute_forward($msg);
			break;
		}

		if($domain and !$user) {
			// Save a domain attribute
			if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $$attrib))
			  $success = 1;
		} elseif($user) {
			// Save a user attribute
			if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib, $$attrib))
			  $success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'), array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;

      case "delete";
		if($domain and !$user) {
			// Save a domain attribute
			if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, ''))
			  $success = 1;
//		} elseif($user and !$domain) {
// Currently no DELETE option
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully deleted %what%'), array('what' => $attrib)) . ".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
