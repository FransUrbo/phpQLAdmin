<?php
// attribute plugin for
// mailalternateaddress (emailaddress alias)
// $Id: attrib.mailalternateaddress.inc,v 2.37 2007-09-29 21:15:13 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
	global $_pql, $LANG;
	$attrib = $_REQUEST["attrib"];
    
    // is typed in ?
    if($_REQUEST[$attrib] == "") {
		$error[$attrib] = $LANG->_('Invalid');
		return false;
    }
    
	// Build the COMPLETE email address
	if(! ereg("@", $_REQUEST[$attrib])) {
		if($_REQUEST["email_domain"])
		  $_REQUEST[$attrib] = $_REQUEST[$attrib] . "@" . $_REQUEST["email_domain"];
		elseif($_REQUEST["defaultdomain"])
		  $_REQUEST[$attrib] = $_REQUEST[$attrib] . "@" . $_REQUEST["defaultdomain"];
	}

    // valid
    if(!pql_check_email($_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Invalid');
		return false;
    }
    
    // if type is modify, check existance of old value, else the new one
	if(!eregi('^rfc822', $attrib)) {
		if($type == "add") {
			// add
			if(pql_email_exists($_REQUEST[$attrib])) {
				$error[$attrib] = $LANG->_('Already exists');
				return false;
			}
		} else {
			// modify
			if($_REQUEST["oldvalue"] != $_REQUEST[$attrib] and pql_email_exists($_REQUEST[$attrib])) {
				$error[$attrib] = $LANG->_('Already exists');
				return false;
			}
		}
	}
    
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
    global $error, $_pql, $LANG;

	$attrib               = $_REQUEST["attrib"];
	$defaultdomain		  = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	$additionaldomainname = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
<?php
	if(eregi('^rfc822member', $attrib)) {
		echo $LANG->_('Add subscriber address');
	} elseif(eregi('^rfc822sender', $attrib)) {
		echo $LANG->_('Add allowed sender address');
	} elseif(eregi('^rfc822moderator', $attrib)) {
		echo $LANG->_('Add moderator address');
	} else {
        if($_REQUEST["oldvalue"] == "") {
			echo $LANG->_('Create new alias');
		} else {
			echo $LANG->_('Change alias');
		}
	}
?>
      </th>
<?php
    // We're changing a MailAlternateAddress. Give the old as reference
    if($_REQUEST["oldvalue"] != "") {
		echo "<tr class=\"c2\">";
		echo "  <td class=\"title\">".$LANG->_('Old address')."</td>";
		echo "  <td>" . $_REQUEST[$attrib] . "</td>";
		echo "</tr>";
    }
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td>
          <?php echo pql_format_error_span($error[$attrib]); ?>
<?php	if(!eregi('^rfc822', $attrib)) { ?>
          <input type="text" name="<?=$attrib?>" value="">
<?php	 	if(is_array($additionaldomainname)) { ?>
            <b>@ <select name="email_domain"></b>
              <option value="<?=$defaultdomain?>"><?=pql_maybe_idna_decode($defaultdomain)?></option>
<?php			foreach($additionaldomainname as $additional) { ?>
              <option value="<?=$additional?>"><?=pql_maybe_idna_decode($additional)?></option>
<?php   		} ?>
            </select>
<?php 		} else { ?>
            <b>@<?=pql_maybe_idna_decode($defaultdomain)?></b>
<?php 		}
		} else {
?>
          <input type="text" name="<?=$attrib?>" value="" size="40">
<?php	} ?>
        </td>
      </tr>

      <tr class="subtitle">
        <td colspan="2">
<?php if(!eregi('^rfc822', $attrib)) { ?>
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('An alias must be unique to the domain')?></td>
<?php } ?>
      </tr>
    </table>

    <input type="hidden" name="submit"        value="1">
    <input type="hidden" name="attrib"        value="<?=$attrib?>">
    <input type="hidden" name="oldvalue"      value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"        value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"        value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"          value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"          value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="defaultdomain" value="<?=$defaultdomain?>">
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

    switch($type) {
      case "add":
		if(pql_modify_attribute(urldecode($_REQUEST["user"]), $attrib, '', $_REQUEST[$attrib])) {
			$tmp  = split('@', $_REQUEST[$attrib]);
			$mail = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);

			$msg = pql_complete_constant($LANG->_('Successfully added %what% %new%'),
										 array('what' => $LANG->_('alias'),
											   'new'  => $mail));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to add %what%'),
									   array('what' => $LANG->_('alias'))) . ": " . pql_format_error(0);
		
		if (pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $_REQUEST["rootdn"])) {
			$url = "user_sendmail.php?root=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["branch"]."&email="
			  . urlencode($_REQUEST[$attrib]) . "&user=" . urlencode($_REQUEST["user"])
			  . "&msg=" . urlencode($msg);
			pql_header($url);
		} else
		  attribute_forward($msg);
		break;

      case "modify":
		if($_REQUEST[$attrib] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('alias')));
			attribute_forward($msg);
			break;
		}
		
		if(pql_modify_attribute(urldecode($_REQUEST["user"]), $attrib,
									$_REQUEST["oldvalue"], $_REQUEST[$attrib])) {
			$tmp  = split('@', $_REQUEST[$attrib]);
			$mail = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);

			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('alias'),
											   'new'  => $mail));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('alias'))) . ": " . pql_format_error(0);
		
		if(pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $_REQUEST["rootdn"])) {
			$url = "user_sendmail.php?root=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["branch"]."&email="
			  . urlencode($_REQUEST[$attrib]) . "&user=" . urlencode($_REQUEST["user"])
			  . "&msg=" . urlencode($msg);
			pql_header($url);
		} else
		  attribute_forward($msg);
		break;

      case "delete":
		// Get the old values, so we can remove the one we're modifying
		// (delete the one we want to delete, keep the others etc)
		$values = $_pql->get_attribute($_REQUEST["user"], $attrib);

		// Add the old values to the resulting array.
		$OLD[$attrib] = array();
		if(is_array($values)) {
			foreach($values as $value)
			  $OLD[$attrib][] = $value;
		} elseif($values)
		  $OLD[$attrib][] = $values;
		unset($values); // We're done with this array/value.

		// Delete value from old values
		foreach($OLD[$attrib] as $value) {
			if(!eregi($_REQUEST[$attrib], $value))
			  $values[] = $value;
		}
		$OLD[$attrib] = $values;

		if(pql_modify_attribute($_REQUEST["user"], '', '', $OLD))
		  $success = 1;

		if(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_822_MODERATOR"))))
		  $what = $LANG->_('group moderator');
		elseif(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_DN_MEMBER"))) or
			   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_822_MEMBER"))))
		  $what = $LANG->_('group member');
		elseif(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_DN_SENDER"))) or
			   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_822_SENDER"))))
		  $what = $LANG->_('group allowed sender');

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'), array('what' => $what));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $what))
			. ": " . ldap_error($_pql->ldap_linkid);
		
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
