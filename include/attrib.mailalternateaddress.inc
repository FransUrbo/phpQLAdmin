<?php
// attribute plugin for
// mailalternateaddress (emailaddress alias)
// $Id: attrib.mailalternateaddress.inc,v 2.23 2004-03-26 08:55:43 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
	global $_pql, $LANG;
    
    // is typed in ?
    if($_REQUEST["mailalternateaddress"] == "") {
		$error["mailalternateaddress"] = $LANG->_('Invalid');
		return false;
    }
    
	// Build the COMPLETE email address
	if(! ereg("@", $_REQUEST["mailalternateaddress"])) {
		if($_REQUEST["email_domain"])
		  $_REQUEST["mailalternateaddress"] = $_REQUEST["mailalternateaddress"] . "@" . $_REQUEST["email_domain"];
		elseif($_REQUEST["defaultdomain"])
		  $_REQUEST["mailalternateaddress"] = $_REQUEST["mailalternateaddress"] . "@" . $_REQUEST["defaultdomain"];
	}

    // valid
    if(!pql_check_email($_REQUEST["mailalternateaddress"])) {
		$error["mailalternateaddress"] = $LANG->_('Invalid');
		return false;
    }
    
    // if type is modify, check existance of old value, else the new one
    if($type == "add") {
		// add
		if(pql_email_exists($_pql, $_REQUEST["mailalternateaddress"])) {
			$error["mailalternateaddress"] = $LANG->_('Already exists');
			return false;
		}
    } else {
		// modify
		if($_REQUEST["oldvalue"] != $_REQUEST["mailalternateaddress"] and pql_email_exists($_pql, $_REQUEST["mailalternateaddress"])) {
			$error["mailalternateaddress"] = $LANG->_('Already exists');
			return false;
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

	$defaultdomain		  = pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
	$additionaldomainname = pql_domain_get_value($_pql, $_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <?php if($_REQUEST["oldvalue"] == ""){echo $LANG->_('Create new alias');}else{echo $LANG->_('Change alias');} ?>
      </th>
<?php
    // We're changing a MailAlternateAddress. Give the old as reference
    if($_REQUEST["oldvalue"] != "") {
		echo "<tr class=\"c2\">";
		echo "  <td class=\"title\">".$LANG->_('Old address')."</td>";
		echo "  <td>" . $_REQUEST["mailalternateaddress"] . "</td>";
		echo "</tr>";
    }
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td>
          <?php echo pql_format_error_span($error["mailalternateaddress"]); ?>
          <input type="text" name="mailalternateaddress" value="">
<?php 	if(is_array($additionaldomainname)) { ?>
            <b>@ <select name="email_domain"></b>
              <option value="<?=$defaultdomain?>"><?=pql_maybe_idna_decode($defaultdomain)?></option>
<?php		foreach($additionaldomainname as $additional) { ?>
              <option value="<?=$additional?>"><?=pql_maybe_idna_decode($additional)?></option>
<?php   	} ?>
            </select>
<?php 	} else { ?>
            <b>@<?=pql_maybe_idna_decode($defaultdomain)?></b>
<?php 	} ?>
        </td>
      </tr>

      <tr class="subtitle">
        <td colspan="2">
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('An alias must be unique to the domain')?></td>
      </tr>
    </table>

    <input type="hidden" name="submit"        value="1">
    <input type="hidden" name="attrib"        value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue"      value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"        value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"        value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"          value="<?=$_REQUEST["user"]?>">
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
    
    switch($type) {
      case "add":
		if(pql_modify_userattribute($_pql->ldap_linkid, $_REQUEST["user"],
									pql_get_define("PQL_ATTR_MAILALTERNATE"),
									'', $_REQUEST["mailalternateaddress"])) {
			$tmp  = split('@', $_REQUEST["mailalternateaddress"]);
			$mail = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);

			$msg = pql_complete_constant($LANG->_('Successfully added %what% %new%'),
										 array('what' => $LANG->_('alias'),
											   'new'  => $mail));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to add %what%'),
									   array('what' => $LANG->_('alias'))) . ": " . pql_format_error(0);
		
		if (pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $_REQUEST["rootdn"])) {
			$url = "user_sendmail.php?root=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["branch"]."&email="
			  . urlencode($_REQUEST["mailalternateaddress"]) . "&user=" . urlencode($_REQUEST["user"])
			  . "&msg=" . urlencode($msg);
			header("Location: " . pql_get_define("PQL_CONF_URI") . $url);
		} else
		  attribute_forward($msg);
		break;

      case "modify";
		if($_REQUEST["mailalternateaddress"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('alias')));
			attribute_forward($msg);
			break;
		}
		
		if(pql_modify_userattribute($_pql->ldap_linkid, $_REQUEST["user"],
									pql_get_define("PQL_ATTR_MAILALTERNATE"),
									$_REQUEST["oldvalue"], $_REQUEST["mailalternateaddress"])) {
			$tmp  = split('@', $_REQUEST["mailalternateaddress"]);
			$mail = $tmp[0].'@'.pql_maybe_idna_decode($tmp[1]);

			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('alias'),
											   'new'  => $mail));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('alias'))) . ": " . pql_format_error(0);
		
		if(pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND", $_REQUEST["rootdn"])) {
			$url = "user_sendmail.php?root=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["branch"]."&email="
			  . urlencode($_REQUEST["mailalternateaddress"]) . "&user=" . urlencode($_REQUEST["user"])
			  . "&msg=" . urlencode($msg);
			header("Location: " . pql_get_define("PQL_CONF_URI") . $url);
		} else
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
