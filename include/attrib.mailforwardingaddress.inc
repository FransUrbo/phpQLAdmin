<?php
// attribute plugin for
// mailforwardingaddress (forward all mails to this address)
// $Id: attrib.mailforwardingaddress.inc,v 2.15.2.1 2003-12-15 20:33:07 dlw Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    // global $mailforwardingaddress, $error, $LANG;
    global $error, $LANG;

	// is typed in ?
	if(empty($_REQUEST["mailforwardingaddress"])) {
		$error["mailforwardingaddress"] = $LANG->_('Missing');
		return false;
  	}

	// IDNA encode email address
	$_REQUEST["mailforwardingaddress"] = pql_maybe_idna_encode($_REQUEST["mailforwardingaddress"]);

	// valid
	if(!pql_check_email($_REQUEST["mailforwardingaddress"])) {
		$error["mailforwardingaddress"] = $LANG->_('Invalid');
		return false;
	}

	// maybe check existence for modifying
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
	// dummy function
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	//global $mailforwardingaddress, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <?php if($_REQUEST["oldvalue"] == ""){echo $LANG->_('Create new forwarding address');}else{echo $LANG->_('Change forwarding address');} ?>
      </th>
<?php
	// We're changing a MailForwardingAddress. Give the old as reference
	if($_REQUEST["oldvalue"] != "") {
		echo "<tr class=\"c2\">";
		echo "  <td class=\"title\">".$LANG->_('Old address')."</td>";
		echo "  <td>" . $_REQUEST["mailforwardingaddress"] . "</td>";
		echo "</tr>";
	}
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td>
          <?php echo pql_format_error_span($error["mailforwardingaddress"]); ?><input type="text" name="mailforwardingaddress" value="">
        </td>
      </tr>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"     value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
  //global $domain, $user, $attrib, $oldvalue, $view, $LANG;
  //global $mailforwardingaddress, $_pql;
    global $_pql, $LANG;

	switch($type) {
	  case "add":
		if(pql_modify_userattribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_GLOB_ATTR_FORWARDS"),
									'', $_REQUEST["mailforwardingaddress"]))
		  $msg = pql_complete_constant($LANG->_('Successfully added %what% %new%'),
									   array('what' => $LANG->_('forwarding address'),
											 'new'  => pql_maybe_idna_decode($_REQUEST["mailforwardingaddress"])));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to add %what%'),
									   array('what' => $LANG->_('forwarding address'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;

	  case "modify";
		$email = $_REQUEST["mailforwardingaddress"];
		
		if($email == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('forwarding address')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_userattribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_GLOB_ATTR_FORWARDS"),
									$_REQUEST["oldvalue"], $email))
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
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
