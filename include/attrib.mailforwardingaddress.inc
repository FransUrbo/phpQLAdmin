<?php
// attribute plugin for
// mailforwardingaddress (forward all mails to this address)
// $Id: attrib.mailforwardingaddress.inc,v 2.12 2003-08-11 19:49:52 turbo Exp $

function attribute_check($type) {
	global $mailforwardingaddress, $error, $LANG;

	// is typed in ?
	if($mailforwardingaddress == "") {
		$error["mailforwardingaddress"] = $LANG->_('Missing');
		return false;
  	}

	// valid
	if(!check_email($mailforwardingaddress)) {
		$error["mailforwardingaddress"] = $LANG->_('Invalid');
		return false;
	}

	// maybe check existence for modifying
	return true;
}

function attribute_init() {
	// dummy function
}

function attribute_print_form() {
	global $mailforwardingaddress, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <?php if($oldvalue == ""){echo $LANG->_('Create new forwarding address');}else{echo $LANG->_('Change forwarding address');} ?>
      </th>
<?php
	// We're changing a MailForwardingAddress. Give the old as reference
	if($oldvalue != "") {
		echo "<tr class=\"c2\">";
		echo "  <td class=\"title\">".$LANG->_('Old address')."</td>";
		echo "  <td>" . $mailforwardingaddress . "</td>";
		echo "</tr>";
	}
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td>
          <?php echo format_error($error["mailforwardingaddress"]); ?><input type="text" name="mailforwardingaddress" value="">
        </td>
      </tr>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// '
function attribute_save($type) {
	global $domain, $user, $attrib, $oldvalue, $view, $LANG;
	global $mailforwardingaddress, $_pql;

	switch($type) {
	  case "add":
		if(pql_modify_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_FORWARDS"),
									'', $mailforwardingaddress))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('forwarding address'),
											 'new'  => $mailforwardingaddress));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to set %what%'),
									   array('what' => $LANG->_('forwarding address'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;

	  case "modify";
		$email = $mailforwardingaddress;
		
		if($email == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('forwarding address')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_FORWARDS"), $oldvalue, $email))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('forwarding address'),
											 'new'  => $email));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('forwarding address'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
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
