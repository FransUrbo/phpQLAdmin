<?php
// attribute plugin for
// mailforwardingaddress (forward all mails to this address)
// $Id: attrib.mailforwardingaddress.inc,v 2.25 2006-12-16 12:02:14 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
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
	global $rootdn, $domain, $user, $error, $LANG, $_pql;

	// URLify the basic values...
	$url["rootdn"] = urlencode($rootdn);
	$url["domain"] = urlencode($domain);
	$url["user"]   = urlencode($user);
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
<?php $modes = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_MODE"));
      if(!is_array($modes)) $modes = array($modes);

      $dotmode = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"));
      if(!is_array($dotmode)) $dotmode = array($dotmode);

      if(!in_array('noprogram', $modes) and !in_array('none', $dotmode)) {
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Force no local delivery')?></td>
        <td><input type="checkbox" name="force_no_local_delivery"></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2">
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_("If \bForce no local delivery\B isn't set, a copy will be saved in the local mailbox.")?>
        </td>
      </tr>
<?php } ?>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	switch($type) {
	  case "add":
		if(pql_modify_attribute($_pql->ldap_linkid, urldecode($_REQUEST["user"]), pql_get_define("PQL_ATTR_FORWARDS"),
									'', $_REQUEST["mailforwardingaddress"]))
		  $msg = pql_complete_constant($LANG->_('Successfully added %what% %new%'),
									   array('what' => $LANG->_('forwarding address'),
											 'new'  => pql_maybe_idna_decode($_REQUEST["mailforwardingaddress"])));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to add %what%'),
									   array('what' => $LANG->_('forwarding address'))) . ": " . pql_format_error(0);
		
		if($_REQUEST["force_no_local_delivery"] == 'on') {
		  pql_modify_attribute($_pql->ldap_linkid, urldecode($_REQUEST["user"]), pql_get_define("PQL_ATTR_MODE"), '', 'noprogram');
		  pql_modify_attribute($_pql->ldap_linkid, urldecode($_REQUEST["user"]), pql_get_define("PQL_ATTR_DOTMODE"), '', 'none');
		}
		break;

	  case "modify":
		$email = $_REQUEST["mailforwardingaddress"];
		
		if($email == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('forwarding address')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_attribute($_pql->ldap_linkid, urldecode($_REQUEST["user"]), pql_get_define("PQL_ATTR_FORWARDS"),
									$_REQUEST["oldvalue"], $email))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('forwarding address'),
											 'new'  => $email));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('forwarding address'))) . ": " . pql_format_error(0);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}

	if(pql_get_define("PQL_CONF_DEBUG_ME"))
	  die();
	else
	  attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
